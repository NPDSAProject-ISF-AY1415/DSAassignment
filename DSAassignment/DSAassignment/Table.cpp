#include "Table.h"

using namespace eku;
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
	int max_size = 0, graphNum = 0;
	for (Graph &g : tableList){
		if (g.getXValueArr().size() > max_size)
			max_size = g.getXValueArr().size();
		graphNum++;
	}
	//Notify user of highest max size
	cout << green << "Maximum Size of all lists implementations: " << max_size << endl << endl;

	//Start on printing stuff
	printSeperator();
	cout << centerString(operation.c_str(), 10) << "|";
	//Get the title of the graphs
	for (Graph &g : tableList){
		cout << centerString(g.getTitle().c_str(), 16) << "|";
	}
	cout << endl;
	printSeperator();
	cout << centerString("n", 10) << "|";
	for (int i = 0; i < graphNum; i++)
		cout << centerString("CPU", 7) << "|" << centerString("RAM", 8) << "|";
	cout << endl;
	printSeperator();
	
	for (int i = 0; i < max_size; i++){
		if (shouldPlot(max_size, i)){
			cout << centerString(to_string(i).c_str(), 10) << "|";
			//Get the value out of all the graphs
			for (Graph &g : tableList){
				vector<double> cpuTime = g.getXValueArr();
				vector<double> memUse = g.getYValueArr();
				//Check that size of the graph object contains the current index and the vectors are not null
				if (cpuTime.size() > i && !cpuTime.empty())
					cout << centerDouble(cpuTime[i], 7, 2) << "|" << centerString(convertMemoryToHumanReadableSht(memUse[i]).c_str(), 8) << "|";
				else
					cout << centerString(" ", 7) << "|" << centerString(" ", 8) << "|";
				cpuTime.clear();
				memUse.clear();
			}
			cout << endl;
		}
	}
	printSeperator();
	return 0;
}