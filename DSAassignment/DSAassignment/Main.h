#include <Windows.h>	//Console Utility
#include <iostream>
#include <vector>
#include "Utility.h"
#include "UnsortedPtr.h"
#include "sortedArr.h"
#include "Memory.h"
#include "GlobalIdentifiers.h"
#include "DblLinkList.h"
#include "Table.h"

extern vector<string> wordDataset, songDataset, lyricDataset;

static inline void initFilesLoadbar(unsigned int x, unsigned int n, clock_t beginClock, SIZE_T beginPMem, SIZE_T beginVMem, unsigned int w);
void parseLyrics();
void parseSong();
void parseWords();
void initFileParse();
void mainMenu();
int main();