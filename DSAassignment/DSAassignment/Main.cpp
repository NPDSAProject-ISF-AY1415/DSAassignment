#include "Main.h"

using namespace std;
using namespace utility;
using namespace consolecolors;

//Init Vector of Datasets
vector<string> wordDataset(0);
vector<string> songDataset(0);
vector<string> lyricDataset(0);


/* An Inline Progress Bar
@param x Current Index
@param n Total Number of Progress
@param beginClock Start of elapsed time
@param beginPMem Start of PMem
@param beginVMem Start of VMem
@param w Width of the bar
*/
static inline void initFilesLoadbar(unsigned int x, unsigned int n, clock_t beginClock, SIZE_T beginPMem, SIZE_T beginVMem, unsigned int w = 10)
{
	if ((x != n) && (x % (n / 100 + 1) != 0) && n >= 2000) return;

	float ratio = x / (float)n;
	int   c = (int) (ratio * w);

	//Print Progress Bar
	cout << setw(3) << white << "Parsed: " << cyan << x << white << "/" << green << n << yellow << " [" << red;
	for (int x = 0; x < c; x++) cout << "=";
	for (unsigned int x = c; x < w; x++) cout << " ";
	cout << yellow << "] " << (int)(ratio * 100) << "%" << white << " Time: " << cyan << setprecision(2) << fixed << calculateElapsed(beginClock, clock()) << " sec";
	cout << white << " Mem: " << gray << convertMemoryToHumanReadableSht(getPMUsed() - beginPMem) << white << "/" << dark_white << convertMemoryToHumanReadableSht(getVMUsed() - beginVMem);
	//Get Console Cursor Pos
	CONSOLE_SCREEN_BUFFER_INFO SBInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &SBInfo);
	int currentlen = SBInfo.dwCursorPosition.X;
	//Zero out stuff
	for (int i = currentlen - 1; i < CONSOLE_WINDOW_SIZE; i++)
		cout << " ";
	cout << "\r" << flush;
	settextcolor(white);
}

/*
Stores the Lyrics Dataset into a vector
PUSH_BACK Estimated Time: ~82.19 secs (~1 min 22.19 secs)
RESERVE Estimated Time: ~81.66 secs (~1 min 21.66 secs)
*/
void parseLyrics(){
	bool verboseMode = false; //Enable Verbose Mode
	string str;
	int internalCounter = 0, progressCounter = LYRIC_DATASET_LENGTH;
	ifstream file("mxm_dataset_train.txt");
	printMenuTitle("Initializing Song Lyrics...");
	lyricDataset.reserve(LYRIC_DATASET_LENGTH);
	
	//Get Start Memory (Virtual, Physical) and CPU Clock
	clock_t beginClock = clock();
	SIZE_T bVMem = getVMUsed(), bPMem = getPMUsed();

	while (getline(file, str)){
		//Skip Comments
		if (str[0] == '#' || str[0] == '%') continue;
		if (internalCounter >= progressCounter) break;
		lyricDataset.push_back(str);
		initFilesLoadbar(internalCounter, progressCounter, beginClock, bPMem, bVMem);
		internalCounter++;
	}

	initFilesLoadbar(progressCounter, progressCounter, beginClock, bPMem, bVMem);
	cout << endl;
	cout << "Length of Vector: " << lyricDataset.size() << endl;	//DEBUG Code
}

/*
Stores the Song Dataset into a vector
PUSH_BACK Estimated Time: ~73.80 secs (~1 min 13.80 secs)
RESERVE Estimated Time: ~71.06 secs (~1 min 11.06 secs)
*/
void parseSong(){
	bool verboseMode = false; //Enable Verbose Mode
	string str;
	int internalCounter = 0, progressCounter = SONG_DATASET_LENGTH;
	ifstream file("mxm_779k_matches.txt");
	printMenuTitle("Initializing Song Data...");
	songDataset.reserve(SONG_DATASET_LENGTH);

	//Get Start Memory (Virtual, Physical)
	clock_t beginClock = clock();
	SIZE_T bVMem = getVMUsed(), bPMem = getPMUsed();

	while (getline(file, str)){
		if (internalCounter >= progressCounter) break;
		if (verboseMode) cout << str << endl;
		//Ignore Comments
		if (str[0] == '#') continue;
		//Parse Music Details Line
		songDataset.push_back(str);
		initFilesLoadbar(internalCounter, progressCounter, beginClock, bPMem, bVMem);
		internalCounter++;
	}
	initFilesLoadbar(progressCounter, progressCounter, beginClock, bPMem, bVMem);
	cout << endl;
	cout << "Length of Vector: " << songDataset.size() << endl;	//DEBUG Code
}



