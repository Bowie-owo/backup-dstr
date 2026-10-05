// ARRAY implementation entry point.

// Compile: run build_array.bat, or use:
// g++ -std=c++17 -O2 array_program.cpp array_data.cpp array_sort.cpp array_search.cpp array_update.cpp array_analysis.cpp -o array_program.exe

#include "array_module.h"

int main() {

    //Load all three datasets before starting Array
    if (!loadAll()) {

        cout << "Make sure the three dataset CSV files are in the same folder as the .exe.\n"; //err msg

        return 1;
    }

    while (true) {

        cout << "\n========== ARRAY IMPLEMENTATION ==========\n"
                "1. Sorting\n"
                "2. Searching\n"
                "3. Insert / Delete experiments\n"
                "4. Analysis\n"
                "5. Back / Exit\n";

        int c = askInt("Choice: ", 1, 5);

        if (c == 1)
            sortingMenu();

        else if (c == 2)
            searchMenu();

        else if (c == 3)
            insertDeleteMenu();

        else if (c == 4)
            analysisMenu(buildAnalytics);

        else
            break;
    }

    return 0;
}