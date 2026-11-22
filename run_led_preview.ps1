Set-Location $PSScriptRoot
$env:PATH = "C:\msys64\ucrt64\bin;" + $env:PATH

g++ -std=c++17 -I. led_preview.cpp hardware\BreadboardView.cpp hardware\Draw.cpp hardware\LEDView.cpp -o led_preview.exe -lsfml-graphics -lsfml-window -lsfml-system
if ($LASTEXITCODE -eq 0) { .\led_preview.exe }
