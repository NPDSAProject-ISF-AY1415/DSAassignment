/*
sortedArr.cpp
Class File for Sorted Array-based List Implementation
Team Name: Wat.jpg
Team Member 1: Soh Yan Quan, Kenneth (S10147249G)
Team Member 2: Soh Hak Wee (S10146869K)
Features to Highlight: Add into list, Display list, Remove list, Sequential Search, Binary Search, Insertion Sort
*/

#include "sortedArr.h"

using namespace std;
using namespace consolecolors;
using namespace utility;

namespace sortedArr {
	int musicInfoFileLength = 779074;
	//All the counters
	double addMElapsed = -1, addWElapsed = -1, addLElapsed = -1, displayMElapsed = -1, displayWElapsed = -1, sequSearchElapsed = -1, binaSearchElapsed = -1;
	double removeElapsed = -1;
	void readTopWords(ListArray &list);
	void readSongLyricCount(ListArray &list, int count);
	void readMatchFile2(ListArray &list, int count);
	//Memory Counters
	SIZE_T addMVTime = -1, addWVTime = -1, addLVTime = -1, displayMVTime = -1, displayWVTime = -1, sequSearchVTime = -1, binaSearchVTime = -1, removeVTime = -1;	//Virtual Mem
	SIZE_T addMPTime = -1, addWPTime = -1, addLPTime = -1, displayMPTime = -1, displayWPTime = -1, sequSearchPTime = -1, binaSearchPTime = -1, removePTime = -1;	//Physical Mem

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

	vector<double> timingBinaSearchMCounter(0);
	vector<double> memoryPBinaSearchMCounter(0);
	vector<double> memoryVBinaSearchMCounter(0);

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
	@return Created Lyric File
	*/
	Lyric parseLyricData(string lyricStr){
		istringstream lyrStream(lyricStr);
		string token;
		Lyric l;
		int ct = 0;	//0 - Track ID, 1 - MusicXMatch ID, >2 Word Count
		while (getline(lyrStream, token, ',')){
			switch (ct){
			case 0: l.setTrackID(token); ct++; break;
			case 1: l.setMusicXMatchID(token); ct++; break;
			default: l.addWordAndCount(token); break;	//Parse Word and Count
			}
		}
		return l;
	}

	//END OF UTILITY