/*
Stores the Words Dataset into a vector
PUSH_BACK Estimated Time: ~0.34 secs
RESERVE Estimated Time: ~0.32 secs
*/
void parseWords(){
	bool verboseMode = false; //Enable Verbose Mode
	string str;
	int internalCounter = 0, progressCounter = WORD_DATASET_LENGTH;
	ifstream file("mxm_dataset_train.txt");
	printMenuTitle("Initializing Words in Lyric...");
	wordDataset.reserve(WORD_DATASET_LENGTH);

	//Get Start Memory (Virtual, Physical) and Clock for prog bar
	clock_t beginClock = clock();
	SIZE_T bVMem = getVMUsed(), bPMem = getPMUsed();

	while (getline(file, str)){
		//Skip Comments
		if (str[0] == '#') continue;
		//Check if its top words
		if (str[0] == '%'){
			//Parse Top words based on comma
			str.erase(0, 1);
			istringstream ss(str);
			string topwrd;
			while (getline(ss, topwrd, ',')){
				if (verboseMode)
					cout << topwrd << endl;
				wordDataset.push_back(topwrd);	//Add to back of word dataset temp vector
				initFilesLoadbar(internalCounter, progressCounter, beginClock, bPMem, bVMem);
				internalCounter++;
			}
			break;
		}
	}
	initFilesLoadbar(progressCounter, progressCounter, beginClock, bPMem, bVMem);
	cout << endl;
	cout << "Length of Vector: " << wordDataset.size() << endl;	//DEBUG Code
}

//START OF GRAPH GENERATION METHODS

/*
Make a Graph with x axis being the length of the list and the y axis being time taken
*/
void makeAddTimeGraph(){
	vector<Graph> ptrArrList(0);
	if (unsorteddll::addMElapsed == -1)	cout << dark_red << "No data logged for Unsorted Doubly Linked List. Run the add operation to view its data." << endl;
	else {
		Graph udllG("Unsorted Doubly Linked List", unsorteddll::timingAddMCounter.size(), unsorteddll::timingAddMCounter);
		ptrArrList.push_back(udllG);
	}
	if (unsortedptr::addMElapsed == -1) cout << dark_red << "No data logged for Unsorted Pointer-based List. Run the add operation to view its data." << endl;
	else {
		Graph uptrG("Unsorted Pointer-based List", unsortedptr::timingAddMCounter.size(), unsortedptr::timingAddMCounter);
		ptrArrList.push_back(uptrG);
	}
	if (sortedArr::addWElapsed == -1)	cout << dark_red << "No data logged for Sorted Array-Based List. Run the add operation to view its data." << endl;
	else {
		Graph sptrG("Sorted Array-based List", sortedArr::timingAddMCounter.size(), sortedArr::timingAddMCounter);
		ptrArrList.push_back(sptrG);
	}


	if (unsortedArr::addWElapsed == -1)	cout << dark_red << "No data logged for Unsorted Array-Based List. Run the add operation to view its data." << endl;
	else {
		Graph sptrG("Unsorted Array-based List", unsortedArr::timingAddMCounter.size(), unsortedArr::timingAddMCounter);
		ptrArrList.push_back(sptrG);
	}
	if (ptrArrList.size() > 0 && ptrArrList.size() <= 4)	plotGraph(ptrArrList, "Add Operation Comparision (CPU)");
}

/*
Make a Graph with x axis being the length of the list and the y axis being time taken
*/
void makeDisplayTimeGraph(){
	vector<Graph> ptrArrList(0);
	if (unsorteddll::displayMElapsed == -1)	cout << dark_red << "No data logged for Unsorted Doubly Linked List. Run the display operation to view its data." << endl;
	else {
		Graph udllG("Unsorted Doubly Linked List", unsorteddll::timingDisplayMCounter.size(), unsorteddll::timingDisplayMCounter);
		ptrArrList.push_back(udllG);
	}
	if (unsortedptr::displayMElapsed == -1) cout << dark_red << "No data logged for Unsorted Pointer-based List. Run the display operation to view its data." << endl;
	else {
		Graph uptrG("Unsorted Pointer-based List", unsortedptr::timingDisplayMCounter.size(), unsortedptr::timingDisplayMCounter);
		ptrArrList.push_back(uptrG);
	}
	if (sortedArr::displayMElapsed == -1)	cout << dark_red << "No data logged for Sorted Array-Based List. Run the display operation to view its data." << endl;
	else {
		Graph sptrG("Sorted Array-based List", sortedArr::timingDisplayMCounter.size(), sortedArr::timingDisplayMCounter);
		ptrArrList.push_back(sptrG);
	}

	if (unsortedArr::displayMElapsed == -1)	cout << dark_red << "No data logged for Unsorted Array-Based List. Run the display operation to view its data." << endl;
	else {
		Graph sptrG("Unsorted Array-based List", unsortedArr::timingDisplayMCounter.size(), unsortedArr::timingDisplayMCounter);
		ptrArrList.push_back(sptrG);
	}
	if (ptrArrList.size() > 0 && ptrArrList.size() <= 4)	plotGraph(ptrArrList, "Display Operation Comparision (CPU)");
}

