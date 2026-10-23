<#
.SYNOPSIS
    Safe Dual-Boot Windows BCD Installer for the Decompiled ntoskrnl.exe Replica.

.DESCRIPTION
    Copies `build\ntoskrnl_hybrid.exe` (or `-KernelPath`) into
    `C:\Windows\System32\ntoskrnl_decompiled.exe` WITHOUT touching the stock
    `C:\Windows\System32\ntoskrnl.exe`, clones `{current}` in the Windows Boot
    Configuration Data (BCD) store, and configures `kernel ntoskrnl_decompiled.exe`
    with `testsigning on` and `nointegritychecks on` on the cloned boot entry only.

    Your primary Windows boot entry remains 100% untouched as an instant fallback.

.PARAMETER KernelPath
    Path to `ntoskrnl_hybrid.exe` (host-backed staged kernel), `ntoskrnl_dropin.exe`
    (legacy all-grafted research kernel), or `ntoskrnl.exe` (pure from-source image).

.PARAMETER EnableKernelDebug
    Enable COM1 (115200 8N1) WinDbg kernel debugging on the cloned BCD entry.

.PARAMETER Uninstall
    Remove the cloned BCD boot entry and delete `System32\ntoskrnl_decompiled.exe`.
#>
[CmdletBinding()]
param(
    [string]$KernelPath = (Join-Path $PSScriptRoot "..\build\ntoskrnl_hybrid.exe"),
    [string]$TargetFileName = "ntoskrnl_decompiled.exe",
    [string]$BootDescription = "Windows 10/11 (ntrev host-backed staged hybrid)",
    [switch]$EnableKernelDebug,
    [switch]$Uninstall
)

$ErrorActionPreference = "Stop"

$principal = New-Object Security.Principal.WindowsPrincipal([Security.Principal.WindowsIdentity]::GetCurrent())
if (-not $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) {
    throw "Run this script from an Administrator PowerShell prompt."
}

$sys32Target = Join-Path $env:SystemRoot ("System32\" + $TargetFileName)
$guidStateFile = Join-Path $env:SystemRoot "System32\ntoskrnl_decompiled_bcd_guid.txt"

if ($Uninstall) {
    if (Test-Path $guidStateFile) {
        $savedGuid = (Get-Content $guidStateFile -Raw).Trim()
        if ($savedGuid -match "^\{[0-9a-fA-F\-]+\}$") {
            Write-Host "[*] Removing BCD entry $savedGuid ..."
            & bcdedit.exe /delete $savedGuid /cleanup | Out-Host
        }
        Remove-Item -Force $guidStateFile
    }
    if (Test-Path $sys32Target) {
        Write-Host "[*] Removing $sys32Target ..."
        Remove-Item -Force $sys32Target
    }
    Write-Host "[OK] Uninstalled decompiled kernel boot entry cleanly."
    exit 0
}

$resolvedKernel = (Resolve-Path $KernelPath).Path
$bytes = [System.IO.File]::ReadAllBytes($resolvedKernel)
if ($bytes.Length -lt 4096 -or $bytes[0] -ne 0x4D -or $bytes[1] -ne 0x5A) {
    throw "Invalid PE image: $resolvedKernel"
}
$peOff = [BitConverter]::ToInt32($bytes, 0x3C)
$machine = [BitConverter]::ToUInt16($bytes, $peOff + 4)
$magic = [BitConverter]::ToUInt16($bytes, $peOff + 24)
$subsys = [BitConverter]::ToUInt16($bytes, $peOff + 24 + 68)
if ($machine -ne 0x8664 -or $magic -ne 0x020B -or $subsys -ne 1) {
    throw "Expected AMD64 PE32+ IMAGE_SUBSYSTEM_NATIVE (1); got machine=0x$($machine.ToString('x')) subsys=$subsys"
}

try {
    if (Confirm-SecureBootUEFI) {
        Write-Warning "UEFI Secure Boot is currently ENABLED. Disable Secure Boot in UEFI Firmware Setup before booting the custom kernel entry so winload.efi honors 'nointegritychecks on'."
    }
} catch {
    # Legacy BIOS or cmdlet unavailable — ignore
}

Write-Host "[*] Copying $resolvedKernel -> $sys32Target (stock System32\ntoskrnl.exe is NOT modified) ..."
Copy-Item -Path $resolvedKernel -Destination $sys32Target -Force

Write-Host "[*] Cloning {current} Windows BCD boot entry ..."
$copyOut = & bcdedit.exe /copy "{current}" /d $BootDescription 2>&1 | Out-String
if ($copyOut -notmatch "(\{[0-9a-fA-F\-]+\})") {
    throw "Failed to parse new BCD GUID from bcdedit output: $copyOut"
}
$newGuid = $Matches[1]
Set-Content -Path $guidStateFile -Value $newGuid -Encoding ascii

Write-Host "[*] Configuring BCD entry $newGuid -> kernel=$TargetFileName ..."
& bcdedit.exe /set $newGuid kernel $TargetFileName | Out-Host
& bcdedit.exe /set $newGuid testsigning on | Out-Host
& bcdedit.exe /set $newGuid nointegritychecks on | Out-Host
& bcdedit.exe /set $newGuid bootmenupolicy Legacy | Out-Host
& bcdedit.exe /timeout 10 | Out-Host

if ($EnableKernelDebug) {
    & bcdedit.exe /debug $newGuid on | Out-Host
    & bcdedit.exe /dbgsettings serial debugport:1 baudrate:115200 | Out-Host
}

Write-Host ""
Write-Host "[OK] Installed Decompiled Windows NT Kernel into dual-boot BCD!"
Write-Host "     Binary : $sys32Target"
Write-Host "     BCD ID : $newGuid ($BootDescription)"
Write-Host "     Reboot and select '$BootDescription' at the Windows Boot Manager menu."
Write-Host "     To remove later: .\Install-DecompiledKernel.ps1 -Uninstall"