	/*
	Method that calls the other file parsing methods
	@param musInfoList List for Music Information
	@param wordList List for Top Words in Lyrics
	@param lyricList List for lyrics in Songs
	*/
	void parseFiles(ListArray &musInfoList, ListArray &wordList, ListArray &lyricList){
		timingAddLCounter.clear(), timingAddMCounter.clear(), timingAddWCounter.clear();
		memoryPAddLCounter.clear(), memoryPAddMCounter.clear(), memoryPAddWCounter.clear();
		memoryVAddLCounter.clear(), memoryVAddMCounter.clear(), memoryVAddWCounter.clear();
		printMemoryInfo();
		int sortOption;
		while (true){
			cout << pink << "What kind of sort do you want? (1- Insertion Sort, 2 - Read pre-sorted): ";
			settextcolor(cyan);
			cin >> sortOption;
			if (sortOption == 1 || sortOption == 2){
				break;
			}
			cout << endl << dark_red << "Invalid Option." << endl;
		}
		cout << endl << pink << "How many lines to read in Music File? (-1 to read all): ";
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
		if (sortOption == 1)
			readMatchFile2(musInfoList, count);
		else
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
	@param &list Linked list to store the music data lines in
	*/
	void readTopWords(ListArray &list){
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
			list.add(str);

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
	@param &list Linked list to store the music data lines in
	*/
	void readSongLyricCount(ListArray &list, int count){
		bool verboseMode = false; //Enable Verbose Mode

		int internalCounter = 0, fullCounter = 210519, progressCounter = count;	//Latter 2 are hardcoded

		settextcolor(white);
		if (count > LYRIC_DATASET_LENGTH || count <= -1){
			cout << "Lines to read specified exceeds lines in file. Defaulting to read all" << endl;
			count = -1;
		}
		if (count == -1){
			progressCounter = fullCounter;
			cout << "As the file is extremely large, this may take a couple of minutes..." << endl;
		}
		settextcolor(yellow);

		printMenuTitle("Parsing Songs Lyrics Count...");


		//Get Start Memory (Virtual, Physical) and CPU Time
		clock_t beginClock = clock();
		SIZE_T bVMem = getVMUsed(), bPMem = getPMUsed();

		for (string &str : lyricDataset){
			if (str[0] == '#' || str[0] == '%') continue;	//Skip Comments and Words
			if (internalCounter >= progressCounter) break;	//Reach/Exceed amount to process

			list.add(str);

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
	@param &list Linked list to store the music data lines in
	@param count How many lines in the text file to process
	*/
	void readMatchFile(ListArray &list, int count){
		bool verboseMode = false; //Enable Verbose Mode

		int internalCounter = 0, progressCounter = count;
		settextcolor(white);
		if (count > SONG_DATASET_LENGTH || count <= -1){
			cout << "Lines to read specified exceeds lines in file. Defaulting to read all" << endl;
			count = -1;
		}
		if (count == -1){
			progressCounter = SONG_DATASET_LENGTH;
			cout << "As the file is extremely large, this may take a while..." << endl;
		}
		printMenuTitle("Parsing Song Information...");

		//Get Start Memory (Virtual, Physical) and CPU Time
		clock_t beginClock = clock();
		SIZE_T bVMem = getVMUsed(), bPMem = getPMUsed();

		for (string &str : sortedSongDataset){
			if (internalCounter >= progressCounter)	break;
			if (verboseMode) cout << str << endl;
			if (str[0] == '#') continue; 	//Check if string is a comment

			list.add(str);	//Parse Music Details Line

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
	Insertion Sort the 779k Match Text File
	@param &list Linked list to store the music data lines in
	@param count How many lines in the text file to process
	*/
	void readMatchFile2(ListArray &list, int count){
		bool verboseMode = false; //Enable Verbose Mode

		int internalCounter = 0, progressCounter = count;
		settextcolor(white);
		if (count > SONG_DATASET_LENGTH || count <= -1){
			cout << "Lines to read specified exceeds lines in file. Defaulting to read all" << endl;
			count = -1;
		}
		if (count == -1){
			progressCounter = SONG_DATASET_LENGTH;
			cout << "As the file is extremely large, this may take a while..." << endl;
		}
		printMenuTitle("Parsing Song Information...");

		//Get Start Memory (Virtual, Physical) and CPU Time
		clock_t beginClock = clock();
		SIZE_T bVMem = getVMUsed(), bPMem = getPMUsed();
		int pos = 0;
		int counter;
		for (string &str : songDataset)
		{
			if (internalCounter >= progressCounter)	break;
			if (verboseMode) cout << str << endl;
			if (str[0] == '#') continue; 	//Check if string is a comment
			if (list.getLength() == 0)
			{
				list.add(str);
				continue;
			}
			//Music musInfo = parseMusicItem(str);
			counter = list.getLength();
			for (int k = counter; k >= 0; k--)
			{
				//Music musInfoCom = parseMusicItem(list.get(k));
				if (str >list.get(k))
				{
					pos = k + 1; break; //means position in front
				}
				else if (str < list.get(k))
				{
					pos = k; 
				}
			}
			list.add(pos, str);

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
	Prints out the main Array-based list menu
	*/
	void mainPtrArrMenu(){
		printMenuTitle("Main Array-based List Menu");
		cout << "1) " << yellow << "View Songs in Database" << white << endl;
		cout << "2) " << yellow << "View All the Top Words found in Lyrics" << white << endl;
		cout << "3) " << yellow << "Search for a song in database with name" << white << endl;
		cout << "4) " << yellow << "Remove a song from the database" << white << endl;
		cout << "5) " << yellow << "View Plotted Graph" << white << endl;
		cout << "8) " << yellow << "Performance Utilities" << white << endl;
		cout << "9) " << yellow << "Return to Main Menu" << white << endl;
		cout << "0) " << yellow << "Quit" << white << endl;
	
	}

	/*
	Option 2 : Search for a song
	@param &list Linked List of the songs
	*/
	void searchSong(ListArray &list){
		printMenuTitle("Search Songs");
		string target, empty;
		getline(cin, empty);
		cout << pink << "Enter Exact Track ID: " << cyan;
		getline(cin, target);
		settextcolor(white);

		//Clear and rereserve memory for logs
		timingSeqSearchMCounter.clear(), memoryPSeqSearchMCounter.clear(), memoryVSeqSearchMCounter.clear();
		timingSeqSearchMCounter.reserve(list.getLength()), memoryPSeqSearchMCounter.reserve(list.getLength()), memoryVSeqSearchMCounter.reserve(list.getLength());
		timingBinaSearchMCounter.clear(), memoryPBinaSearchMCounter.clear(), memoryVBinaSearchMCounter.clear();
		timingBinaSearchMCounter.reserve(list.getLength()), memoryPBinaSearchMCounter.reserve(list.getLength()), memoryVBinaSearchMCounter.reserve(list.getLength());

		//Get Start Memory (Virtual, Physical) and CPU Time
		clock_t start = clock();
		SIZE_T bVMem = getVMUsed(), bPMem = getPMUsed();
		clock_t startBina = clock();
		SIZE_T bVMemBina = getVMUsed(), bPMemBina = getPMUsed();
		int criticalInstructionsSeq = 0, criticalInstructionsBin = 0;
		
		
		bool found = false;
		for (int i = 1; i <= list.getLength(); i++){
			Music musIfo = parseMusicItem(list.get(i));

			//Log Memory and CPU Time
			timingSeqSearchMCounter.push_back(calculateElapsed(start, clock()));
			memoryPSeqSearchMCounter.push_back((double)(getPMUsed() - bPMem));
			memoryVSeqSearchMCounter.push_back((double)(getVMUsed() - bVMem));
			criticalInstructionsSeq++;

			loadbar(i, list.getLength(), start, bPMem, bVMem);

			if (musIfo.getTid() == target){
				loadbar(i, i, start, bPMem, bVMem);
				cout << endl << yellow << "Music Found! Details of the music file is found below:" << endl;
				//printMusicInfo(musIfo);
				musIfo.printMusicInfo();
				cout << endl;
				found = true;
				timingSeqSearchMCounter.resize(i);
				memoryPSeqSearchMCounter.resize(i);
				memoryVSeqSearchMCounter.resize(i);
				break;
			}
			else if (musIfo.getTid() > target)
			{
				break;
				timingSeqSearchMCounter.resize(i);
				memoryPSeqSearchMCounter.resize(i);
				memoryVSeqSearchMCounter.resize(i);
			}
		}
		if (!found){
			loadbar(list.getLength(), list.getLength(), start, bPMem, bVMem);
			cout << endl << dark_red << "Unable to find a music file matching the search term" << endl;
			cout << yellow << "Number of Critical Instructions: " << cyan << criticalInstructionsSeq << endl;
		}

		cout << endl << green << "Doing Binary Search Now..." << endl;
		bool found2 = false;
		bool verboseMode = false;
		int first = 0;
		int last = list.getLength() -1;
		while (first <= last)
		{
			int mid = (first + last) / 2;
			Music musInf = parseMusicItem(list.get(mid));
			timingBinaSearchMCounter.push_back(calculateElapsed(startBina, clock()));
			memoryPBinaSearchMCounter.push_back((double)(getPMUsed() - bPMemBina));
			memoryVBinaSearchMCounter.push_back((double)(getVMUsed() - bVMemBina));
			if (verboseMode == true){ cout << mid << endl; }
			criticalInstructionsBin++;
			loadbar(criticalInstructionsBin, list.getLength(), start, bPMem, bVMem);
			if (musInf.getTid() == target)
			{
				loadbar(criticalInstructionsBin, criticalInstructionsBin, start, bPMem, bVMem);
				bool found2 = true;
				timingBinaSearchMCounter.resize(mid);
				memoryPBinaSearchMCounter.resize(mid);
				memoryVBinaSearchMCounter.resize(mid);
				break;
			}
			else
			if (target < musInf.getTid())
			{
				last = mid - 1;
			}
			else
			{
				first = mid + 1;
			}
		}
		//Calculate Memory Used (Virtual, Physical) and CPU Time
		sequSearchPTime = (getPMUsed() - bPMem);
		sequSearchVTime = (getVMUsed() - bVMem);
		sequSearchElapsed = calculateElapsed(start, clock());
		binaSearchPTime = (getPMUsed() - bPMemBina);
		binaSearchVTime = (getVMUsed() - bVMemBina);
		binaSearchElapsed = calculateElapsed(startBina, clock());
		cout << endl;

		cout << yellow << "Elapsed Time for Sequential Search: " << cyan << setprecision(2) << fixed << sequSearchElapsed << " seconds." << endl;
		cout << yellow << "Page Memory Use Changes (Sequential): " << cyan << convertMemoryToHumanReadable(sequSearchVTime) << endl;
		cout << yellow << "RAM Use Changes (Sequential): " << cyan << convertMemoryToHumanReadable(sequSearchPTime) << endl;
		cout << yellow << "Number of Critical Instructions (Sequential): " << cyan << criticalInstructionsSeq << endl << endl;
		cout << yellow << "Elapsed Time for Binary Search: " << cyan << setprecision(2) << fixed << binaSearchElapsed << " seconds." << endl;
		cout << yellow << "Page Memory Use Changes (Binary): " << cyan << convertMemoryToHumanReadable(binaSearchVTime) << endl;
		cout << yellow << "RAM Use Changes (Binary): " << cyan << convertMemoryToHumanReadable(binaSearchPTime) << endl;
		cout << yellow << "Number of Critical Instructions (Binary): " << cyan << criticalInstructionsBin << endl << endl;
	}
	
	
	
	

	/*
	Option 1 : List of all songs currently in the linked list
	@param &list Linked List of the songs
	*/
	void listAllSongs(ListArray &list){
		
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


	/* Option 4: Display List of Top Words in Lyrics
	@param list List of Top Words
	*/
	void listTopWords(ListArray &list){
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
	Option 5: Remove an item from the music file
	@param list List of Music File
	*/
	void removeMusicInfo(ListArray &list){
		
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
	Option 3 : Prints out the statistics (timing/mem usage) of list
	*/
	void printStats(){
		printMenuTitle("Array-based List Statistic (Timings)");
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

		//Binary Search
		cout << red << centerString("Binary Search", 40) << yellow << "|        " << cyan;
		if (binaSearchElapsed != -1) cout << setprecision(2) << fixed << binaSearchElapsed << " Seconds ";
		else cout << "  Untested ";
		cout << endl;

		//Memory (Sequence 26 | 25 | 25)
		printMenuTitle("Binary-based List Statistics (Memory Usage Changes)");
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

		//Binary Search
		cout << red << centerString("Binary Search", 26) << yellow << "|" << cyan;
		if (binaSearchPTime != -1) cout << centerString(convertMemoryToHumanReadable(binaSearchPTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << yellow << "|" << cyan;
		if (binaSearchVTime != -1) cout << centerString(convertMemoryToHumanReadable(binaSearchVTime).c_str(), 25);
		else cout << centerString("Untested", 25);
		cout << endl;
		printSeperator();
	}
	

	/*
	Make a Graph with x axis being the length of the list and the y axis being time taken
	*/
	

	void makeAddTimeGraph(){
		vector<Graph> ArrList(0);
		//Make Graph for Lyric and get string
		if (addLElapsed == -1)	cout << dark_red << "There is no data for adding Lyrics. Please run the add operation before doing this." << endl;
		else{
			Graph lycG("Lyrics", timingAddLCounter.size(), timingAddLCounter);
			ArrList.push_back(lycG);
		}
		//Make Graph for Songs
		if (addMElapsed == -1)	cout << dark_red << "There is no data for adding Song Data. Please run the add operation before doing this." << endl;
		else {
			Graph sonG("Song Data", timingAddMCounter.size(), timingAddMCounter);
			ArrList.push_back(sonG);
		}
		//Make Graph for Words
		if (addWElapsed == -1)	cout << dark_red << "There is no data for adding top words. Please run the add operation before doing this." << endl;
		else {
			Graph wrdG("Top Lyric Words", timingAddWCounter.size(), timingAddWCounter);
			ArrList.push_back(wrdG);
		}
		if (ArrList.size() > 0 && ArrList.size() <= 3)	plotGraph(ArrList, "Array-Based List Timings Graph (ADD)");
	}

	/*
	Make a Graph with x axis being the length of the list and the y axis being time taken
	*/
	void makeDisplayTimeGraph(){
		vector<Graph> ArrList;
		if (displayMElapsed == -1)	cout << dark_red << "There is no data for displaying song data. Please run the display operation before doing this." << endl;
		else{
			Graph sonG("Song Data", timingDisplayMCounter.size(), timingDisplayMCounter);
			ArrList.push_back(sonG);
		}
		if (displayWElapsed == -1)	cout << dark_red << "There is no data for displaying top words. Please run the display operation before doing this." << endl;
		else{
			Graph wrdG("Top Lyric Words", timingDisplayWCounter.size(), timingDisplayWCounter);
			ArrList.push_back(wrdG);
		}
		if (ArrList.size() > 0 && ArrList.size() <= 2)	plotGraph(ArrList, "Array-Based List Timings Graph (DISPLAY)");
	}

	/*
	Make a Graph with x axis being the length of the list and the y axis being time taken
	*/
	void makeRemoveTimeGraph(){
		vector<Graph> ArrList;
		if (removeElapsed == -1)	cout << dark_red << "There is no data for removing song data. Please run the remove operation before doing this." << endl;
		else {
			Graph sonG("Song Data", timingRemoveMCounter.size(), timingRemoveMCounter);
			ArrList.push_back(sonG);
		}
		if (ArrList.size() == 1)	plotGraph(ArrList, "Array-Based List Timings Graph (REMOVE)");
	}

	/*
	Make a Graph with x axis being the length of the list and the y axis being time taken
	*/
	void makeSeqSearchTimeGraph(){
		vector<Graph> ArrList;
		if (sequSearchElapsed == -1)	cout << dark_red << "There is no data for searching song data sequentially. Please run the seq. search operation before doing this." << endl;
		else {
			Graph sonG("Song Data", timingSeqSearchMCounter.size(), timingSeqSearchMCounter);
			ArrList.push_back(sonG);
		}
		if (ArrList.size() == 1)	plotGraph(ArrList, "Array-Based List Timings Graph (SEQUENTIAL SEARCH)");
	}
	void makeBinaSearchTimeGraph(){
		vector<Graph> ArrList;
		if (binaSearchElapsed == -1)	cout << dark_red << "There is no data for searching song data sequentially. Please run the seq. search operation before doing this." << endl;
		else {
			Graph sonG("Song Data", timingBinaSearchMCounter.size(), timingBinaSearchMCounter);
			ArrList.push_back(sonG);
		}
		if (ArrList.size() == 1)	plotGraph(ArrList, "Array-Based List Timings Graph (SEQUENTIAL SEARCH)");
	}

	/*
	Make a Graph with x axis being the length of the list and the y axis being memory use
	*/
	void makeAddMemGraph(){
		vector<Graph> ArrList(0);
		//Make Graph for Lyric and get string
		if (addLPTime == -1)	cout << dark_red << "There is no data for adding Lyrics. Please run the add operation before doing this." << endl;
		else{
			Graph lycG1("Lyrics (RAM)", memoryPAddLCounter.size(), memoryPAddLCounter);
			ArrList.push_back(lycG1);
			Graph lycG2("Lyrics (Page)", memoryVAddLCounter.size(), memoryVAddLCounter);
			ArrList.push_back(lycG2);
		}
		//Make Graph for Songs
		if (addMPTime == -1)	cout << dark_red << "There is no data for adding Song Data. Please run the add operation before doing this." << endl;
		else {
			Graph sonG1("Song Data (RAM)", memoryPAddMCounter.size(), memoryPAddMCounter);
			ArrList.push_back(sonG1);
			Graph sonG2("Song Data (Page)", memoryVAddMCounter.size(), memoryVAddMCounter);
			ArrList.push_back(sonG2);
		}
		//Make Graph for Words
		if (addWPTime == -1)	cout << dark_red << "There is no data for adding top words. Please run the add operation before doing this." << endl;
		else {
			Graph wrdG1("Top Lyric Words (RAM)", memoryPAddWCounter.size(), memoryPAddWCounter);
			ArrList.push_back(wrdG1);
			Graph wrdG2("Top Lyric Words (Page)", memoryVAddWCounter.size(), memoryVAddWCounter);
			ArrList.push_back(wrdG2);
		}
		if (ArrList.size() > 0 && ArrList.size() <= 6)	plotGraph(ArrList, "Array-Based List Memory Graph (ADD)");
	}

	/*
	Make a Graph with x axis being the length of the list and the y axis being memory use
	*/
	void makeDisplayMemGraph(){
		vector<Graph> ArrList(0);
		if (displayMPTime == -1)	cout << dark_red << "There is no data for displaying song data. Please run the display operation before doing this." << endl;
		else{
			Graph sonG1("Song Data (RAM)", memoryPDisplayMCounter.size(), memoryPDisplayMCounter);
			ArrList.push_back(sonG1);
			Graph sonG2("Song Data (Page)", memoryVDisplayMCounter.size(), memoryVDisplayMCounter);
			ArrList.push_back(sonG2);
		}
		if (displayWPTime == -1)	cout << dark_red << "There is no data for displaying top words. Please run the display operation before doing this." << endl;
		else{
			Graph wrdG1("Top Lyric Words (RAM)", memoryPDisplayWCounter.size(), memoryPDisplayWCounter);
			ArrList.push_back(wrdG1);
			Graph wrdG2("Top Lyric Words (Page)", memoryVDisplayWCounter.size(), memoryVDisplayWCounter);
			ArrList.push_back(wrdG2);
		}
		if (ArrList.size() > 0 && ArrList.size() <= 4)	plotGraph(ArrList, "Array-Based List Memory Graph (DISPLAY)");
	}

	/*
	Make a Graph with x axis being the length of the list and the y axis being memory use
	*/
	void makeRemoveMemGraph(){
		vector<Graph> ArrList(0);
		if (removePTime == -1)	cout << dark_red << "There is no data for removing song data. Please run the remove operation before doing this." << endl;
		else {
			Graph sonG1("Song Data (RAM)", memoryPRemoveMCounter.size(), memoryPRemoveMCounter);
			ArrList.push_back(sonG1);
			Graph sonG2("Song Data (Page)", memoryVRemoveMCounter.size(), memoryVRemoveMCounter);
			ArrList.push_back(sonG2);
		}
		if (ArrList.size() == 2)	plotGraph(ArrList, "Array-Based List Timings Graph (REMOVE)");
	}

	/*
	Make a Graph with x axis being the length of the list and the y axis being memory use
	*/
	void makeSeqSearchMemGraph(){
		vector<Graph> ArrList(0);
		if (sequSearchPTime == -1)	cout << dark_red << "There is no data for searching song data sequentially. Please run the seq. search operation before doing this." << endl;
		else {
			Graph sonG1("Song Data (RAM)", memoryPSeqSearchMCounter.size(), memoryPSeqSearchMCounter);
			ArrList.push_back(sonG1);
			Graph sonG2("Song Data (Page)", memoryVSeqSearchMCounter.size(), memoryVSeqSearchMCounter);
			ArrList.push_back(sonG2);
		}
		if (ArrList.size() == 2)	plotGraph(ArrList, "Array-Based List Timings Graph (SEQUENTIAL SEARCH)");
	}
	void makeBinaSearchMemGraph(){
		vector<Graph> ArrList(0);
		if (binaSearchPTime == -1)	cout << dark_red << "There is no data for searching song data sequentially. Please run the seq. search operation before doing this." << endl;
		else {
			Graph sonG1("Song Data (RAM)", memoryPBinaSearchMCounter.size(), memoryPBinaSearchMCounter);
			ArrList.push_back(sonG1);
			Graph sonG2("Song Data (Page)", memoryVBinaSearchMCounter.size(), memoryVBinaSearchMCounter);
			ArrList.push_back(sonG2);
		}
		if (ArrList.size() == 2)	plotGraph(ArrList, "Sorted Array-Based List Timings Graph (Binary SEARCH)");
	}

	/*
	Plot Graph Menu
	*/
	void plotGraphMenu(){
		printMenuTitle("Select a graph to view");
		settextcolor(white);
		cout << "1) " << yellow << "Add Items (Timings)" << white << endl;
		cout << "2) " << yellow << "Add Items (Memory)" << white << endl;
		cout << "3) " << yellow << "Display Items (Timings)" << white << endl;
		cout << "4) " << yellow << "Display Items (Memory)" << white << endl;
		cout << "5) " << yellow << "Remove Item (Timings)" << white << endl;
		cout << "6) " << yellow << "Remove Item (Memory)" << white << endl;
		cout << "7) " << yellow << "Sequential Search Items (Timings)" << white << endl;
		cout << "8) " << yellow << "Sequential Search Items (Memory)" << white << endl;
		cout << "9) " << yellow << "Binary Search Items (Timings)" << white << endl;
		cout << "10) " << yellow << "Binary Search Items (Memory)" << white << endl;
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
			//case 9: makeBinaSearchTImeGraph(); break;
			//case 10: makeBinaSearchMemGraph(); break;
			case 0: return;
			default: cout << dark_red << "Invalid Selection." << endl; break;
			}
		}
		else {
			cout << dark_red << "Selection must be an integer" << endl;
		}
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
		else {
			cout << dark_red << "Selection must be an integer" << endl;
		}
	}

	/*
	Main Pointer Based List Code
	@return Error Code (-1 for continue)
	*/
	int mainLoop(){
		ListArray mainMusicList, mainWordList, mainLyricList;
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
				case 3: searchSong(mainMusicList); break;
				case 4: removeMusicInfo(mainMusicList); break;
				case 5: plotGraphMenu(); break;
				case 8: performanceMenu(); break;
				case 9: return -1;
				case 0: return 0;
					//case 4: mainList.print(); break;
				default: cout << dark_red << "Invalid Selection." << endl; break;
				}
			}
			else {
				cout << dark_red << "Selection must be an integer" << endl;
			}
		}
		return 0;
	}
}
