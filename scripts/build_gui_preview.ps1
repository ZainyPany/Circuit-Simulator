# Builds the hardware GUI preview into build\gui_preview.exe and runs it.
#
#   .\scripts\build_gui_preview.ps1                  build and open the window
#   .\scripts\build_gui_preview.ps1 -Picture a.png   build and save a picture instead
#   .\scripts\build_gui_preview.ps1 -NoRun           only build
#
# Needs SFML 3 from MSYS2 UCRT64 (see docs\sfml-guide).

param(
    [string]$Picture = "",
    [switch]$NoRun
)

$root = Split-Path $PSScriptRoot -Parent
$env:PATH = "C:\msys64\ucrt64\bin;" + $env:PATH

New-Item -ItemType Directory -Force "$root\build" | Out-Null

$sources = @("$root\tools\gui_preview.cpp") + (Get-ChildItem "$root\ui\hardware\*.cpp").FullName

g++ -std=c++17 -Wall -I"$root" $sources -o "$root\build\gui_preview.exe" `
    -lsfml-graphics -lsfml-window -lsfml-system
if ($LASTEXITCODE -ne 0) { exit $LASTEXITCODE }

if (-not $NoRun) {
    if ($Picture -ne "") { & "$root\build\gui_preview.exe" $Picture }
    else { & "$root\build\gui_preview.exe" }
}
