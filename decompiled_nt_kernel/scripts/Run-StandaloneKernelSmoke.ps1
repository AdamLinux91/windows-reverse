<#
.SYNOPSIS
    Application smoke gate for a Windows VM booted with the generated standalone kernel.

.DESCRIPTION
    Run this inside the disposable Windows VM after installing the image produced by
    `ntrev whole-kernel --standalone-output`. It exercises ordinary application-facing
    paths and writes a JSON report. Passing this script is necessary, but not sufficient,
    evidence that the recompiled kernel behaves like Windows.
#>
[CmdletBinding()]
param(
    [string]$OutputPath = (Join-Path ([Environment]::GetFolderPath('Desktop')) 'ntrev-standalone-smoke.json'),
    [string]$KernelImage = (Join-Path $env:SystemRoot 'System32\ntoskrnl_decompiled.exe')
)

$ErrorActionPreference = 'Stop'
$results = [System.Collections.Generic.List[object]]::new()
$root = Join-Path $env:TEMP ("ntrev-standalone-smoke-" + [Guid]::NewGuid().ToString('N'))
New-Item -ItemType Directory -Path $root -Force | Out-Null

function Invoke-SmokeTest {
    param([string]$Name, [scriptblock]$Action)
    $watch = [Diagnostics.Stopwatch]::StartNew()
    try {
        & $Action
        $results.Add([pscustomobject]@{ name = $Name; passed = $true; milliseconds = $watch.ElapsedMilliseconds; error = $null })
    } catch {
        $results.Add([pscustomobject]@{ name = $Name; passed = $false; milliseconds = $watch.ElapsedMilliseconds; error = $_.Exception.ToString() })
    }
}

Invoke-SmokeTest 'cim-and-process-enumeration' {
    $os = Get-CimInstance Win32_OperatingSystem
    if (-not $os.Caption) { throw 'Win32_OperatingSystem did not return a caption' }
    if (-not (Get-Process -Id $PID)) { throw 'current process is not enumerable' }
}

Invoke-SmokeTest 'process-create-and-wait' {
    $child = Start-Process -FilePath $env:ComSpec -ArgumentList '/c','exit','23' -PassThru -WindowStyle Hidden
    Wait-Process -Id $child.Id -Timeout 30
    $child.Refresh()
    if ($child.ExitCode -ne 23) { throw "unexpected exit code: $($child.ExitCode)" }
}

Invoke-SmokeTest 'file-io' {
    $path = Join-Path $root 'file.bin'
    $expected = [byte[]](0..255)
    [IO.File]::WriteAllBytes($path, $expected)
    $actual = [IO.File]::ReadAllBytes($path)
    if ([Convert]::ToBase64String($actual) -ne [Convert]::ToBase64String($expected)) { throw 'file contents differ' }
    Remove-Item -LiteralPath $path -Force
}

Invoke-SmokeTest 'registry-io' {
    $key = 'HKCU:\Software\ntrev-standalone-' + [Guid]::NewGuid().ToString('N')
    New-Item -Path $key -Force | Out-Null
    New-ItemProperty -Path $key -Name Value -Value 'standalone' -PropertyType String -Force | Out-Null
    if ((Get-ItemPropertyValue -Path $key -Name Value) -ne 'standalone') { throw 'registry value mismatch' }
    Remove-Item -Path $key -Recurse -Force
}

Invoke-SmokeTest 'loopback-network' {
    $listener = [Net.Sockets.TcpListener]::new([Net.IPAddress]::Loopback, 0)
    $listener.Start()
    try {
        $port = ([Net.IPEndPoint]$listener.LocalEndpoint).Port
        $client = [Net.Sockets.TcpClient]::new('127.0.0.1', $port)
        $server = $listener.AcceptTcpClient()
        $payload = [Text.Encoding]::ASCII.GetBytes('ntrev-standalone')
        $client.GetStream().Write($payload, 0, $payload.Length)
        $actual = New-Object byte[] $payload.Length
        $read = 0
        while ($read -lt $actual.Length) {
            $chunk = $server.GetStream().Read($actual, $read, $actual.Length - $read)
            if ($chunk -le 0) { throw 'loopback connection closed early' }
            $read += $chunk
        }
        if ([Text.Encoding]::ASCII.GetString($actual) -ne 'ntrev-standalone') { throw 'network payload mismatch' }
        $server.Dispose(); $client.Dispose()
    } finally { $listener.Stop() }
}

try {
    $report = [ordered]@{
        schema = 1
        mode = 'standalone-generated-kernel-smoke'
        computer = $env:COMPUTERNAME
        os = (Get-CimInstance Win32_OperatingSystem | Select-Object Caption, Version, BuildNumber)
        kernel_image = $KernelImage
        kernel_sha256 = if (Test-Path $KernelImage) { (Get-FileHash -Algorithm SHA256 $KernelImage).Hash } else { $null }
        tests = @($results)
        passed = (@($results | Where-Object { -not $_.passed }).Count -eq 0)
    }
    $report | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $OutputPath -Encoding UTF8
} finally {
    Remove-Item -LiteralPath $root -Recurse -Force -ErrorAction SilentlyContinue
}

if (-not $report.passed) {
    Write-Error "Standalone kernel smoke gate FAILED: $OutputPath"
    exit 1
}
Write-Host "Standalone kernel smoke gate PASSED: $OutputPath"
exit 0
