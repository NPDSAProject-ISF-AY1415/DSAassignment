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
	int menErr = -1;
	while (menErr == -1){
		mainMenu();
		string selection;
		cout << pink << "Select an option: ";
		getStringInput(selection);
		if (is_number(selection)){
			switch (stoi(selection))
			{
			case 1:
			case 2:
			case 3:
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