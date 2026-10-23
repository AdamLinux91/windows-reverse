"""Byte-preserving whole-kernel artifact and verification.

A recompiled C function is not the original Windows kernel function.  Microsoft’s
source, compiler version, linker layout, hot-patch policy, and build flags are not
available, so recompilation cannot be used as an exactness proof.  The only artifact
that can make an unconditional byte-level claim from the supplied inputs is the
original PE image itself (or a separately proven machine-code-preserving relink).

This module deliberately makes that boundary executable instead of relying on a
README disclaimer:

* ``export_exact_kernel`` copies the input PE without transforming it;
* it verifies the AMD64/PE identity and, when supplied, the matching PDB RSDS identity;
* it records SHA-256 hashes for the complete image, every loaded section, and every
  discovered procedure; and
* ``verify_exact_kernel`` re-hashes a candidate later and fails on any mutation.

The resulting ``ntoskrnl.exact.exe`` is the artifact to use when the requirement is
"run the original kernel behavior".  The C/ASM tree remains a review/decompilation
artifact and must not be substituted for this file without a separate Windows
 differential or formal-equivalence gate.
"""

from __future__ import annotations

import hashlib
import json
import shutil
from dataclasses import dataclass, field
from pathlib import Path
from typing import Any

from ntrev.frontend import load_binary_image
from ntrev.models import BinaryImage, PDBDatabase
from ntrev.symbols import load_pdb_database


_EXACT_MANIFEST_VERSION = 1


@dataclass(slots=True)
class ExactVerificationResult:
    """Result of checking a byte-preserving kernel artifact."""

    passed: bool
    image_path: str
    manifest_path: str
    image_sha256: str = ""
    expected_sha256: str = ""
    checked_sections: int = 0
    checked_functions: int = 0
    errors: list[str] = field(default_factory=list)


@dataclass(slots=True)
class ExactKernelArtifact:
    """Paths and coverage telemetry for a byte-identical kernel copy."""

    source_image: str
    artifact_image: str
    manifest_path: str
    source_sha256: str
    artifact_sha256: str
    function_count: int
    exported_function_count: int
    executable_section_count: int
    executable_raw_bytes: int
    verified: bool

    def to_dict(self) -> dict[str, Any]:
        return {
            "source_image": self.source_image,
            "artifact_image": self.artifact_image,
            "manifest_path": self.manifest_path,
            "source_sha256": self.source_sha256,
            "artifact_sha256": self.artifact_sha256,
            "function_count": self.function_count,
            "exported_function_count": self.exported_function_count,
            "executable_section_count": self.executable_section_count,
            "executable_raw_bytes": self.executable_raw_bytes,
            "verified": self.verified,
        }


def _sha256_bytes(data: bytes) -> str:
    return hashlib.sha256(data).hexdigest()


