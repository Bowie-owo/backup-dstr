#include "array_module.h"

// **Insertion Sort**
void insertionSort(PatientArray& a, int key, long& comps) {

    //Start from the 2nd record
    //insert to correct position
    for (int i = 1; i < a.size; i++) {

        Patient t = a.data[i];
        int j = i - 1;

        // Move larger record to the right until in correct pos
        while (j >= 0) {
            comps++;

            if (cmpKey(a.data[j], t, key) > 0) {
                a.data[j + 1] = a.data[j];
                j--;
            } else
                break;
        }

        // Place the current record in correct position
        a.data[j + 1] = t;
    }
}

// **Bubble Sort**
void bubbleSort(PatientArray& a, int key, long& comps) {

    // Repeat the passes until array sorted
    for (int i = 0; i < a.size - 1; i++) {

        bool swapped = false;

        //compare left right records
        //swap them if in the wrong order
        for (int j = 0; j < a.size - 1 - i; j++) {
            comps++;

            if (cmpKey(a.data[j], a.data[j + 1], key) > 0) {

                Patient t = a.data[j];
                a.data[j] = a.data[j + 1];
                a.data[j + 1] = t;

                swapped = true;
            }
        }

        // Stop early if no records were swap
        if (!swapped)
            break;
    }
}

// **Selection Sort**
void selectionSort(PatientArray& a, int key, long& comps) {

    // Find smallest record for each position
    for (int i = 0; i < a.size - 1; i++) {

        int m = i;

        //search remaining records for smallest value
        for (int j = i + 1; j < a.size; j++) {
            comps++;

            if (cmpKey(a.data[j], a.data[m], key) < 0)
                m = j;
        }

        //swap smallest record into current position
        if (m != i) {
            Patient t = a.data[i];
            a.data[i] = a.data[m];
            a.data[m] = t;
        }
    }
}

// Run the sorting algorithm
void runSort(PatientArray& a, int algo, int key, long& comps) {

    if (algo == 0)
        insertionSort(a, key, comps);
    else if (algo == 1)
        bubbleSort(a, key, comps);
    else
        selectionSort(a, key, comps);
}

//sort dataset
//record sorting performance
static void doSort(int d, int key, int algo, bool show) {

    //make a copy so the original dataset is not changed
    PatientArray work;
    work.copyFrom(orig[d]);

    long comps = 0;

    //start clock before the sorting operation
    auto s = Clock::now();

    runSort(work, algo, key, comps);

    //stop timing
    auto e = Clock::now();

    double us = elapsedUs(s, e);

    //save sorting performance to the performance_log
    logResult("Array", DS_NAMES[d], "Sort", ALGO_NAMES[algo],
              KEY_NAMES[key], "Unsorted", us, comps, work.memoryBytes());

    //save sorted data for later searching (copy)
    sortedData[d][key].copyFrom(work);
    hasSorted[d][key] = true;

    //display sorting performance result
    printSortRow(d, key, algo, work.size, us, comps, work.memoryBytes());

    //display all records
    if (show) {
        cout << "\nAll records after sorting by " << KEY_NAMES[key] << ":\n";

        printPatientHeader();

        for (int i = 0; i < work.size; i++)
            printPatientRow(work.data[i]);
    }
}

//sorting menu
void sortingMenu() {

    while (true) {

        cout << "\n--- ARRAY SORTING ---\n"
                "1. Sort one dataset (choose field + algorithm)\n"
                "2. Full benchmark (4 datasets x 3 fields x 3 algorithms)\n"
                "3. Back\n";

        int c = askInt("Choice: ", 1, 3);

        if (c == 3)
            return;

        if (c == 1) {

            //choose dataset/field /sorting algorithm
            int d = pickDataset(false);
            int key = pickKey();
            int algo = pickAlgo();

            cout << "\n";
            printSortHeader();

            //sort selected dataset then show result
            doSort(d, key, algo, true);

        } else {

            //clear the previous benchmark b4 new test
            ofstream clearLog(LOG_FILE, ios::trunc);

            cout << "\n";
            printSortHeader();

            //test 4 datasets x 3 fields x3 sorting
            for (int d = 0; d < 4; d++)
                for (int k = 0; k < 3; k++)
                    for (int a = 0; a < 3; a++)
                        doSort(d, k, a, false);

            cout << "\nAll results saved to " << LOG_FILE << ".\n";
        }
    }
}