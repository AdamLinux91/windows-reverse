"""
Symbol & Structural Mapper.

Maps low-level x86-64 memory displacements (`[reg + disp]`, `gs:[0x188]`, `gs:[0x20]`) into
high-level C structure dereferences (`Process->UniqueProcessId`, `KeGetCurrentThread()->ApcState.Process`,
`Thread->Cid.UniqueThread`, `CONTAINING_RECORD(...)`), and synthesizes reconstructed
`struct _RECON_*` definitions for untyped pointer parameters from observed memory access patterns.
"""

from __future__ import annotations

from dataclasses import dataclass

from ntrev.frontend.asm_detector import CANONICAL_GPR64
from ntrev.models import (
    FieldAccessRef,
    FunctionCFG,
    PDBDatabase,
    StructDef,
    StructField,
)
from ntrev.symbols.nt_types import (
     KNOWN_WDK_PROTOTYPES,
    KPCR_GS_OFFSETS,
    get_builtin_nt_structs,
)

WIN64_PARAM_REGS = ["rcx", "rdx", "r8", "r9"]


@dataclass
class TrackedPointer:
    struct_name: str
    c_expr: str


class StructuralMapper:
    """Applies PDB, WDK, & built-in NT types to CFG memory operands and reconstructs unknown structs."""

    def __init__(self, pdb: PDBDatabase | None = None) -> None:
        self.structs: dict[str, StructDef] = dict(get_builtin_nt_structs())
        self.pdb = pdb
        if pdb is not None:
            for sname, sdef in pdb.structs.items():
                self.structs[sname] = sdef

    def map_function_structures(
        self,
        cfg: FunctionCFG,
        param_names: dict[str, str] | None = None,
    ) -> tuple[list[FieldAccessRef], list[StructDef], dict[str, str]]:
        p_names = dict(
            param_names
            or {
                "rcx": "arg1",
                "rdx": "arg2",
                "r8": "arg3",
                "r9": "arg4",
            }
        )
        reg_ptrs: dict[str, TrackedPointer] = {}
        inferred_param_structs: dict[str, str] = {}
        explicit_typed_regs: set[str] = set()

        # 1. Seed pointer types from authoritative PDB S_GPROC32 prototype if available
        seeded_from_pdb = False
        if self.pdb is not None:
            rva = cfg.address - self._guess_image_base(cfg.address)
            sym = self.pdb.symbols_by_name.get(cfg.name) or self.pdb.symbols_by_rva.get(rva)
            if sym and sym.param_types:
                seeded_from_pdb = True
                for idx, (pname, ptype) in enumerate(sym.param_types[:4]):
                    reg = WIN64_PARAM_REGS[idx]
                    p_names[reg] = pname
                    if ptype.strip() not in ("void*", "PVOID"):
                        explicit_typed_regs.add(reg)
                    sname = self._resolve_pointee_struct(ptype)
                    if sname and sname in self.structs:
                        reg_ptrs[reg] = TrackedPointer(struct_name=sname, c_expr=pname)
                        inferred_param_structs[reg] = ptype

        # 2. Fall back to documented WDK prototype catalog if stripped public PDB had no S_GPROC32
        if not seeded_from_pdb and cfg.name in KNOWN_WDK_PROTOTYPES:
            _wdk_ret, wdk_params = KNOWN_WDK_PROTOTYPES[cfg.name]
            for idx, (pname, ptype) in enumerate(wdk_params[:4]):
                reg = WIN64_PARAM_REGS[idx]
                adj_ptype = self._adapt_wdk_type(ptype)
                p_names[reg] = pname
                if adj_ptype.strip() not in ("void*", "PVOID"):
                    explicit_typed_regs.add(reg)
                sname = self._resolve_pointee_struct(adj_ptype)
                if sname and sname in self.structs:
                    reg_ptrs[reg] = TrackedPointer(struct_name=sname, c_expr=pname)
                    inferred_param_structs[reg] = adj_ptype

        # 3. Collect raw displacement accesses on remaining untyped parameter registers
        untyped_accesses: dict[str, dict[int, tuple[int, bool]]] = {}
        for blk in cfg.sorted_blocks():
            for ins in blk.instructions:
                for op_idx, op in enumerate(ins.operands):
                    if op.kind == "mem" and op.base and op.index is None and op.segment is None:
                        canon_base = CANONICAL_GPR64.get(op.base)
                        if (
                            canon_base in WIN64_PARAM_REGS
                            and canon_base not in reg_ptrs
                            and canon_base not in explicit_typed_regs
                            and op.disp >= 0
                        ):
                            is_w = op_idx == 0 and ins.mnemonic not in ("cmp", "test")
                            untyped_accesses.setdefault(canon_base, {})[op.disp] = (
                                op.size or 8,
                                is_w,
                            )

        # Match untyped parameter access patterns against known PDB structs, or synthesize one
        synthesized: list[StructDef] = []
        for reg, disp_map in untyped_accesses.items():
            if len(disp_map) < 2:
                continue
            matched = self._match_existing_struct(disp_map)
            pname = p_names.get(reg, reg)
            if matched is not None:
                reg_ptrs[reg] = TrackedPointer(struct_name=matched.name, c_expr=pname)
                kw = "union" if matched.is_union else "struct"
                ptr_t = matched.pointer_typedef or f"{kw} {matched.name}*"
                inferred_param_structs[reg] = ptr_t
            else:
                recon = self._synthesize_struct(cfg.name, reg, disp_map)
                self.structs[recon.name] = recon
                synthesized.append(recon)
                reg_ptrs[reg] = TrackedPointer(struct_name=recon.name, c_expr=pname)
                inferred_param_structs[reg] = f"struct {recon.name}*"

        # Forward dataflow walk to resolve field accesses, KPCR gs:[], and CONTAINING_RECORD
        field_refs: list[FieldAccessRef] = []

        for blk in cfg.sorted_blocks():
            for ins in blk.instructions:
                # 1. Check `gs:[disp]` KPCR / KPRCB reads
                for op_idx, op in enumerate(ins.operands):
                    if op.kind == "mem" and op.segment == "gs":
                        kpcr_hit = KPCR_GS_OFFSETS.get(op.disp)
                        c_expr = (
                            kpcr_hit[0]
                            if kpcr_hit
                            else f"__ntrev_readgsqword(0x{op.disp:x})"
                        )
                        c_type = kpcr_hit[1] if kpcr_hit else "uint64_t"
                        pointee = kpcr_hit[2] if kpcr_hit else None
                        field_refs.append(
                            FieldAccessRef(
                                insn_addr=ins.address,
                                base_reg="gs",
                                disp=op.disp,
                                access_size=op.size or 8,
                                struct_name="_KPCR",
                                field_name=f"gs_0x{op.disp:x}",
                                field_c_type=c_type,
                                c_expr=c_expr,
                                is_write=False,
                                is_kpcr_gs=True,
                            )
                        )
                        if (
                            ins.mnemonic == "mov"
                            and op_idx == 1
                            and ins.operands[0].kind == "reg"
                            and ins.operands[0].reg
                            and pointee
                            and pointee in self.structs
                        ):
                            dst_canon = CANONICAL_GPR64.get(ins.operands[0].reg)
                            if dst_canon:
                                reg_ptrs[dst_canon] = TrackedPointer(
                                    struct_name=pointee, c_expr=c_expr
                                )

                # 2. Check CONTAINING_RECORD: `lea dst, [base - offset]`
                if (
                    ins.mnemonic == "lea"
                    and len(ins.operands) == 2
                    and ins.operands[0].kind == "reg"
                    and ins.operands[1].kind == "mem"
                    and ins.operands[1].disp < 0
                    and ins.operands[1].base
                ):
                    pos_off = -ins.operands[1].disp
                    cr_match = self._find_containing_record(pos_off)
                    dst_canon = CANONICAL_GPR64.get(ins.operands[0].reg or "")
                    src_canon = CANONICAL_GPR64.get(ins.operands[1].base)
                    if cr_match and dst_canon and src_canon:
                        st_def, list_fld = cr_match
                        src_expr = p_names.get(src_canon, src_canon)
                        cr_expr = (
                            f"CONTAINING_RECORD({src_expr}, struct {st_def.name}, {list_fld.name})"
                        )
                        reg_ptrs[dst_canon] = TrackedPointer(
                            struct_name=st_def.name, c_expr=cr_expr
                        )
                        field_refs.append(
                            FieldAccessRef(
                                insn_addr=ins.address,
                                base_reg=src_canon,
                                disp=ins.operands[1].disp,
                                access_size=8,
                                struct_name=st_def.name,
                                field_name=list_fld.name,
                                field_c_type=f"struct {st_def.name}*",
                                c_expr=cr_expr,
                                is_containing_record=True,
                            )
                        )
                        continue

                # 3. Resolve `[tracked_ptr + disp]` struct field accesses (including nested paths!)
                for op_idx, op in enumerate(ins.operands):
                    if op.kind == "mem" and op.base and op.index is None and op.segment is None:
                        canon_base = CANONICAL_GPR64.get(op.base)
                        if not canon_base or canon_base not in reg_ptrs:
                            continue
                        tracked = reg_ptrs[canon_base]
                        sdef = self.structs.get(tracked.struct_name)
                        if not sdef:
                            continue
                        acc_sz = op.size or 8
                        is_lea = ins.mnemonic == "lea"
                        nested = sdef.resolve_nested_field_path(
                            op.disp, acc_sz, self.structs, is_address_only=is_lea
                        )
                        if nested is not None:
                            fpath, fld = nested
                        else:
                            fld_fb = sdef.field_at_offset(op.disp)
                            if not fld_fb:
                                continue
                            fpath, fld = fld_fb.name, fld_fb

                        is_write = op_idx == 0 and ins.mnemonic not in ("cmp", "test", "lea")
                        base_str = (
                            f"({tracked.c_expr})"
                            if " " in tracked.c_expr
                            else tracked.c_expr
                        )
                        c_expr = (
                            f"&({base_str}->{fpath})"
                            if is_lea
                            else f"{base_str}->{fpath}"
                        )
                        field_refs.append(
                            FieldAccessRef(
                                insn_addr=ins.address,
                                base_reg=canon_base,
                                disp=op.disp,
                                access_size=op.size or fld.size,
                                struct_name=sdef.name,
                                field_name=fpath,
                                field_c_type=fld.c_type,
                                c_expr=c_expr,
                                is_write=is_write,
                            )
                        )

                        # Propagate pointer field loads (`mov rdx, [rcx + 0x98]`)
                        if (
                            ins.mnemonic == "mov"
                            and op_idx == 1
                            and ins.operands[0].kind == "reg"
                            and ins.operands[0].reg
                        ):
                            dst_canon = CANONICAL_GPR64.get(ins.operands[0].reg)
                            if dst_canon:
                                pt = fld.pointee_type or self._resolve_pointee_struct(fld.c_type)
                                if fld.is_pointer and pt in self.structs:
                                    reg_ptrs[dst_canon] = TrackedPointer(
                                        struct_name=pt,
                                        c_expr=c_expr,
                                    )
                                elif dst_canon == canon_base:
                                    reg_ptrs.pop(dst_canon, None)

                # Copy propagation: `mov rbx, rcx`
                if (
                    ins.mnemonic == "mov"
                    and len(ins.operands) == 2
                    and ins.operands[0].kind == "reg"
                    and ins.operands[1].kind == "reg"
                ):
                    dst_c = CANONICAL_GPR64.get(ins.operands[0].reg or "")
                    src_c = CANONICAL_GPR64.get(ins.operands[1].reg or "")
                    if dst_c and src_c:
                        if src_c in reg_ptrs:
                            reg_ptrs[dst_c] = reg_ptrs[src_c]
                        else:
                            reg_ptrs.pop(dst_c, None)

        return field_refs, synthesized, inferred_param_structs

    def _adapt_wdk_type(self, c_type: str) -> str:
        """Adapt WDK struct/union qualifiers to match loaded PDB definitions."""
        clean = c_type.strip()
        if clean == "struct _ETHREAD*" and "_ETHREAD" not in self.structs and "_KTHREAD" in self.structs:
            return "struct _KTHREAD*"
        sname = self._resolve_pointee_struct(clean)
        if sname and sname in self.structs:
            kw = "union" if self.structs[sname].is_union else "struct"
            const_pfx = "const " if clean.startswith("const ") else ""
            return f"{const_pfx}{kw} {sname}*"
        return clean

    def _resolve_pointee_struct(self, c_type: str) -> str | None:
        clean = c_type.strip()
        if clean.startswith("const "):
            clean = clean[len("const ") :].strip()
        for pfx in ("struct ", "union "):
            if clean.startswith(pfx) and clean.endswith("*"):
                return clean[len(pfx) : -1].strip()
        for sname, sdef in self.structs.items():
            if sdef.pointer_typedef and clean == sdef.pointer_typedef:
                return sname
            if sdef.typedef_name and clean == f"{sdef.typedef_name}*":
                return sname
        return None

    def _match_existing_struct(
        self, disp_map: dict[int, tuple[int, bool]]
    ) -> StructDef | None:
        best: StructDef | None = None
        best_score = 0
        for sdef in self.structs.values():
            if sdef.source == "reconstructed":
                continue
            hits = 0
            all_match = True
            for disp, (sz, _) in disp_map.items():
                fld = sdef.field_at_offset(disp)
                if fld and fld.offset == disp and (fld.size == sz or fld.size == 0):
                    hits += 1
                else:
                    all_match = False
                    break
            if all_match and hits > best_score and hits >= 2:
                best = sdef
                best_score = hits
        return best

    def _synthesize_struct(
        self, fn_name: str, reg: str, disp_map: dict[int, tuple[int, bool]]
    ) -> StructDef:
        sname = f"_RECON_{fn_name.upper()}_{reg.upper()}"
        fields: list[StructField] = []
        max_end = 0
        size_to_type = {1: "uint8_t", 2: "uint16_t", 4: "uint32_t", 8: "uint64_t"}

        for disp in sorted(disp_map.keys()):
            sz, _ = disp_map[disp]
            ctype = size_to_type.get(sz, "uint64_t")
            fields.append(
                StructField(
                    name=f"field_0x{disp:x}",
                    offset=disp,
                    size=sz,
                    c_type=ctype,
                )
            )
            max_end = max(max_end, disp + sz)

        aligned_size = ((max_end + 7) // 8) * 8
        return StructDef(
            name=sname,
            typedef_name=sname.lstrip("_"),
            pointer_typedef=f"P{sname.lstrip('_')}",
            size=aligned_size,
            fields=fields,
            source="reconstructed",
        )

    def _find_containing_record(self, pos_offset: int) -> tuple[StructDef, StructField] | None:
        # Prioritize canonical intrusive links (`_EPROCESS.ActiveProcessLinks`, `_OBJECT_HEADER.Body`)
        priority_names = ["_OBJECT_HEADER", "_EPROCESS", "_KTHREAD", "_ETHREAD", "_DEVICE_OBJECT"]
        ordered = [self.structs[n] for n in priority_names if n in self.structs] + [
            s for n, s in self.structs.items() if n not in priority_names
        ]
        for sdef in ordered:
            for fld in sdef.fields:
                if fld.offset == pos_offset and (
                    "LIST_ENTRY" in fld.c_type
                    or fld.name.endswith("Links")
                    or (sdef.name == "_OBJECT_HEADER" and fld.name == "Body")
                ):
                    return sdef, fld
        return None

    @staticmethod
    def _guess_image_base(va: int) -> int:
        if va >= 0x140000000:
            return 0x140000000
        return va & ~0xFFFFF
