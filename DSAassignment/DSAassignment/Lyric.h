#ifndef LYRIC
#define LYRIC

#include <string>
#include <iostream>
#include <sstream>
#include "ListPointer.h"

using namespace std;

class Lyric{
private:
	string tid;
	string mxmid;
	ListPointer word;
	ListPointer count;
public:
	Lyric();
	Lyric(string tid, string mxmid);
	string getTrackID();
	void setTrackID(string tid);
	string getMusicXMatchID();
	void setMusicXMatchID(string mxmid);
	void addWordAndCount(string word);
	string getWordCount();
};

#endif