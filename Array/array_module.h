// Prevent this header file from being included more than 1
#ifndef ARRAY_MODULE_H
#define ARRAY_MODULE_H

#include "../common.h"

// Array to store patient records
class PatientArray {
public:

    Patient* data;  //storing patient records
    int size;       // Number of records currently stored
    int cap;        //Current storage capacity

    //create empty array
    PatientArray() : data(0), size(0), cap(0) {}

    //free the memory used
    ~PatientArray() { delete[] data; }

    //prevent copying whole array using the default copy operation
    PatientArray(const PatientArray&) = delete;
    PatientArray& operator=(const PatientArray&) = delete;

    //increase the array capacity when more space needed
    void reserve(int n) {
        if (n <= cap) return;

        Patient* nd = new Patient[n];

        //copy existing records into new memory
        for (int i = 0; i < size; i++)
            nd[i] = data[i];

        delete[] data;
        data = nd;
        cap = n;
    }

    //add new patient record in array end
    void push(const Patient& p) {

        //increase the capacity if the array=full
        if (size == cap)
            reserve(cap ? cap * 2 : 16);

        data[size++] = p;
    }

    //copy all records from another PatientArray
    void copyFrom(const PatientArray& o) {

        size = 0;
        reserve(o.size);

        for (int i = 0; i < o.size; i++)
            data[i] = o.data[i];

        size = o.size;
    }

    //insert patient at specified index
    void insertAt(int idx, const Patient& p) {

        if (size == cap)
            reserve(cap ? cap * 2 : 16);

        //move records to the right to create space
        for (int i = size; i > idx; i--)
            data[i] = data[i - 1];

        data[idx] = p;
        size++;
    }

    //remove the patient at the specified index
    void removeAt(int idx) {

        //move later records left to fill the empty position
        for (int i = idx; i < size - 1; i++)
            data[i] = data[i + 1];

        size--;
    }

    //calculate the memory used by the array
    long memoryBytes() const {
        return (long)sizeof(*this) +
               (long)cap * (long)sizeof(Patient);
    }
};

// Shared datasets used by the Array implementation
extern PatientArray orig[4];

// Store sorted copies for each dataset and sorting key
extern PatientArray sortedData[4][3];

// Track sorted copy is already available
extern bool hasSorted[4][3];

// Sorting functions
void insertionSort(PatientArray&, int, long&);
void bubbleSort(PatientArray&, int, long&);
void selectionSort(PatientArray&, int, long&);
void runSort(PatientArray&, int, int, long&);
void sortingMenu();

// Searching functions
int linearSearch(const PatientArray&, const SearchRange&, bool, long&, Patient*, int);
int lowerBound(const PatientArray&, int, const Patient&, long&);
int binarySearch(const PatientArray&, const SearchRange&, long&, Patient*, int);
int findFirst(const PatientArray&, int, const Patient&, long&);
void searchMenu();

// Insert and delete experiments
void insertDeleteMenu();

// Build analytics data from a selected dataset
void buildAnalytics(int, Analytics&);

// Load all datasets into the Array structure
bool loadAll();

#endif