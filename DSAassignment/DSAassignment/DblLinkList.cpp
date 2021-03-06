/*
DblLinkList.cpp
Class File for Unsorted Doubly Linked List Implementation
Team Name: Wat.jpg
Team Member 1: Soh Yan Quan, Kenneth (S10147249G)
Team Member 2: Soh Hak Wee (S10146869K)
Features to Highlight: Add into list, Display list, Remove list, Sequential Search
*/

#include "DblLinkList.h"

using namespace std;
using namespace consolecolors;
using namespace utility;

namespace unsorteddll{
	int musicInfoFileLength = 779074;
	//All the counters
	double addMElapsed = -1, addWElapsed = -1, addLElapsed = -1, displayMElapsed = -1, displayWElapsed = -1, sequSearchElapsed = -1;
	double removeElapsed = -1;

	//Memory Counters
	SIZE_T addMVTime = -1, addWVTime = -1, addLVTime = -1, displayMVTime = -1, displayWVTime = -1, sequSearchVTime = -1, removeVTime = -1;	//Virtual Mem
	SIZE_T addMPTime = -1, addWPTime = -1, addLPTime = -1, displayMPTime = -1, displayWPTime = -1, sequSearchPTime = -1, removePTime = -1;	//Physical Mem

	//Array Logger
	vector<double> timingAddWCounter(0);
	vector<double> memoryPAddWCounter(0);
	vector<double> memoryVAddWCounter(0);
	vector<double> timingDisplayWCounter(0);
	vector<double> memoryPDisplayWCounter(0);
	vector<double> memoryVDisplayWCounter(0);

	vector<double> timingAddMCounter(0);
	vector<double> memoryPAddMCounter(0);
	vector<double> memoryVAddMCounter(0);

	vector<double> timingAddLCounter(0);
	vector<double> memoryPAddLCounter(0);
	vector<double> memoryVAddLCounter(0);

	vector<double> timingDisplayMCounter(0);
	vector<double> memoryPDisplayMCounter(0);
	vector<double> memoryVDisplayMCounter(0);

	vector<double> timingSeqSearchMCounter(0);
	vector<double> memoryPSeqSearchMCounter(0);
	vector<double> memoryVSeqSearchMCounter(0);

	vector<double> timingRemoveMCounter(3);
	vector<double> memoryPRemoveMCounter(3);
	vector<double> memoryVRemoveMCounter(3);

	//START OF UTILITY

