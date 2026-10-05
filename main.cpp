// main.cpp - main menu launcher: 1. Array  2. Singly Linked List  3. Performance Test  4. Exit
// Compile: g++ -std=c++17 -O2 main.cpp -o main
#include "common.h"

//set command and log file path based on OS
#ifdef _WIN32
const char* const ARRAY_CMD = "cd Array && array_program.exe";
const char* const LIST_CMD  = "cd LinkedList && linkedlist_program.exe";
const char* const ARRAY_LOG = "Array/performance_log.txt";
const char* const LIST_LOG  = "LinkedList/performance_log.txt";
#else
const char* const ARRAY_CMD = "cd Array && ./array_program";
const char* const LIST_CMD  = "cd LinkedList && ./linkedlist_program";
const char* const ARRAY_LOG = "Array/performance_log.txt";
const char* const LIST_LOG  = "LinkedList/performance_log.txt";
#endif

int main() {
    //keep showing main menu until user chooses to exit
    while (true) {
        cout << "\n=============================================\n"
                "   METROHEALTH PATIENT DATA - MAIN MENU\n"
                "=============================================\n"
                "1. Array\n2. Singly Linked List\n3. Performance Test\n4. Exit\n";
        int c = askInt("Choice: ", 1, 4);
        if (c == 1) system(ARRAY_CMD); //open array program
        else if (c == 2) system(LIST_CMD); //linked list program
        //display performance test results for 2 DS
        else if (c == 3) {
            cout << "\n================ ARRAY PERFORMANCE RESULTS ================\n";
            showPerformanceLog(ARRAY_LOG);
            cout << "\n============ SINGLY LINKED LIST PERFORMANCE RESULTS ============\n";
            showPerformanceLog(LIST_LOG);
            cout << "\nPress 1 to clear both logs, or 2 to go back: ";
            
            int x = askInt("", 1, 2);

            if (x == 1) {
                //clear both log files by opening them in trunc mode (empty file first)
                ofstream c1(ARRAY_LOG, ios::trunc), c2(LIST_LOG, ios::trunc);
                cout << "Logs cleared.\n";
            }
            //quit loop when user select 4
        } else break;
    }
    cout << "Exiting MetroHealth System...\n";
    return 0;
}
