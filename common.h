// common.h - shared helpers used by array_program.cpp, linkedlist_program.cpp and main.cpp
// NOTE: contains NO container. Records are held only in the self-made Array / Linked List classes.
#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <chrono>
using namespace std;

const int MAX_RECORDS = 5000;
const int MAX_SEARCH_RESULTS = MAX_RECORDS * 3;
const char* const DS_FILES[3]  = {"dataset1_facility_a.csv", "dataset2_facility_b.csv", "dataset3_facility_c.csv"};
const char* const DS_NAMES[4]  = {"Dataset 1 (Facility A)", "Dataset 2 (Facility B)",
                                  "Dataset 3 (Facility C)", "Combined (A+B+C)"};
const char* const KEY_NAMES[3] = {"Age", "Visit Duration", "Care Type"};
const char* const ALGO_NAMES[3] = {"Insertion Sort", "Bubble Sort", "Selection Sort"};
const char* const LOG_FILE = "performance_log.txt";

// ------------------------------------------------------------------ record
struct Patient {
    char   id[16];
    int    age;
    char   care[24];
    double los;      // length of stay / visit duration (hours)
    double rate;     // base cost per hour (MYR)
    int    visits;   // days visits per year
};

struct SearchRange {
    int key;
    int ageMin;
    int ageMax;
    double losMin;
    double losMax;
    char care[24];
};

inline bool matchesSearchRange(const Patient& p, const SearchRange& r) {
    if (r.key == 0) return p.age >= r.ageMin && p.age <= r.ageMax;
    if (r.key == 1) return p.los >= r.losMin && p.los <= r.losMax;
    return strcmp(p.care, r.care) == 0;
}

inline double patientCost(const Patient& p) { return p.los * p.rate * p.visits; }

// compare two patients on the chosen key (0 = Age, 1 = Visit Duration, 2 = Care Type)
inline int cmpKey(const Patient& a, const Patient& b, int key) {
    if (key == 0) return a.age < b.age ? -1 : (a.age > b.age ? 1 : 0);
    if (key == 1) return a.los < b.los ? -1 : (a.los > b.los ? 1 : 0);
    return strcmp(a.care, b.care);
}

// ------------------------------------------------------------------ CSV loading
inline void trimLine(char* s) {
    int n = (int)strlen(s);
    while (n > 0 && (s[n - 1] == '\r' || s[n - 1] == '\n' || s[n - 1] == ' ')) s[--n] = 0;
}

inline bool parseLine(char* line, Patient& p) {
    char* f[6];
    int n = 0;
    char* c = line;
    f[n++] = c;
    while (*c && n < 6) {
        if (*c == ',') { *c = 0; f[n++] = c + 1; }
        c++;
    }
    if (n < 6) return false;
    strncpy(p.id, f[0], 15);   p.id[15] = 0;
    p.age = atoi(f[1]);
    strncpy(p.care, f[2], 23); p.care[23] = 0;
    p.los = atof(f[3]);
    p.rate = atof(f[4]);
    p.visits = atoi(f[5]);
    return true;
}

// reads a CSV into a plain temporary buffer, returns number of records
inline int readCsv(const char* path, Patient* buf, int maxN) {
    ifstream in(path);
    if (!in) { cout << "ERROR: cannot open " << path << "\n"; return 0; }
    char line[256];
    int n = 0;
    in.getline(line, 256);                       // skip header
    while (n < maxN && in.getline(line, 256)) {
        trimLine(line);
        if (line[0] == 0) continue;
        if (parseLine(line, buf[n])) n++;
    }
    return n;
}

// ------------------------------------------------------------------ timing + log
typedef chrono::high_resolution_clock Clock;
inline double elapsedUs(Clock::time_point s, Clock::time_point e) {
    return chrono::duration<double, micro>(e - s).count();
}

inline void logResult(const char* structure, const char* dataset, const char* op, const char* algo,
                      const char* key, const char* state, double us, long steps, long mem) {
    ofstream out(LOG_FILE, ios::app);
    out << structure << "|" << dataset << "|" << op << "|" << algo << "|" << key << "|" << state
        << "|" << fixed << setprecision(3) << us << "|" << steps << "|" << mem << "\n";
}

