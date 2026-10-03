#include "array_module.h"

void buildAnalytics(int d, Analytics& a) {
    for (int i = 0; i < orig[d].size; i++) a.add(orig[d].data[i]);
}