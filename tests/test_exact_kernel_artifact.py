"""Tests for the byte-preserving exact-kernel behavior path."""

from __future__ import annotations

import json
from pathlib import Path

from ntrev.whole_kernel import export_exact_kernel, verify_exact_kernel


def test_exact_kernel_artifact_is_byte_identical_and_audited(
    kernel_fixture_paths: tuple[Path, Path],
    tmp_path: Path,
) -> None:
    pe_path, pdb_path = kernel_fixture_paths
    artifact = export_exact_kernel(pe_path, tmp_path / "exact", pdb_path)

    assert artifact.verified
    assert artifact.source_sha256 == artifact.artifact_sha256
    assert Path(artifact.source_image).read_bytes() == Path(artifact.artifact_image).read_bytes()
    assert artifact.function_count == 6

    manifest = json.loads(Path(artifact.manifest_path).read_text(encoding="utf-8"))
    assert manifest["mode"] == "byte-preserving-original-pe"
    assert manifest["coverage"]["complete_image_hash_is_authoritative"] is True
    assert len(manifest["functions"]) == 6

    checked = verify_exact_kernel(artifact.artifact_image, artifact.manifest_path)
    assert checked.passed
    assert checked.checked_functions == 6
    assert checked.checked_sections >= 1


def test_exact_kernel_verification_rejects_mutated_image(
    kernel_fixture_paths: tuple[Path, Path],
    tmp_path: Path,
) -> None:
    pe_path, pdb_path = kernel_fixture_paths
    artifact = export_exact_kernel(pe_path, tmp_path / "exact", pdb_path)
    image = Path(artifact.artifact_image)
    original = image.read_bytes()
    image.write_bytes(original[:-1] + bytes([original[-1] ^ 0x01]))

    checked = verify_exact_kernel(image, artifact.manifest_path)
    assert not checked.passed
    assert any("SHA-256 mismatch" in error for error in checked.errors)