inline void rule(int w) { cout << setfill('-') << setw(w) << "" << setfill(' ') << "\n"; }

inline void showPerformanceLog(const char* path = LOG_FILE) {
    ifstream in(path);
    if (!in) { cout << "\nNo performance data found at " << path << " yet. Run some experiments first.\n"; return; }
    const char* heads[9] = {"Structure", "Dataset", "Operation", "Algorithm", "Sort/Search Key",
                            "Data State", "Time (us)", "Steps", "Memory (B)"};
    int w[9] = {12, 24, 11, 17, 17, 12, 14, 10, 12};
    int total = 0;
    for (int i = 0; i < 9; i++) total += w[i];
    cout << "\n===== PERFORMANCE TEST RESULTS (" << path << ") =====\n";
    rule(total);
    for (int i = 0; i < 9; i++) cout << left << setw(w[i]) << heads[i];
    cout << "\n";
    rule(total);
    char line[300];
    int rows = 0;
    while (in.getline(line, 300)) {
        trimLine(line);
        char* f[9];
        int n = 0;
        char* c = line;
        f[n++] = c;
        while (*c && n < 9) {
            if (*c == '|') { *c = 0; f[n++] = c + 1; }
            c++;
        }
        if (n < 9) continue;
        for (int i = 0; i < 9; i++) cout << left << setw(w[i]) << f[i];
        cout << "\n";
        rows++;
    }
    rule(total);
    cout << rows << " record(s). Steps = comparisons. Memory = footprint of the structure used.\n";
}

// ------------------------------------------------------------------ input helpers
inline int askInt(const char* prompt, int lo, int hi) {
    int v;
    while (true) {
        cout << prompt;
        if (cin >> v && v >= lo && v <= hi) { cin.ignore(1000, '\n'); return v; }
        if (cin.eof()) exit(0);                  // input closed: stop instead of looping forever
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input, try again.\n";
    }
}

inline double askDouble(const char* prompt) {
    double v;
    while (true) {
        cout << prompt;
        if (cin >> v) { cin.ignore(1000, '\n'); return v; }
        if (cin.eof()) exit(0);
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Invalid input, try again.\n";
    }
}

// returns 0..3 (or 4 = all datasets when allowAll)
inline int pickDataset(bool allowAll) {
    cout << "\nSelect dataset:\n";
    for (int i = 0; i < 4; i++) cout << "  " << i + 1 << ". " << DS_NAMES[i] << "\n";
    if (allowAll) cout << "  5. All datasets\n";
    return askInt("Choice: ", 1, allowAll ? 5 : 4) - 1;
}

inline int pickKey() {
    cout << "\nSelect field:\n  1. Age\n  2. Visit Duration\n  3. Care Type\n";
    return askInt("Choice: ", 1, 3) - 1;
}

inline int pickAlgo() {
    cout << "\nSelect sorting algorithm:\n  1. Insertion Sort\n  2. Bubble Sort\n  3. Selection Sort\n";
    return askInt("Choice: ", 1, 3) - 1;
}

// builds a probe record holding only the value to search for
inline Patient askProbe(int key) {
    Patient p;
    memset(&p, 0, sizeof(p));
    if (key == 0)      p.age = askInt("Age to search for: ", 0, 120);
    else if (key == 1) p.los = askDouble("Visit duration (hours) to search for: ");
    else {
        cout << "Care type to search for (e.g. Emergency): ";
        cin.getline(p.care, 24);
    }
    return p;
}

inline Patient askNewPatient() {
    Patient p;
    memset(&p, 0, sizeof(p));
    strcpy(p.id, "PT9999");
    p.age = askInt("New record - age: ", 0, 120);
    cout << "New record - care type: ";
    cin.getline(p.care, 24);
    p.los = askDouble("New record - visit duration (hours): ");
    p.rate = 100.0;
    p.visits = 1;
    return p;
}

