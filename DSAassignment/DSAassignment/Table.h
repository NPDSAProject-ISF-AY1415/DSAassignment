#ifndef TABLE_PLOT
#define TABLE_PLOT

#include <vector>
#include <iomanip>
#include "Graph.h"
#include "concol.h"
#include "Utility.h"
#include "Memory.h"

static inline void loadbar(unsigned int x, unsigned int n, clock_t beginClock, unsigned int w);

int plotTable(vector<Graph> &graphList, string title);

#endif