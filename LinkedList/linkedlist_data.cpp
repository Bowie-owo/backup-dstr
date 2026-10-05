#include "linkedlist_module.h"

PatientList orig[4];
PatientList sortedData[4][3];
bool hasSorted[4][3];

bool loadAll() {

    // Temporary storage for CSV records
    Patient* buf = new Patient[MAX_RECORDS];

    for (int i = 0; i < 3; i++) {
        int n = readCsv(DS_FILES[i], buf, MAX_RECORDS);

        // Stop if the dataset cannot be loaded
        if (n == 0) {
            delete[] buf;
            return false;
        }

        for (int j = 0; j < n; j++) {

            // Store records in the original and combined datasets
            orig[i].append(buf[j]);
            orig[3].append(buf[j]);
        }
    }

    // Release temporary storage
    delete[] buf;

    // Mark all sorted copies as unavailable
    for (int d = 0; d < 4; d++)
        for (int k = 0; k < 3; k++)
            hasSorted[d][k] = false;

    cout << "Loaded into SINGLY LINKED LISTS: ";

    // Display the number of records in each dataset
    for (int d = 0; d < 4; d++)
        cout << DS_NAMES[d] << " = " << orig[d].size << "   ";

    cout << "\n";
    return true;
}