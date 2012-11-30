//#include "SDL.h"
#include "GL\glut.h"
#include <GL\GL.h>
#include <GL\glfw.h>
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <istream>

using namespace std;

const int width = 800;
const int height = 600;
float angle = 0.0f, red = 1.0f, blue = 1.0f, green = 1.0f;
bool* keyStates = new bool[256]; // Create an array of boolean values of length 256
bool* keySpecialStates = new bool[246]; 
bool  fullscreen = false, mouseDown = false; 
float xLocation = 0.0f; // Keep track of our position on the x axis.
float yLocation = 0.0f; // Keep track of our position on the y axis.
float yRotationAngle = 0.0f; // The angle of rotation for our object
float xRotationAngle = 0.0f; // The angle of rotation for our object
float xdiff = 0.0f;
float ydiff = 0.0f;


void pressNormalKeys(unsigned char key, int x, int y) {keyStates[key] = true; }
void releaseNormalKeys(unsigned char key, int x, int y) {keyStates[key] = false; }
void pressSpecialKeys(int key, int x, int y) {keySpecialStates[key] = true;}
void releaseSpecialKeys(int key, int x, int y) {keySpecialStates[key] = false;}

void keyOperations (void) { 
	if (keyStates[27]) {exit(1);}

}	
void specialKeyOperations(void){
	if(keySpecialStates[GLUT_KEY_F1 ]){
		fullscreen = !fullscreen;
		if (fullscreen)
			glutFullScreen();
		else{
			glutReshapeWindow(width, height);
			glutPositionWindow(50, 50);
		}
	}
	if(keySpecialStates[GLUT_KEY_F2 ]){
		red = 1.0;
		green = 0.0;
		blue = 0.0;
	}
	if(keySpecialStates[GLUT_KEY_F3 ]){
		red = 0.0;
		green = 1.0;
		blue = 0.0;
	}
	if(keySpecialStates[GLUT_KEY_F4 ]){
		red = 0.0;
		green = 0.0;
		blue = 1.0;
	}
	if(keySpecialStates[GLUT_KEY_LEFT]){
		cout<< "Left is press."<< endl;
		yRotationAngle -= 0.005f; // Increment our rotation value
		if (yRotationAngle < 0.0f) // If we have rotated beyond 360 degrees (a full rotation)
			yRotationAngle += 360.0f; // Add 360 degrees off of our rotation
		
	}
	if(keySpecialStates[GLUT_KEY_RIGHT]){
		cout<< "Right is press."<< endl;
		yRotationAngle += 0.005f; // Increment our rotation value

		if (yRotationAngle > 360.0f) // If we have rotated beyond 360 degrees (a full rotation)
			yRotationAngle -= 360.0f; // Subtract 360 degrees off of our rotation
	}
	if(keySpecialStates[GLUT_KEY_UP]){
		
		if (yLocation > 3.0f) ;
		else
			yLocation += 0.0005f; // Move up along our yLocation
		
		
	}
	if(keySpecialStates[GLUT_KEY_DOWN]){
		if (yLocation < -3.0f) ;
		else
			yLocation -= 0.0005f; // Move up along our yLocation
	}


}
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		mouseDown = true;

		xdiff = x - yRotationAngle;
		ydiff = -y + xRotationAngle;
	}
	else
		mouseDown = false;
}

void mouseMotion(int x, int y)
{
	if (mouseDown)
	{
		yRotationAngle = x - xdiff;
		xRotationAngle = y + ydiff;

		glutPostRedisplay();
	}
}
void renderPrimitive (void) {
	glColor3f(0.0f, 0.0f, 1.0f); // Set the colour of the square to blue

	glBegin(GL_QUADS); // Start drawing a quad primitive
	glVertex3f(-1.0f, -1.0f, 0.0f); // The bottom left corner
	glVertex3f(-1.0f, 1.0f, 0.0f); // The top left corner
	glVertex3f(1.0f, 1.0f, 0.0f); // The top right corner
	glVertex3f(1.0f, -1.0f, 0.0f); // The bottom right corner
	glEnd();
}


void drawSky(void){
	int i;
	GLfloat ang, x, y, z = -50;
	srand(54654);
	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glColor3f(red,green,blue);
	glBegin(GL_TRIANGLES);
	glVertex3f(-0.5,-0.5,0.0);
	glVertex3f(0.5,0.0,0.0);
	glVertex3f(0.0,0.5,0.0);

	glEnd();

	glutSwapBuffers();

}

void display (void) {
	keyOperations();
	specialKeyOperations();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear the background of our window to red
	glClear(GL_COLOR_BUFFER_BIT); //Clear the color buffer (more buffers later on)
	glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
	
	glTranslatef(0.0f, 0.0f, -5.0f); // Push everything 5 units back into the scene, otherwise we won't see the primitive

	glTranslatef(0.0f, yLocation, 0.0f); // Translate our object along the y axis

	//glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate our object around the y axis
	glRotatef(xRotationAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f);

	glutWireCube(2.0f); // Render the primitive  
	//renderPrimitive(); // Render the primitive
	//drawSky();
	glutSwapBuffers(); // Flush the OpenGL buffers to the window

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

	//glfwDisable(GLFW_MOUSE_CURSOR); // Hide the mouse cursor
	//GLUT Window Initialization
	glutInit(&(argc),args);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(width,height);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glutCreateWindow("skyGesture Pre-Alpha Build v0.001a");

	//register callback
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutIdleFunc(display);
	glutKeyboardFunc(pressNormalKeys);
	glutKeyboardUpFunc(releaseNormalKeys);
	glutSpecialFunc(pressSpecialKeys);
	glutSpecialUpFunc(releaseSpecialKeys);
	glutMouseFunc(mouse);
	glutMotionFunc(mouseMotion);

	//initialize loop
	for(int i = 0; i < 256; i++) 
		keyStates[i] = false; 
	for(int i = 0; i < 246; i++)
		keySpecialStates[i] = false; 
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glClearDepth(1.0f);
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