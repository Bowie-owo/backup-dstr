#include "array_module.h"
//sinyi
//compare patient record with selected search range
static int rangeCompare(const Patient& p, const SearchRange& r) {

    if (r.key == 0)
        return p.age < r.ageMin ? -1 : (p.age > r.ageMax ? 1 : 0);

        if (r.key == 1)
        return p.los < r.losMin ? -1 : (p.los > r.losMax ? 1 : 0);

        return strcmp(p.care, r.care);
}

//create temp patient record using staring value of search range
static Patient rangeProbe(const SearchRange& r) {
    Patient p;
    memset(&p, 0, sizeof(p));

    if (r.key == 0) p.age = r.ageMin;
    else if (r.key == 1) p.los = r.losMin;
    else strcpy(p.care, r.care);

    return p;
}

//search through array one record one time
int linearSearch(const PatientArray& a, const SearchRange& r, bool isSorted, long& comps, Patient* out, int maxShow) {

    int found = 0;

    for (int i = 0; i < a.size; i++) { //check each record until end of array/ pass range
        comps++;

        int cmp = rangeCompare(a.data[i], r);

        if (matchesSearchRange(a.data[i], r)){

            if (found < maxShow)
                out[found] = a.data[i];

            found++;
            //stop early if datat exceed search range
        } else if (isSorted && cmp > 0)
            break;
     }

     return found;
}

//find first position where search value can be insert
int lowerBound(const PatientArray& a, int key, const Patient& probe, long& comps) {

    int lo = 0, hi = a.size;

    //use binary search find starting position
    while (lo < hi){

        int mid = (lo + hi) / 2;
        comps++;

        if (cmpKey(a.data[mid], probe, key) < 0)
            lo = mid + 1;
        else
            hi = mid;
    }

    return lo;
}

// ***binary search*** on sorted data and find all matching records.
int binarySearch(const PatientArray& a, const SearchRange& r,
                 long& comps, Patient* out, int maxShow) {

    Patient low = rangeProbe(r);

    // Find first possible matching position
    int i = lowerBound(a, r.key, low, comps);
    int found = 0;

    // Check records from the starting position until passed range
    while (i < a.size) {

        comps++;

        if (rangeCompare(a.data[i], r) > 0)
            break;

        if (matchesSearchRange(a.data[i], r)) {

            if (found < maxShow)
                out[found] = a.data[i];

            found++;
        }

        i++;
    }

    return found;
}

// Find first matching record using a linear scan
int findFirst(const PatientArray& a, int key, const Patient& probe, long& comps) {

    for (int i = 0; i < a.size; i++) {

        comps++;

        if (cmpKey(a.data[i], probe, key) == 0)
            return i;
    }

    return -1;
}

// Create and save a sorted copy if one does not already exist
static void ensureSorted(int d, int key) {

    if (hasSorted[d][key])
        return;

    cout << "(No saved sorted copy for " << DS_NAMES[d]
         << " by " << KEY_NAMES[key]
         << " - building one with insertion sort, not timed)\n";

    // Copy the original data before sorting it
    sortedData[d][key].copyFrom(orig[d]);

    // Sort the copied data - insertion sort
    long c = 0;
    insertionSort(sortedData[d][key], key, c);

    hasSorted[d][key] = true;
}

