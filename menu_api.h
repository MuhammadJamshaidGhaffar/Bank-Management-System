#ifndef MENU_API_H
#define MENU_API_H

//------- includes------------
#include <iostream> // for std::cout
#include <windows.h> // for SetConsoleCursorPosition(x,y)
#include <vector> //for vector
#include <string> //for strings
#include <conio.h>
//------- Global Variables-------------
const char KEY_UP = 72;  // this is the ASCII code of keys
const char KEY_DOWN = 80;
const char KEY_LEFT = 75;
const char KEY_RIGHT = 77;
const char KEY_ENTER = 13;
const char KEY_ESC = 27;

//---- classes -------
class MenuClass{
	private:
	const int m_OPTIONS_COORD_X ;   // This is  the Options  Base Coords X and Y
	const int m_OPTIONS_COORD_Y ;   // Change this to change your Positions Coords
	const int m_GAP_BTW_OPTIONS ; // it is the gap between two options.. Change it to change the gap        
	const std::vector <std::string> m_options ; //  Add your Options Text Here
	const int m_max_options;
	public:
		MenuClass(std::vector <std::string> p_options , const int p_OPTIONS_COORD_X = 14 ,const int p_OPTIONS_COORD_Y = 5, const int p_GAP_BTW_OPTIONS = 2 );
		void printOptions(HANDLE hstdout , COORD destCoord);
		int optionsLoop(HANDLE hStdout , COORD destCoord);
		void removeCursor(HANDLE hStdout , COORD destCoord , int option);
		void printCursor(HANDLE hStdout , COORD destCoord , int option);
		
};

//------- functions --------------
HANDLE createHandle();


#endif
