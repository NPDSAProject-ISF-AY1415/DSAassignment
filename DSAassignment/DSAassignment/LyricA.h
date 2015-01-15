#ifndef LYRICA
#define LYRICA

#include <string>
#include <iostream>
#include <sstream>
#include "ListArray.h"

using namespace std;

class LyricA{
private:
	string tid;
	string mxmid;
	ListArray word;
	ListArray count;
public:
	LyricA();
	LyricA(string tid, string mxmid);
	string getTrackID();
	void setTrackID(string tid);
	string getMusicXMatchID();
	void setMusicXMatchID(string mxmid);
	void addWordAndCount(string word);
	string getWordCount();
};

#endif