	/* An Inline Progress Bar
	@param x Current Index
	@param n Total Number of Progress
	@param beginClock Start of elapsed time
	@param beginPMem Start of PMem
	@param beginVMem Start of VMem
	@param w Width of the bar
	*/
	static inline void loadbar(unsigned int x, unsigned int n, clock_t beginClock, SIZE_T beginPMem, SIZE_T beginVMem, unsigned int w = 10)
	{
		if ((x != n) && (x % (n / 100 + 1) != 0) && n >= 2000) return;

		float ratio = x / (float)n;
		int   c = (int) (ratio * w);

		//Print Progress Bar
		cout << setw(3) << white << "Processed: " << cyan << x << white << "/" << green << n << yellow << " [" << red;
		for (int x = 0; x < c; x++) cout << "=";
		for (unsigned int x = c; x < w; x++) cout << " ";
		cout << yellow << "] " << (int)(ratio * 100) << "%" << white << " Time: " << cyan << setprecision(2) << fixed << calculateElapsed(beginClock, clock()) << " sec";
		cout << white << " Mem: " << gray << convertMemoryToHumanReadableSht((getPMUsed() - beginPMem)) << white << "/" << dark_white << convertMemoryToHumanReadableSht((getVMUsed() - beginVMem));
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
	Parsing each Lyric String File
	@param lyricStr String of the Lyric
	@param l Points to the lyric object to load to
	*/
	void parseLyricData(string &lyricStr, Lyric *l){
		istringstream lyrStream(lyricStr);
		string token;
		//LyricDLL l;
		int ct = 0;	//0 - Track ID, 1 - MusicXMatch ID, >2 Word Count
		while (getline(lyrStream, token, ',')){
			switch (ct){
			case 0: l->setTrackID(token); ct++; break;
			case 1: l->setMusicXMatchID(token); ct++; break;
			default: l->addWordAndCount(token); break;	//Parse Word and Count
			}
		}
		//return l;
	}

	//END OF UTILITY


	/*
	Method that calls the other file parsing methods
	VECTORADD Estimated Total Time: ~11.66 secs
	@param musInfoList List for Music Information
	@param wordList List for Top Words in Lyrics
	@param lyricList List for lyrics in Songs
	*/
	void parseFiles(DoubleLinkedList &musInfoList, DoubleLinkedList &wordList, DoubleLinkedList &lyricList){
		//Clears all memory vectors
		timingAddLCounter.clear(), timingAddMCounter.clear(), timingAddWCounter.clear();
		memoryPAddLCounter.clear(), memoryPAddMCounter.clear(), memoryPAddWCounter.clear();
		memoryVAddLCounter.clear(), memoryVAddMCounter.clear(), memoryVAddWCounter.clear();
		printMemoryInfo();
		cout << pink << "How many lines to read in Music File? (-1 to read all): ";
		settextcolor(cyan);
		int count;
		cin >> count;
		if (count != -1 && count >= 0){
			timingAddMCounter.reserve(count);
			memoryPAddMCounter.reserve(count);
			memoryVAddMCounter.reserve(count);
		}
		else {
			timingAddMCounter.reserve(SONG_DATASET_LENGTH);
			memoryPAddMCounter.reserve(SONG_DATASET_LENGTH);
			memoryVAddMCounter.reserve(SONG_DATASET_LENGTH);
		}
		printMenuTitle("Parsing Text Files...");
		readMatchFile(musInfoList, count);
		readTopWords(wordList);
		cout << pink << "How many lines to read in Lyric Count File? (-1 to read all): ";
		settextcolor(cyan);
		cin >> count;
		if (count != -1 && count >= 0){
			timingAddLCounter.reserve(count);
			memoryPAddLCounter.reserve(count);
			memoryVAddLCounter.reserve(count);
		}
		else {
			timingAddLCounter.reserve(LYRIC_DATASET_LENGTH);
			memoryPAddLCounter.reserve(LYRIC_DATASET_LENGTH);
			memoryVAddLCounter.reserve(LYRIC_DATASET_LENGTH);
		}
		readSongLyricCount(lyricList, count);
		printMenuTitle("Parse Completed");
		cout << endl;
		printMemoryInfo();
	}

	/*
	Reads the dataset train Text File for top words
	VECTORADD Estimated Time: ~0.35 secs
	@param &list Linked list to store the music data lines in
	*/
	void readTopWords(DoubleLinkedList &list){
		bool verboseMode = false; //Enable Verbose Mode

		timingAddWCounter.reserve(WORD_DATASET_LENGTH);
		memoryPAddWCounter.reserve(WORD_DATASET_LENGTH);
		memoryVAddWCounter.reserve(WORD_DATASET_LENGTH);

		int internalCounter = 0, progressCounter = WORD_DATASET_LENGTH;
		printMenuTitle("Parsing Song Lyrics...");

		//Get Start Memory (Virtual, Physical) and CPU Time
		clock_t beginClock = clock();
		SIZE_T bVMem = getVMUsed(), bPMem = getPMUsed();

		for (string &str : wordDataset){
			list.addBack(str);

			//Log Memory and CPU Time
			timingAddWCounter.push_back(calculateElapsed(beginClock, clock()));
			memoryPAddWCounter.push_back((double)(getPMUsed() - bPMem));
			memoryVAddWCounter.push_back((double)(getVMUsed() - bVMem));

			loadbar(internalCounter, progressCounter, beginClock, bPMem, bVMem);
			internalCounter++; //Increment counter
		}
		loadbar(progressCounter, progressCounter, beginClock, bPMem, bVMem);

		//Calculate Memory Used (Virtual, Physical)
		addWPTime = (getPMUsed() - bPMem);
		addWVTime = (getVMUsed() - bVMem);

		settextcolor(yellow);
		addWElapsed = calculateElapsed(beginClock, clock());
		cout << endl << "Finished Parsing Song Lyrics." << endl;
		cout << yellow << "Elapsed Time to add: " << cyan << setprecision(2) << fixed << addWElapsed << " seconds" << endl;
		cout << yellow << "Total Words Read: " << cyan << internalCounter << endl;
		cout << yellow << "Total Word List Length: " << cyan << list.getLength() << endl;
		cout << yellow << "Page Memory Use: " << cyan << convertMemoryToHumanReadable(addWPTime) << endl;
		cout << yellow << "RAM Use: " << cyan << convertMemoryToHumanReadable(addWVTime) << endl << endl;
	}

	/*
	Reads the dataset train Text File for song lyrics count
	VECTORADD Estimated Time: ~2.76 secs
	@param &list Linked list to store the music data lines in
	*/
	void readSongLyricCount(DoubleLinkedList &list, int count){
		bool verboseMode = false; //Enable Verbose Mode

		int internalCounter = 0, progressCounter = count;	//Latter 2 are hardcoded

		settextcolor(white);
		if (count > LYRIC_DATASET_LENGTH || count < -1){
			cout << "Lines to read specified exceeds lines in file. Defaulting to read all" << endl;
			count = -1;
		}
		if (count == -1) progressCounter = LYRIC_DATASET_LENGTH;
		printMenuTitle("Parsing Songs Lyrics Count...");


		//Get Start Memory (Virtual, Physical) and CPU Time
		clock_t beginClock = clock();
		SIZE_T bVMem = getVMUsed(), bPMem = getPMUsed();

		for (string &str : lyricDataset){
			if (str[0] == '#' || str[0] == '%') continue;	//Skip Comments and Words
			if (internalCounter >= progressCounter) break;	//Reach/Exceed amount to process

			list.addBack(str);

			//Log Memory and CPU Time
			timingAddLCounter.push_back(calculateElapsed(beginClock, clock()));
			memoryPAddLCounter.push_back((double)(getPMUsed() - bPMem));
			memoryVAddLCounter.push_back((double)(getVMUsed() - bVMem));

			loadbar(internalCounter, progressCounter, beginClock, bPMem, bVMem);
			internalCounter++;	//Increment counter
		}
		loadbar(progressCounter, progressCounter, beginClock, bPMem, bVMem);

		//Calculate Memory Used (Virtual, Physical) and CPU Time
		addLPTime = (getPMUsed() - bPMem);
		addLVTime = (getVMUsed() - bVMem);
		addLElapsed = calculateElapsed(beginClock, clock());

		settextcolor(yellow);
		cout << endl << "Finished Parsing Song Lyrics Count." << endl;
		cout << yellow << "Elapsed Time to add: " << cyan << setprecision(2) << fixed << addLElapsed << " seconds" << endl;
		cout << yellow << "Total Lyric Lines Read: " << cyan << internalCounter << endl;
		cout << yellow << "Total Lyric List Length: " << cyan << list.getLength() << endl;
		cout << yellow << "Page Memory Use: " << cyan << convertMemoryToHumanReadable(addLPTime) << endl;
		cout << yellow << "RAM Use: " << cyan << convertMemoryToHumanReadable(addLVTime) << endl << endl;
	}

	/*
	Reads the 779k Match Text File
	VECTORADD Estimated Time: ~8.55 secs
	@param &list Linked list to store the music data lines in
	@param count How many lines in the text file to process
	*/
	void readMatchFile(DoubleLinkedList &list, int count){
		bool verboseMode = false; //Enable Verbose Mode

		int internalCounter = 0, progressCounter = count;
		settextcolor(white);
		if (count > SONG_DATASET_LENGTH || count < -1){
			cout << "Lines to read specified exceeds lines in file. Defaulting to read all" << endl;
			count = -1;
		}
		if (count == -1) progressCounter = SONG_DATASET_LENGTH;
		printMenuTitle("Parsing Song Information...");

		//Get Start Memory (Virtual, Physical) and CPU Time
		clock_t beginClock = clock();
		SIZE_T bVMem = getVMUsed(), bPMem = getPMUsed();

		for (string &str : songDataset){
			if (internalCounter >= progressCounter)	break;
			if (verboseMode) cout << str << endl;
			if (str[0] == '#') continue; 	//Check if string is a comment

			list.addBack(str);	//Parse Music Details Line

			//Log Memory and CPU Time
			timingAddMCounter.push_back(calculateElapsed(beginClock, clock()));
			memoryPAddMCounter.push_back((double)(getPMUsed() - bPMem));
			memoryVAddMCounter.push_back((double)(getVMUsed() - bVMem));

			loadbar(internalCounter, progressCounter, beginClock, bPMem, bVMem);
			internalCounter++;	//Increment counter
		}

		loadbar(progressCounter, progressCounter, beginClock, bPMem, bVMem);

		//Calculate Memory Used (Virtual, Physical) and CPU Time
		addMPTime = (getPMUsed() - bPMem);
		addMVTime = (getVMUsed() - bVMem);
		addMElapsed = calculateElapsed(beginClock, clock());

		settextcolor(yellow);
		cout << endl << "Finished Parsing and Adding Song Information." << endl;
		cout << yellow << "Elapsed Time to add: " << cyan << setprecision(2) << fixed << addMElapsed << " seconds" << endl;
		cout << yellow << "Total Lines Read: " << cyan << internalCounter << endl;
		cout << yellow << "Total Music List Length: " << cyan << list.getLength() << endl;
		cout << yellow << "Page Memory Use: " << cyan << convertMemoryToHumanReadable(addMPTime) << endl;
		cout << yellow << "RAM Use: " << cyan << convertMemoryToHumanReadable(addMVTime) << endl << endl;
	}

	/*
	Prints out the main pointer-based list menu
	*/
	void mainPtrArrMenu(){
		printMenuTitle("Main Doubly Linked List Menu");
		cout << "1) " << yellow << "View Songs in Database" << white << endl;
		cout << "2) " << yellow << "View All the Top Words found in Lyrics" << white << endl;
		cout << "3) " << yellow << "Search for a song in database with name" << white << endl;
		//cout << "4) " << yellow << "Search for a song in database with name and get song lyrics" << white << endl;
		cout << "4) " << yellow << "Remove a song from the database" << white << endl;
		cout << "5) " << yellow << "View Plotted Graph" << white << endl;
		cout << "8) " << yellow << "Performance Utilities" << white << endl;
		cout << "9) " << yellow << "Return to Main Menu" << white << endl;
		cout << "0) " << yellow << "Quit" << white << endl;
	}

	void getLyrics(Music &musicInfo, DoubleLinkedList &words, DoubleLinkedList &lyrics){
		cout << green << "Searching for possible lyrics of song" << white << endl;
		bool found = false;
		clock_t start = clock();
		SIZE_T startPM = getPMUsed(), startVM = getVMUsed();
		int sizeOfLyrics = lyrics.getLength();
		for (int i = 1; i <= sizeOfLyrics; i++){
			Lyric *l = new Lyric();
			parseLyricData(lyrics.get(i), l);
			loadbar(i, sizeOfLyrics, start, startPM, startVM);
			if (l->getTrackID() == musicInfo.getTid() && l->getMusicXMatchID() == musicInfo.getMid()){
				vector<string> wordList = l->getWords();
				vector<string> counts = l->getCounts();
				loadbar(i, i, start, startPM, startVM);
				//Start Printing
				cout << endl << green << "Lyrics Found! Parsing count of lyrics!" << white << endl;
				found = true;
				//string announceTitle = ;
				printMenuTitle("Lyric for " + musicInfo.getMTitle());
				cout << centerString("Word", 40) << "|" << centerString("No. of times it appears", 37) << endl;
				printSeperator();
				int wrdSize = wordList.size();
				for (int i = 1; i <= wrdSize; i++){
					int word = stoi(wordList[i]);
					cout << centerString(words.get(word).c_str(), 10) << "|" << centerString(counts[i].c_str(), 10) << endl;
				}
				printSeperator();
				delete l;
				break;
			}
			delete l;
		}
		if (!found){
			loadbar(sizeOfLyrics, sizeOfLyrics, start, startPM, startVM);
			cout << endl << dark_red << "No lyrics is found for this song" << endl;
		}
	}

	/*
	Sequential Search Operation: Search for a song
	@param &list Linked List of the songs
	@param &wrdlist Pass in Word List
	@param &lyricList Pass in the lyric list
	@param findLyrics Check if you need to find lyrics or not
	*/
	void searchSong(DoubleLinkedList &list, DoubleLinkedList &wrdList, DoubleLinkedList &lyricList, bool findLyrics){
		printMenuTitle("Search Songs");
		string target, empty;
		getline(cin, empty);
		cout << pink << "Enter Exact Track ID: " << cyan;
		getline(cin, target);
		settextcolor(white);

		//Clear and rereserve memory for logs
		timingSeqSearchMCounter.clear(), memoryPSeqSearchMCounter.clear(), memoryVSeqSearchMCounter.clear();
		timingSeqSearchMCounter.reserve(list.getLength()), memoryPSeqSearchMCounter.reserve(list.getLength()), memoryVSeqSearchMCounter.reserve(list.getLength());


		//Get Start Memory (Virtual, Physical) and CPU Time
		clock_t start = clock();
		SIZE_T bVMem = getVMUsed(), bPMem = getPMUsed();
		int criticalInstructions = 0;

		bool found = false;
		for (int i = 1; i <= list.getLength(); i++){
			Music musIfo = parseMusicItem(list.get(i));

			//Log Memory and CPU Time
			timingSeqSearchMCounter.push_back(calculateElapsed(start, clock()));
			memoryPSeqSearchMCounter.push_back((double)(getPMUsed() - bPMem));
			memoryVSeqSearchMCounter.push_back((double)(getVMUsed() - bVMem));

			loadbar(i, list.getLength(), start, bPMem, bVMem);
			criticalInstructions++;

			if (musIfo.getTid() == target){
				loadbar(i, i, start, bPMem, bVMem);
				cout << endl << yellow << "Music Found! Details of the music file is found below:" << endl;
				//printMusicInfo(musIfo);
				musIfo.printMusicInfo();
				if (findLyrics)
					getLyrics(musIfo, wrdList, lyricList);
				cout << endl;
				found = true;
				timingSeqSearchMCounter.resize(i);
				memoryPSeqSearchMCounter.resize(i);
				memoryVSeqSearchMCounter.resize(i);
				break;
			}
		}
		if (!found){
			loadbar(list.getLength(), list.getLength(), start, bPMem, bVMem);
			cout << endl << dark_red << "Unable to find a music file matching the search term" << endl;
			cout << yellow << "Number of Critical Instructions: " << cyan << criticalInstructions << endl;
		}

		//Calculate Memory Used (Virtual, Physical) and CPU Time
		sequSearchPTime = (getPMUsed() - bPMem);
		sequSearchVTime = (getVMUsed() - bVMem);
		sequSearchElapsed = calculateElapsed(start, clock());

		cout << yellow << "Elapsed Time for Sequential Search: " << cyan << setprecision(2) << fixed << sequSearchElapsed << " seconds." << endl;
		cout << yellow << "Page Memory Use Changes: " << cyan << convertMemoryToHumanReadable(sequSearchVTime) << endl;
		cout << yellow << "RAM Use Changes: " << cyan << convertMemoryToHumanReadable(sequSearchPTime) << endl << endl;
		cout << yellow << "Number of Critical Instructions: " << cyan << criticalInstructions << endl;
	}

	/*
	Display Operation: List of all songs currently in the linked list
	@param &list Linked List of the songs
	*/
	void listAllSongs(DoubleLinkedList &list){
		printMenuTitle("List All Songs");

		//Clears log and rereserve memory
		timingDisplayMCounter.clear(), memoryPDisplayMCounter.clear(), memoryVDisplayMCounter.clear();
		timingDisplayMCounter.reserve(list.getLength()), memoryPDisplayMCounter.reserve(list.getLength()), memoryVDisplayMCounter.reserve(list.getLength());

		//Get Start Memory (Virtual, Physical) and CPU Time
		SIZE_T bVMem = getVMUsed(), bPMem = getPMUsed();
		clock_t start = clock();

		for (int i = 1; i <= list.getLength(); i++){
			Music musIfo = parseMusicItem(list.get(i));
			cout << yellow << "=========================================================" << endl;
			cout << red << "                 Song " << i << endl;
			cout << yellow << "=========================================================" << endl;
			musIfo.printMusicInfo();
			cout << yellow << "=========================================================" << endl;
			//Log Memory and CPU Time
			timingDisplayMCounter.push_back(calculateElapsed(start, clock()));
			memoryPDisplayMCounter.push_back((double)(getPMUsed() - bPMem));
			memoryVDisplayMCounter.push_back((double)(getVMUsed() - bVMem));
		}

		//Calculate Memory Used (Virtual, Physical)
		displayMPTime = (getPMUsed() - bPMem);
		displayMVTime = (getVMUsed() - bVMem);

		displayMElapsed = calculateElapsed(start, clock());
		cout << yellow << "Elapsed Time for displaying songs: " << cyan << setprecision(2) << fixed << displayMElapsed << " seconds." << endl;
		cout << yellow << "Page Memory Use Changes: " << cyan << convertMemoryToHumanReadable(displayMVTime) << endl;
		cout << yellow << "RAM Use Changes: " << cyan << convertMemoryToHumanReadable(displayMPTime) << endl << endl;
	}

	/* 
	Display Operation: Display List of Top Words in Lyrics
	@param list List of Top Words
	*/
	void listTopWords(DoubleLinkedList &list){
		printMenuTitle("List Top Words in Lyrics");
		int modder = 1;		//To split print every 6 lines

		clock_t start = clock();
		//Get Start Memory (Virtual, Physical)
		SIZE_T bVMem = getVMUsed(), bPMem = getPMUsed();

		//Clears memory & CPU Logs and rereserve stuff
		timingDisplayWCounter.clear(), memoryPDisplayWCounter.clear(), memoryVDisplayWCounter.clear();
		timingDisplayWCounter.reserve(list.getLength()), memoryPDisplayWCounter.reserve(list.getLength()), memoryVDisplayWCounter.reserve(list.getLength());

		for (int i = 1; i <= list.getLength(); i++){
			if (modder % 6 == 0){ cout << endl;	modder = 1; }
			string wordString = list.get(i);
			cout << "  " << white << wordString << yellow << "  |";

			//Log Memory and CPU Time
			timingDisplayWCounter.push_back(calculateElapsed(start, clock()));
			memoryPDisplayWCounter.push_back((double)(getPMUsed() - bPMem));
			memoryVDisplayWCounter.push_back((double)(getVMUsed() - bVMem));

			modder++;
		}
		cout << endl;

		//Calculate Memory Used (Virtual, Physical)
		displayWPTime = (getPMUsed() - bPMem);
		displayWVTime = (getVMUsed() - bVMem);
		displayWElapsed = calculateElapsed(start, clock());

		cout << yellow << "Elapsed Time for displaying top words in lyrics: " << cyan << setprecision(2) << fixed << displayWElapsed << " seconds." << endl;
		cout << yellow << "Page Memory Use Changes: " << cyan << convertMemoryToHumanReadable(displayWVTime) << endl;
		cout << yellow << "RAM Use Changes: " << cyan << convertMemoryToHumanReadable(displayWPTime) << endl << endl;
	}

	/*
	Remove Operation: Remove an item from the music file
	@param list List of Music File
	*/
	void removeMusicInfo(DoubleLinkedList &list){
		printMenuTitle("Remove Music Data");
		int size = list.getLength();
		string indexToRemove;
		cout << pink << "Enter index to remove (1 - " << size << "): ";
		getStringInput(indexToRemove);

		//Validation
		if (!is_number(indexToRemove)){
			cout << dark_red << "Please enter a positive number as the index" << white << endl;
			return;
		}
		if (stoi(indexToRemove) > size){
			cout << dark_red << "Entered Value is higher than the total size in the list" << white << endl;
			return;
		}

		Music toRemove = parseMusicItem(list.get(stoi(indexToRemove)));

		cout << red << "Are you sure you wish to remove the following music data from the list? " << endl;
		toRemove.printMusicInfo();
		cout << red << "To remove, Enter y or Y (default N): ";
		string confirm;
		getStringInput(confirm);			//Check if user confirm
		cout << endl;
		if (confirm[0] == 'y' || confirm[0] == 'Y'){

			//Confirmed Remove
			clock_t start = clock();
			//Get Start Memory (Virtual, Physical)
			SIZE_T bVMem = getVMUsed(), bPMem = getPMUsed();

			list.remove(stoi(indexToRemove));
			cout << green << "Music Data (" << toRemove.getMTitle() << ") has been removed from the list!" << endl;


			//Calculate Memory Used (Virtual, Physical) and CPU Time
			clock_t end = clock();
			removePTime = (getPMUsed() - bPMem);
			removeVTime = (getVMUsed() - bVMem);

			//Log Memory and CPU Time
			timingRemoveMCounter[0] = 0;
			memoryPRemoveMCounter[0] = 0;
			memoryVRemoveMCounter[0] = 0;
			timingRemoveMCounter[1] = calculateElapsed(start, end);
			memoryPRemoveMCounter[1] = (double)(getPMUsed() - bPMem);
			memoryVRemoveMCounter[1] = (double)(getVMUsed() - bVMem);
			timingRemoveMCounter[2] = calculateElapsed(start, end);
			memoryPRemoveMCounter[2] = (double)(getPMUsed() - bPMem);
			memoryVRemoveMCounter[2] = (double)(getVMUsed() - bVMem);
			removeElapsed = calculateElapsed(start, end);

			cout << yellow << "Elapsed Time for removing music: " << cyan << setprecision(2) << fixed << removeElapsed << " seconds." << endl;
			cout << yellow << "Page Memory Use Changes: " << cyan << convertMemoryToHumanReadable(removeVTime) << endl;
			cout << yellow << "RAM Use Changes: " << cyan << convertMemoryToHumanReadable(removePTime) << endl << endl;
		}
		else {
			cout << green << "Cancelled Removal of Music Data from list" << endl;
		}
	}

	/*
	DEPRECRATED Memory View: Prints out the statistics (timing/mem usage) of list
	*/
	void printStats(){
		//Timings (Sequence 40 | 37)
		printMenuTitle("Pointer-based List Statistic (Timings)");
		//Add Music
		cout << red << centerString("Add (Music)", 40) << yellow << "|        " << cyan;
		if (addMElapsed != -1)	cout << setprecision(2) << fixed << addMElapsed << " Seconds ";
		else cout << "  Untested ";
		cout << endl;

		//Add Music
		cout << red << centerString("Add (Words)", 40) << yellow << "|        " << cyan;
		if (addWElapsed != -1)	cout << setprecision(2) << fixed << addWElapsed << " Seconds ";
		else cout << "  Untested ";
		cout << endl;

		//Add Lyric
		cout << red << centerString("Add (Lyric)", 40) << yellow << "|        " << cyan;
		if (addLElapsed != -1)	cout << setprecision(2) << fixed << addLElapsed << " Seconds ";
		else cout << "  Untested ";
		cout << endl;

		//Display Music
		cout << red << centerString("Display (Music)", 40) << yellow << "|        " << cyan;
		if (displayMElapsed != -1)	cout << setprecision(2) << fixed << displayMElapsed << " Seconds ";
		else cout << "  Untested ";
		cout << endl;

		//Display Top Words
		cout << red << centerString("Display (Top Words)", 40) << yellow << "|        " << cyan;
		if (displayWElapsed != -1)	cout << setprecision(2) << fixed << displayWElapsed << " Seconds ";
		else cout << "  Untested ";
		cout << endl;

		//Remove
		cout << red << centerString("Remove Item", 40) << yellow << "|        " << cyan;
		if (removeElapsed != -1) cout << setprecision(2) << fixed << removeElapsed << " Seconds ";
		else cout << "  Untested ";
		cout << endl;

		//Seq Search
		cout << red << centerString("Sequential Search", 40) << yellow << "|        " << cyan;
		if (sequSearchElapsed != -1) cout << setprecision(2) << fixed << sequSearchElapsed << " Seconds ";
		else cout << "  Untested ";
		cout << endl;

		//Memory (Sequence 26 | 25 | 25)
		printMenuTitle("Pointer-based List Statistics (Memory Usage Changes)");
		cout << red << centerString("Operation", 26) << yellow << "|" << red << centerString("RAM Use", 25) << yellow << "|" << red << centerString("V.Mem Use (Page File)", 25) << endl;
		printSeperator();
		//Add Music
		cout << red << centerString("Add (Music)", 26) << yellow << "|" << cyan;
		if (addMPTime != -1) cout << centerString(convertMemoryToHumanReadable(addMPTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << yellow << "|" << cyan;
		if (addMVTime != -1) cout << centerString(convertMemoryToHumanReadable(addMVTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << endl;

		//Add Words
		cout << red << centerString("Add (Words)", 26) << yellow << "|" << cyan;
		if (addWPTime != -1) cout << centerString(convertMemoryToHumanReadable(addWPTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << yellow << "|" << cyan;
		if (addWVTime != -1) cout << centerString(convertMemoryToHumanReadable(addWVTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << endl;

		//Add Lyric
		cout << red << centerString("Add (Lyric)", 26) << yellow << "|" << cyan;
		if (addLPTime != -1) cout << centerString(convertMemoryToHumanReadable(addLPTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << yellow << "|" << cyan;
		if (addLVTime != -1) cout << centerString(convertMemoryToHumanReadable(addLVTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << endl;

		//Display Music
		cout << red << centerString("Display (Music)", 26) << yellow << "|" << cyan;
		if (displayMPTime != -1) cout << centerString(convertMemoryToHumanReadable(displayMPTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << yellow << "|" << cyan;
		if (displayMVTime != -1) cout << centerString(convertMemoryToHumanReadable(displayMVTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << endl;

		//Display Top Words
		cout << red << centerString("Display (Top Words)", 26) << yellow << "|" << cyan;
		if (displayWPTime != -1) cout << centerString(convertMemoryToHumanReadable(displayWPTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << yellow << "|" << cyan;
		if (displayWVTime != -1) cout << centerString(convertMemoryToHumanReadable(displayWVTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << endl;

		//Remove
		cout << red << centerString("Remove Item", 26) << yellow << "|" << cyan;
		if (removePTime != -1) cout << centerString(convertMemoryToHumanReadable(removePTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << yellow << "|" << cyan;
		if (removeVTime != -1) cout << centerString(convertMemoryToHumanReadable(removeVTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << endl;

		//Seq Search
		cout << red << centerString("Sequential Search", 26) << yellow << "|" << cyan;
		if (sequSearchPTime != -1) cout << centerString(convertMemoryToHumanReadable(sequSearchPTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << yellow << "|" << cyan;
		if (sequSearchVTime != -1) cout << centerString(convertMemoryToHumanReadable(sequSearchVTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << endl;
		printSeperator();
	}

	//START OF GRAPH GENERATION METHODS

	/*
	Make a Graph with x axis being the length of the list and the y axis being time taken
	*/
	void makeAddTimeGraph(){
		vector<Graph> ptrArrList(0);
		//Make Graph for Lyric and get string
		if (addLElapsed == -1)	cout << dark_red << "There is no data for adding Lyrics. Please run the add operation before doing this." << endl;
		else{
			Graph lycG("Lyrics", timingAddLCounter.size(), timingAddLCounter);
			//string lycGStr = lycG.createGraphString();
			ptrArrList.push_back(lycG);
		}
		//Make Graph for Songs
		if (addMElapsed == -1)	cout << dark_red << "There is no data for adding Song Data. Please run the add operation before doing this." << endl;
		else {
			Graph sonG("Song Data", timingAddMCounter.size(), timingAddMCounter);
			//string sonGStr = sonG.createGraphString();
			ptrArrList.push_back(sonG);
		}
		//Make Graph for Words
		if (addWElapsed == -1)	cout << dark_red << "There is no data for adding top words. Please run the add operation before doing this." << endl;
		else {
			Graph wrdG("Top Lyric Words", timingAddWCounter.size(), timingAddWCounter);
			//string wrdGStr = wrdG.createGraphString();
			ptrArrList.push_back(wrdG);
		}
		if (ptrArrList.size() > 0 && ptrArrList.size() <= 3)	plotGraph(ptrArrList, "Unsorted Pointer-Based List Timings Graph (ADD)");
	}

	/*
	Make a Graph with x axis being the length of the list and the y axis being time taken
	*/
	void makeDisplayTimeGraph(){
		vector<Graph> ptrArrList(0);
		if (displayMElapsed == -1)	cout << dark_red << "There is no data for displaying song data. Please run the display operation before doing this." << endl;
		else{
			Graph sonG("Song Data", timingDisplayMCounter.size(), timingDisplayMCounter);
			//string sonGStr = sonG.createGraphString();
			ptrArrList.push_back(sonG);
		}
		if (displayWElapsed == -1)	cout << dark_red << "There is no data for displaying top words. Please run the display operation before doing this." << endl;
		else{
			Graph wrdG("Top Lyric Words", timingDisplayWCounter.size(), timingDisplayWCounter);
			//string wrdGStr = wrdG.createGraphString();
			ptrArrList.push_back(wrdG);
		}
		if (ptrArrList.size() > 0 && ptrArrList.size() <= 2)	plotGraph(ptrArrList, "Unsorted Pointer-Based List Timings Graph (DISPLAY)");
	}

	/*
	Make a Graph with x axis being the length of the list and the y axis being time taken
	*/
	void makeRemoveTimeGraph(){
		vector<Graph> ptrArrList(0);
		if (removeElapsed == -1)	cout << dark_red << "There is no data for removing song data. Please run the remove operation before doing this." << endl;
		else {
			Graph sonG("Song Data", timingRemoveMCounter.size(), timingRemoveMCounter);
			//string sonGStr = sonG.createGraphString();
			ptrArrList.push_back(sonG);
		}
		if (ptrArrList.size() == 1)	plotGraph(ptrArrList, "Unsorted Pointer-Based List Timings Graph (REMOVE)");
	}

	/*
	Make a Graph with x axis being the length of the list and the y axis being time taken
	*/
	void makeSeqSearchTimeGraph(){
		vector<Graph> ptrArrList(0);
		if (sequSearchElapsed == -1)	cout << dark_red << "There is no data for searching song data sequentially. Please run the seq. search operation before doing this." << endl;
		else {
			Graph sonG("Song Data", timingSeqSearchMCounter.size(), timingSeqSearchMCounter);
			//string sonGStr = sonG.createGraphString();
			ptrArrList.push_back(sonG);
		}
		if (ptrArrList.size() == 1)	plotGraph(ptrArrList, "Unsorted Pointer-Based List Timings Graph (SEQUENTIAL SEARCH)");
	}

	/*
	Make a Graph with x axis being the length of the list and the y axis being memory use
	*/
	void makeAddMemGraph(){
		vector<Graph> ptrArrList(0);
		//Make Graph for Lyric and get string
		if (addLPTime == -1)	cout << dark_red << "There is no data for adding Lyrics. Please run the add operation before doing this." << endl;
		else{
			Graph lycG1("Lyrics (RAM)", memoryPAddLCounter.size(), memoryPAddLCounter);
			//string lycGStr = lycG1.createGraphString();
			ptrArrList.push_back(lycG1);
			Graph lycG2("Lyrics (Page)", memoryVAddLCounter.size(), memoryVAddLCounter);
			//lycGStr = lycG2.createGraphString();
			ptrArrList.push_back(lycG2);
		}
		//Make Graph for Songs
		if (addMPTime == -1)	cout << dark_red << "There is no data for adding Song Data. Please run the add operation before doing this." << endl;
		else {
			Graph sonG1("Song Data (RAM)", memoryPAddMCounter.size(), memoryPAddMCounter);
			//string sonGStr = sonG1.createGraphString();
			ptrArrList.push_back(sonG1);
			Graph sonG2("Song Data (Page)", memoryVAddMCounter.size(), memoryVAddMCounter);
			//sonGStr = sonG2.createGraphString();
			ptrArrList.push_back(sonG2);
		}
		//Make Graph for Words
		if (addWPTime == -1)	cout << dark_red << "There is no data for adding top words. Please run the add operation before doing this." << endl;
		else {
			Graph wrdG1("Top Lyric Words (RAM)", memoryPAddWCounter.size(), memoryPAddWCounter);
			//string wrdGStr = wrdG1.createGraphString();
			ptrArrList.push_back(wrdG1);
			Graph wrdG2("Top Lyric Words (Page)", memoryVAddWCounter.size(), memoryVAddWCounter);
			//wrdGStr = wrdG2.createGraphString();
			ptrArrList.push_back(wrdG2);
		}
		if (ptrArrList.size() > 0 && ptrArrList.size() <= 6)	plotGraph(ptrArrList, "Unsorted Pointer-Based List Memory Graph (ADD)");
	}

	/*
	Make a Graph with x axis being the length of the list and the y axis being memory use
	*/
	void makeDisplayMemGraph(){
		vector<Graph> ptrArrList(0);
		if (displayMPTime == -1)	cout << dark_red << "There is no data for displaying song data. Please run the display operation before doing this." << endl;
		else{
			Graph sonG1("Song Data (RAM)", memoryPDisplayMCounter.size(), memoryPDisplayMCounter);
			string sonGStr = sonG1.createGraphString();
			ptrArrList.push_back(sonG1);
			Graph sonG2("Song Data (Page)", memoryVDisplayMCounter.size(), memoryVDisplayMCounter);
			sonGStr = sonG2.createGraphString();
			ptrArrList.push_back(sonG2);
		}
		if (displayWPTime == -1)	cout << dark_red << "There is no data for displaying top words. Please run the display operation before doing this." << endl;
		else{
			Graph wrdG1("Top Lyric Words (RAM)", memoryPDisplayWCounter.size(), memoryPDisplayWCounter);
			string wrdGStr = wrdG1.createGraphString();
			ptrArrList.push_back(wrdG1);
			Graph wrdG2("Top Lyric Words (Page)", memoryVDisplayWCounter.size(), memoryVDisplayWCounter);
			wrdGStr = wrdG2.createGraphString();
			ptrArrList.push_back(wrdG2);
		}
		if (ptrArrList.size() > 0 && ptrArrList.size() <= 4)	plotGraph(ptrArrList, "Unsorted Pointer-Based List Memory Graph (DISPLAY)");
	}

	/*
	Make a Graph with x axis being the length of the list and the y axis being memory use
	*/
	void makeRemoveMemGraph(){
		vector<Graph> ptrArrList(0);
		if (removePTime == -1)	cout << dark_red << "There is no data for removing song data. Please run the remove operation before doing this." << endl;
		else {
			Graph sonG1("Song Data (RAM)", memoryPRemoveMCounter.size(), memoryPRemoveMCounter);
			//string sonGStr = sonG1.createGraphString();
			ptrArrList.push_back(sonG1);
			Graph sonG2("Song Data (Page)", memoryVRemoveMCounter.size(), memoryVRemoveMCounter);
			//sonGStr = sonG2.createGraphString();
			ptrArrList.push_back(sonG2);
		}
		if (ptrArrList.size() == 2)	plotGraph(ptrArrList, "Unsorted Pointer-Based List Timings Graph (REMOVE)");
	}

	/*
	Make a Graph with x axis being the length of the list and the y axis being memory use
	*/
	void makeSeqSearchMemGraph(){
		vector<Graph> ptrArrList(0);
		if (sequSearchPTime == -1)	cout << dark_red << "There is no data for searching song data sequentially. Please run the seq. search operation before doing this." << endl;
		else {
			Graph sonG1("Song Data (RAM)", memoryPSeqSearchMCounter.size(), memoryPSeqSearchMCounter);
			string sonGStr = sonG1.createGraphString();
			ptrArrList.push_back(sonG1);
			Graph sonG2("Song Data (Page)", memoryVSeqSearchMCounter.size(), memoryVSeqSearchMCounter);
			sonGStr = sonG2.createGraphString();
			ptrArrList.push_back(sonG2);
		}
		if (ptrArrList.size() == 2)	plotGraph(ptrArrList, "Unsorted Pointer-Based List Timings Graph (SEQUENTIAL SEARCH)");
	}
	//END OF GRAPH GENERATION METHODS

	/*
	Plot Graph Menu
	*/
	void plotGraphMenu(){
		printMenuTitle("Select a graph to view");
		cout << "1) " << yellow << "Add Items (Timings)" << white << endl;
		cout << "2) " << yellow << "Add Items (Memory)" << white << endl;
		cout << "3) " << yellow << "Display Items (Timings)" << white << endl;
		cout << "4) " << yellow << "Display Items (Memory)" << white << endl;
		cout << "5) " << yellow << "Remove Item (Timings)" << white << endl;
		cout << "6) " << yellow << "Remove Item (Memory)" << white << endl;
		cout << "7) " << yellow << "Sequential Search Items (Timings)" << white << endl;
		cout << "8) " << yellow << "Sequential Search Items (Memory)" << white << endl;
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
			case 0: return;
			default: cout << dark_red << "Invalid Selection." << endl; break;
			}
		}
		else { cout << dark_red << "Selection must be an integer" << endl; }
	}

	/*
	Performance Menu
	*/
	void performanceMenu(){
		printMenuTitle("Select a Performance Option");
		cout << "1) " << yellow << "View Performance Statistics" << white << endl;
		cout << "2) " << yellow << "View Program Memory Usage" << white << endl;
		cout << "0) " << yellow << "Go Back" << white << endl;
		string selection;
		cout << pink << "Select an option: ";
		getStringInput(selection);
		if (is_number(selection)){
			switch (stoi(selection))
			{
			case 1: printStats(); break;
			case 2: printMemoryInfo(); break;
			case 0: return;
			default: cout << dark_red << "Invalid Selection." << endl; break;
			}
		}
		else { cout << dark_red << "Selection must be an integer" << endl; }
	}

	/*
	Main Pointer Based List Code
	@return Error Code (-1 for continue)
	*/
	int mainLoop(){
		DoubleLinkedList mainMusicList, mainWordList, mainLyricList;
		if (songDataset.size() == 0) {
			cout << dark_red << "Make sure you load the dataset into the vectors before continuing" << endl;
			return -1;
		}
		parseFiles(mainMusicList, mainWordList, mainLyricList);

		if (mainMusicList.getLength() == 0){
			settextcolor(red);
			cout << "As Database do not have any music item, this section will quit." << endl;
			return -1;
		}

		//Main Menu
		while (true) {
			mainPtrArrMenu();
			string selection;
			cout << pink << "Select an option: ";
			getStringInput(selection);
			if (is_number(selection)){
				switch (stoi(selection))
				{
				case 1: listAllSongs(mainMusicList); break;
				case 2: listTopWords(mainWordList); break;
				case 3: searchSong(mainMusicList, mainWordList, mainLyricList, false); break;
				//case 4: searchSong(mainMusicList, mainWordList, mainLyricList, true); break;
				case 4: removeMusicInfo(mainMusicList); break;
				case 5: plotGraphMenu(); break;
				case 8: performanceMenu(); break;
				case 9: return -1;
				case 0: return 0;
				default: cout << dark_red << "Invalid Selection." << endl; break;
				}
			}
			else { cout << dark_red << "Selection must be an integer" << endl; }
		}
		return 0;
	}
}