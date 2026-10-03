#ifndef ARRAY_MODULE_H
#define ARRAY_MODULE_H
#include "../common.h"

class PatientArray {
public:
    Patient* data;
    int size;
    int cap;
    PatientArray() : data(0), size(0), cap(0) {}
    ~PatientArray() { delete[] data; }
    PatientArray(const PatientArray&) = delete;
    PatientArray& operator=(const PatientArray&) = delete;
    void reserve(int n) {
        if (n <= cap) return;
        Patient* nd = new Patient[n];
        for (int i = 0; i < size; i++) nd[i] = data[i];
        delete[] data; data = nd; cap = n;
    }
    void push(const Patient& p) {
        if (size == cap) reserve(cap ? cap * 2 : 16);
        data[size++] = p;
    }
    void copyFrom(const PatientArray& o) {
        size = 0; reserve(o.size);
        for (int i = 0; i < o.size; i++) data[i] = o.data[i];
        size = o.size;
    }
    void insertAt(int idx, const Patient& p) {
        if (size == cap) reserve(cap ? cap * 2 : 16);
        for (int i = size; i > idx; i--) data[i] = data[i - 1];
        data[idx] = p; size++;
    }
    void removeAt(int idx) {
        for (int i = idx; i < size - 1; i++) data[i] = data[i + 1];
        size--;
    }
    long memoryBytes() const { return (long)sizeof(*this) + (long)cap * (long)sizeof(Patient); }
};
extern PatientArray orig[4];
extern PatientArray sortedData[4][3];
extern bool hasSorted[4][3];
void insertionSort(PatientArray&, int, long&);
void bubbleSort(PatientArray&, int, long&);
void selectionSort(PatientArray&, int, long&);
void runSort(PatientArray&, int, int, long&);
void sortingMenu();
int linearSearch(const PatientArray&, const SearchRange&, bool, long&, Patient*, int);
int lowerBound(const PatientArray&, int, const Patient&, long&);
int binarySearch(const PatientArray&, const SearchRange&, long&, Patient*, int);
int findFirst(const PatientArray&, int, const Patient&, long&);
void searchMenu();
void insertDeleteMenu();
void buildAnalytics(int, Analytics&);
bool loadAll();
#endif