/*
Make a Graph with x axis being the length of the list and the y axis being time taken
*/
void makeRemoveTimeGraph(){
	vector<Graph> ptrArrList(0);
	if (unsorteddll::removeElapsed == -1)	cout << dark_red << "No data logged for Unsorted Doubly Linked List. Run the remove operation to view its data." << endl;
	else {
		Graph udllG("Unsorted Doubly Linked List", unsorteddll::timingRemoveMCounter.size(), unsorteddll::timingRemoveMCounter);
		ptrArrList.push_back(udllG);
	}
	if (unsortedptr::removeElapsed == -1) cout << dark_red << "No data logged for Unsorted Pointer-based List. Run the remove operation to view its data." << endl;
	else {
		Graph uptrG("Unsorted Pointer-based List", unsortedptr::timingRemoveMCounter.size(), unsortedptr::timingRemoveMCounter);
		ptrArrList.push_back(uptrG);
	}
	if (sortedArr::removeElapsed == -1)	cout << dark_red << "No data logged for Sorted Array-Based List. Run the remove operation to view its data." << endl;
	else {
		Graph sptrG("Sorted Array-based List", sortedArr::timingRemoveMCounter.size(), sortedArr::timingRemoveMCounter);
		//string wrdGStr = wrdG.createGraphString();
		ptrArrList.push_back(sptrG);
	}

	if (unsortedArr::removeElapsed == -1)	cout << dark_red << "No data logged for Unsorted Array-Based List. Run the remove operation to view its data." << endl;
	else {
		Graph sptrG("Unsorted Array-based List", unsortedArr::timingRemoveMCounter.size(), unsortedArr::timingRemoveMCounter);
		//string wrdGStr = wrdG.createGraphString();
		ptrArrList.push_back(sptrG);
	}
	if (ptrArrList.size() > 0 && ptrArrList.size() <= 4)	plotGraph(ptrArrList, "Remove Operation Comparision (CPU)");
}

/*
Make a Graph with x axis being the length of the list and the y axis being time taken
*/
void makeSeqSearchTimeGraph(){
	vector<Graph> ptrArrList(0);
	if (unsorteddll::sequSearchElapsed == -1)	cout << dark_red << "No data logged for Unsorted Doubly Linked List. Run the seq search operation to view its data." << endl;
	else {
		Graph udllG("Unsorted Doubly Linked List", unsorteddll::timingSeqSearchMCounter.size(), unsorteddll::timingSeqSearchMCounter);
		ptrArrList.push_back(udllG);
	}
	if (unsortedptr::sequSearchElapsed == -1) cout << dark_red << "No data logged for Unsorted Pointer-based List. Run the seq search operation to view its data." << endl;
	else {
		Graph uptrG("Unsorted Pointer-based List", unsortedptr::timingSeqSearchMCounter.size(), unsortedptr::timingSeqSearchMCounter);
		ptrArrList.push_back(uptrG);
	}
	if (sortedArr::sequSearchElapsed == -1)	cout << dark_red << "No data logged for Sorted Array-Based List. Run the seq search operation to view its data." << endl;
	else {
		Graph sptrG("Sorted Array-based List", sortedArr::timingSeqSearchMCounter.size(), sortedArr::timingSeqSearchMCounter);
		//string wrdGStr = wrdG.createGraphString();
		ptrArrList.push_back(sptrG);
	}

	if (unsortedArr::sequSearchElapsed == -1)	cout << dark_red << "No data logged for Unsorted Array-Based List. Run the seq search operation to view its data." << endl;
	else {
		Graph sptrG("Unsorted Array-based List", unsortedArr::timingSeqSearchMCounter.size(), unsortedArr::timingSeqSearchMCounter);
		//string wrdGStr = wrdG.createGraphString();
		ptrArrList.push_back(sptrG);
	}
	if (ptrArrList.size() > 0 && ptrArrList.size() <= 4)	plotGraph(ptrArrList, "Sequential Search Operation Comparision (CPU)");
}

