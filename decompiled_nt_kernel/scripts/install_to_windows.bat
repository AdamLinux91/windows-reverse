@echo off
setlocal
REM Safe Dual-Boot Windows BCD Installer for Decompiled ntoskrnl.exe Replica
REM Run as Administrator on an x64 Windows 10 / Windows 11 installation.

set "SCRIPT_DIR=%~dp0"
set "KERNEL_IMG=%~1"
if "%KERNEL_IMG%"=="" (
    if exist "%SCRIPT_DIR%..\build\ntoskrnl_hybrid.exe" (
        set "KERNEL_IMG=%SCRIPT_DIR%..\build\ntoskrnl_hybrid.exe"
    ) else if exist "%SCRIPT_DIR%..\build\ntoskrnl_dropin.exe" (
        set "KERNEL_IMG=%SCRIPT_DIR%..\build\ntoskrnl_dropin.exe"
    ) else (
        set "KERNEL_IMG=%SCRIPT_DIR%..\build\ntoskrnl.exe"
    )
)

powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%SCRIPT_DIR%Install-DecompiledKernel.ps1" -KernelPath "%KERNEL_IMG%"
endlocal
