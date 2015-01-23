#include "Utility.h"

using namespace std;
using namespace consolecolors;

namespace utility {

	/*
	Color Legend
	Yellow - Progress Bar/Status Message
	Pink - Input Message
	Cyan - Input/Numbers
	White - Progress Message
	Blue - String Message
	Red - Title
	*/

	/*
	Check if string is a positive number
	@param s String to check
	@return Whether it is a number or not
	*/
	bool is_number(const string& s)
	{
		return !s.empty() && find_if(s.begin(),
			s.end(), [](char c) { return !isdigit(c); }) == s.end();
	}

	/*
	Prints the seperator (===) (78 chars)
	*/
	void printSeperator(){
		cout << yellow << "===============================================================================" << white << endl;
	}

	void printMenuTitle(string title){
		printSeperator();
		cout << red << centerString(title.c_str(), CONSOLE_WINDOW_SIZE) << white << endl;
		printSeperator();
	}

	/*
	Calculate Elapsed Time in Seconds
	@param start Start of the clock
	@param end The time where it is clocked
	@return Elapsed time in seconds
	*/
	double calculateElapsed(clock_t start, clock_t end){
		return double(end - start) / CLOCKS_PER_SEC;
	}

	/*
	Parses a music info string and return a music object from it
	@param music Music Info String
	@return Music Object
	*/
	Music parseMusicItem(string music){
		string parsed[6];

		string tmpToken;
		size_t pos = 0;
		string delimiter = "<SEP>";
		int column = 0;

		while ((pos = music.find(delimiter)) != string::npos) {
			tmpToken = music.substr(0, pos);
			parsed[column] = tmpToken;
			music.erase(0, pos + delimiter.length());
			column++;
		}
		tmpToken = music;
		parsed[5] = tmpToken;

		Music musicResult(parsed[0], parsed[1], parsed[2], parsed[3], parsed[4], parsed[5]);

		return musicResult;
	}

	/*
	Print ASCII Art
	*/
	void printAscii(){
		ifstream file("ascii.txt");
		string print;
		while (getline(file, print)){
			cout << print << endl;
		}
	}

	/*
	Get Input from String
	@param storein Stores the input into this string
	*/
	void getStringInput(string &storein){
		settextcolor(cyan);
		cin >> storein;
		settextcolor(white);
	}

	/*
	Centers a string
	@param s String to center
	@param size Size of String to figure out what to center
	@return Centered String
	*/
	string centerString(const char* s, int size){
		int len = strlen(s);
		int pos = (int)((size - len) / 2);
		stringstream centeredStr;
		for (int i = 0; i < pos; i++)
			centeredStr << " ";
		centeredStr << s;
		int fillOutRemaining = size - pos - len;
		for (int i = 0; i < fillOutRemaining; i++)
			centeredStr << " ";
		return centeredStr.str();
	}

	/*
	Centers a double if needed
	@param value the Double to center
	@param size Size of the "cell" to center
	@param precision Rounds off the double to how many DP
	@return Centered Double String
	*/
	string centerDouble(double value, int size, int precision){
		stringstream ss;
		ss << setprecision(precision) << fixed << value;
		return centerString(ss.str().c_str(), size);
	}

	/*
	Checks if this point should be plotted in a graph or not
	@param size Size of the vector/list
	@param index Current index
	@return Whether it should be plotted or not
	*/
	bool shouldPlot(int size, int index){
		//Case Extreme: Size of Arr > 100000
		if (size >= 100000){
			//If > arrSize - 10, all
			//> arrSize-100, %10
			//> arrSize-1000, %100
			//> 100000, %1000
			//%10000
			if (index > (size - 10)){}
			else if (index < (size - 100) && (index % 10 != 0)) return false;
			else if (index < (size - 1000) && (index % 100 != 0)) return false;
			else if (index > 100000 && (index % 1000 != 0)) return false;
			else if (index % 10000 != 0) return false;
		}
		//Case 1: Size of Arr more than 1000
		if (size > 1000){
			//If > arrSize - 10, all
			//> 1000, %10
			//>100, %50
			//%100
			if (index > (size - 10)){}
			else if (index > 1000 && (index % 10 != 0)) return false;
			else if (index > 100 && (index % 50 != 0)) return false;
			else if (index % 100 != 0) return false;
		}
		//Case 2: Size of Arr Less than 1000 but more than 100
		else if (size > 100){
			//>990, all
			//>900, %5
			//>500, %10
			//>100, %50
			//%100
			if (index > 990) {}
			else if (index > 900 && (index % 5 != 0)) return false;
			else if (index > 500 && (index % 10 != 0)) return false;
			else if (index > 100 && (index % 50 != 0)) return false;
			else if (index % 100 != 0) return false;
		}
		//Case 3: Size of Arr Less than 100
		else {
			//>90, all
			//>50, %5
			//>10, %10
			if (index > 90){}
			else if (index > 50 && (index % 5 != 0)) return false;
			else if (index > 10 && (index % 10 != 0)) return false;
		}
		return true;
	}


}