// ------------------------------------------------------------------ printing records
inline void printPatientHeader() {
    cout << left << setw(10) << "PatientID" << setw(6) << "Age" << setw(20) << "Care Type"
         << setw(12) << "Stay (hrs)" << setw(13) << "Rate (RM/h)" << setw(8) << "Visits"
         << setw(16) << "Total Cost (RM)" << "\n";
    rule(85);
}

inline void printPatientRow(const Patient& p) {
    cout << left << setw(10) << p.id << setw(6) << p.age << setw(20) << p.care << fixed
         << setprecision(1) << setw(12) << p.los << setprecision(2) << setw(13) << p.rate
         << setw(8) << p.visits << setw(16) << patientCost(p) << "\n";
}

// ------------------------------------------------------------------ sort printing
inline void printSortHeader() {
    cout << left << setw(24) << "Dataset" << setw(16) << "Sort Key" << setw(16) << "Algorithm"
         << setw(10) << "Records" << setw(14) << "Time (us)" << setw(14) << "Comparisons"
         << setw(12) << "Memory (B)" << "\n";
    rule(106);
}

inline void printSortRow(int d, int key, int algo, int n, double us, long comps, long mem) {
    cout << left << setw(24) << DS_NAMES[d] << setw(16) << KEY_NAMES[key] << setw(16)
         << ALGO_NAMES[algo] << setw(10) << n << fixed << setprecision(3) << setw(14) << us
         << setw(14) << comps << setw(12) << mem << "\n";
}

// ------------------------------------------------------------------ search printing
struct SearchResult {
    double us;
    long   comps;
    int    found;       // total matches
    int    shown;       // how many records stored in out[]
    Patient* out;
    SearchResult() : us(0), comps(0), found(0), shown(0), out(new Patient[MAX_SEARCH_RESULTS]) {}
    ~SearchResult() { delete[] out; }
    SearchResult(const SearchResult& other) : us(other.us), comps(other.comps), found(other.found), shown(other.shown), out(new Patient[MAX_SEARCH_RESULTS]) {
        for (int i = 0; i < other.shown && i < MAX_SEARCH_RESULTS; i++) out[i] = other.out[i];
    }
    SearchResult& operator=(const SearchResult& other) {
        if (this == &other) return *this;
        us = other.us; comps = other.comps; found = other.found; shown = other.shown;
        for (int i = 0; i < other.shown && i < MAX_SEARCH_RESULTS; i++) out[i] = other.out[i];
        return *this;
    }
};

inline void printSearchDetail(const SearchResult& r) {
    cout << "\nMatches found: " << r.found << "   Comparisons: " << r.comps << "   Time: "
         << fixed << setprecision(3) << r.us << " us\n";
    if (r.found == 0) return;
    printPatientHeader();
    int n = r.found < r.shown ? r.found : r.shown;
    for (int i = 0; i < n; i++) printPatientRow(r.out[i]);
}

inline void printSearchCompare(const SearchResult r[3]) {
    const char* names[3] = {"Linear (unsorted data)", "Linear (sorted data)", "Binary (sorted data)"};
    cout << "\n" << left << setw(26) << "Search Method" << setw(10) << "Matches" << setw(14)
         << "Comparisons" << setw(14) << "Time (us)" << "\n";
    rule(64);
    for (int i = 0; i < 3; i++)
        cout << left << setw(26) << names[i] << setw(10) << r[i].found << setw(14) << r[i].comps
             << fixed << setprecision(3) << setw(14) << r[i].us << "\n";
}

inline void printUpdateReport(const char* op, bool ok, int before, int after, int pos, double us,
                              long comps, long mem) {
    cout << "\n" << op << (ok ? " completed" : " FAILED (no matching record)")
         << " (temporary experiment - dataset not changed)\n";
    rule(70);
    cout << "Records before : " << before << "\nRecords after  : " << after;
    if (ok) cout << "\nPosition       : " << pos;
    cout << "\nComparisons    : " << comps << "\nTime           : " << fixed << setprecision(3) << us
         << " us\nMemory         : " << mem << " bytes\n";
}

// ------------------------------------------------------------------ analytics
const char* const AG_LABELS[5] = {"0-17 (Pediatrics & Adolescents)",
                                  "18-25 (Young Adults / University Students)",
                                  "26-45 (Working Adults - Early Career)",
                                  "46-60 (Working Adults - Late Career)",
                                  "61-100 (Senior Citizens / Geriatric Care)"};

