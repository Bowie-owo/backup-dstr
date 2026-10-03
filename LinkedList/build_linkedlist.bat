@echo off
g++ -std=c++17 -O2 linkedlist_program.cpp linkedlist_data.cpp linkedlist_sort.cpp linkedlist_search.cpp linkedlist_update.cpp linkedlist_analysis.cpp -o linkedlist_program.exe
if errorlevel 1 (
    echo Linked-list build failed.
    exit /b 1
)
echo Linked-list build succeeded.