/*
Make a Graph with x axis being the length of the list and the y axis being time taken
*/
void makeBinSearchTimeGraph(){
	vector<Graph> ptrArrList(0);
	if (sortedArr::sequSearchElapsed == -1)	cout << dark_red << "No data logged for Sorted Array-Based List. Run the binary search operation to view its data." << endl;
	else {
		Graph sptrG("Sorted Array-based List", sortedArr::timingBinaSearchMCounter.size(), sortedArr::timingBinaSearchMCounter);
		//string wrdGStr = wrdG.createGraphString();
		ptrArrList.push_back(sptrG);
	}
	if (ptrArrList.size() == 1)	plotGraph(ptrArrList, "Binary Search Operation Comparision (CPU)");
}

/*
Make a Graph with x axis being the length of the list and the y axis being memory use
*/
void makeAddMemGraph(){
	vector<Graph> ptrArrList(0);
	if (unsorteddll::addMPTime == -1)	cout << dark_red << "No data logged for Unsorted Doubly Linked List. Run the add operation to view its data." << endl;
	else{
		Graph lycG1("Unsorted Doubly Linked List (RAM)", unsorteddll::memoryPAddMCounter.size(), unsorteddll::memoryPAddMCounter);
		ptrArrList.push_back(lycG1);
		Graph lycG2("Unsorted Doubly Linked List (Page)", unsorteddll::memoryVAddMCounter.size(), unsorteddll::memoryVAddMCounter);
		ptrArrList.push_back(lycG2);
	}
	if (unsortedptr::addMPTime == -1)	cout << dark_red << "No data logged for Unsorted Pointer-based List. Run the add operation to view its data." << endl;
	else {
		Graph sonG1("Unsorted Pointer-based List (RAM)", unsortedptr::memoryPAddMCounter.size(), unsortedptr::memoryPAddMCounter);
		ptrArrList.push_back(sonG1);
		Graph sonG2("Unsorted Pointer-based List (Page)", unsortedptr::memoryVAddMCounter.size(), unsortedptr::memoryVAddMCounter);
		ptrArrList.push_back(sonG2);
	}
	if (sortedArr::addMPTime == -1)	cout << dark_red << "No data logged for Sorted Array-based List. Run the add operation to view its data." << endl;
	else {
		Graph wrdG1("Sorted Array-based List (RAM)", sortedArr::memoryPAddMCounter.size(), sortedArr::memoryPAddMCounter);
		ptrArrList.push_back(wrdG1);
		Graph wrdG2("Sorted Array-based List (Page)", sortedArr::memoryVAddMCounter.size(), sortedArr::memoryVAddMCounter);
		ptrArrList.push_back(wrdG2);
	}

	if (unsortedArr::addMPTime == -1)	cout << dark_red << "No data logged for Unsorted Array-based List. Run the add operation to view its data." << endl;
	else {
		Graph wrdG1("Unsorted Array-based List (RAM)", unsortedArr::memoryPAddMCounter.size(), unsortedArr::memoryPAddMCounter);
		ptrArrList.push_back(wrdG1);
		Graph wrdG2("Unsorted Array-based List (Page)", unsortedArr::memoryVAddMCounter.size(), unsortedArr::memoryVAddMCounter);
		ptrArrList.push_back(wrdG2);
	}
	if (ptrArrList.size() > 0 && ptrArrList.size() <= 8)	plotGraph(ptrArrList, "Add Operation Comparision (Memory)");
}

