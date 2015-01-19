#ifndef UNSORTED_DOUBLE_LINKED_LIST
#define UNSORTED_DOUBLE_LINKED_LIST

#include <iostream>		//String
#include <fstream>		//File reading
#include <Windows.h>	//Console Utility
#include <ctime>		//Elapsed Time
#include "concol.h"		//Console Colors
#include "DoubleLinkedList.h"
#include "Music.h"
#include "Utility.h"
#include "LyricDLL.h"
#include "Graph.h"
#include "plotGraph.h"
#include "Memory.h"
#include "Main.h"
#include <iomanip>		//std::setw
#include <sstream>		//String Stream
#include <vector>		//Vector
#include "GlobalIdentifiers.h"

namespace unsorteddll{
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
	void parseLyricData(string &lyricStr, LyricDLL *l);
	void parseFiles(DoubleLinkedList &musInfoList, DoubleLinkedList &wordList, DoubleLinkedList &lyricList);
	void readTopWords(DoubleLinkedList &list);
	void readMatchFile(DoubleLinkedList &list, int count);
	void readSongLyricCount(DoubleLinkedList &list, int count);
	void parseNewFiles(DoubleLinkedList &musInfoList, DoubleLinkedList &wordList, DoubleLinkedList &lyricList, bool findLyrics);
	void readNewTopWords(DoubleLinkedList &list);
	void readNewMatchFile(DoubleLinkedList &list, int count);
	void readNewSongLyricCount(DoubleLinkedList &list, int count);
	void mainPtrArrMenu();
	void getLyrics(Music &musicInfo, DoubleLinkedList &words, DoubleLinkedList &lyrics);
	void searchSong(DoubleLinkedList &list, DoubleLinkedList &wrdList, DoubleLinkedList &lyricList);
	void listAllSongs(DoubleLinkedList &list);
	void listTopWords(DoubleLinkedList &list);
	void removeMusicInfo(DoubleLinkedList &list);
	void printStats();
	void makeAddTimeGraph();
	void makeDisplayTimeGraph();
	void makeRemoveTimeGraph();
	void makeSeqSearchTimeGraph();
	void makeAddMemGraph();
	void makeDisplayMemGraph();
	void makeRemoveMemGraph();
	void makeSeqSearchMemGraph();
	void plotGraphMenu();
	void performanceMenu();
	int mainLoop();
}
#endif