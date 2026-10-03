#include "array_module.h"
//sinyi
static int rangeCompare(const Patient& p, const SearchRange& r) {
    if (r.key == 0) return p.age < r.ageMin ? -1 : (p.age > r.ageMax ? 1 : 0);
    if (r.key == 1) return p.los < r.losMin ? -1 : (p.los > r.losMax ? 1 : 0);
    return strcmp(p.care, r.care);
}
static Patient rangeProbe(const SearchRange& r) {
    Patient p; memset(&p, 0, sizeof(p));
    if (r.key == 0) p.age = r.ageMin;
    else if (r.key == 1) p.los = r.losMin;
    else strcpy(p.care, r.care);
    return p;
}
int linearSearch(const PatientArray& a, const SearchRange& r, bool isSorted, long& comps, Patient* out, int maxShow) {
    int found = 0; for (int i = 0; i < a.size; i++) { comps++; int cmp = rangeCompare(a.data[i], r); if (matchesSearchRange(a.data[i], r)) { if (found < maxShow) out[found] = a.data[i]; found++; } else if (isSorted && cmp > 0) break; } return found;
}
int lowerBound(const PatientArray& a, int key, const Patient& probe, long& comps) {
    int lo = 0, hi = a.size; while (lo < hi) { int mid = (lo + hi) / 2; comps++; if (cmpKey(a.data[mid], probe, key) < 0) lo = mid + 1; else hi = mid; } return lo;
}
int binarySearch(const PatientArray& a, const SearchRange& r, long& comps, Patient* out, int maxShow) {
    Patient low = rangeProbe(r); int i = lowerBound(a, r.key, low, comps), found = 0;
    while (i < a.size) { comps++; if (rangeCompare(a.data[i], r) > 0) break; if (matchesSearchRange(a.data[i], r)) { if (found < maxShow) out[found] = a.data[i]; found++; } i++; }
    return found;
}
int findFirst(const PatientArray& a, int key, const Patient& probe, long& comps) { for (int i = 0; i < a.size; i++) { comps++; if (cmpKey(a.data[i], probe, key) == 0) return i; } return -1; }
static void ensureSorted(int d, int key) {
    if (hasSorted[d][key]) return; cout << "(No saved sorted copy for " << DS_NAMES[d] << " by " << KEY_NAMES[key] << " - building one with insertion sort, not timed)\n";
    sortedData[d][key].copyFrom(orig[d]); long c = 0; insertionSort(sortedData[d][key], key, c); hasSorted[d][key] = true;
}
static SearchRange chooseRange(const PatientArray& data, const char* datasetName, int key) {
    SearchRange r; memset(&r, 0, sizeof(r)); r.key = key;
    if (key == 0) {
        cout << "\nAge group:\n1. 0-17   : Pediatrics & Adolescents\n2. 18-25  : Young Adults / University Students\n"
                "3. 26-45  : Working Adults (Early Career)\n4. 46-60  : Working Adults (Late Career)\n"
                "5. 61-100 : Senior Citizens / Geriatric Care\n";
        int group = askInt("Choose age group: ", 1, 5);
        const int mins[5] = {0, 18, 26, 46, 61};
        const int maxs[5] = {17, 25, 45, 60, 100};
        r.ageMin = mins[group - 1]; r.ageMax = maxs[group - 1];
    } else if (key == 1) {
        double minValue = data.data[0].los, maxValue = data.data[0].los;
        for (int i = 1; i < data.size; i++) { if (data.data[i].los < minValue) minValue = data.data[i].los; if (data.data[i].los > maxValue) maxValue = data.data[i].los; }
        cout << "\nAvailable visit-duration range in " << datasetName << ": " << minValue << " to " << maxValue << " hours\n";
        r.losMin = askDouble("Minimum visit duration (hours): ");
        r.losMax = askDouble("Maximum visit duration (hours): ");
        while (r.losMin > r.losMax) { cout << "Minimum cannot exceed maximum.\n"; r.losMin = askDouble("Minimum visit duration (hours): "); r.losMax = askDouble("Maximum visit duration (hours): "); }
    } else {
        cout << "\nCare type:\n1. Emergency\n2. Inpatient\n3. Outpatient\n4. Routine Checkup\n5. Vaccination\n6. Rehabilitation\n";
        const char* names[6] = {"Emergency", "Inpatient", "Outpatient", "Routine Checkup", "Vaccination", "Rehabilitation"};
        strcpy(r.care, names[askInt("Choose care type: ", 1, 6) - 1]);
    }
    return r;
}
static SearchResult doSearch(int d, const SearchRange& range, int mode) {
    SearchResult r; r.comps = 0; r.shown = 0; const PatientArray* src;
    if (mode == 0) src = &orig[d]; else { ensureSorted(d, range.key); src = &sortedData[d][range.key]; }
    auto s = Clock::now(); if (mode == 2) r.found = binarySearch(*src, range, r.comps, r.out, MAX_SEARCH_RESULTS); else r.found = linearSearch(*src, range, mode == 1, r.comps, r.out, MAX_SEARCH_RESULTS); r.shown = r.found; auto e = Clock::now();
    r.us = elapsedUs(s, e); logResult("Array", DS_NAMES[d], "Search", mode == 2 ? "Binary" : "Linear", KEY_NAMES[range.key], mode == 0 ? "Unsorted" : "Sorted", r.us, r.comps, src->memoryBytes()); return r;
}
void searchMenu() {
    while (true) {
        cout << "\n--- ARRAY SEARCHING ---\n1. Linear search (unsorted data)\n2. Linear search (sorted data)\n3. Binary search (sorted data)\n4. Compare all three\n5. Back\n";
        int c = askInt("Choice: ", 1, 5); if (c == 5) return; int d = pickDataset(false), key = pickKey(); SearchRange range = chooseRange(orig[d], DS_NAMES[d], key);
        if (c <= 3) { SearchResult r = doSearch(d, range, c - 1); printSearchDetail(r); }
        else { SearchResult r[3]; for (int m = 0; m < 3; m++) r[m] = doSearch(d, range, m); printSearchCompare(r); printSearchDetail(r[2]); }
    }
}