/*
Make a Graph with x axis being the length of the list and the y axis being memory use
*/
void makeDisplayMemGraph(){
	vector<Graph> ptrArrList(0);
	if (unsorteddll::displayMPTime == -1)	cout << dark_red << "No data logged for Unsorted Doubly Linked List. Run the display operation to view its data." << endl;
	else{
		Graph lycG1("Unsorted Doubly Linked List (RAM)", unsorteddll::memoryPDisplayMCounter.size(), unsorteddll::memoryPDisplayMCounter);
		ptrArrList.push_back(lycG1);
		Graph lycG2("Unsorted Doubly Linked List (Page)", unsorteddll::memoryVDisplayMCounter.size(), unsorteddll::memoryVDisplayMCounter);
		ptrArrList.push_back(lycG2);
	}
	if (unsortedptr::displayMPTime == -1)	cout << dark_red << "No data logged for Unsorted Pointer-based List. Run the display operation to view its data." << endl;
	else {
		Graph sonG1("Unsorted Pointer-based List (RAM)", unsortedptr::memoryPDisplayMCounter.size(), unsortedptr::memoryPDisplayMCounter);
		ptrArrList.push_back(sonG1);
		Graph sonG2("Unsorted Pointer-based List (Page)", unsortedptr::memoryVDisplayMCounter.size(), unsortedptr::memoryVDisplayMCounter);
		ptrArrList.push_back(sonG2);
	}
	if (sortedArr::displayMPTime == -1)	cout << dark_red << "No data logged for Sorted Array-based List. Run the display operation to view its data." << endl;
	else {
		Graph wrdG1("Sorted Array-based List (RAM)", sortedArr::memoryPDisplayMCounter.size(), sortedArr::memoryPDisplayMCounter);
		ptrArrList.push_back(wrdG1);
		Graph wrdG2("Sorted Array-based List (Page)", sortedArr::memoryVDisplayMCounter.size(), sortedArr::memoryVDisplayMCounter);
		ptrArrList.push_back(wrdG2);
	}

	if (unsortedArr::displayMPTime == -1)	cout << dark_red << "No data logged for Unsorted Array-based List. Run the display operation to view its data." << endl;
	else {
		Graph wrdG1("Unsorted Array-based List (RAM)", unsortedArr::memoryPDisplayMCounter.size(), unsortedArr::memoryPDisplayMCounter);
		ptrArrList.push_back(wrdG1);
		Graph wrdG2("Unsorted Array-based List (Page)", unsortedArr::memoryVDisplayMCounter.size(), unsortedArr::memoryVDisplayMCounter);
		ptrArrList.push_back(wrdG2);
	}
	if (ptrArrList.size() > 0 && ptrArrList.size() <= 8)	plotGraph(ptrArrList, "Display Operation Comparision (Memory)");
}

/*
Make a Graph with x axis being the length of the list and the y axis being memory use
*/
void makeRemoveMemGraph(){
	vector<Graph> ptrArrList(0);
	if (unsorteddll::removePTime == -1)	cout << dark_red << "No data logged for Unsorted Doubly Linked List. Run the remove operation to view its data." << endl;
	else{
		Graph lycG1("Unsorted Doubly Linked List (RAM)", unsorteddll::memoryPRemoveMCounter.size(), unsorteddll::memoryPRemoveMCounter);
		ptrArrList.push_back(lycG1);
		Graph lycG2("Unsorted Doubly Linked List (Page)", unsorteddll::memoryVRemoveMCounter.size(), unsorteddll::memoryVRemoveMCounter);
		ptrArrList.push_back(lycG2);
	}
	if (unsortedptr::removePTime == -1)	cout << dark_red << "No data logged for Unsorted Pointer-based List. Run the remove operation to view its data." << endl;
	else {
		Graph sonG1("Unsorted Pointer-based List (RAM)", unsortedptr::memoryPRemoveMCounter.size(), unsortedptr::memoryPRemoveMCounter);
		ptrArrList.push_back(sonG1);
		Graph sonG2("Unsorted Pointer-based List (Page)", unsortedptr::memoryVRemoveMCounter.size(), unsortedptr::memoryVRemoveMCounter);
		ptrArrList.push_back(sonG2);
	}
	if (sortedArr::removePTime == -1)	cout << dark_red << "No data logged for Sorted Array-based List. Run the remove operation to view its data." << endl;
	else {
		Graph wrdG1("Sorted Array-based List (RAM)", sortedArr::memoryPRemoveMCounter.size(), sortedArr::memoryPRemoveMCounter);
		ptrArrList.push_back(wrdG1);
		Graph wrdG2("Sorted Array-based List (Page)", sortedArr::memoryVRemoveMCounter.size(), sortedArr::memoryVRemoveMCounter);
		ptrArrList.push_back(wrdG2);
	}

	if (unsortedArr::removePTime == -1)	cout << dark_red << "No data logged for Unsorted Array-based List. Run the remove operation to view its data." << endl;
	else {
		Graph wrdG1("Unsorted Array-based List (RAM)", unsortedArr::memoryPRemoveMCounter.size(), unsortedArr::memoryPRemoveMCounter);
		ptrArrList.push_back(wrdG1);
		Graph wrdG2("Unsorted Array-based List (Page)", unsortedArr::memoryVRemoveMCounter.size(), unsortedArr::memoryVRemoveMCounter);
		ptrArrList.push_back(wrdG2);
	}
	if (ptrArrList.size() > 0 && ptrArrList.size() <= 8)	plotGraph(ptrArrList, "Remove Operation Comparision (Memory)");
}

