#include "plotGraph.h"

//Command is something that goes
//"plot <data> title <title>[, <2nd data> title <2nd title>,]..."

using namespace eku;
using namespace utility;

int plotGraph(vector<string> &graphList, string title){
	FILE * gnuplotPipe = _popen("gnuplot -persistent", "w");
	string titleStr = "set title \"" + title + "\" \n";
	fprintf(gnuplotPipe, titleStr.c_str());
	//Set The Display, Add, Remove, Binary Search, Sequential Search
	//fprintf(gnuplotPipe, "set xtics offset character 0,0,0 norangelimit (\"Add\" 1, \"Remove\" 2, \"Display\" 3, \"Binary Search\" 4, \"Sequential Search\" 5) \n");
	bool first_line = true;
	for (int j = 0; j < graphList.size(); j++){
		Graph g = Graph::createGraphObjFromString(graphList[j]);

		//DEBUG CODE
		cout << "Get Title of Graph: " << j + 1 << endl;
		//END OF DEBUG CODE

		if (first_line)
			fprintf(gnuplotPipe, "plot '-' t \"");
		else
			fprintf(gnuplotPipe, "'-' t \"");
		first_line = false;
		fprintf(gnuplotPipe, g.getTitle().c_str());
		fprintf(gnuplotPipe, "\" w linesp,");
	}
	fprintf(gnuplotPipe, "\n");
	for (int j = 0; j < graphList.size(); j++){
		Graph g = Graph::createGraphObjFromString(graphList[j]);
		//cout << g.getValues(1) << "lol" << endl; 
		int size = g.getXValueArr().size();
		vector<double> yVals = g.getYValueArr();
		vector<double> xVals = g.getXValueArr();
		
		//DEBUG CODE
		cout << "Plotting Graph: " << j + 1 << endl;
		//END OF DEBUG CODE

		for (int k = 0; k < g.getXValueArr().size(); k++)
		{
			if (shouldPlot(size, k))
				fprintf(gnuplotPipe, "%ld %lf \n", (int)xVals[k], yVals[k]);
		}
		fprintf(gnuplotPipe, "e\n");
	}
	fflush(gnuplotPipe);
	cout << red << "Please ensure that you have closed the graph before continuing! Press any key to continue" << endl;
	cin.get();
	cin.get();
	return 0;
}