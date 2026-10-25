[CmdletBinding()]
param(
    [switch] $Check,
    [string[]] $SourcePath = @("src", "ui", "apps", "test"),
    [string] $ClangFormatPath
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$repositoryRoot = [System.IO.Path]::GetFullPath((Join-Path $PSScriptRoot ".."))
$sourceExtensions = @(".c", ".cc", ".cpp", ".cxx", ".h", ".hh", ".hpp", ".hxx")
$excludedDirectoryPattern = "(^|[\\/])(?:\.git|build|lib|Testing|CMakeFiles|_deps|generated)([\\/]|$)"
$excludedFileNames = @("qcustomplot.cpp", "qcustomplot.h")

function Resolve-ClangFormat
{
    param([string] $RequestedPath)

    if ($RequestedPath)
    {
        if (-not (Test-Path -LiteralPath $RequestedPath -PathType Leaf))
        {
            throw "clang-format does not exist: $RequestedPath"
        }
        return (Resolve-Path -LiteralPath $RequestedPath).Path
    }

    $command = Get-Command clang-format.exe -ErrorAction SilentlyContinue
    if (-not $command)
    {
        $command = Get-Command clang-format -ErrorAction SilentlyContinue
    }
    if ($command)
    {
        return $command.Source
    }

    $knownLocations = @(
        "C:\Program Files\LLVM\bin\clang-format.exe",
        "C:\Program Files (x86)\LLVM\bin\clang-format.exe",
        "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\IDE\VC\vcpackages\clang-format.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Tools\Llvm\x64\bin\clang-format.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Tools\Llvm\x64\bin\clang-format.exe",
        "C:\Program Files\Microsoft Visual Studio\2022\Enterprise\VC\Tools\Llvm\x64\bin\clang-format.exe"
    )

    foreach ($candidate in $knownLocations)
    {
        if (Test-Path -LiteralPath $candidate -PathType Leaf)
        {
            return $candidate
        }
    }

    throw "clang-format was not found. Install LLVM or pass -ClangFormatPath explicitly."
}

function Get-SourceFiles
{
    $files = New-Object System.Collections.Generic.List[System.IO.FileInfo]

    foreach ($entry in $SourcePath)
    {
        $candidate = if ([System.IO.Path]::IsPathRooted($entry))
        {
            $entry
        }
        else
        {
            Join-Path $repositoryRoot $entry
        }

        if (-not (Test-Path -LiteralPath $candidate))
        {
            Write-Warning "Source path does not exist and was skipped: $entry"
            continue
        }

        $item = Get-Item -LiteralPath $candidate
        $candidates = if ($item.PSIsContainer)
        {
            Get-ChildItem -LiteralPath $item.FullName -File -Recurse
        }
        else
        {
            @($item)
        }

        foreach ($file in $candidates)
        {
            if ($sourceExtensions -notcontains $file.Extension.ToLowerInvariant())
            {
                continue
            }

            if ($excludedFileNames -contains $file.Name.ToLowerInvariant())
            {
                continue
            }

            $relativePath = $file.FullName.Substring($repositoryRoot.Length).TrimStart('\', '/')
            if ($relativePath -match $excludedDirectoryPattern)
            {
                continue
            }

            $files.Add($file)
        }
    }

    return $files | Sort-Object FullName -Unique
}

function Get-TextFile
{
    param([string] $Path)

    $bytes = [System.IO.File]::ReadAllBytes($Path)
    $offset = 0

    if ($bytes.Length -ge 3 -and $bytes[0] -eq 0xEF -and $bytes[1] -eq 0xBB -and $bytes[2] -eq 0xBF)
    {
        $encoding = New-Object System.Text.UTF8Encoding($true)
        $offset = 3
    }
    elseif ($bytes.Length -ge 2 -and $bytes[0] -eq 0xFF -and $bytes[1] -eq 0xFE)
    {
        $encoding = New-Object System.Text.UnicodeEncoding($false, $true)
        $offset = 2
    }
    elseif ($bytes.Length -ge 2 -and $bytes[0] -eq 0xFE -and $bytes[1] -eq 0xFF)
    {
        $encoding = New-Object System.Text.UnicodeEncoding($true, $true)
        $offset = 2
    }
    else
    {
        $encoding = New-Object System.Text.UTF8Encoding($false, $true)
    }

    return @{
        Text = $encoding.GetString($bytes, $offset, $bytes.Length - $offset)
        Encoding = $encoding
        HasBom = ($offset -gt 0)
    }
}

function Get-DefinitionSpacedText
{
    param([string] $Text)

    # With unindented namespaces, a function's final brace is at column zero.
    # Namespace braces carry comments and records end in "};".
    # Add a blank line only when such a brace is immediately followed by code.
    $pattern = '(?m)^}[ \t]*\r?\n(?=(?![ \t]*(?:}|else\b|catch\b|while\b|#))[^\r\n]*\S)'
    $newline = if ($Text.Contains("`r`n")) { "`r`n" } else { "`n" }
    return [System.Text.RegularExpressions.Regex]::Replace($Text, $pattern, "}$newline$newline")
}

function Set-TextFile
{
    param(
        [string] $Path,
        [string] $Text,
        [System.Text.Encoding] $Encoding,
        [bool] $HasBom
    )

    $contentBytes = $Encoding.GetBytes($Text)
    if ($HasBom)
    {
        $preamble = $Encoding.GetPreamble()
        $outputBytes = New-Object byte[] ($preamble.Length + $contentBytes.Length)
        [System.Array]::Copy($preamble, 0, $outputBytes, 0, $preamble.Length)
        [System.Array]::Copy($contentBytes, 0, $outputBytes, $preamble.Length, $contentBytes.Length)
        [System.IO.File]::WriteAllBytes($Path, $outputBytes)
        return
    }

    [System.IO.File]::WriteAllBytes($Path, $contentBytes)
}

$clangFormat = Resolve-ClangFormat $ClangFormatPath
$files = @(Get-SourceFiles)

if ($files.Count -eq 0)
{
    Write-Host "No C/C++ source files were found."
    exit 0
}

Write-Host "clang-format: $clangFormat"
Write-Host "Source files: $($files.Count)"

if ($Check)
{
    $unformattedFiles = New-Object System.Collections.Generic.List[string]

    foreach ($file in $files)
    {
        $replacements = & $clangFormat -style=file -output-replacements-xml $file.FullName
        if ($LASTEXITCODE -ne 0)
        {
            throw "clang-format failed: $($file.FullName)"
        }

        if (($replacements -join "`n") -match "<replacement ")
        {
            $unformattedFiles.Add($file.FullName)
            continue
        }

        $textFile = Get-TextFile $file.FullName
        if ((Get-DefinitionSpacedText $textFile.Text) -cne $textFile.Text)
        {
            $unformattedFiles.Add($file.FullName)
        }
    }

    if ($unformattedFiles.Count -gt 0)
    {
        Write-Host "Files requiring formatting: $($unformattedFiles.Count)" -ForegroundColor Yellow
        $unformattedFiles | ForEach-Object { Write-Host "  $_" }
        exit 1
    }

    Write-Host "Formatting check passed." -ForegroundColor Green
    exit 0
}

foreach ($file in $files)
{
    & $clangFormat -style=file -i $file.FullName
    if ($LASTEXITCODE -ne 0)
    {
        throw "clang-format failed: $($file.FullName)"
    }

    $textFile = Get-TextFile $file.FullName
    $spacedText = Get-DefinitionSpacedText $textFile.Text
    if ($spacedText -cne $textFile.Text)
    {
        Set-TextFile $file.FullName $spacedText $textFile.Encoding $textFile.HasBom
    }
}

Write-Host "Formatted $($files.Count) source files." -ForegroundColor Green
