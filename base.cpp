//#include "SDL.h"
//#include "glut.h"

#include "draw.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main( int argc, char* args[] )
{
	Draw *d = new Draw;
	string input;
	ofstream myfile;
	cout << "Welcome to skyGesture Application.\nThis program requires a .cvs file.\nPlease note that it has to be in the same directory as this executable.\n"<<endl;
	cout << "Enter file name: "<< endl;
	cin >> input;
	
	do{
		getline(cin, input);
		if(input.compare("line")==0)
			d->draw(Draw::line);
		if(input.compare("triangle")==0)
			d->draw(Draw::triangle);
		if(input.compare("square")==0)
			d->draw(Draw::square);
		if(input.compare("better square")==0)
			d->draw(Draw::better);
		if(input.compare("sky")==0)
			d->draw(Draw::sky);
	} while(input.compare("quit") != 0);

	delete d;
	//Start SDL
	//SDL_Init( SDL_INIT_VIDEO );

	//Quit SDL
	//SDL_Quit();

	return 0;    
}