#ifndef LINKEDLIST_MODULE_H
#define LINKEDLIST_MODULE_H

#include "../common.h"

// Node used to store each patient record
struct Node {
    Patient data;
    Node* next;
};

class PatientList {
public:
    Node* head;
    Node* tail;
    int size;

    // Create an empty linked list
    PatientList()
        : head(0), tail(0), size(0) {}

    // Free all nodes when the list is destroyed
    ~PatientList() {
        clear();
    }

    PatientList(const PatientList&) = delete;
    PatientList& operator=(const PatientList&) = delete;

    // Remove all nodes from the list
    void clear() {
        while (head) {
            Node* t = head;
            head = head->next;
            delete t;
        }

        tail = 0;
        size = 0;
    }

    // Add a patient to the end of the list
    void append(const Patient& p) {
        Node* n = new Node;
        n->data = p;
        n->next = 0;

        if (tail)
            tail->next = n;
        else
            head = n;

        tail = n;
        size++;
    }

    // Copy all records from another linked list
    void copyFrom(const PatientList& o) {
        clear();

        for (Node* c = o.head; c; c = c->next)
            append(c->data);
    }

    // Insert a patient at the given position
    void insertAt(int idx, const Patient& p) {
        if (idx >= size) {
            append(p);
            return;
        }

        Node* n = new Node;
        n->data = p;

        if (idx == 0) {
            n->next = head;
            head = n;
        } else {
            Node* prev = head;

            for (int i = 0; i < idx - 1; i++)
                prev = prev->next;

            n->next = prev->next;
            prev->next = n;
        }

        size++;
    }

    // Remove a patient at the given position
    void removeAt(int idx) {
        if (idx < 0 || idx >= size)
            return;

        Node* del;

        if (idx == 0) {
            del = head;
            head = head->next;

            if (!head)
                tail = 0;
        } else {
            Node* prev = head;

            for (int i = 0; i < idx - 1; i++)
                prev = prev->next;

            del = prev->next;
            prev->next = del->next;

            if (del == tail)
                tail = prev;
        }

        delete del;
        size--;
    }

    // Calculate linked list memory usage
    long memoryBytes() const {
        return (long)sizeof(*this) + (long)size * (long)sizeof(Node);
    }
};

extern PatientList orig[4];
extern PatientList sortedData[4][3];
extern bool hasSorted[4][3];

// Sorting functions
void insertionSort(PatientList&, int, long&);
void bubbleSort(PatientList&, int, long&);
void selectionSort(PatientList&, int, long&);
void runSort(PatientList&, int, int, long&);
void sortingMenu();

// Searching functions
int linearSearch(const PatientList&, const SearchRange&, bool, long&, Patient*, int);
int lowerBound(const PatientList&, int, const Patient&, long&, Node** = 0);
int binarySearch(const PatientList&, const SearchRange&, long&, Patient*, int);
int findFirst(const PatientList&, int, const Patient&, long&);
bool matchAt(const PatientList&, int, int, const Patient&);
void searchMenu();

// Insert, delete, analysis and data loading
void insertDeleteMenu();
void buildAnalytics(int, Analytics&);
bool loadAll();

#endif