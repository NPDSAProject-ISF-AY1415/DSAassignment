#include "plotGraph.h"

//Command is something that goes
//"plot <data> title <title>[, <2nd data> title <2nd title>,]..."

using namespace consolecolors;
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
	int   c = (int) (ratio * w);

	//Print Progress Bar
	cout << setw(3) << white << "Plotted: " << cyan << x << white << "/" << green << n << yellow << " [" << red;
	for (int x = 0; x < c; x++) cout << "=";
	for (unsigned int x = c; x < w; x++) cout << " ";
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

/*
Plots the graph passed in by graph objects
@param &graphList Memory Address off graph strings
@param title Title of the graph
@return When its done
*/
int plotGraph(vector<Graph> &graphList, string title){
	cout << green << "Starting to plot your graphs. This may take a while depending on how many points there are in your graphs" << white << endl;
	FILE * gnuplotPipe = _popen("gnuplot -persistent", "w");
	string titleStr = "set title \"" + title + "\" \n";
	fprintf(gnuplotPipe, titleStr.c_str());
	fprintf(gnuplotPipe, "set ylabel \"CPU Time (sec)/Memory Usage (Bytes)\" \n");
	fprintf(gnuplotPipe, "set xlabel \"Iterations/Critical Instructions\" \n");
	bool first_line = true;
	for (Graph &g : graphList){
		if (first_line)
			fprintf(gnuplotPipe, "plot '-' t \"");
		else
			fprintf(gnuplotPipe, "'-' t \"");
		first_line = false;
		fprintf(gnuplotPipe, g.getTitle().c_str());
		fprintf(gnuplotPipe, "\" w linesp,");
	}
	fprintf(gnuplotPipe, "\n");
	for (Graph &g : graphList){
		//cout << g.getValues(1) << "lol" << endl; 
		int size = g.getXValueArr().size();
		vector<double> yVals = g.getYValueArr();
		vector<double> xVals = g.getXValueArr();

		cout << "Plotting Graph: " << yellow << g.getTitle() << white << endl;
		clock_t startClock = clock();
		for (unsigned int k = 0; k < g.getXValueArr().size(); k++)
		{
			loadbar(k, size, startClock);
			if (shouldPlot(size, k))
				fprintf(gnuplotPipe, "%ld %lf \n", (int)xVals[k], yVals[k]);
		}
		fprintf(gnuplotPipe, "e\n");
		loadbar(size, size, startClock);
		cout << endl;
	}
	cout << green << "Graph Processed. Plotting and Launching Graph Window... May take a while..." << white << endl;
	fflush(gnuplotPipe);
	cout << red << "Please ensure that you have closed the graph window before continuing! Press any key to continue" << white << endl;
	cin.get();
	cin.get();
	return 0;
}