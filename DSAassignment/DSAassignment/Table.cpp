/*
Table.cpp
Class File that prints out the tabulation table
Team Name: Wat.jpg
Team Member 1: Soh Yan Quan, Kenneth (S10147249G)
Team Member 2: Soh Hak Wee (S10146869K)
Features to Highlight: Tabulation table printing
*/

#include "Table.h"

using namespace consolecolors;
using namespace utility;


/*TABLE DESIGN
===============================================================================
OPERATE   |  UNSORTED PTR  |  UNSORTED ARR  |   SORTED ARR   | DOUBLY LINKED  |
===============================================================================
    n     |  CPU  |  RAM   |  CPU  |  RAM   |  CPU  |  RAM   |  CPU  |  RAM   |
===============================================================================


===============================================================================
OPERATIONS
* ADD			-	Add into list
* REMOVE		-	Remove from list
* SEQ SEARCH	-	Do a Sequential Search on list
* BIN SEARCH	-	Do a Binary Search on list
* DISPLAY		-	Display all from list
*/

int plotTable(vector<Graph> &tableList, string operation){
	cout << green << "Processing Table. This may take a while" << white << endl;
	//Check max size of all of the vectors
	unsigned int max_size = 0, graphNum = 0;
	for (Graph &g : tableList){
		if (g.getXValueArr().size() > max_size)
			max_size = g.getXValueArr().size();
		graphNum++;
	}
	//Notify user of highest max size
	cout << green << "Maximum Size of all lists implementations: " << max_size << endl << endl;

	//Start on printing stuff
	printSeperator();
	cout << green << centerString(operation.c_str(), 10) << yellow << "|" << white;
	//Get the title of the graphs
	for (Graph &g : tableList){
		cout << red << centerString(g.getTitle().c_str(), 16) << yellow << "|" << white;
	}
	cout << endl;
	printSeperator();
	cout << pink << centerString("n", 10) << yellow << "|" << white;
	for (unsigned int i = 0; i < graphNum; i++)
		cout << cyan << centerString("CPU", 7) << yellow << "|" << gray << centerString("RAM", 8) << yellow << "|" << white;
	cout << endl;
	printSeperator();
	
	for (unsigned int i = 0; i < max_size; i++){
		if (shouldPlot(max_size, i)){
			cout << pink << centerString(to_string(i).c_str(), 10) << yellow << "|" << white;
			//Get the value out of all the graphs
			for (Graph &g : tableList){
				vector<double> cpuTime = g.getXValueArr();
				vector<double> memUse = g.getYValueArr();
				//Check that size of the graph object contains the current index and the vectors are not null
				if (cpuTime.size() > i && !cpuTime.empty())
					cout << cyan << centerDouble(cpuTime[i], 7, 2) << yellow << "|" << gray << centerString(convertMemoryToHumanReadableSht((SIZE_T)memUse[i]).c_str(), 8) << yellow << "|" << white;
				else
					cout << cyan << centerString("-", 7) << yellow << "|" << gray << centerString("-", 8) << yellow << "|" << white;
				cpuTime.clear();
				memUse.clear();
			}
			cout << endl;
		}
	}
	printSeperator();
	return 0;
}