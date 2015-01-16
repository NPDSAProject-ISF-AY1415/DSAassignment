#include "Main.h"

using namespace std;
using namespace utility;
using namespace eku;

//Init Vector of Datasets
vector<string> wordDataset(WORD_DATASET_LENGTH);
vector<string> songDataset(SONG_DATASET_LENGTH);
vector<string> lyricDataset(LYRIC_DATASET_LENGTH);

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

	//Get Elapsed Time and memory used
	double elapsedSec = calculateElapsed(beginClock, clock());
	SIZE_T pMem = (getPMUsed() - beginPMem);
	SIZE_T vMem = (getVMUsed() - beginVMem);

	float ratio = x / (float)n;
	int   c = ratio * w;

	//Print Progress Bar
	cout << setw(3) << white << "Parsed: " << cyan << x << white << "/" << green << n << yellow << " [" << red;
	for (int x = 0; x < c; x++) cout << "=";
	for (int x = c; x < w; x++) cout << " ";
	cout << yellow << "] " << (int)(ratio * 100) << "%" << white << " Time: " << cyan << setprecision(2) << fixed << elapsedSec << " sec";
	cout << white << " Mem: " << gray << convertMemoryToHumanReadableSht(pMem) << white << "/" << dark_white << convertMemoryToHumanReadableSht(vMem);
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

void parseLyrics(){
	bool verboseMode = false; //Enable Verbose Mode
	string str;
	int internalCounter = 0, progressCounter = LYRIC_DATASET_LENGTH;
	ifstream file("mxm_dataset_train.txt");
	printMenuTitle("Initializing Song Lyrics...");
	
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
}

void parseSong(){
	bool verboseMode = false; //Enable Verbose Mode
	string str;
	int internalCounter = 0, progressCounter = SONG_DATASET_LENGTH;
	ifstream file("mxm_779k_matches.txt");
	printMenuTitle("Initializing Song Data...");

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
}

void parseWords(){
	bool verboseMode = false; //Enable Verbose Mode
	string str;
	int internalCounter = 0, progressCounter = WORD_DATASET_LENGTH;
	ifstream file("mxm_dataset_train.txt");
	printMenuTitle("Initializing Lyric Word Count...");

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
}

void initFileParse(){
	printMemoryInfo();
	printMenuTitle("Initializing and loading text files to temp buffer");
	parseLyrics();
	parseSong();
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
	settextcolor(white);
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
			case 0: return 0;
				//case 4: mainList.print(); break;
			default: cout << dark_red << "Invalid Selection." << endl; break;
			}
		}
		else {
			cout << dark_red << "Selection must be an integer" << endl;
		}
	}

	return menErr;
}