/*
Make a Graph with x axis being the length of the list and the y axis being memory use
*/
void makeSeqSearchMemGraph(){
	vector<Graph> ptrArrList(0);
	if (unsorteddll::sequSearchPTime == -1)	cout << dark_red << "No data logged for Unsorted Doubly Linked List. Run the seq search operation to view its data." << endl;
	else{
		Graph lycG1("Unsorted Doubly Linked List (RAM)", unsorteddll::memoryPSeqSearchMCounter.size(), unsorteddll::memoryPSeqSearchMCounter);
		ptrArrList.push_back(lycG1);
		Graph lycG2("Unsorted Doubly Linked List (Page)", unsorteddll::memoryVSeqSearchMCounter.size(), unsorteddll::memoryVSeqSearchMCounter);
		ptrArrList.push_back(lycG2);
	}
	if (unsortedptr::sequSearchPTime == -1)	cout << dark_red << "No data logged for Unsorted Pointer-based List. Run the seq search operation to view its data." << endl;
	else {
		Graph sonG1("Unsorted Pointer-based List (RAM)", unsortedptr::memoryPSeqSearchMCounter.size(), unsortedptr::memoryPSeqSearchMCounter);
		ptrArrList.push_back(sonG1);
		Graph sonG2("Unsorted Pointer-based List (Page)", unsortedptr::memoryVSeqSearchMCounter.size(), unsortedptr::memoryVSeqSearchMCounter);
		ptrArrList.push_back(sonG2);
	}
	if (sortedArr::sequSearchPTime == -1)	cout << dark_red << "No data logged for Sorted Array-based List. Run the seq search operation to view its data." << endl;
	else {
		Graph wrdG1("Sorted Array-based List (RAM)", sortedArr::memoryPSeqSearchMCounter.size(), sortedArr::memoryPSeqSearchMCounter);
		ptrArrList.push_back(wrdG1);
		Graph wrdG2("Sorted Array-based List (Page)", sortedArr::memoryVSeqSearchMCounter.size(), sortedArr::memoryVSeqSearchMCounter);
		ptrArrList.push_back(wrdG2);
	}

	if (unsortedArr::sequSearchPTime == -1)	cout << dark_red << "No data logged for Unsorted Array-based List. Run the seq search operation to view its data." << endl;
	else {
		Graph wrdG1("Unsorted Array-based List (RAM)", unsortedArr::memoryPSeqSearchMCounter.size(), unsortedArr::memoryPSeqSearchMCounter);
		ptrArrList.push_back(wrdG1);
		Graph wrdG2("Unsorted Array-based List (Page)", unsortedArr::memoryVSeqSearchMCounter.size(), unsortedArr::memoryVSeqSearchMCounter);
		ptrArrList.push_back(wrdG2);
	}
	if (ptrArrList.size() > 0 && ptrArrList.size() <= 8)	plotGraph(ptrArrList, "Sequential Search Operation Comparision (Memory)");
}

/*
Make a Graph with x axis being the length of the list and the y axis being memory use
*/
void makeBinSearchMemGraph(){
	vector<Graph> ptrArrList(0);
	if (sortedArr::sequSearchPTime == -1)	cout << dark_red << "No data logged for Sorted Array-based List. Run the binary search operation to view its data." << endl;
	else {
		Graph wrdG1("Sorted Array-based List (RAM)", sortedArr::memoryPBinaSearchMCounter.size(), sortedArr::memoryPBinaSearchMCounter);
		ptrArrList.push_back(wrdG1);
		Graph wrdG2("Sorted Array-based List (Page)", sortedArr::memoryVBinaSearchMCounter.size(), sortedArr::memoryVBinaSearchMCounter);
		ptrArrList.push_back(wrdG2);
	}
	if (ptrArrList.size() == 2)	plotGraph(ptrArrList, "Binary Search Operation Comparision (Memory)");
}
//END OF GRAPH GENERATION METHODS

//TABLE GENERATION METHODS
void doAddTable(){
	vector<Graph> tblVec(0);
	Graph dblnkPtr("DOUBLY LINKED", unsorteddll::timingAddMCounter, unsorteddll::memoryPAddMCounter);
	tblVec.push_back(dblnkPtr);
	Graph unsrtPtr("UNSORTED PTR", unsortedptr::timingAddMCounter, unsortedptr::memoryPAddMCounter);
	tblVec.push_back(unsrtPtr);
	Graph srtArr("SORTED ARR", sortedArr::timingAddMCounter, sortedArr::memoryPAddMCounter);
	tblVec.push_back(srtArr);
	Graph unsrtArr("UNSORTED ARR", unsortedArr::timingAddMCounter, unsortedArr::memoryPAddMCounter);
	tblVec.push_back(unsrtArr);
	plotTable(tblVec, "ADD");
}

