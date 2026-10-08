# MetroHealth Patient Data System

This project compares patient-data operations implemented with:

- a dynamic array
- a singly linked list

The root program provides one launcher for both implementations and displays
their performance logs.

## Requirements

For Windows, install a C++ compiler that provides `g++` (for example,
MinGW-w64 or MSYS2). Make sure `g++` is added to the system `PATH`.

Verify the installation in **Command Prompt** or **PowerShell**:

```text
g++ --version
```

The compiler must support C++17.

## Files required

Keep the folder structure unchanged after extracting the ZIP:

```text
backup-dstr/
|-- main.cpp
|-- common.h
|-- Array/
|   |-- array_*.cpp
|   |-- array_module.h
|   |-- dataset1_facility_a.csv
|   |-- dataset2_facility_b.csv
|   |-- dataset3_facility_c.csv
|-- LinkedList/
    |-- linkedlist_*.cpp
    |-- linkedlist_module.h
    |-- dataset1_facility_a.csv
    |-- dataset2_facility_b.csv
    |-- dataset3_facility_c.csv
```

The CSV files must remain in their respective `Array` and `LinkedList`
folders. Do not rename the folders, because the launcher uses these paths.

## Recommended Windows setup and run

1. Extract the ZIP file.
2. Open PowerShell or Command Prompt.
3. Change to the extracted project folder (the folder containing `main.cpp`):

   ```text
   cd C:\path\to\backup-dstr
   ```

4. Build the Array program:

   ```text
   cd Array
   build_array.bat
   cd ..
   ```

5. Build the Singly Linked List program:

   ```text
   cd LinkedList
   build_linkedlist.bat
   cd ..
   ```

6. Build the main launcher:

   ```text
   g++ -std=c++17 -O2 main.cpp -o main.exe
   ```

7. Run the launcher **from the project root**:

   ```text
   .\main.exe
   ```

   Running it from the root folder is important because the launcher changes
   into `Array` and `LinkedList` and reads the performance logs using relative
   paths.

## Alternative: build without the batch files

Run these commands from the project root:

```text
g++ -std=c++17 -O2 Array\array_program.cpp Array\array_data.cpp Array\array_sort.cpp Array\array_search.cpp Array\array_update.cpp Array\array_analysis.cpp -o Array\array_program.exe

g++ -std=c++17 -O2 LinkedList\linkedlist_program.cpp LinkedList\linkedlist_data.cpp LinkedList\linkedlist_sort.cpp LinkedList\linkedlist_search.cpp LinkedList\linkedlist_update.cpp LinkedList\linkedlist_analysis.cpp -o LinkedList\linkedlist_program.exe

g++ -std=c++17 -O2 main.cpp -o main.exe
```

## Using the program

From the main menu:

1. **Array** opens the array implementation.
2. **Singly Linked List** opens the linked-list implementation.
3. **Performance Test** displays the results saved in:
   - `Array\performance_log.txt`
   - `LinkedList\performance_log.txt`
4. **Exit** closes the launcher.

The performance log files are updated when sorting or searching benchmarks
are run. Choosing the clear option in the Performance Test menu empties both
log files.

## Troubleshooting

- **`g++` is not recognized**: install MinGW-w64/MSYS2 and add its `bin`
  directory to `PATH`, then reopen the terminal.
- **CSV files cannot be loaded**: confirm that all three CSV files are in
  both `Array` and `LinkedList`.
- **The launcher cannot open a subprogram**: confirm that both
  `Array\array_program.exe` and `LinkedList\linkedlist_program.exe` were
  built successfully, and run `main.exe` from the project root.
- **Permission or antivirus warning**: compile the programs locally rather
  than relying on the precompiled `.exe` files included in the ZIP.

## Instructions for the lecturer

Please extract the ZIP while preserving the folder structure, install a
C++17 compiler with `g++`, build the two subprograms and the launcher using
the steps above, and run `main.exe` from the extracted `backup-dstr` root
folder.
