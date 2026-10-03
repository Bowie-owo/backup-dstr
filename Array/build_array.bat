@echo off
g++ -std=c++17 -O2 array_program.cpp array_data.cpp array_sort.cpp array_search.cpp array_update.cpp array_analysis.cpp -o array_program.exe
if errorlevel 1 (
    echo Array build failed.
    exit /b 1
)
echo Array build succeeded.
