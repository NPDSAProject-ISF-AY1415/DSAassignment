#ifndef UNSORT_PTR_LIST
#define UNSORT_PTR_LIST

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
#include "Memory.h"
#include "Main.h"
#include <iomanip>		//std::setw
#include <sstream>		//String Stream
#include <vector>		//Vector
#include "GlobalIdentifiers.h"

namespace unsortedptr {
	extern int musicInfoFileLength;
	extern double addMElapsed, addWElapsed, addLElapsed, displayMElapsed, displayWElapsed, sequSearchElapsed;
	extern double removeElapsed;

	extern SIZE_T addMVTime, addWVTime, addLVTime, displayMVTime, displayWVTime, sequSearchVTime, removeVTime;
	extern SIZE_T addMPTime, addWPTime, addLPTime, displayMPTime, displayWPTime, sequSearchPTime, removePTime;

	extern vector<double> timingAddMCounter;
	extern vector<double> timingAddLCounter;
	extern vector<double> timingAddWCounter;
	extern vector<double> timingDisplayMCounter;
	extern vector<double> timingSeqSearchMCounter;
	extern vector<double> timingRemoveMCounter;
	extern vector<double> timingDisplayWCounter;
	extern vector<double> memoryPAddMCounter;
	extern vector<double> memoryPAddLCounter;
	extern vector<double> memoryPAddWCounter;
	extern vector<double> memoryPDisplayMCounter;
	extern vector<double> memoryPSeqSearchMCounter;
	extern vector<double> memoryPRemoveMCounter;
	extern vector<double> memoryPDisplayWCounter;
	extern vector<double> memoryVAddMCounter;
	extern vector<double> memoryVAddLCounter;
	extern vector<double> memoryVAddWCounter;
	extern vector<double> memoryVDisplayMCounter;
	extern vector<double> memoryVSeqSearchMCounter;
	extern vector<double> memoryVRemoveMCounter;
	extern vector<double> memoryVDisplayWCounter;

	static inline void loadbar(unsigned int x, unsigned int n, clock_t beginClock, SIZE_T beginPMem, SIZE_T beginVMem, unsigned int w);
	Lyric parseLyricData(string lyricStr);
	void parseFiles(ListPointer &musInfoList, ListPointer &wordList, ListPointer &lyricList);
	void readTopWords(ListPointer &list);
	void readMatchFile(ListPointer &list, int count);
	void readSongLyricCount(ListPointer &list, int count);
	void parseNewFiles(ListPointer &musInfoList, ListPointer &wordList, ListPointer &lyricList);
	void readNewTopWords(ListPointer &list);
	void readNewMatchFile(ListPointer &list, int count);
	void readNewSongLyricCount(ListPointer &list, int count);
	void mainPtrArrMenu();
	void searchSong(ListPointer &list);
	void listAllSongs(ListPointer &list);
	void listTopWords(ListPointer &list);
	void removeMusicInfo(ListPointer &list);
	void printStats();
	void makeAddTimeGraph();
	void makeDisplayTimeGraph();
	void makeRemoveTimeGraph();
	void makeSeqSearchTimeGraph();
	void plotGraphMenu();
	void performanceMenu();
	int mainLoop();
}

#endif