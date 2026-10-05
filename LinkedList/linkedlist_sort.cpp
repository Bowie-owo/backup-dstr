#include "linkedlist_module.h"

// Swap patient data between two nodes
static void swapData(Node* a, Node* b) {
    Patient t = a->data;
    a->data = b->data;
    b->data = t;
}

// Sort linked list using insertion sort
void insertionSort(PatientList& L, int key, long& comps) {
    Node* sortedHead = 0;
    Node* cur = L.head;

    while (cur) {
        Node* nxt = cur->next;
        bool atFront = !sortedHead;

        if (sortedHead) {
            comps++;
            if (cmpKey(sortedHead->data, cur->data, key) > 0)
                atFront = true;
        }

        if (atFront) {
            cur->next = sortedHead;
            sortedHead = cur;
        } else {
            Node* s = sortedHead;

            // Find the correct position in the sorted part
            while (s->next) {
                comps++;
                if (cmpKey(s->next->data, cur->data, key) > 0)
                    break;
                s = s->next;
            }

            cur->next = s->next;
            s->next = cur;
        }

        cur = nxt;
    }

    L.head = sortedHead;
    L.tail = L.head;

    // Update tail after sorting
    while (L.tail && L.tail->next)
        L.tail = L.tail->next;
}

// Sort linked list using bubble sort
void bubbleSort(PatientList& L, int key, long& comps) {
    if (L.size < 2)
        return;

    Node* lastSorted = 0;
    bool swapped;

    do {
        swapped = false;
        Node* c = L.head;

        // Compare neighbouring nodes
        while (c->next != lastSorted) {
            comps++;
            if (cmpKey(c->data, c->next->data, key) > 0) {
                swapData(c, c->next);
                swapped = true;
            }
            c = c->next;
        }

        lastSorted = c;
    } while (swapped);
}

// Sort linked list using selection sort
void selectionSort(PatientList& L, int key, long& comps) {
    for (Node* i = L.head; i && i->next; i = i->next) {
        Node* m = i;

        // Find the smallest record in the remaining list
        for (Node* j = i->next; j; j = j->next) {
            comps++;
            if (cmpKey(j->data, m->data, key) < 0)
                m = j;
        }

        if (m != i)
            swapData(i, m);
    }
}

// Run the selected sorting algorithm
void runSort(PatientList& L, int algo, int key, long& comps) {
    if (algo == 0)
        insertionSort(L, key, comps);
    else if (algo == 1)
        bubbleSort(L, key, comps);
    else
        selectionSort(L, key, comps);
}

// Run sorting and record performance
static void doSort(int d, int key, int algo, bool show) {
    PatientList work;

    // Copy original data to keep it unchanged
    work.copyFrom(orig[d]);

    long comps = 0;
    auto s = Clock::now();

    runSort(work, algo, key, comps);

    auto e = Clock::now();
    double us = elapsedUs(s, e);

    // Save sorting performance results
    logResult("LinkedList", DS_NAMES[d], "Sort", ALGO_NAMES[algo],
              KEY_NAMES[key], "Unsorted", us, comps, work.memoryBytes());

    // Save sorted data for later use
    sortedData[d][key].copyFrom(work);
    hasSorted[d][key] = true;

    printSortRow(d, key, algo, work.size, us, comps, work.memoryBytes());

    if (show) {
        cout << "\nAll records after sorting by " << KEY_NAMES[key] << ":\n";
        printPatientHeader();

        // Display sorted records
        for (Node* c = work.head; c; c = c->next)
            printPatientRow(c->data);
    }
}

// Display linked list sorting menu
void sortingMenu() {
    while (true) {
        cout << "\n--- LINKED LIST SORTING ---\n"
                "1. Sort one dataset (choose field + algorithm)\n"
                "2. Full benchmark (4 datasets x 3 fields x 3 algorithms)\n"
                "3. Back\n";

        int c = askInt("Choice: ", 1, 3);

        if (c == 3)
            return;

        if (c == 1) {
            int d = pickDataset(false);
            int key = pickKey();
            int algo = pickAlgo();

            cout << "\n";
            printSortHeader();
            doSort(d, key, algo, true);
        } else {
            // Clear previous benchmark results
            ofstream clearLog(LOG_FILE, ios::trunc);

            cout << "\n";
            printSortHeader();

            // Run all sorting combinations
            for (int d = 0; d < 4; d++)
                for (int k = 0; k < 3; k++)
                    for (int a = 0; a < 3; a++)
                        doSort(d, k, a, false);

            cout << "\nAll results saved to " << LOG_FILE << ".\n";
        }
    }
}