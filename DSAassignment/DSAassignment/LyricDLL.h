#ifndef LYRICDLL
#define LYRICDLL

#include <string>
#include <iostream>
#include <sstream>
#include "DoubleLinkedList.h"

using namespace std;

class LyricDLL{
private:
	string tid;
	string mxmid;
	DoubleLinkedList word;
	DoubleLinkedList count;
public:
	LyricDLL();
	LyricDLL(string tid, string mxmid);
	string getTrackID();
	void setTrackID(string tid);
	string getMusicXMatchID();
	void setMusicXMatchID(string mxmid);
	void addWordAndCount(string word);
	string getWordCount();
	DoubleLinkedList getWords();
	DoubleLinkedList getCounts();
};

#endif