def _sha256_file(path: Path) -> str:
    digest = hashlib.sha256()
    with path.open("rb") as stream:
        for chunk in iter(lambda: stream.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def _normal_guid(guid: str) -> str:
    return guid.strip().upper()


def _validate_pdb_identity(image: BinaryImage, pdb: PDBDatabase | None) -> None:
    """Reject a PDB that is not the one identified by the PE RSDS directory."""
    if pdb is None or image.rsds is None:
        return
    if _normal_guid(pdb.guid) != _normal_guid(image.rsds.guid) or int(pdb.age) != int(image.rsds.age):
        raise ValueError(
            "PDB/PE identity mismatch: "
            f"image RSDS={image.rsds.guid}/{image.rsds.age}, "
            f"PDB={pdb.guid}/{pdb.age}. Refusing an exact artifact with unrelated symbols."
        )


def _section_record(image: BinaryImage, section: Any) -> dict[str, Any]:
    return {
        "name": section.name,
        "rva": int(section.virtual_address),
        "virtual_size": int(section.virtual_size),
        "raw_size": len(section.raw_data),
        "executable": bool(section.is_executable),
        "writable": bool(section.is_writable),
        "sha256": _sha256_bytes(section.raw_data),
    }


def _function_records(
    image: BinaryImage,
    pdb: PDBDatabase | None,
    lifter: Any,
) -> tuple[list[dict[str, Any]], list[str]]:
    """Build hashes for the same complete function universe used by whole-kernel mode."""
    index = lifter.build_symbol_index(image, pdb)
    records: list[dict[str, Any]] = []
    errors: list[str] = []
    exported_by_rva: dict[int, list[str]] = {}
    for name, rva in image.export_by_name.items():
        exported_by_rva.setdefault(int(rva), []).append(name)

    pdb_names = pdb.symbols_by_rva if pdb is not None else {}
    for rva in sorted(index.functions_by_rva):
        meta = index.functions_by_rva[rva]
        blob = image.read_va(image.image_base + meta.rva, meta.size)
        if blob is None or len(blob) != meta.size:
            errors.append(
                f"function {meta.c_name} RVA 0x{meta.rva:x} has incomplete image bytes "
                f"({0 if blob is None else len(blob)} != {meta.size})"
            )
            continue
        p = pdb_names.get(meta.rva)
        records.append(
            {
                "name": meta.orig_name,
                "c_name": meta.c_name,
                "rva": int(meta.rva),
                "size": int(meta.size),
                "section": meta.section_name,
                "exported": bool(meta.is_exported),
                "export_names": sorted(exported_by_rva.get(meta.rva, [])),
                "pdb_name": p.name if p is not None else None,
                "machine_code_sha256": _sha256_bytes(blob),
                "machine_code": "preserved-from-input-pe",
            }
        )
    return records, errors


def export_exact_kernel(
    binary_path: str | Path,
    out_root: str | Path,
    pdb_path: str | Path | None = None,
    *,
    artifact_name: str = "ntoskrnl.exact.exe",
    manifest_name: str = "ntoskrnl.exact.manifest.json",
) -> ExactKernelArtifact:
    """Create and verify a byte-for-byte copy of a real AMD64 PE kernel.

    This function never disassembles and reassembles the bytes that are copied to the
    artifact.  Disassembly is used only to produce an auditable function hash manifest.
    If symbol discovery cannot map a procedure back to complete input bytes, the
    operation fails rather than silently producing a partial exactness claim.
    """
    source = Path(binary_path).resolve()
    if not source.is_file():
        raise FileNotFoundError(source)

    image = load_binary_image(source)
    if image.format != "PE32+" or image.machine != "AMD64":
        raise ValueError(
            f"Exact kernel mode requires an AMD64 PE32+ image, got {image.format}/{image.machine}"
        )

    pdb: PDBDatabase | None = None
    if pdb_path is not None:
        pdb = load_pdb_database(pdb_path, binary=image)
        _validate_pdb_identity(image, pdb)

    # Import lazily to avoid a module cycle during package initialization.
    from ntrev.whole_kernel.lifter_c import WholeKernelCLifter

    lifter = WholeKernelCLifter()
    function_records, function_errors = _function_records(image, pdb, lifter)
    if function_errors:
        raise ValueError(
            "Cannot establish complete machine-code coverage for exact artifact:\n"
            + "\n".join(function_errors[:20])
        )
    if not function_records:
        raise ValueError("Exact kernel mode discovered no procedures in the AMD64 image")

    destination_root = Path(out_root).resolve()
    destination_root.mkdir(parents=True, exist_ok=True)
    artifact = destination_root / artifact_name
    manifest_path = destination_root / manifest_name
    if artifact.resolve() == source:
        raise ValueError("Refusing to overwrite the source kernel with its exact artifact")

    # copyfile copies bytes only; no PE field, timestamp, checksum, or signature is
    # regenerated.  That is intentional: any such rewrite would invalidate identity.
    shutil.copyfile(source, artifact)
    source_hash = _sha256_file(source)
    artifact_hash = _sha256_file(artifact)
    if source_hash != artifact_hash:
        raise IOError("Exact artifact copy failed SHA-256 verification")

    sections = [_section_record(image, section) for section in image.sections]
    executable_sections = [section for section in image.sections if section.is_executable]
    manifest: dict[str, Any] = {
        "manifest_version": _EXACT_MANIFEST_VERSION,
        "mode": "byte-preserving-original-pe",
        "claim": (
            "artifact_image is byte-for-byte identical to source_image. This preserves "
            "the original machine code, PE metadata, relocations, imports, unwind data, "
            "and embedded data. It is not a claim that a recompiled C tree is equivalent."
        ),
        "source": {
            "path": str(source),
            "size": source.stat().st_size,
            "sha256": source_hash,
        },
        "artifact": {
            "path": str(artifact),
            "size": artifact.stat().st_size,
            "sha256": artifact_hash,
        },
        "identity": {
            "format": image.format,
            "machine": image.machine,
            "image_base": image.image_base,
            "entry_point_rva": image.entry_point_rva,
            "rsds_guid": image.rsds.guid if image.rsds else None,
            "rsds_age": image.rsds.age if image.rsds else None,
            "pdb_filename": image.rsds.pdb_filename if image.rsds else None,
        },
        "pdb": {
            "path": str(Path(pdb_path).resolve()) if pdb_path is not None else None,
            "guid": pdb.guid if pdb is not None else None,
            "age": pdb.age if pdb is not None else None,
        },
        "sections": sections,
        "functions": function_records,
        "coverage": {
            "function_count": len(function_records),
            "exported_function_count": sum(1 for record in function_records if record["exported"]),
            "executable_section_count": len(executable_sections),
            "executable_raw_bytes": sum(len(section.raw_data) for section in executable_sections),
            "function_bytes_hashed": sum(record["size"] for record in function_records),
            "function_ranges_are_audit_only": True,
            "complete_image_hash_is_authoritative": True,
        },
    }
    manifest_path.write_text(json.dumps(manifest, indent=2) + "\n", encoding="utf-8")

    verification = verify_exact_kernel(artifact, manifest_path)
    if not verification.passed:
        raise IOError("Fresh exact artifact failed verification: " + "; ".join(verification.errors))

    return ExactKernelArtifact(
        source_image=str(source),
        artifact_image=str(artifact),
        manifest_path=str(manifest_path),
        source_sha256=source_hash,
        artifact_sha256=artifact_hash,
        function_count=len(function_records),
        exported_function_count=sum(1 for record in function_records if record["exported"]),
        executable_section_count=len(executable_sections),
        executable_raw_bytes=sum(len(section.raw_data) for section in executable_sections),
        verified=True,
    )


def verify_exact_kernel(
    image_path: str | Path,
    manifest_path: str | Path,
) -> ExactVerificationResult:
    """Verify image, section, and discovered-procedure hashes against a manifest."""
    image_file = Path(image_path).resolve()
    manifest_file = Path(manifest_path).resolve()
    result = ExactVerificationResult(
        passed=False,
        image_path=str(image_file),
        manifest_path=str(manifest_file),
    )
    if not image_file.is_file():
        result.errors.append(f"image does not exist: {image_file}")
        return result
    if not manifest_file.is_file():
        result.errors.append(f"manifest does not exist: {manifest_file}")
        return result

    try:
        manifest = json.loads(manifest_file.read_text(encoding="utf-8"))
    except (OSError, ValueError) as exc:
        result.errors.append(f"invalid manifest: {exc}")
        return result

    if manifest.get("manifest_version") != _EXACT_MANIFEST_VERSION:
        result.errors.append(
            f"unsupported manifest version: {manifest.get('manifest_version')!r}"
        )
    if manifest.get("mode") != "byte-preserving-original-pe":
        result.errors.append(f"unsupported exact mode: {manifest.get('mode')!r}")

    expected = str(manifest.get("artifact", {}).get("sha256", ""))
    result.expected_sha256 = expected
    result.image_sha256 = _sha256_file(image_file)
    if not expected:
        result.errors.append("manifest has no artifact SHA-256")
    elif result.image_sha256 != expected:
        result.errors.append(
            f"image SHA-256 mismatch: expected {expected}, got {result.image_sha256}"
        )

    source_info = manifest.get("source", {})
    source_path = Path(source_info.get("path", "")) if source_info.get("path") else None
    source_expected = str(source_info.get("sha256", ""))
    if source_path is not None and source_path.is_file() and source_expected:
        source_actual = _sha256_file(source_path)
        if source_actual != source_expected:
            result.errors.append(
                f"source SHA-256 mismatch: expected {source_expected}, got {source_actual}"
            )
        if expected and source_actual != expected:
            result.errors.append("source and artifact SHA-256 values differ")

    try:
        image = load_binary_image(image_file)
    except Exception as exc:  # pragma: no cover - parser-specific diagnostics
        result.errors.append(f"cannot parse artifact image: {exc}")
        return result

    identity = manifest.get("identity", {})
    if identity.get("machine") and image.machine != identity["machine"]:
        result.errors.append(f"machine mismatch: expected {identity['machine']}, got {image.machine}")
    if identity.get("image_base") is not None and image.image_base != int(identity["image_base"]):
        result.errors.append("image base mismatch")
    if identity.get("entry_point_rva") is not None and image.entry_point_rva != int(identity["entry_point_rva"]):
        result.errors.append("entry-point RVA mismatch")

    sections_by_name = {section.name: section for section in image.sections}
    for expected_section in manifest.get("sections", []):
        result.checked_sections += 1
        section = sections_by_name.get(expected_section.get("name"))
        if section is None:
            result.errors.append(f"missing section: {expected_section.get('name')}")
            continue
        actual_hash = _sha256_bytes(section.raw_data)
        if actual_hash != expected_section.get("sha256"):
            result.errors.append(
                f"section {section.name} SHA-256 mismatch: "
                f"expected {expected_section.get('sha256')}, got {actual_hash}"
            )

    for expected_function in manifest.get("functions", []):
        result.checked_functions += 1
        rva = int(expected_function.get("rva", -1))
        size = int(expected_function.get("size", -1))
        blob = image.read_va(image.image_base + rva, size) if rva >= 0 and size >= 0 else None
        if blob is None or len(blob) != size:
            result.errors.append(
                f"function {expected_function.get('name', '<unnamed>')} has unavailable bytes"
            )
            continue
        actual_hash = _sha256_bytes(blob)
        if actual_hash != expected_function.get("machine_code_sha256"):
            result.errors.append(
                f"function {expected_function.get('name', '<unnamed>')} RVA 0x{rva:x} changed"
            )

    result.passed = not result.errors
    return result


__all__ = [
    "ExactKernelArtifact",
    "ExactVerificationResult",
    "export_exact_kernel",
    "verify_exact_kernel",
]
