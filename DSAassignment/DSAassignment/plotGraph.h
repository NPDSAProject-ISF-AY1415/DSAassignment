#include <stdlib.h>
#include <stdio.h>
#include "Graph.h"
#include "concol.h"
#include "Utility.h"
#include <iostream>
#include <vector>
#include <iomanip>		//std::setw

static inline void loadbar(unsigned int x, unsigned int n, clock_t beginClock, unsigned int w);
int plotGraph(vector<string> &graphList, string title);