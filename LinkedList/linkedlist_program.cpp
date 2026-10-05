// Singly linked-list implementation entry point

// Compile: run build_linkedlist.bat, or use:
// g++ -std=c++17 -O2 linkedlist_program.cpp linkedlist_data.cpp linkedlist_sort.cpp linkedlist_search.cpp linkedlist_update.cpp linkedlist_analysis.cpp -o linkedlist_program.exe

#include "linkedlist_module.h"

int main() {

    // Load all datasets before starting the system
    if (!loadAll()) {
        cout << "Make sure the three dataset CSV files are in the same folder as the .exe.\n";
        return 1;
    }

    // Keep displaying the main menu until exit
    while (true) {

        cout << "\n====== SINGLY LINKED LIST IMPLEMENTATION ======\n1. Sorting\n2. Searching\n"
                "3. Insert / Delete experiments\n4. Analysis\n5. Back / Exit\n";

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