#ifndef Sorted_Arr_LIST
#define Sorted_Arr_LIST

#include <iostream>		//String
#include <fstream>		//File reading
#include <Windows.h>	//Console Utility
#include <ctime>		//Elapsed Time
#include "concol.h"		//Console Colors
#include "ListArray.h"
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

namespace sortedArr {
	extern int musicInfoFileLength;
	extern double addMElapsed, addWElapsed, addLElapsed, displayMElapsed, displayWElapsed, sequSearchElapsed, binaSearchElapsed;
	extern double removeElapsed;

	extern SIZE_T addMVTime, addWVTime, addLVTime, displayMVTime, displayWVTime, sequSearchVTime, binaSearchVTime, removeVTime;
	extern SIZE_T addMPTime, addWPTime, addLPTime, displayMPTime, displayWPTime, sequSearchPTime, binaSearchPTime, removePTime;

	extern vector<double> timingAddMCounter;
	extern vector<double> timingAddLCounter;
	extern vector<double> timingAddWCounter;
	extern vector<double> timingDisplayMCounter;
	extern vector<double> timingSeqSearchMCounter;
	extern vector<double> timingBinaSeachMCounter;
	extern vector<double> timingRemoveMCounter;
	extern vector<double> timingDisplayWCounter;
	extern vector<double> timingBinaSearchMCounter;
	extern vector<double> memoryPAddMCounter;
	extern vector<double> memoryPAddLCounter;
	extern vector<double> memoryPAddWCounter;
	extern vector<double> memoryPDisplayMCounter;
	extern vector<double> memoryPSeqSearchMCounter;
	extern vector<double> memoryPBinaSearchMCounter;
	extern vector<double> memoryPRemoveMCounter;
	extern vector<double> memoryPDisplayWCounter;
	extern vector<double> memoryVAddMCounter;
	extern vector<double> memoryVAddLCounter;
	extern vector<double> memoryVAddWCounter;
	extern vector<double> memoryVDisplayMCounter;
	extern vector<double> memoryVSeqSearchMCounter;
	extern vector<double> memoryVBinaSearchMCounter;
	extern vector<double> memoryVRemoveMCounter;
	extern vector<double> memoryVDisplayWCounter;

	static inline void loadbar(unsigned int x, unsigned int n, clock_t beginClock, SIZE_T beginPMem, SIZE_T beginVMem, unsigned int);
	Lyric parseLyricData(string lyricStr);
	void parseFiles(ListArray &musInfoList, ListArray &wordList, ListArray &lyricList, bool soUn);
	void readTopWords(ListArray &list);
	void readMatchFile(ListArray &list, int count);
	void readMatchFile2(ListArray &list, int count);
	void readSongLyricCount(ListArray &list, int count);
	void mainPtrArrMenu();
	void searchSong(ListArray &list);
	void listAllSongs(ListArray &list);
	void listTopWords(ListArray &list);
	void removeMusicInfo(ListArray &list);
	void printStats();
	void makeAddTimeGraph();
	void makeDisplayTimeGraph();
	void makeRemoveTimeGraph();
	void makeSeqSearchTimeGraph();
	void makeBinaSearchTimeGraph();
	void makeAddMemGraph();
	void makeDisplayMemGraph();
	void makeRemoveMemGraph();
	void makeSeqSearchMemGraph();
	void makeBinaSearchMemGraph();
	void plotGraphMenu();
	void performanceMenu();
	int mainLoop();
}

#endif