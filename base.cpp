//#include "SDL.h"
//#include "glut.h"

#include "draw.h"
#include <iostream>
#include <fstream>
#include <string>
#include <istream>
using namespace std;

int main( int argc, char* args[] )
{
	Draw *d = new Draw;
	string input, buffer;
	ofstream myfile;
	cout << "Welcome to skyGesture Application.\nThis program requires a .cvs file.\nPlease note that it has to be in the same directory as this executable.\n"<<endl;
	cout << "Enter file name: "<< endl;
	cin >> input;
	myfile.open (input);
	if (myfile.is_open())
	{
		while ( myfile.good() )
		{
			getline(myfile,buffer);
			cout << buffer << endl;
		}
		myfile.close();
	}

	else cout << "Unable to open file"; 


	do{
		getline(cin, input);
		if(input.compare("draw points")==0)
			d->draw(Draw::sky);
	} while(input.compare("quit") != 0);

	delete d;


	return 0;    
}