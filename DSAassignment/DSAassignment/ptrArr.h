#include <iostream>		//String
#include <fstream>		//File reading
#include <Windows.h>	//Console Utility
#include <ctime>		//Elapsed Time
#include "concol.h"		//Console Colors
#include "ListPointer.h"
#include "Music.h"
#include "Utility.h"
#include "Lyric.h"
#include "Graph.h"
#include "plotGraph.h"
#include <iomanip>		//std::setw
#include <sstream>		//String Stream

namespace ptrarr {
	extern int musicInfoFileLength;
	extern double addMElapsed, addWElapsed, addLElapsed, displayMElapsed, displayWElapsed, sequSearchElapsed;
	extern double removeElapsed;
	static inline void loadbar(unsigned int x, unsigned int n, clock_t beginClock, unsigned int w = 25);
	Lyric parseLyricData(string lyricStr);
	void parseFiles(ListPointer &musInfoList, ListPointer &wordList, ListPointer &lyricList);
	void readTopWords(ListPointer &list);
	void readMatchFile(ListPointer &list, int count);
	void readSongLyricCount(ListPointer &list, int count);
	void mainPtrArrMenu();
	void searchSong(ListPointer &list);
	void listAllSongs(ListPointer &list);
	void listTopWords(ListPointer &list);
	void removeMusicInfo(ListPointer &list);
	void printStats();
	void makeGraph();
	int mainLoop();
}