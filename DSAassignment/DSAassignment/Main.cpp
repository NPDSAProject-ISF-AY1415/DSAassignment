#include <Windows.h>	//Console Utility
#include <iostream>
#include "Utility.h"
#include "PtrArr.h"
#include "Memory.h"

using namespace std;
using namespace utility;
using namespace eku;

/*
Prints out the main menu
*/
void mainMenu(){
	printSeperator();
	cout << red << "                      Main Menu (Select a storage type)" << endl;
	printSeperator();
	settextcolor(white);
	cout << "1) " << yellow << "Unsorted Pointer-Based List" << white << endl;
	cout << "2) " << yellow << "Sorted Pointer-Based List" << white << endl;
	cout << "3) " << yellow << "Unsorted Array-Based List" << white << endl;
	cout << "4) " << yellow << "Sorted Array-Based List" << white << endl;
	cout << "5) " << yellow << "Performance Utilities" << white << endl;
	cout << "6) " << yellow << "Plot Performance Graphs" << white << endl;
	cout << "0) " << yellow << "Quit" << white << endl;
}


/*
Main Method
@return End Error Code
*/
int main(){
	//Initialization
	SetConsoleTitle(TEXT("Data Structure Performance Analysis (DSA Assignment)"));
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
			case 1: menErr = ptrarr::mainLoop(); break;
			case 2: cout << dark_red << "Soon to be implemented" << endl; break;	//TODO Replace with main loop for sorted pointer
			case 3: cout << dark_red << "Soon to be implemented" << endl; break;	//TODO Replace with main loop for unsorted array
			case 4: cout << dark_red << "Soon to be implemented" << endl; break;	//TODO Replace with main loop for sorted array
			case 5: ptrarr::performanceMenu(); break;		//Temp, soon will be a menu specifically for main menu
			case 6: ptrarr::plotGraphMenu(); break;			//Temp, soon will be a menu designed for main menu
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