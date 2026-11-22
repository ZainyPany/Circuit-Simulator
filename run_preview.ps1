# Builds and runs the hardware GUI preview.  Usage:  .\run_preview.ps1

Set-Location $PSScriptRoot
$env:PATH = "C:\msys64\ucrt64\bin;" + $env:PATH

g++ -std=c++17 -I. (Get-ChildItem hardware\*.cpp).FullName -o preview.exe -lsfml-graphics -lsfml-window -lsfml-system
if ($LASTEXITCODE -eq 0) { .\preview.exe }
