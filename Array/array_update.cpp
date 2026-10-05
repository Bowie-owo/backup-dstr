#include "array_module.h"

//check sorted copy is available for the insert/delete testing
static void ensureSortedForUpdate(int d, int key, PatientArray& work) {

    if (!hasSorted[d][key]) {

        cout << "(No saved sorted copy for " << DS_NAMES[d]
             << " by " << KEY_NAMES[key]
             << " - building one with insertion sort, not timed)\n";

        //create copy
        sortedData[d][key].copyFrom(orig[d]);

        long c = 0;

        //sort copy b4 insert
        insertionSort(sortedData[d][key], key, c);

        hasSorted[d][key] = true;
    }

    //use sorted data for experiment
    work.copyFrom(sortedData[d][key]);
}

//**Delete**
void insertDeleteMenu() {

    while (true) {

        cout << "\n--- ARRAY INSERT / DELETE (temporary experiments) ---\n"
                "1. Insert into UNSORTED data\n"
                "2. Insert into SORTED data\n"
                "3. Delete from UNSORTED data\n"
                "4. Delete from SORTED data\n"
                "5. Back\n";

        int c = askInt("Choice: ", 1, 5);

        if (c == 5)
            return;

        int d = pickDataset(false);
        int key = pickKey();

        //2,4 use sorted data
        bool sortedMode = (c == 2 || c == 4);

        PatientArray work;

        //sorted/not sorted data
        if (sortedMode)
            ensureSortedForUpdate(d, key, work);
        else
            work.copyFrom(orig[d]);

        int before = work.size;
        int pos = -1;
        long comps = 0;
        double us;
        bool ok = true;

        //Insertion op
        if (c <= 2) {

            Patient np = askNewPatient();

            //start clock
            auto s = Clock::now();

            //sorted data uses *binary search* to find insertion position
            //unsorted data adds record at the end
            pos = sortedMode ? lowerBound(work, key, np, comps)
                             : work.size;

            work.insertAt(pos, np);

            // Stop clock
            auto e = Clock::now();
            us = elapsedUs(s, e);

        } else {

            //prompt record to delete based on key
            Patient probe = askProbe(key);

            //record deletion operation
            auto s = Clock::now();

            if (sortedMode) {

                //binary search find the record in sorted data
                int lb = lowerBound(work, key, probe, comps);
                comps++;

                if (lb < work.size &&
                    cmpKey(work.data[lb], probe, key) == 0)
                    pos = lb;

            } else {

                //search from beginning for record in unsorted data
                pos = findFirst(work, key, probe, comps);
            }

            //ddelete the record if it was found
            if (pos >= 0)
                work.removeAt(pos);
            else
                ok = false;

            // Stop timing deletion
            auto e = Clock::now();
            us = elapsedUs(s, e);
        }

        // Set operation, method names for performance log
        const char* op = (c <= 2) ? "Insert" : "Delete";

        const char* algo = (c == 1) ? "Append"
                         : (c == 2) ? "Ordered"
                         : (c == 3) ? "Linear"
                         : "Binary";

        //save performance results
        logResult("Array", DS_NAMES[d], op, algo, KEY_NAMES[key],
                  sortedMode ? "Sorted" : "Unsorted",
                  us, comps, work.memoryBytes());

        printUpdateReport(op, ok, before, work.size, pos,
                          us, comps, work.memoryBytes());
    }
}