#include "array_module.h"
PatientArray orig[4];
PatientArray sortedData[4][3];
bool hasSorted[4][3];
bool loadAll() {
    Patient* buf = new Patient[MAX_RECORDS];
    for (int i = 0; i < 3; i++) {
        int n = readCsv(DS_FILES[i], buf, MAX_RECORDS);
        if (n == 0) { delete[] buf; return false; }
        for (int j = 0; j < n; j++) { orig[i].push(buf[j]); orig[3].push(buf[j]); }
    }
    delete[] buf;
    for (int d = 0; d < 4; d++) for (int k = 0; k < 3; k++) hasSorted[d][k] = false;
    cout << "Loaded into ARRAYS: ";
    for (int d = 0; d < 4; d++) cout << DS_NAMES[d] << " = " << orig[d].size << "   ";
    cout << "\n";
    return true;
}