// Ask the user choose search range
static SearchRange chooseRange(const PatientArray& data,
                               const char* datasetName, int key) {

    SearchRange r;
    memset(&r, 0, sizeof(r));
    r.key = key;

    if (key == 0) {

        cout << "\nAge group:\n1. 0-17   : Pediatrics & Adolescents\n"
                "2. 18-25  : Young Adults / University Students\n"
                "3. 26-45  : Working Adults (Early Career)\n"
                "4. 46-60  : Working Adults (Late Career)\n"
                "5. 61-100 : Senior Citizens / Geriatric Care\n";

        int group = askInt("Choose age group: ", 1, 5);

        const int mins[5] = {0, 18, 26, 46, 61};
        const int maxs[5] = {17, 25, 45, 60, 100};

        // Set the min max age based on the selected group
        r.ageMin = mins[group - 1];
        r.ageMax = maxs[group - 1];

    } else if (key == 1) {

        // Find the minimum and maximum visit duration in the dataset
        double minValue = data.data[0].los;
        double maxValue = data.data[0].los;

        for (int i = 1; i < data.size; i++) {
            if (data.data[i].los < minValue)
                minValue = data.data[i].los;

            if (data.data[i].los > maxValue)
                maxValue = data.data[i].los;
        }

        cout << "\nAvailable visit-duration range in " << datasetName
             << ": " << minValue << " to " << maxValue << " hours\n";

        r.losMin = askDouble("Minimum visit duration (hours): ");
        r.losMax = askDouble("Maximum visit duration (hours): ");

        while (r.losMin > r.losMax) { // Make sure the min is not > max
            cout << "Minimum cannot exceed maximum.\n";
            r.losMin = askDouble("Minimum visit duration (hours): ");
            r.losMax = askDouble("Maximum visit duration (hours): ");
        }

    } else {

        cout << "\nCare type:\n1. Emergency\n2. Inpatient\n3. Outpatient\n"
                "4. Routine Checkup\n5. Vaccination\n6. Rehabilitation\n";

        const char* names[6] = {
            "Emergency", "Inpatient", "Outpatient",
            "Routine Checkup", "Vaccination", "Rehabilitation"
        };

        // Store the selected care type in the search range
        strcpy(r.care, names[askInt("Choose care type: ", 1, 6) - 1]);
    }

    return r;
}

// Run the selected search method and record its performance
static SearchResult doSearch(int d, const SearchRange& range, int mode) {

    SearchResult r;
    r.comps = 0;
    r.shown = 0;

    const PatientArray* src;

    // Select original data/ sorted copy
    if (mode == 0)
        src = &orig[d];
    else {
        ensureSorted(d, range.key);
        src = &sortedData[d][range.key];
    }

    // Start record search operation time
    auto s = Clock::now();

    // Run *binary search // linear search*
    if (mode == 2)
        r.found = binarySearch(*src, range, r.comps, r.out, MAX_SEARCH_RESULTS);
    else
        r.found = linearSearch(*src, range, mode == 1,
                               r.comps, r.out, MAX_SEARCH_RESULTS);

    r.shown = r.found;

    // Stop timing and calculate search time
    auto e = Clock::now();
    r.us = elapsedUs(s, e);

    // Save result to performance_log
    logResult("Array", DS_NAMES[d], "Search",
              mode == 2 ? "Binary" : "Linear",
              KEY_NAMES[range.key],
              mode == 0 ? "Unsorted" : "Sorted",
              r.us, r.comps, src->memoryBytes());

    return r;
}

// **search menu and handle the user input
void searchMenu() {

    while (true) {

        cout << "\n--- ARRAY SEARCHING ---\n"
                "1. Linear search (unsorted data)\n"
                "2. Linear search (sorted data)\n"
                "3. Binary search (sorted data)\n"
                "4. Compare all three\n"
                "5. Back\n";

        int c = askInt("Choice: ", 1, 5);

        if (c == 5)
            return;

        int d = pickDataset(false);
        int key = pickKey();

        // Get search range
        SearchRange range = chooseRange(orig[d], DS_NAMES[d], key);

        if (c <= 3) {

            // Run the selected search method
            //display result
            SearchResult r = doSearch(d, range, c - 1);
            printSearchDetail(r);

        } else {

            // Run all three search methods (comparison)
            SearchResult r[3];

            for (int m = 0; m < 3; m++)
                r[m] = doSearch(d, range, m);

            printSearchCompare(r);

            printSearchDetail(r[2]); //binary search detailed result
        }
    }
}