void doDisplayTable(){
	vector<Graph> tblVec(0);
	Graph dblnkPtr("DOUBLY LINKED", unsorteddll::timingDisplayMCounter, unsorteddll::memoryPDisplayMCounter);
	tblVec.push_back(dblnkPtr);
	Graph unsrtPtr("UNSORTED PTR", unsortedptr::timingDisplayMCounter, unsortedptr::memoryPDisplayMCounter);
	tblVec.push_back(unsrtPtr);
	Graph srtArr("SORTED ARR", sortedArr::timingDisplayMCounter, sortedArr::memoryPDisplayMCounter);
	tblVec.push_back(srtArr);
	Graph unsrtArr("UNSORTED ARR", unsortedArr::timingDisplayMCounter, unsortedArr::memoryPDisplayMCounter);
	tblVec.push_back(unsrtArr);
	plotTable(tblVec, "DISPLAY");
}

void doRemoveTable(){
	vector<Graph> tblVec(0);
	Graph dblnkPtr("DOUBLY LINKED", unsorteddll::timingRemoveMCounter, unsorteddll::memoryPRemoveMCounter);
	tblVec.push_back(dblnkPtr);
	Graph unsrtPtr("UNSORTED PTR", unsortedptr::timingRemoveMCounter, unsortedptr::memoryPRemoveMCounter);
	tblVec.push_back(unsrtPtr);
	Graph srtArr("SORTED ARR", sortedArr::timingRemoveMCounter, sortedArr::memoryPRemoveMCounter);
	tblVec.push_back(srtArr);
	Graph unsrtArr("UNSORTED ARR", unsortedArr::timingRemoveMCounter, unsortedArr::memoryPRemoveMCounter);
	tblVec.push_back(unsrtArr);
	plotTable(tblVec, "REMOVE");
}

void doSeqSearchTable(){
	vector<Graph> tblVec(0);
	Graph dblnkPtr("DOUBLY LINKED", unsorteddll::timingSeqSearchMCounter, unsorteddll::memoryPSeqSearchMCounter);
	tblVec.push_back(dblnkPtr);
	Graph unsrtPtr("UNSORTED PTR", unsortedptr::timingSeqSearchMCounter, unsortedptr::memoryPSeqSearchMCounter);
	tblVec.push_back(unsrtPtr);
	Graph srtArr("SORTED ARR", sortedArr::timingSeqSearchMCounter, sortedArr::memoryPSeqSearchMCounter);
	tblVec.push_back(srtArr);
	Graph unsrtArr("UNSORTED ARR", unsortedArr::timingSeqSearchMCounter, unsortedArr::memoryPSeqSearchMCounter);
	tblVec.push_back(unsrtArr);
	plotTable(tblVec, "SEQ SEARCH");
}

void doBinSearchTable(){
	vector<Graph> tblVec(0);
	//Only Sorted Array has Binary Search
	Graph srtArr("SORTED ARR", sortedArr::timingBinaSearchMCounter, sortedArr::memoryPBinaSearchMCounter);
	tblVec.push_back(srtArr);
	plotTable(tblVec, "BIN SEARCH");
}

//END OF TABLE GENERATION METHODS

/*
Plot Main Graph Menu
*/
void plotGraphMenu(){
	printMenuTitle("Select a Song Data graph to view");
	cout << "1) " << yellow << "Add (CPU)" << white << endl;
	cout << "2) " << yellow << "Add (Memory)" << white << endl;
	cout << "3) " << yellow << "Display (CPU)" << white << endl;
	cout << "4) " << yellow << "Display (Memory)" << white << endl;
	cout << "5) " << yellow << "Remove (CPU)" << white << endl;
	cout << "6) " << yellow << "Remove (Memory)" << white << endl;
	cout << "7) " << yellow << "Sequential Search (CPU)" << white << endl;
	cout << "8) " << yellow << "Sequential Search (Memory)" << white << endl;
	cout << "9) " << yellow << "Binary Search (CPU)" << white << endl;
	cout << "10) " << yellow << "Binary Search (Memory)" << white << endl;
	cout << "0) " << yellow << "Return to Menu" << white << endl;
	string selection;
	cout << pink << "Select an option: ";
	getStringInput(selection);
	if (is_number(selection)){
		switch (stoi(selection))
		{
		case 1: makeAddTimeGraph(); break;
		case 2: makeAddMemGraph(); break;
		case 3: makeDisplayTimeGraph(); break;
		case 4: makeDisplayMemGraph(); break;
		case 5: makeRemoveTimeGraph(); break;
		case 6: makeRemoveMemGraph(); break;
		case 7: makeSeqSearchTimeGraph(); break;
		case 8: makeSeqSearchMemGraph(); break;
		case 9: makeBinSearchTimeGraph(); break;
		case 10: makeBinSearchMemGraph(); break;
		case 0: return;
		default: cout << dark_red << "Invalid Selection." << endl; break;
		}
	}
	else { cout << dark_red << "Selection must be an integer" << endl; }
}

