#include "Main.h"

using namespace std;
using namespace utility;
using namespace eku;

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
	int   c = ratio * w;

	//Print Progress Bar
	cout << setw(3) << white << "Parsed: " << cyan << x << white << "/" << green << n << yellow << " [" << red;
	for (int x = 0; x < c; x++) cout << "=";
	for (int x = c; x < w; x++) cout << " ";
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

/*
Calls all the 3 methods to load the dataset into the vectors
PUSH_BACK Estimated Time: ~156.33 secs (~2 mins 36.33 secs)
RESERVE Estimated Time: ~153.04 secs (~2 mins 33.04 secs)
*/
void initFileParse(){
	printMemoryInfo();
	printMenuTitle("Initializing and loading text files to temp buffer");
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
	cout << "1) " << yellow << "Unsorted Pointer-Based List" << white << endl;
	cout << "2) " << yellow << "Sorted Pointer-Based List" << white << endl;
	cout << "3) " << yellow << "Unsorted Array-Based List" << white << endl;
	cout << "4) " << yellow << "Sorted Array-Based List" << white << endl;
	cout << "5) " << yellow << "Performance Utilities" << white << endl;
	cout << "6) " << yellow << "Plot Performance Graphs" << white << endl;
	cout << "9) " << yellow << "(Test) Initialize data into vector datasets" << white << endl;
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
			case 1: menErr = unsortedptr::mainLoop(); break;
			case 2: cout << dark_red << "Soon to be implemented" << endl; break;	//TODO Replace with main loop for sorted pointer
			case 3: cout << dark_red << "Soon to be implemented" << endl; break;	//TODO Replace with main loop for unsorted array
			case 4: cout << dark_red << "Soon to be implemented" << endl; break;	//TODO Replace with main loop for sorted array
			case 5: unsortedptr::performanceMenu(); break;		//Temp, soon will be a menu specifically for main menu
			case 6: unsortedptr::plotGraphMenu(); break;			//Temp, soon will be a menu designed for main menu
			case 9: initFileParse(); break;		//Test Vectors speed as per LCY suggestion
			case 0: cout << green << "Freeing Memory taken by variables and containers. This may take a while..." << white << endl; 
				return 0;
			default: cout << dark_red << "Invalid Selection." << endl; break;
			}
		}
		else { cout << dark_red << "Selection must be an integer" << endl; }
	}

	return menErr;
}