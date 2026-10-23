<#!
.SYNOPSIS
    Runs an application-level smoke gate inside the Windows guest booted with a
    host-backed ntrev hybrid kernel.

.DESCRIPTION
    This script is deliberately run AFTER booting ntoskrnl_hybrid.exe. It exercises
    the ordinary kernel paths required by applications while the stock host kernel
    remains authoritative for every non-allowlisted entry point:
      * process creation and wait/exit
      * file create/write/read/delete
      * registry create/read/delete
      * named-pipe IPC
      * loopback TCP connect/accept
      * PowerShell/CIM process enumeration

    It writes a JSON report and returns non-zero if any test fails. Passing this gate
    demonstrates that the host-backed staged kernel can run these application paths;
    it is not a proof for a standalone clean-room kernel or for an untested function.

.PARAMETER OutputPath
    JSON report path. Defaults to the current user's Desktop.

.PARAMETER KernelImage
    The hybrid image copied into System32 before boot. The file is hashed for the report.

.PARAMETER PatchManifest
    JSON allowlist used to build the hybrid image.
#>
[CmdletBinding()]
param(
    [string]$OutputPath = (Join-Path ([Environment]::GetFolderPath('Desktop')) 'ntrev-kernel-smoke.json'),
    [string]$KernelImage = (Join-Path $env:SystemRoot 'System32\ntoskrnl_decompiled.exe'),
    [string]$PatchManifest = ''
)

$ErrorActionPreference = 'Stop'
$started = Get-Date
$results = [System.Collections.Generic.List[object]]::new()
$root = Join-Path $env:TEMP ("ntrev-smoke-" + [Guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $root -Force | Out-Null

function Add-TestResult {
    param([string]$Name, [scriptblock]$Action)
    $sw = [Diagnostics.Stopwatch]::StartNew()
    try {
        & $Action
        $results.Add([pscustomobject]@{ name = $Name; passed = $true; milliseconds = $sw.ElapsedMilliseconds; error = $null })
    } catch {
        $results.Add([pscustomobject]@{ name = $Name; passed = $false; milliseconds = $sw.ElapsedMilliseconds; error = $_.Exception.ToString() })
    }
}

Add-TestResult 'process-create-wait-exit' {
    $p = Start-Process -FilePath $env:ComSpec -ArgumentList '/c','exit','37' -PassThru -WindowStyle Hidden
    Wait-Process -Id $p.Id -Timeout 30
    $p.Refresh()
    if ($p.ExitCode -ne 37) { throw "unexpected child exit code $($p.ExitCode)" }
}

Add-TestResult 'file-write-read-delete' {
    $path = Join-Path $root 'io.bin'
    $bytes = [byte[]](0..255)
    [IO.File]::WriteAllBytes($path, $bytes)
    $read = [IO.File]::ReadAllBytes($path)
    if (-not ($read -ceq $bytes)) { throw 'file contents changed' }
    Remove-Item -LiteralPath $path -Force
    if (Test-Path -LiteralPath $path) { throw 'file was not deleted' }
}

Add-TestResult 'registry-create-read-delete' {
    $key = 'HKCU:\Software\ntrev-smoke-' + [Guid]::NewGuid().ToString('N')
    New-Item -Path $key -Force | Out-Null
    New-ItemProperty -Path $key -Name Value -Value 'ntrev' -PropertyType String -Force | Out-Null
    if ((Get-ItemPropertyValue -Path $key -Name Value) -ne 'ntrev') { throw 'registry value mismatch' }
    Remove-Item -Path $key -Recurse -Force
}

Add-TestResult 'named-pipe-ipc' {
    $pipeName = 'ntrev-smoke-' + [Guid]::NewGuid().ToString('N')
    $server = [IO.Pipes.NamedPipeServerStream]::new($pipeName, [IO.Pipes.PipeDirection]::InOut, 1, [IO.Pipes.PipeTransmissionMode]::Byte, [IO.Pipes.PipeOptions]::Asynchronous)
    try {
        $clientTask = [IO.Pipes.NamedPipeClientStream]::new('.', $pipeName, [IO.Pipes.PipeDirection]::Out)
        $clientTask.Connect(5000)
        $server.WaitForConnection()
        $payload = [Text.Encoding]::UTF8.GetBytes('ntrev-pipe')
        $clientTask.Write($payload, 0, $payload.Length)
        $clientTask.Flush()
        $received = New-Object byte[] $payload.Length
        [void]$server.Read($received, 0, $received.Length)
        if ([Text.Encoding]::UTF8.GetString($received) -ne 'ntrev-pipe') { throw 'pipe payload mismatch' }
        $clientTask.Dispose()
    } finally { $server.Dispose() }
}

Add-TestResult 'loopback-tcp' {
    $listener = [Net.Sockets.TcpListener]::new([Net.IPAddress]::Loopback, 0)
    $listener.Start()
    try {
        $port = ([Net.IPEndPoint]$listener.LocalEndpoint).Port
        $client = [Net.Sockets.TcpClient]::new()
        $client.Connect('127.0.0.1', $port)
        $serverClient = $listener.AcceptTcpClient()
        $out = $client.GetStream()
        $payload = [Text.Encoding]::ASCII.GetBytes('ntrev-tcp')
        $out.Write($payload, 0, $payload.Length)
        $buf = New-Object byte[] $payload.Length
        [void]$serverClient.GetStream().Read($buf, 0, $buf.Length)
        if ([Text.Encoding]::ASCII.GetString($buf) -ne 'ntrev-tcp') { throw 'TCP payload mismatch' }
        $serverClient.Dispose(); $client.Dispose()
    } finally { $listener.Stop() }
}

Add-TestResult 'cim-process-enumeration' {
    $os = Get-CimInstance Win32_OperatingSystem
    if (-not $os.Caption) { throw 'CIM did not return an operating-system caption' }
    if (-not (Get-Process -Id $PID)) { throw 'current process was not enumerable' }
}

try {
    $kernelHash = if (Test-Path -LiteralPath $KernelImage) { (Get-FileHash -Algorithm SHA256 $KernelImage).Hash } else { $null }
    $patchDoc = if ($PatchManifest -and (Test-Path -LiteralPath $PatchManifest)) { Get-Content $PatchManifest -Raw | ConvertFrom-Json } else { $null }
    $report = [ordered]@{
        schema = 1
        mode = 'host-backed-staged-hybrid'
        started_utc = $started.ToUniversalTime().ToString('o')
        finished_utc = (Get-Date).ToUniversalTime().ToString('o')
        computer = $env:COMPUTERNAME
        os = (Get-CimInstance Win32_OperatingSystem | Select-Object Caption, Version, BuildNumber)
        kernel_image = $KernelImage
        kernel_sha256 = $kernelHash
        patch_manifest = $PatchManifest
        patched_functions = if ($patchDoc) { @($patchDoc.functions | ForEach-Object { $_.name }) } else { @() }
        tests = @($results)
        passed = (@($results | Where-Object { -not $_.passed }).Count -eq 0)
    }
    $report | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $OutputPath -Encoding UTF8
} finally {
    Remove-Item -LiteralPath $root -Recurse -Force -ErrorAction SilentlyContinue
}

if (-not $report.passed) {
    Write-Error "ntrev Windows kernel smoke gate FAILED; report: $OutputPath"
    exit 1
}
Write-Host "ntrev Windows kernel smoke gate PASSED: $OutputPath"
exit 0
