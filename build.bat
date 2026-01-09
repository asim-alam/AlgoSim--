@echo off
g++ engine.cpp Graphics.cpp Grid.cpp Dijkstra.cpp -o simulation.exe -lgdi32
if %errorlevel% neq 0 (
    echo Compilation Failed!
    pause
) else (
    echo Compilation Successful!
    simulation.exe
)
