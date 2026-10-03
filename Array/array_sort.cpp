#include "array_module.h"
void insertionSort(PatientArray& a, int key, long& comps) {
    for (int i = 1; i < a.size; i++) { Patient t = a.data[i]; int j = i - 1;
        while (j >= 0) { comps++; 
            if (cmpKey(a.data[j], t, key) > 0) { a.data[j + 1] = a.data[j]; j--; } else break; }
        a.data[j + 1] = t;
    }
}
void bubbleSort(PatientArray& a, int key, long& comps) {
    for (int i = 0; i < a.size - 1; i++) { bool swapped = false;
        for (int j = 0; j < a.size - 1 - i; j++) { comps++; 
            if (cmpKey(a.data[j], a.data[j + 1], key) > 0) {
            Patient t = a.data[j]; a.data[j] = a.data[j + 1]; a.data[j + 1] = t; swapped = true; } }
        if (!swapped) break;
    }
}
void selectionSort(PatientArray& a, int key, long& comps) {
    for (int i = 0; i < a.size - 1; i++) { int m = i;
        for (int j = i + 1; j < a.size; j++) { comps++; 
            if (cmpKey(a.data[j], a.data[m], key) < 0) m = j; }
        if (m != i) { Patient t = a.data[i]; a.data[i] = a.data[m]; a.data[m] = t; }
    }
}
void runSort(PatientArray& a, int algo, int key, long& comps) {
    if (algo == 0) insertionSort(a, key, comps); else if (algo == 1) bubbleSort(a, key, comps); else selectionSort(a, key, comps);
}
static void doSort(int d, int key, int algo, bool show) {
    PatientArray work; work.copyFrom(orig[d]); long comps = 0; auto s = Clock::now(); runSort(work, algo, key, comps); auto e = Clock::now();
    double us = elapsedUs(s, e); logResult("Array", DS_NAMES[d], "Sort", ALGO_NAMES[algo], KEY_NAMES[key], "Unsorted", us, comps, work.memoryBytes());
    sortedData[d][key].copyFrom(work); hasSorted[d][key] = true; printSortRow(d, key, algo, work.size, us, comps, work.memoryBytes());
    if (show) { cout << "\nAll records after sorting by " << KEY_NAMES[key] << ":\n"; printPatientHeader(); for (int i = 0; i < work.size; i++) printPatientRow(work.data[i]); }
}
void sortingMenu() {
    while (true) {
        cout << "\n--- ARRAY SORTING ---\n1. Sort one dataset (choose field + algorithm)\n2. Full benchmark (4 datasets x 3 fields x 3 algorithms)\n3. Back\n";
        int c = askInt("Choice: ", 1, 3); if (c == 3) return;
        if (c == 1) { int d = pickDataset(false), key = pickKey(), algo = pickAlgo(); cout << "\n"; printSortHeader(); doSort(d, key, algo, true); }
        else { cout << "\n"; printSortHeader(); for (int d = 0; d < 4; d++) for (int k = 0; k < 3; k++) for (int a = 0; a < 3; a++) doSort(d, k, a, false); cout << "\nAll results saved to " << LOG_FILE << ".\n"; }
    }
}
