//#include "SDL.h"
#include "GL\glut.h"

//#include "draw.h"
#include <iostream>
#include <fstream>
#include <string>
#include <istream>

using namespace std;

const int width = 800;
const int height = 600;

void drawSky(void){
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	float x[150];
	float y[150];
	float z[150];
	srand(54654);

	glBegin(GL_POINTS);
	glColor3f(0.3,0.7,0.9);
	for (int i = 0; i < 150; i++){
		x[i] = rand()%width;
		y[i] = rand()%height;
		z[i] = rand()%2;
		glVertex3f(x[i],y[i],z[i]);
	}
	glEnd();
	glutSwapBuffers();
}


void setupWindow(int argc, char* args[]){
	//GLUT Window Initialization
	glutInit(&(argc),args);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(width,height);
	glutCreateWindow("skyGesture Pre-Alpha Build v0.001a");

	//register callback
	glutDisplayFunc(drawSky);
	//enter GLUT event processing cycle
	glutMainLoop();
}








int main( int argc, char* args[] )
{
	
	string input, buffer;
	ofstream myfile;
	cout << "Welcome to skyGesture Application.\nThis program requires a .cvs file.\nPlease note that it has to be in the same directory as this executable.\n"<<endl;
	/*
	cout << "Enter file name: "<< endl;
	cin >> input;
	myfile.open (input);
	if (myfile.is_open()){
		while ( myfile.good() ){
			getline(myfile,buffer);
			cout << buffer << endl;
		}
		myfile.close();
	}

	else cout << "Unable to open file"; 
	*/
		cout << "Type in 'draw points' to start and 'quit' to quit:"<< endl;
	do{
		cout << ">>";
		getline(cin, input);
		if(input.compare("draw points")==0){
			setupWindow(argc,args);
		}
	} while(input.compare("quit") != 0);

	return 0;    
}