[CmdletBinding()]
param(
    [string]$QtDir = 'D:/AppData/Qt/Qt5.12.2/5.12.2/msvc2017_64',
    [string]$IfwDir = '',
    [string]$VtkDir = 'D:/VTK/VTK-release/bin',
    [string]$CrtDir = '',
    [string]$BuildDir = '',
    [string]$OutputDir = ''
)
$ErrorActionPreference = 'Stop'
Import-Module Microsoft.PowerShell.Utility -ErrorAction Stop
. (Join-Path $PSScriptRoot 'runtime_helpers.ps1')
$project = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..'))
if (-not $BuildDir) { $BuildDir = Join-Path $project 'build/installer-release' }
if (-not $OutputDir) { $OutputDir = $PSScriptRoot }
if (-not $IfwDir) {
    $creator = Get-ChildItem -LiteralPath (Join-Path $project 'build/tools/qtifw-4.7') `
        -Filter binarycreator.exe -Recurse -File -ErrorAction SilentlyContinue | Select-Object -First 1
    if ($creator) { $IfwDir = $creator.DirectoryName }
}
if (-not $IfwDir) { throw 'IFW missing; pass -IfwDir or unpack IFW under build/tools/qtifw-4.7' }
foreach ($tool in @((Join-Path $QtDir 'bin/windeployqt.exe'), (Join-Path $IfwDir 'binarycreator.exe'))) {
    if (-not (Test-Path -LiteralPath $tool -PathType Leaf)) { throw "Required build tool missing: $tool" }
}
function Invoke-Checked([string]$Program, [string[]]$Arguments) {
    & $Program @Arguments
    if ($LASTEXITCODE -ne 0) { throw "$Program exited with $LASTEXITCODE" }
}
# Import compiler environment; filesystem operations stay in PowerShell.
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$vsRoot = (& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 `
    -property installationPath | Select-Object -First 1)
