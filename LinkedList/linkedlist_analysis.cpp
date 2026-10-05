#include "linkedlist_module.h"

// Add all records from the selected dataset into Analytics
void buildAnalytics(int d, Analytics& a) {
    for (Node* c = orig[d].head; c; c = c->next)
        a.add(c->data);
}