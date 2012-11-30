#include <windows.h>
#include <iostream>
using namespace std;

void LeftHold()
{
	INPUT Input={0};	// Create our input, 0 for mouse, 1 for keyboard, 2 for hardware
	Input.type = INPUT_MOUSE;	// Let input know we are using the mouse.
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;	// We are setting left mouse button down.
	SendInput( 1, &Input, sizeof(INPUT) );	// Send the input.
	ZeroMemory(&Input,sizeof(INPUT));	//Clear memory
}

void LeftRelease()
{
	INPUT Input={0};							
	Input.type = INPUT_MOUSE;									
	Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;	// setting left mouse button up.
	SendInput( 1, &Input, sizeof(INPUT) );								
}
void RightHold()
{
	INPUT Input = {0};
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput( 1, &Input, sizeof(INPUT));
	ZeroMemory(&Input, sizeof(INPUT));
}

void RightRelease()
{
	INPUT Input = {0};
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags  = MOUSEEVENTF_RIGHTUP;
	SendInput( 1, &Input, sizeof(INPUT) );
}

void LeftClick()
{  
	INPUT Input={0};													
	Input.type = INPUT_MOUSE;									
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;							
	SendInput( 1, &Input, sizeof(INPUT) );								

	ZeroMemory(&Input,sizeof(INPUT));									
	Input.type = INPUT_MOUSE;									
	Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;								
	SendInput( 1, &Input, sizeof(INPUT) );								
}


void RightClick()
{
	INPUT Input = {0};
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput( 1, &Input, sizeof(INPUT));

	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags  = MOUSEEVENTF_RIGHTUP;
	SendInput( 1, &Input, sizeof(INPUT) );
}

// Returns the mouses current position, m holds x and y - May need to calculate where the mouse should  move based on sensor?
POINT GetMousePosition()
{
	static POINT m;
	POINT mouse;
	GetCursorPos(&mouse);
	m.x = mouse.x;
	m.y = mouse.y;
	return m;
}

// Sets the mouses position.
void SetMousePosition(POINT mp)
{
	long fScreenWidth = GetSystemMetrics( SM_CXSCREEN ) - 1; 
	long fScreenHeight = GetSystemMetrics( SM_CYSCREEN ) - 1; 

	// http://msdn.microsoft.com/en-us/library/ms646260(VS.85).aspx
	// If MOUSEEVENTF_ABSOLUTE value is specified, dx and dy contain normalized absolute coordinates between 0 and 65,535.
	// The event procedure maps these coordinates onto the display surface.
	// Coordinate (0,0) maps onto the upper-left corner of the display surface, (65535,65535) maps onto the lower-right corner.
	float fx = mp.x * ( 65535.0f / fScreenWidth  );
	float fy = mp.y * ( 65535.0f / fScreenHeight );		  

	INPUT Input = { 0 };			
	Input.type = INPUT_MOUSE;

	Input.mi.dwFlags = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;

	//Storing x, y coordinates into mouse input
	Input.mi.dx	= (long)fx;
	Input.mi.dy	= (long)fy;

	//Sends it to input to be processed
	SendInput(1,&Input,sizeof(INPUT));
}
					

int main()
{
	//Get the current console window.
	HWND myWindow;
	AllocConsole();
	myWindow = FindWindowA( "ConsoleWindowClass", NULL );

	// Variables
	bool runme = true;
	int i = 0;
	int x = 0;
	int y = 0;

	while(runme)
	{
		long fScreenWidth = GetSystemMetrics( SM_CXSCREEN ) - 1; 
		long fScreenHeight = GetSystemMetrics( SM_CYSCREEN ) - 1; 
		float fx = x * ( 65535.0f / fScreenWidth  );
		float fy = y * ( 65535.0f / fScreenHeight );		  

		INPUT Input = { 0 };			
		Input.type = INPUT_MOUSE;

		Input.mi.dwFlags = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;

		Input.mi.dx = (long)fx;
		Input.mi.dy	= (long)fy;
		cout<< "x" << fx << "\ty" << fy << endl;
		//This sleep timer, just slows down the mouse cursor, it went too fast so ya
		Sleep(2);
		SendInput(1,&Input,sizeof(INPUT));
		x++;
		y++;
		i++;
		if(i > 1000){
			runme = false;

			//This will move the mouse to that coordinate, useful if we want to pass it to a hand
			static POINT goPos;
			goPos.x = 480;
			goPos.y = 240;
			SetMousePosition(goPos);
		}

	}
	system("pause");
}
