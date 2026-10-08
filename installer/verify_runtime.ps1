[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)][string]$PayloadDir,
    [string[]]$DependencyDirs = @(),
    [switch]$Repair,
    [string]$ReportPath
)
$ErrorActionPreference = 'Stop'
Import-Module Microsoft.PowerShell.Utility -ErrorAction Stop
. (Join-Path $PSScriptRoot 'runtime_helpers.ps1')
$payload = (Resolve-Path -LiteralPath $PayloadDir).Path
$dumpbin = (Get-Command dumpbin.exe -ErrorAction Stop).Source
# Only Windows 10/11 system libraries may be resolved outside the payload.
$windowsLibraries = @(
    'kernel32.dll', 'kernelbase.dll', 'ntdll.dll', 'user32.dll', 'gdi32.dll',
    'advapi32.dll', 'shell32.dll', 'shlwapi.dll', 'shcore.dll', 'ole32.dll',
    'oleaut32.dll', 'oleacc.dll', 'comctl32.dll', 'comdlg32.dll', 'ws2_32.dll', 'wsock32.dll',
    'mswsock.dll', 'winmm.dll', 'winspool.drv', 'imm32.dll', 'uxtheme.dll',
    'dwmapi.dll', 'opengl32.dll', 'glu32.dll', 'version.dll', 'rpcrt4.dll',
    'secur32.dll', 'sspicli.dll', 'crypt32.dll', 'cryptbase.dll', 'bcrypt.dll',
    'bcryptprimitives.dll', 'wintrust.dll', 'userenv.dll', 'psapi.dll',
    'dbghelp.dll', 'dbgcore.dll', 'imagehlp.dll', 'setupapi.dll', 'cfgmgr32.dll', 'iphlpapi.dll',
    'dnsapi.dll', 'dhcpcsvc.dll', 'netapi32.dll', 'mpr.dll', 'winhttp.dll',
    'wininet.dll', 'normaliz.dll', 'hid.dll', 'winscard.dll', 'powrprof.dll',
    'd3d9.dll', 'd3d11.dll', 'd3d12.dll', 'dxgi.dll', 'dxguid.dll', 'dwrite.dll',
    'd2d1.dll', 'dcomp.dll', 'dinput8.dll', 'dsound.dll', 'avrt.dll',
    'wtsapi32.dll', 'wldap32.dll', 'usp10.dll', 'msimg32.dll', 'ucrtbase.dll'
)
function Get-BinaryImports([string]$Path) {
    $bytes = [System.IO.File]::ReadAllBytes($Path)
    if ($bytes.Length -lt 64 -or $bytes[0] -ne 77 -or $bytes[1] -ne 90) { throw "Not a PE binary: $Path" }
    $peOffset = [BitConverter]::ToInt32($bytes, 60)
    if ($peOffset -lt 0 -or $peOffset + 6 -gt $bytes.Length -or
        [BitConverter]::ToUInt16($bytes, $peOffset + 4) -ne 0x8664) { throw "Expected x64 binary: $Path" }
    $lines = @(& $dumpbin /nologo /dependents $Path)
    if ($LASTEXITCODE -ne 0) { throw "dumpbin failed: $Path" }
    foreach ($line in $lines) {
        if ($line -match '^\s+([^\s]+\.(?:dll|drv))\s*$') { $Matches[1] }
    }
}
$queue = New-Object 'System.Collections.Generic.Queue[string]'
foreach ($file in Get-ChildItem -LiteralPath $payload -Recurse -File) {
    # IFW's maintenance executable is a separate self-contained installer tool.
    if ($file.Name -eq 'FOSD-Pro Maintenance Tool.exe') { continue }
    if ($file.Extension -in @('.exe', '.dll')) { $queue.Enqueue($file.FullName) }
}
$seen = @{}
$audit = New-Object 'System.Collections.Generic.List[object]'
while ($queue.Count -gt 0) {
    $binary = $queue.Dequeue()
    if ($seen.ContainsKey($binary)) { continue }
    $seen[$binary] = $true
    if ([IO.Path]::GetFileName($binary) -match '^(Qt5.*d|vtk.*-8\.2d)\.dll$') { throw "Debug DLL in package: $binary" }
    $imports = @(Get-BinaryImports $binary | Sort-Object -Unique)
    foreach ($dependency in $imports) {
        if ($dependency -match '^(api-ms-|ext-ms-)' -or $dependency -in $windowsLibraries) { continue }
        $local = Join-Path $payload $dependency
        $beside = Join-Path (Split-Path -Parent $binary) $dependency
        if (Test-Path -LiteralPath $beside -PathType Leaf) { $local = $beside }
        if (-not (Test-Path -LiteralPath $local -PathType Leaf)) {
            $source = $null
            if ($Repair) {
                foreach ($directory in $DependencyDirs) {
                    $candidate = Join-Path $directory $dependency
                    if (Test-Path -LiteralPath $candidate -PathType Leaf) { $source = $candidate; break }
                }
            }
            if (-not $source) { throw "Missing runtime $dependency, imported by $binary" }
            Copy-Item -LiteralPath $source -Destination $local
            Write-Host "  Runtime: $dependency"
        }
        $queue.Enqueue($local)
    }
    $audit.Add([PSCustomObject]@{
        file = $binary.Substring($payload.Length + 1).Replace('\', '/')
        sha256 = Get-PayloadHash $binary
        imports = $imports
    })
}
foreach ($required in @('FOSD-Pro.exe', 'Qt5Core.dll', 'Qt5Gui.dll', 'Qt5Widgets.dll',
    'Qt5Network.dll', 'Qt5SerialPort.dll', 'Qt5OpenGL.dll', 'Qt5PrintSupport.dll',
    'platforms/qwindows.dll', 'vtkCommonCore-8.2.dll', 'vtkRenderingOpenGL2-8.2.dll',
    'msvcp140.dll', 'vcruntime140.dll', 'data/QSS/MaterialDark-B.qss')) {
    if (-not (Test-Path -LiteralPath (Join-Path $payload $required) -PathType Leaf)) { throw "Required file missing: $required" }
}
if ($ReportPath) {
    $audit | Sort-Object file | ConvertTo-Json -Depth 5 | Set-Content -LiteralPath $ReportPath -Encoding UTF8
}
Write-Host "Runtime audit passed: $($audit.Count) x64 binaries; all non-system dependencies packaged."
