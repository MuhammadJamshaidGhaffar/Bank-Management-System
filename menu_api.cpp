#include "menu_api.h"

HANDLE createHandle()
{
	HANDLE hStdout;       
	return GetStdHandle(STD_OUTPUT_HANDLE);
}
WORD GetColor(HANDLE hStdout){
    CONSOLE_SCREEN_BUFFER_INFO info;
    GetConsoleScreenBufferInfo(hStdout, &info);
    return info.wAttributes;
}

MenuClass::MenuClass(std::vector <std::string> p_options , WORD p_markerColor , const int p_OPTIONS_COORD_X,const int p_OPTIONS_COORD_Y, const int p_GAP_BTW_OPTIONS)
:	m_OPTIONS_COORD_X {p_OPTIONS_COORD_X} , 
	m_OPTIONS_COORD_Y {p_OPTIONS_COORD_Y} , 
	m_GAP_BTW_OPTIONS {p_GAP_BTW_OPTIONS} ,
	m_options { p_options} ,
	m_max_options { m_options.size()} , 
	m_markerColor { p_markerColor}
{}

void MenuClass::printOptions(HANDLE hStdout , COORD destCoord){
	for(int i=0; i<m_max_options; i++)  // Loop will iterate through options vector and print it on the screen
	{
		destCoord.X = m_OPTIONS_COORD_X ;
		destCoord.Y = m_OPTIONS_COORD_Y+( i*m_GAP_BTW_OPTIONS ) ;  
		SetConsoleCursorPosition(hStdout, destCoord);
		std::cout << m_options[i];
	}
}

void MenuClass::removeMarker(HANDLE hStdout , COORD destCoord , int option)
{
	//---Removing  Cursor on currently seleceted option before chnaging option
	destCoord.X = m_OPTIONS_COORD_X;
	destCoord.Y = m_OPTIONS_COORD_Y + ((option-1) *m_GAP_BTW_OPTIONS);  // options - 1 becuase cursor coords starts from zero not from 1 
	SetConsoleCursorPosition(hStdout, destCoord);   
	std::cout << m_options[option-1];
}
void MenuClass::printMarker(HANDLE hStdout , COORD destCoord , int option)
{
	//---Printing Cursor on currently seleceted option
	destCoord.X = m_OPTIONS_COORD_X;
	destCoord.Y = m_OPTIONS_COORD_Y + ((option-1) *m_GAP_BTW_OPTIONS);  // options - 1 becuase cursor coords starts from zero not from 1 
	SetConsoleCursorPosition(hStdout, destCoord); 
	WORD currentColor = GetColor(hStdout);
	SetConsoleTextAttribute(hStdout , m_markerColor);   
	std::cout << m_options[option-1];
	SetConsoleTextAttribute(hStdout , currentColor);
}


int MenuClass::optionsLoop(HANDLE hStdout , COORD destCoord)
{
	printOptions(hStdout , destCoord);
	int option =1; // this is the varaible of currently selected option
	//---Now initial start ends here and we will use infinte loop to move the cursor through options
	while(true)
	{
		//---Printing Cursor on currently seleceted option
		printMarker(hStdout , destCoord , option);   
		char key; // declaring a variable char to hold key from keyboard input
		key = getch(); // waiting and then getting pressed key from keyboard
		switch(key)
		{
			case KEY_DOWN:
				{
					//---Removing  Cursor on currently seleceted option before chnaging option
					removeMarker(hStdout , destCoord , option);      
					if(option >= m_max_options) // if Option is last then set option to 1
					{
						option = 1;
						break;
					}
					++option;   // otherwise just increase option by +1
					break;
				}
			case KEY_UP:
				{
					//---Removing  Cursor on currently seleceted option before chnaging option
					removeMarker(hStdout , destCoord , option);      
					if(option <= 1) // if Option is first then set option to last
					{
						option = m_max_options;
						break;
					}
					--option;   // otherwise just decrease option by -1
					break;
				}
			case KEY_ENTER:
				{	
					return option;
				}
			case KEY_ESC:
				{
					return 0 ; // if ESC key is pressed just return back to previous menu which will be function
				}
				
		}
	}
}

void MenuClass::changeCoords(COORD pos, int p_gap)
{
	m_OPTIONS_COORD_X = pos.X;
	m_OPTIONS_COORD_Y = pos.Y ;
	m_GAP_BTW_OPTIONS = p_gap;
}

COORD MenuClass::getCoords()
{
	return {m_OPTIONS_COORD_X , m_OPTIONS_COORD_Y};
}
int MenuClass::getGapBtwOptions ()
{
	return m_GAP_BTW_OPTIONS;
}
