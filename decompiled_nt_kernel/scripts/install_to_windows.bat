@echo off
setlocal
REM Safe Dual-Boot Windows BCD Installer for a generated standalone ntoskrnl.exe test image
REM Run as Administrator on an x64 Windows 10 / Windows 11 installation.

set "SCRIPT_DIR=%~dp0"
set "KERNEL_IMG=%~1"
if "%KERNEL_IMG%"=="" (
    if exist "%SCRIPT_DIR%..\build\ntoskrnl_standalone.exe" (
        set "KERNEL_IMG=%SCRIPT_DIR%..\build\ntoskrnl_standalone.exe"
    ) else (
        echo Missing build\ntoskrnl_standalone.exe; run ntrev whole-kernel --standalone-output first.
        exit /b 2
    )
)

powershell.exe -NoProfile -ExecutionPolicy Bypass -File "%SCRIPT_DIR%Install-DecompiledKernel.ps1" -KernelPath "%KERNEL_IMG%"
endlocal
