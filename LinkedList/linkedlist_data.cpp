#include "linkedlist_module.h"
PatientList orig[4]; PatientList sortedData[4][3]; bool hasSorted[4][3];
bool loadAll() {
    Patient* buf = new Patient[MAX_RECORDS];
    for (int i = 0; i < 3; i++) { int n = readCsv(DS_FILES[i], buf, MAX_RECORDS); if (n == 0) { delete[] buf; return false; } for (int j = 0; j < n; j++) { orig[i].append(buf[j]); orig[3].append(buf[j]); } }
    delete[] buf; for (int d = 0; d < 4; d++) for (int k = 0; k < 3; k++) hasSorted[d][k] = false;
    cout << "Loaded into SINGLY LINKED LISTS: "; for (int d = 0; d < 4; d++) cout << DS_NAMES[d] << " = " << orig[d].size << "   "; cout << "\n"; return true;
}
