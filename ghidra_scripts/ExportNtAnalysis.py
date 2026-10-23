# -*- coding: utf-8 -*-
# Ghidra Headless Programmatic API Script (Python / PyGhidra compatible)
# @category WindowsReverse.NTKernel
# @author AdamLinux91
#
# Usage:
#   analyzeHeadless /tmp/proj NtRev -import ntoskrnl.exe \
#       -scriptPath ./ghidra_scripts -postScript ExportNtAnalysis.py out.json -deleteProject

import json

try:
    from ghidra.app.decompiler import DecompileOptions, DecompInterface
    from ghidra.program.model.data import Pointer, Structure, Union
    from ghidra.util.task import ConsoleTaskMonitor
except ImportError:
    # Module is loaded outside Ghidra runtime (e.g., static inspection)
    DecompInterface = None


def _export_structures(program):
    dtm = program.getDataTypeManager()
    structures = []
    for dt in dtm.getAllDataTypes():
        is_struct = isinstance(dt, Structure)
        is_union = isinstance(dt, Union)
        if not (is_struct or is_union):
            continue
        name = dt.getName()
        if not name or dt.isZeroLength():
            continue
        fields = []
        for comp in dt.getDefinedComponents():
            f_dt = comp.getDataType()
            is_ptr = isinstance(f_dt, Pointer)
            pointee = (
                f_dt.getDataType().getName()
                if is_ptr and f_dt.getDataType() is not None
                else None
            )
            fields.append(
                {
                    "name": comp.getFieldName() or ("field_0x%x" % comp.getOffset()),
                    "offset": int(comp.getOffset()),
                    "size": int(comp.getLength()),
                    "c_type": f_dt.getDisplayName(),
                    "is_pointer": bool(is_ptr),
                    "pointee_type": pointee,
                }
            )
        structures.append(
            {
                "name": name,
                "size": int(dt.getLength()),
                "is_union": bool(is_union),
                "fields": fields,
            }
        )
    return structures


def _export_functions(program, monitor):
    decomp = DecompInterface()
    opts = DecompileOptions()
    decomp.setOptions(opts)
    decomp.openProgram(program)

    fm = program.getFunctionManager()
    exported_funcs = []

    for func in fm.getFunctions(True):
        if func.isThunk() or func.isExternal():
            continue
        entry_va = int(func.getEntryPoint().getOffset())
        body = func.getBody()
        size = int(body.getNumAddresses())

        params = []
        for p in func.getParameters():
            params.append(
                {
                    "name": p.getName(),
                    "c_type": p.getDataType().getDisplayName(),
                }
            )

        pcode_ops = []
        c_decomp = ""
        res = decomp.decompileFunction(func, 30, monitor)
        if res and res.decompileCompleted():
            if res.getDecompiledFunction() is not None:
                c_decomp = res.getDecompiledFunction().getC()
            high_fn = res.getHighFunction()
            if high_fn is not None:
                op_iter = high_fn.getPcodeOps()
                while op_iter.hasNext():
                    op = op_iter.next()
                    pcode_ops.append(
                        {
                            "mnemonic": op.getMnemonic(),
                            "seq_addr": "0x%x" % op.getSeqnum().getTarget().getOffset(),
                        }
                    )

        exported_funcs.append(
            {
                "name": func.getName(),
                "entry_va": "0x%x" % entry_va,
                "size": size,
                "calling_convention": func.getCallingConventionName(),
                "return_type": func.getReturnType().getDisplayName(),
                "parameters": params,
                "ghidra_c_preview": c_decomp,
                "high_pcode_count": len(pcode_ops),
            }
        )

    decomp.dispose()
    return exported_funcs


def run():
    if DecompInterface is None:
        raise RuntimeError("ExportNtAnalysis.py must be executed inside Ghidra Headless / PyGhidra.")

    args = getScriptArgs()  # noqa: F821 (provided by Ghidra script environment)
    if len(args) < 1:
        raise ValueError("Expected output JSON path as first script argument.")

    out_file = args[0]
    monitor = ConsoleTaskMonitor()
    prog = currentProgram  # noqa: F821 (provided by Ghidra script environment)

    payload = {
        "program_name": prog.getName(),
        "image_base": "0x%x" % prog.getImageBase().getOffset(),
        "compiler": prog.getCompiler(),
        "language_id": str(prog.getLanguageID()),
        "structures": _export_structures(prog),
        "functions": _export_functions(prog, monitor),
    }

    with open(out_file, "w") as fp:
        json.dump(payload, fp, indent=2)
    print("[ntrev] Exported Ghidra analysis bundle to %s" % out_file)


if __name__ == "__main__" and DecompInterface is not None:
    run()
