#ifndef LYRIC
#define LYRIC

#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include "ListPointer.h"

using namespace std;

class Lyric{
private:
	string tid;
	string mxmid;
	vector<string> word;
	vector<string> count;
public:
	Lyric();
	Lyric(string tid, string mxmid);
	string getTrackID();
	void setTrackID(string tid);
	string getMusicXMatchID();
	void setMusicXMatchID(string mxmid);
	void addWordAndCount(string word);
	vector<string> getWords();
	vector<string> getCounts();
};

#endif