inline int ageGroup(int age) {
    if (age <= 17) return 0;
    if (age <= 25) return 1;
    if (age <= 45) return 2;
    if (age <= 60) return 3;
    return 4;
}

struct Tally { char name[24]; int count; double cost; double hours; };

struct TallySet {
    Tally t[12];
    int n;
    TallySet() : n(0) {}
    void add(const char* nm, double cost, double hours) {
        int i;
        for (i = 0; i < n; i++) if (strcmp(t[i].name, nm) == 0) break;
        if (i == n) {
            if (n >= 12) return;
            strcpy(t[n].name, nm);
            t[n].count = 0; t[n].cost = 0; t[n].hours = 0;
            n++;
        }
        t[i].count++; t[i].cost += cost; t[i].hours += hours;
    }
    void sortByCount() {                     // descending patient count
        for (int i = 0; i < n - 1; i++)
            for (int j = 0; j < n - 1 - i; j++)
                if (t[j].count < t[j + 1].count) { Tally x = t[j]; t[j] = t[j + 1]; t[j + 1] = x; }
    }
};

struct Analytics {
    TallySet care;
    TallySet ag[5];
    int    agCount[5];
    double agCost[5];
    int    total;
    double totalCost, totalHours;
    long   totalVisits;
    Analytics() : total(0), totalCost(0), totalHours(0), totalVisits(0) {
        for (int i = 0; i < 5; i++) { agCount[i] = 0; agCost[i] = 0; }
    }
    void add(const Patient& p) {
        double c = patientCost(p);
        total++; totalCost += c; totalHours += p.los; totalVisits += p.visits;
        care.add(p.care, c, p.los);
        int g = ageGroup(p.age);
        ag[g].add(p.care, c, p.los);
        agCount[g]++; agCost[g] += c;
    }
};

inline void printCareAnalysis(const Analytics& a) {
    TallySet s = a.care;
    s.sortByCount();
    cout << left << setw(20) << "Care Type" << setw(10) << "Patients" << setw(20) << "Total Cost (RM)"
         << setw(24) << "Avg Cost/Patient (RM)" << setw(14) << "Total Hours" << setw(14)
         << "Avg Stay (hrs)" << "\n";
    rule(102);
    int hiCost = 0;
    for (int i = 0; i < s.n; i++) {
        if (s.t[i].cost > s.t[hiCost].cost) hiCost = i;
        cout << left << setw(20) << s.t[i].name << setw(10) << s.t[i].count << fixed << setprecision(2)
             << setw(20) << s.t[i].cost << setw(24) << s.t[i].cost / s.t[i].count << setw(14)
             << s.t[i].hours << setw(14) << s.t[i].hours / s.t[i].count << "\n";
    }
    rule(102);
    if (s.n > 0) {
        cout << "Total billing: RM " << fixed << setprecision(2) << a.totalCost << "   Patients: " << a.total << "\n";
        cout << "Highest patient traffic : " << s.t[0].name << " (" << s.t[0].count << " patients)\n";
        cout << "Highest billing         : " << s.t[hiCost].name << " (RM " << s.t[hiCost].cost << ")\n";
    }
}

inline void printAgeGroupAnalysis(const Analytics& a) {
    for (int g = 0; g < 5; g++) {
        cout << "\nAge Group: " << AG_LABELS[g] << "\n";
        if (a.agCount[g] == 0) { cout << "  (no patients in this group)\n"; continue; }
        TallySet s = a.ag[g];
        s.sortByCount();
        rule(86);
        cout << left << setw(20) << "Care Type" << setw(16) << "Patient Count" << setw(20)
             << "Total Cost (RM)" << setw(28) << "Average Cost per Patient (RM)" << "\n";
        rule(86);
        for (int i = 0; i < s.n; i++)
            cout << left << setw(20) << s.t[i].name << setw(16) << s.t[i].count << fixed << setprecision(2)
                 << setw(20) << s.t[i].cost << setw(28) << s.t[i].cost / s.t[i].count << "\n";
        rule(86);
        cout << "Most preferred care type    : " << s.t[0].name << " (" << s.t[0].count << " patients)\n";
        cout << "Total Billing for Age Group : RM " << fixed << setprecision(2) << a.agCost[g] << "\n";
        cout << "Average Cost per Patient    : RM " << a.agCost[g] / a.agCount[g] << "  ("
             << a.agCount[g] << " patients)\n";
    }
}

