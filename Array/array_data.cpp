#include "array_module.h"
PatientArray orig[4]; //store original datasets
PatientArray sortedData[4][3]; //store sorted version of each datasets
bool hasSorted[4][3]; //track which datasets have been sorted (result)

bool loadAll() {
    //create a temp array to store records from CSV
    Patient* buf = new Patient[MAX_RECORDS];

    //load all 3 datasets
    for (int i = 0; i < 3; i++) {

        int n = readCsv(DS_FILES[i], buf, MAX_RECORDS);

        if (n == 0) { delete[] buf;
            return false; }
        for (int j = 0; j < n; j++) {
            orig[i].push(buf[j]); //put current record into original dataset
            orig[3].push(buf[j]); //put same record into combined dataset
        }
    }
    //clear temp memory after complete load
    delete[] buf;

    //mark all sorting result as not available yet
    for (int d = 0; d < 4; d++)
        for (int k = 0; k < 3; k++)
            hasSorted[d][k] = false;

    cout << "Loaded into ARRAYS: ";

    //display number of records in each dataset
    for (int d = 0; d < 4; d++) cout << DS_NAMES[d] << " = " << orig[d].size << "   ";
    cout << "\n";
    return true;
}
