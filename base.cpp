//#include "SDL.h"
#include "GL\glut.h"

#include <iostream>
#include <fstream>
#include <string>
#include <istream>

using namespace std;

const int width = 800;
const int height = 600;
float angle = 0.0f;

void drawSky(void){
	GLfloat x[150];
	GLfloat y[150];
	GLfloat z[150];
	srand(54654);
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//Reset transformation
	glLoadIdentity();
	
	// Set the camera
	gluLookAt(	0.0f, 0.0f, 10.0f,
		0.0f, 0.0f,  0.0f,
		0.0f, 1.0f,  0.0f);

	glRotatef(angle, 0.0f, 1.0f, 0.0f);
	glBegin(GL_POINTS);
	//glVertex3f(1.5f,.5f,0.0f);
	//glVertex3f(0.f,0.0f,0.0f);

	
	for (int i = 0; i < 5; i++){
		x[i] =  rand() / double(RAND_MAX);
		y[i] = rand() / double(RAND_MAX);
		z[i] = rand() / double(RAND_MAX);
		glVertex3f(x[i],y[i],0);
		cout<< "Plotting points at x: "<< x[i] << ", y: " << y[i] <<", z: "<<z[i]<<endl;
	glEnd();
	glutSwapBuffers();
}

void reshape(int w, int h){
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if(h == 0)
		h = 1;
	float ratio = 1.0* w / h;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(45,ratio,1,1000);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);
}
void setupWindow(int argc, char* args[]){
	//GLUT Window Initialization
	glutInit(&(argc),args);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(width,height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glutCreateWindow("skyGesture Pre-Alpha Build v0.001a");

	//register callback
	glutDisplayFunc(drawSky);
	glutReshapeFunc(reshape);
	glutIdleFunc(drawSky);
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