inline void printBillingHeader() {
    cout << left << setw(26) << "Dataset" << setw(10) << "Patients" << setw(20) << "Total Billing (RM)"
         << setw(24) << "Avg Cost/Patient (RM)" << setw(14) << "Total Hours" << setw(16)
         << "Avg Stay (hrs)" << setw(14) << "Visits/Year" << "\n";
    rule(122);
}

inline void printBillingRow(const char* name, const Analytics& a) {
    cout << left << setw(26) << name << setw(10) << a.total << fixed << setprecision(2) << setw(20)
         << a.totalCost << setw(24) << (a.total ? a.totalCost / a.total : 0) << setw(14) << a.totalHours
         << setw(16) << (a.total ? a.totalHours / a.total : 0) << setw(14) << a.totalVisits << "\n";
}

inline void printDatasetSummary(const char* name, const Analytics& a) {
    cout << "\n================================================================================\n"
            "                   DATASET SUMMARY - " << name << "\n"
            "================================================================================\n";
    cout << "Total Patients              : " << a.total << "\n"
         << "Total Stay Hours            : " << fixed << setprecision(0) << a.totalHours << " hrs\n"
         << "Total Medical Cost          : RM " << fixed << setprecision(2) << a.totalCost << "\n"
         << "Average Cost Per Patient    : RM " << (a.total ? a.totalCost / a.total : 0) << "\n\n"
         << "Age Groups Present:\n";
    const char* summaryAgeLabels[5] = {
        "Pediatrics & Adolescents", "Young Adults / University Students",
        "Working Adults (Early Career)", "Working Adults (Late Career)",
        "Senior Citizens / Geriatric Care"
    };
    const char* summaryAgeRanges[5] = {"0-17", "18-25", "26-45", "46-60", "61-100"};
    for (int i = 0; i < 5; i++) {
        if (a.agCount[i] == 0) continue;
        cout << "  - " << left << setw(8) << summaryAgeRanges[i] << ": "
             << setw(36) << summaryAgeLabels[i] << right << setw(4) << a.agCount[i]
             << " patients\n";
    }
    cout << "\nCare Types Available:\n";
    for (int i = 0; i < a.care.n; i++)
        cout << "  - " << left << setw(38) << a.care.t[i].name << right << a.care.t[i].count << " patients\n";
    cout << "================================================================================\n";
}

typedef void (*BuildFn)(int dataset, Analytics& out);

// shared analysis menu; each program supplies its own function that walks ITS data structure
inline void analysisMenu(BuildFn build) {
    while (true) {
        cout << "\n===== ANALYSIS =====\n1. Care Type Analysis\n2. Total Billing Cost Analysis\n"
                "3. Age Group Analysis\n4. Dataset Summary\n5. Back\n";
        int c = askInt("Choice: ", 1, 5);
        if (c == 5) return;
        if (c == 4) {
            for (int i = 0; i < 4; i++) {
                Analytics a;
                build(i, a);
                printDatasetSummary(DS_NAMES[i], a);
            }
            continue;
        }
        int d = pickDataset(true);
        int from = (d == 4) ? 0 : d, to = (d == 4) ? 3 : d;
        if (c == 2) {
            cout << "\n";
            printBillingHeader();
            for (int i = from; i <= to; i++) {
                Analytics a;
                build(i, a);
                printBillingRow(DS_NAMES[i], a);
            }
            rule(122);
        } else {
            for (int i = from; i <= to; i++) {
                Analytics a;
                build(i, a);
                cout << "\n################ " << DS_NAMES[i] << " ################\n";
                if (c == 1) printCareAnalysis(a);
                else        printAgeGroupAnalysis(a);
            }
        }
    }
}

#endif