if (-not $vsRoot) { throw 'Visual Studio C++ tools not found' }
$devCmd = Join-Path $vsRoot 'Common7/Tools/VsDevCmd.bat'
$environment = & cmd.exe /d /s /c "`"$devCmd`" -arch=x64 -host_arch=x64 -vcvars_ver=14.16 >nul && set"
if ($LASTEXITCODE -ne 0) { throw 'Cannot initialize MSVC v141 x64 environment' }
foreach ($line in $environment) {
    if ($line -match '^([^=]+)=(.*)$' -and $Matches[1] -ine 'PSModulePath') {
        [Environment]::SetEnvironmentVariable($Matches[1], $Matches[2], 'Process')
    }
}
$env:PATH = (Join-Path $QtDir 'bin') + ';' + $env:PATH
if (-not $CrtDir) {
    $redist = Get-ChildItem -LiteralPath (Join-Path $vsRoot 'VC/Redist/MSVC') -Directory |
        Where-Object Name -like '14.16.*' | Sort-Object Name -Descending | Select-Object -First 1
    if (-not $redist) { throw 'MSVC v141 redistributable not found; pass -CrtDir' }
    $CrtDir = Join-Path $redist.FullName 'x64/Microsoft.VC141.CRT'
}
if (-not (Test-Path -LiteralPath $CrtDir -PathType Container)) { throw "CRT redist missing: $CrtDir" }
if (-not (Test-Path -LiteralPath $VtkDir -PathType Container)) { throw "VTK DLL directory missing: $VtkDir" }

Write-Host '[1/5] Build latest Release sources'
Invoke-Checked 'cmake.exe' @('-S', $project, '-B', $BuildDir, '-G', 'NMake Makefiles',
    '-DCMAKE_BUILD_TYPE=Release', '-DBUILD_TESTING=OFF', "-DCMAKE_PREFIX_PATH=$QtDir")
Invoke-Checked 'cmake.exe' @('--build', $BuildDir, '--config', 'Release')
$exe = Join-Path $BuildDir 'bin/release/FOSD-Pro.exe'
if (-not (Test-Path -LiteralPath $exe)) { throw "Release EXE missing: $exe" }

Write-Host '[2/5] Stage application and data'
$package = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot 'packages/com.fosd.pro/data'))
# Only this fixed generated directory may be removed recursively.
$expected = [IO.Path]::GetFullPath((Join-Path $project 'installer/packages/com.fosd.pro/data'))
if ($package -ne $expected) { throw 'Unsafe staging path' }
if (Test-Path -LiteralPath $package) { Remove-Item -LiteralPath $package -Recurse -Force }
New-Item -ItemType Directory -Path $package -Force | Out-Null
Copy-Item -LiteralPath $exe -Destination $package
if (Test-Path -LiteralPath ($exe + '.manifest')) { Copy-Item -LiteralPath ($exe + '.manifest') -Destination $package }
Copy-Item -LiteralPath (Join-Path $project 'data') -Destination $package -Recurse
foreach ($generated in @('outputs', 'packet_log.csv')) {
    $path = [IO.Path]::GetFullPath((Join-Path $package "data/$generated"))
    if (-not $path.StartsWith($package + [IO.Path]::DirectorySeparatorChar, [StringComparison]::OrdinalIgnoreCase)) {
        throw 'Unsafe generated-data path'
    }
    if (Test-Path -LiteralPath $path) { Remove-Item -LiteralPath $path -Recurse -Force }
}
Copy-Item -LiteralPath (Join-Path $project 'scripts/send_wavelength_packets.py') -Destination $package
Copy-Item -LiteralPath (Join-Path $project 'doc/wavelength-protocol.md') -Destination $package
foreach ($dll in Get-ChildItem -LiteralPath $CrtDir -Filter '*.dll' -File) {
    Copy-Item -LiteralPath $dll.FullName -Destination $package
}

Write-Host '[3/5] Deploy Qt runtime, plugins, ANGLE and software OpenGL'
Invoke-Checked (Join-Path $QtDir 'bin/windeployqt.exe') @('--release', '--force',
    '--no-translations', '--no-compiler-runtime', '--angle', '-opengl', '-printsupport', '-svg',
    '--dir', $package, (Join-Path $package 'FOSD-Pro.exe'))

Write-Host '[4/5] Resolve and verify DLL dependencies, including VTK'
New-Item -ItemType Directory -Path $OutputDir -Force | Out-Null
& (Join-Path $PSScriptRoot 'verify_runtime.ps1') -PayloadDir $package -Repair `
    -DependencyDirs @($VtkDir, (Join-Path $QtDir 'bin'), $CrtDir) `
    -ReportPath (Join-Path $OutputDir 'FOSD-Pro-runtime-audit.json')
$payloadHash = Get-PayloadHash (Join-Path $package 'FOSD-Pro.exe')
if ($payloadHash -ne (Get-PayloadHash $exe)) { throw 'Staged EXE mismatch' }
$licenses = Join-Path $package 'licenses'
New-Item -ItemType Directory -Path $licenses -Force | Out-Null
foreach ($name in @('LICENSE.LGPL3', 'LICENSE.GPL3', 'LICENSE.GPL3-EXCEPT')) {
    foreach ($root in @($QtDir, (Join-Path (Split-Path $QtDir -Parent) 'Src/qtbase'))) {
        $source = Join-Path $root $name
        if (Test-Path -LiteralPath $source) { Copy-Item -LiteralPath $source -Destination $licenses; break }
    }
}
$vtkLicense = Join-Path $project 'lib/VTK/VTK-release/share/doc/vtk-8.2/copyright'
if (-not (Test-Path -LiteralPath $vtkLicense)) {
    $vtkLicense = Join-Path (Split-Path (Split-Path $VtkDir -Parent) -Parent) 'VTK-8.2.0/VTK-8.2.0/Copyright.txt'
}
if (Test-Path -LiteralPath $vtkLicense) { Copy-Item -LiteralPath $vtkLicense -Destination (Join-Path $licenses 'VTK.txt') }

Write-Host '[5/5] Create offline installer'
$configPath = Join-Path $PSScriptRoot 'config/config.xml'
$metadataPath = Join-Path $PSScriptRoot 'packages/com.fosd.pro/meta/package.xml'
[xml]$config = Get-Content -LiteralPath $configPath -Raw -Encoding UTF8
[xml]$metadata = Get-Content -LiteralPath $metadataPath -Raw -Encoding UTF8
if ($config.Installer.Version -ne $metadata.Package.Version) { throw 'Installer/package version mismatch' }
Copy-Item -LiteralPath (Join-Path $project 'res/icon/favicon.ico') -Destination (Join-Path $PSScriptRoot 'config/installer_icon.ico')
$output = Join-Path $OutputDir 'FOSD-Pro-Setup.exe'
Invoke-Checked (Join-Path $IfwDir 'binarycreator.exe') @('--offline-only', '-c', $configPath,
    '-p', (Join-Path $PSScriptRoot 'packages'), $output)
$installerHash = Get-PayloadHash $output
"$installerHash  FOSD-Pro-Setup.exe" | Set-Content -LiteralPath ($output + '.sha256') -Encoding ASCII
$manifest = [ordered]@{
    version = [string]$config.Installer.Version
    release_date = [string]$metadata.Package.ReleaseDate
    source_commit = ((& git.exe -C $project rev-parse HEAD) -join '').Trim()
    application_sha256 = $payloadHash
    installer_sha256 = $installerHash
    installer_bytes = (Get-Item -LiteralPath $output).Length
    payload_files = @(Get-ChildItem -LiteralPath $package -Recurse -File).Count
    payload_dlls = @(Get-ChildItem -LiteralPath $package -Recurse -Filter '*.dll' -File).Count
    required_os = 'Windows 10/11 x64'
}
$manifest | ConvertTo-Json | Set-Content -LiteralPath (Join-Path $OutputDir 'FOSD-Pro-build-manifest.json') -Encoding UTF8
Write-Host "Installer ready: $output"
Write-Host "SHA256: $installerHash"
