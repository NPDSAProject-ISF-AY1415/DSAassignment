#ifndef UTIL
#define UTIL

#include <algorithm>	//Find If
#include <Windows.h>	//Console Utility
#include <ctime>		//Elapsed Time
#include "concol.h"		//Console Colors
#include "Music.h"
#include <iostream>		//String
#include <fstream>		//File Reading
#include <sstream>		//String Stream
#include <iomanip>		//Set precision
#include <string>
#include "GlobalIdentifiers.h"

using namespace std;

namespace utility{
	bool is_number(const string& s);
	void printSeperator();
	void printMenuTitle(string title);
	double calculateElapsed(clock_t start, clock_t end);
	Music parseMusicItem(string music);
	void printAscii();
	void getStringInput(string &storein);
	string centerString(const char* s, int size);
	string centerDouble(double value, int size, int precision);
	bool shouldPlot(int size, int index);
}
#endif