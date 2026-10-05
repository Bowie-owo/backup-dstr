#include "linkedlist_module.h"

// Make sure a sorted copy is available for the update experiment.
static void ensureSortedForUpdate(int d, int key, PatientList& work) {

    if (!hasSorted[d][key]) {

        cout << "(No saved sorted copy for " << DS_NAMES[d]
             << " by " << KEY_NAMES[key]
             << " - building one with insertion sort, not timed)\n";

        // Create a sorted copy of the original dataset.
        sortedData[d][key].copyFrom(orig[d]);

        long c = 0;

        // Sort the copy before performing the update operation.
        insertionSort(sortedData[d][key], key, c);

        hasSorted[d][key] = true;
    }

    // Use the sorted data for the experiment.
    work.copyFrom(sortedData[d][key]);
}

void insertDeleteMenu() {

    while (true) {

        cout << "\n--- LINKED LIST INSERT / DELETE (temporary experiments) ---\n"
                "1. Insert into UNSORTED data\n"
                "2. Insert into SORTED data\n"
                "3. Delete from UNSORTED data\n"
                "4. Delete from SORTED data\n"
                "5. Back\n";

        int c = askInt("Choice: ", 1, 5);

        // Return to the previous menu.
        if (c == 5)
            return;

        int d = pickDataset(false);
        int key = pickKey();

        // Options 2 and 4 use sorted data.
        bool sortedMode = (c == 2 || c == 4);

        PatientList work;

        // Prepare either sorted or unsorted data for the test.
        if (sortedMode)
            ensureSortedForUpdate(d, key, work);
        else
            work.copyFrom(orig[d]);

        int before = work.size;
        int pos = -1;
        long comps = 0;
        double us;
        bool ok = true;

        // Perform an insertion operation.
        if (c <= 2) {

            Patient np = askNewPatient();

            // Start timing the insertion operation.
            auto s = Clock::now();

            // Find the insertion position for sorted data.
            // For unsorted data, add the record at the end.
            pos = sortedMode ? lowerBound(work, key, np, comps)
                             : work.size;

            work.insertAt(pos, np);

            // Stop timing the insertion.
            auto e = Clock::now();
            us = elapsedUs(s, e);

        } else {

            // Ask the user which record should be deleted.
            Patient probe = askProbe(key);

            // Start timing the deletion operation.
            auto s = Clock::now();

            if (sortedMode) {

                // Use the sorted data to find the possible record position.
                int lb = lowerBound(work, key, probe, comps);
                comps++;

                // Check whether the record at this position matches.
                if (lb < work.size &&
                    matchAt(work, lb, key, probe))
                    pos = lb;

            } else {

                // Search from the beginning for the record.
                pos = findFirst(work, key, probe, comps);
            }

            // Delete the record if it was found.
            if (pos >= 0)
                work.removeAt(pos);
            else
                ok = false;

            // Stop timing the deletion.
            auto e = Clock::now();
            us = elapsedUs(s, e);
        }

        // Set the operation name and search method for the log.
        const char* op = (c <= 2) ? "Insert" : "Delete";

        const char* algo = (c == 1) ? "Append"
                         : (c == 2) ? "Ordered"
                         : (c == 3) ? "Linear"
                         : "Binary";

        // Save the operation's performance results.
        logResult("LinkedList", DS_NAMES[d], op, algo,
                  KEY_NAMES[key],
                  sortedMode ? "Sorted" : "Unsorted",
                  us, comps, work.memoryBytes());

        // Display the result of the operation.
        printUpdateReport(op, ok, before, work.size, pos,
                          us, comps, work.memoryBytes());
    }
}