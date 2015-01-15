#include"Utility.h"
using namespace utility;
using namespace eku;
using namespace std;
void mainMenu();

void mainMenu()
{
	printSeperator();
	cout << red << "                    Main Menu (Select a storage type)" << endl;
	printSeperator();
	settextcolor(white);
	cout << "1) " << yellow << "Unsorted Pointer-Based List" << white << endl;
	cout << "2) " << yellow << "Sorted Pointer-Based List" << white << endl;
	cout << "3) " << yellow << "Unsorted Array-Based List" << white << endl;
	cout << "4) " << yellow << "Sorted Array-Based List" << white << endl;
	cout << "5) " << yellow << "View Performance Statistics" << white << endl;
	cout << "6) " << yellow << "Plot Performance Graphs" << white << endl;
	cout << "0) " << yellow << "Quit" << white << endl;

}

int main()
{
	//Initialization
	SetConsoleTitle(TEXT("DSA Read File Test Project"));
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