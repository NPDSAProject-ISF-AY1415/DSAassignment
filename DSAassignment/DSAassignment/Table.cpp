#include "Table.h"

using namespace eku;
using namespace utility;


/* An Inline Progress Bar
@param x Current Index
@param n Total Number of Progress
@param beginClock Start of time
@param w Width of the bar
*/
static inline void loadbar(unsigned int x, unsigned int n, clock_t beginClock, unsigned int w = 25)
{
	if ((x != n) && (x % (n / 100 + 1) != 0) && n >= 2000) return;

	float ratio = x / (float)n;
	int   c = ratio * w;

	//Print Progress Bar
	cout << setw(3) << white << "Processed: " << cyan << x << white << "/" << green << n << yellow << " [" << red;
	for (int x = 0; x < c; x++) cout << "=";
	for (int x = c; x < w; x++) cout << " ";
	cout << yellow << "] " << (int)(ratio * 100) << "%" << white;
	cout << " Time: " << cyan << setprecision(2) << fixed << calculateElapsed(beginClock, clock()) << " sec" << white;
	//Get Console Cursor Pos
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &SBInfo);
	int currentlen = SBInfo.dwCursorPosition.X;
	//Zero out stuff
	for (int i = currentlen - 1; i < CONSOLE_WINDOW_SIZE; i++)
		cout << " ";
	cout << "\r" << flush;
	settextcolor(white);
}

int plotTable(vector<Graph> &tableList, string operation){
	cout << green << "Processing Table. This may take a while" << white << endl;
	//Check max size of all of the vectors
	int max_size = 0, graphNum = 0;
	for (Graph &g : tableList){
		if (g.getXValueArr().size > max_size)
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
	printSeperator();
	cout << centerString("n", 10) << "|";
	for (int i = 0; i < graphNum; i++)
		cout << centerString("CPU", 7) << "|" << centerString("RAM", 8) << "|";
	printSeperator();
	
	for (int i = 0; i < max_size; i++){
		if (shouldPlot(max_size, i)){
			cout << centerString(to_string(i).c_str(), 10) << "|";
			//Get the value out of all the graphs
			for (Graph &g : tableList){
				vector<double> *cpuTime = &g.getXValueArr();
				vector<double> *memUse = &g.getYValueArr();
				cout << centerString(to_string((*cpuTime)[i]).c_str(), 7) << "|" << centerString(convertMemoryToHumanReadableSht((*memUse)[i]).c_str(), 8) << "|";
				delete cpuTime;
				delete memUse;
			}
		}
	}
	printSeperator();
	return 0;
}