/*
Plot Main Tabulation Table Menu
*/
void plotTableMenu(){
	printMenuTitle("Select a Song Data Table to print");
	cout << "1) " << yellow << "Add" << white << endl;
	cout << "2) " << yellow << "Display" << white << endl;
	cout << "3) " << yellow << "Remove" << white << endl;
	cout << "4) " << yellow << "Sequential Search" << white << endl;
	cout << "5) " << yellow << "Binary Search" << white << endl;
	cout << "0) " << yellow << "Return to Menu" << white << endl;
	string selection;
	cout << pink << "Select an option: ";
	getStringInput(selection);
	if (is_number(selection)){
		switch (stoi(selection))
		{
		case 1: doAddTable(); break;
		case 2: doDisplayTable(); break;
		case 3: doRemoveTable(); break;
		case 4: doSeqSearchTable(); break;
		case 5: doBinSearchTable(); break;
		case 0: return;
		default: cout << dark_red << "Invalid Selection." << endl; break;
		}
	}
	else { cout << dark_red << "Selection must be an integer" << endl; }
}

/*
Calls all the 3 methods to load the dataset into the vectors
PUSH_BACK Estimated Time: ~156.33 secs (~2 mins 36.33 secs)
RESERVE Estimated Time: ~153.04 secs (~2 mins 33.04 secs)
*/
void initFileParse(){
	printMemoryInfo();
	printMenuTitle("Initializing and loading text files to temp vector buffer");
	cout << magenta << "It will take about 2.5 minutes to load the data into the vectors..." << endl << "Please wait..." << endl;
	parseSong();
	parseLyrics();
	parseWords();
	printMenuTitle("Load Completed");
	cout << endl;
	printMemoryInfo();
}

/*
Prints out the main menu
*/
void mainMenu(){
	printMenuTitle("Main Menu (Select a storage type)");
	cout << "1) " << yellow << "Unsorted Double Linked List" << white << endl;
	cout << "2) " << yellow << "Unsorted Pointer-Based List" << white << endl;
	cout << "3) " << yellow << "Unsorted Array-Based List" << white << endl;
	cout << "4) " << yellow << "Sorted Array-Based List" << white << endl;
	cout << "7) " << yellow << "Print Tabulation Table" << white << endl;
	cout << "8) " << yellow << "Plot Performance Graphs for all lists" << white << endl;
	cout << "9) " << yellow << "Memory Usage" << white << endl;
	//cout << "9) " << yellow << "(Test) Initialize data into vector datasets" << white << endl;
	cout << "0) " << yellow << "Quit" << white << endl;
}


/*
Main Method
@return End Error Code
*/
int main(){
	//Initialization
	SetConsoleTitle(TEXT("Data Structures Performance Analysis (DSA Assignment)"));
	concolinit();
	initFileParse();

	printAscii();
	cout << endl;
	//This will be used to check if the array pointers returned an error or not
	//-1 means no error but continue looping, 0 means no error but exit program, anything else = error
	int menErr = -1;
	while (menErr == -1){
		mainMenu();
		string selection;
		cout << pink << "Select an option: ";
		getStringInput(selection);
		if (is_number(selection)){
			switch (stoi(selection))
			{
			case 1: menErr = unsorteddll::mainLoop(); break;
			case 2: menErr = unsortedptr::mainLoop(); break;
			case 3: menErr = unsortedArr::mainLoop(); break;
			case 4: menErr =  sortedArr::mainLoop(); break;	
			case 7: plotTableMenu(); break;
			case 8: plotGraphMenu(); break;
			case 9: printMemoryInfo(); break;
			case 0: cout << green << "Freeing Memory taken by variables and containers. This may take a while..." << white << endl; 
				return 0;
			default: cout << dark_red << "Invalid Selection." << endl; break;
			}
		}
		else { cout << dark_red << "Selection must be an integer" << endl; }
	}
	cout << green << "Freeing Memory taken by variables and containers. This may take a while..." << white << endl;
	return menErr;
}