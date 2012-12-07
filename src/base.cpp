



//Our own header
#include "draw.h"

#define NUM_THREADS 5
#define BUF_SIZE 255


//Special Function Variables
float angle = 0.0f, red = 1.0f, blue = 1.0f, green = 1.0f;
bool* keyStates = new bool[256]; // Create an array of boolean values of length 256
bool* keySpecialStates = new bool[246]; 
bool  fullscreen = false, leftMouseDown = false, rightMouseDown; 

//Variables for translating an object and Zoom
float xLocation = 0.0f; // Keep track of our position on the x axis.
float yLocation = 0.0f; // Keep track of our position on the y axis.
float zLocation = -10.0f;
float yRotationAngle = 0.0f; // The angle of rotation for our object
float xRotationAngle = 0.0f; // The angle of rotation for our object
float xdiff = 0.0f;
float ydiff = 0.0f;
float zdiff = 0.0f;

//FPS Variables
int frameCount = 0;
int frame=0,time,timebase=0;
char s[50];
int argcHold;
char* argsHold;

//  define the window position on screen
int window_x;
int window_y;

// Our screen size
const int width = 800;
const int height = 600;

//Lidar Variables
int point_count = 0; // how many points
vector<double> xcor;
vector<double> ycor;
vector<double> zcor;
vector<int> classification; // holds classification of each point whether it is a point on a tree or the ground	
GLdouble* g_vertex_LIDAR;

//Font Style
GLvoid *font_style = GLUT_BITMAP_TIMES_ROMAN_10;

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
	if(keySpecialStates[GLUT_KEY_HOME]){
		zLocation = -10.0f;
		xLocation = 0.0f;
		yLocation = 0.0f;
		xRotationAngle = 0.0f;
		yRotationAngle = 0.0f;
		glutPostRedisplay();
	}
	if(keySpecialStates[GLUT_KEY_PAGE_UP]){
		zLocation += 1.0f;
		glutPostRedisplay();
	}
	if(keySpecialStates[GLUT_KEY_PAGE_DOWN]){
		zLocation -= 1.0f;

		glutPostRedisplay();
	}
	if(keySpecialStates[GLUT_KEY_LEFT]){
		//cout<< "Left is press."<< endl;

		if (xLocation < -3.0f) ;
		else
			xLocation -= 0.0005f; // Move up along our xLocation

	}
	if(keySpecialStates[GLUT_KEY_RIGHT]){
		//cout<< "Right is press."<< endl;
		if (xLocation > 3.0f) ;
		else
			xLocation += 0.0005f; // Move up along our yLocation


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
	switch(button){
	case GLUT_LEFT_BUTTON:
		leftMouseDown = true;
		rightMouseDown = false;
		xdiff = x - yRotationAngle;
		ydiff = -y + xRotationAngle;
		break;
	case GLUT_RIGHT_BUTTON:
		leftMouseDown = false;
		rightMouseDown = true;
		zdiff = y - yLocation;
		break;
	default:
		leftMouseDown = false;
		rightMouseDown = false;
	}
}

void mouseMotion(int x, int y)
{
	if (leftMouseDown)
	{
		//cout<<"rotate"<<endl;
		yRotationAngle = x - xdiff;
		xRotationAngle = y + ydiff;

		glutPostRedisplay();
	}
	if (rightMouseDown){
		//cout<<"zoom"<<endl;
		zLocation = y - zdiff;
		glutPostRedisplay();
	}
}

void renderBitmapString(float x,float y,float z,void *font,char *string) {

	char *c;
	glRasterPos3f(x, y,z);
	for (c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c);
	}
}
void renderStrokeFontString(float x,float y,float z,void *font,char *string) {  

	char *c;
	glPushMatrix();
	glTranslatef(x, y,z);
	glScalef(0.002f, 0.002f, 0.002f);
	for (c=string; *c != '\0'; c++) {
		glutStrokeCharacter(font, *c);
	}
	glPopMatrix();
}
void restorePerspectiveProjection() {

	glMatrixMode(GL_PROJECTION);
	// restore previous projection matrix
	glPopMatrix();

	// get back to modelview mode
	glMatrixMode(GL_MODELVIEW);
}
void setOrthographicProjection() {

	// switch to projection mode
	glMatrixMode(GL_PROJECTION);

	// save previous matrix which contains the
	//settings for the perspective projection
	glPushMatrix();

	// reset matrix
	glLoadIdentity();

	// set a 2D orthographic projection
	gluOrtho2D(0, width, height, 0);

	// switch back to modelview mode
	glMatrixMode(GL_MODELVIEW);
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

void calcFPS(){
	// Code to compute frames per second
	frame++;

	time=glutGet(GLUT_ELAPSED_TIME);
	if (time - timebase > 1000) {
		sprintf(s,"FPS:%4.2f",
			frame*1000.0/(time-timebase));
		timebase = time;
		frame = 0;
	}

	// Code to display a string (fps) with bitmap fonts
	setOrthographicProjection();

	glPushMatrix();
	glLoadIdentity();
	renderBitmapString(5,30,0,GLUT_BITMAP_TIMES_ROMAN_24,s);
	glPopMatrix();
	restorePerspectiveProjection();
}

void drawSky(void){
	int i;
	//GLfloat ang, x, y, z = -50;
	srand(54654);
	// Clear Color and Depth Buffers
	//glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	double ratioPoints = .000001;
	glColor3f(red,green,blue);
	glBegin(GL_POINTS);
	double tempx,tempy,tempz;
	for(int i = 0; i< point_count; i++){
		tempx = xcor[i]*ratioPoints;
		tempy = ycor[i]*ratioPoints;
		tempz = zcor[i]*ratioPoints;
		//cout<< "Plotting at (" <<tempx<<","<<tempy<<","<<tempz<<")."<<endl;
		glVertex3f(tempx,tempy,tempz);
	}
	glEnd();

	//glutSwapBuffers();

}


void display (void) {
	keyOperations();
	specialKeyOperations();
//	glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Clear the background of our window to red
	glClear(GL_COLOR_BUFFER_BIT); //Clear the color buffer (more buffers later on)
	glLoadIdentity(); // Load the Identity Matrix to reset our drawing locations
	//gluLookAt(0, 0, -10, 0, 0, -1, 0, 1, 0);
	glTranslatef(xLocation, 0.0f, 0.0f); // Push everything 5 units back into the scene, otherwise we won't see the primitive

	glTranslatef(0.0f, yLocation, 0.0f); // Translate our object along the y axis
	glTranslatef(0.0f, 0.0f, zLocation);
	//glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f); // Rotate our object around the y axis
	glRotatef(xRotationAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(yRotationAngle, 0.0f, 1.0f, 0.0f);
	
	//glColor3f(red,green,blue);
	glColor3f( 0.4f, 0.7f, 2.2f );
	glPushMatrix();
	glutSolidTeapot(20.0f);
	glPopMatrix();
	//glPushMatrix();
	//drawSky();
	//glPopMatrix();
	// Draw X/Y/Z Lines
	glPushMatrix();
	glColor3f( 1.0f, 1.0f, 1.0f );
	glBegin( GL_LINES );
	glVertex3f( -100.0f, 0.0f, 0.0f );
	glVertex3f( 100.0f, 0.0f, 0.0f );
	glVertex3f( 0.0f, -100.0f, 0.0f );
	glVertex3f( 0.0f, 100.0f, 0.0f );
	glVertex3f( 0.0f, 0.0f, -100.0f );
	glVertex3f( 0.0f, 0.0f, 100.0f );            
	glEnd();
	glPopMatrix();

	//glutSolidTeapot( 20.0f );
	//calcFPS();
	//drawFPS();
	//renderPrimitive(); // Render the primitive
	//
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


void centerOnScreen ()
{
	window_x = (glutGet (GLUT_SCREEN_WIDTH) - width)/2;
	window_y = (glutGet (GLUT_SCREEN_HEIGHT) - height)/2;
}

void setupWindow(int argc, char* args[]){

	// Hide the mouse cursor
	//GLUT Window Initialization
	glutInit(&(argc),args);
	glutInitDisplayMode(GLUT_RGBA|GLUT_DEPTH|GLUT_DOUBLE);

	glutInitWindowSize(width,height);
	centerOnScreen ();
	glutInitWindowPosition(window_x ,window_y);
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

	// OpenGL init
	//glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	//initialize loop
	for(int i = 0; i < 256; i++) 
		keyStates[i] = false; 
	for(int i = 0; i < 246; i++)
		keySpecialStates[i] = false; 

	//enter GLUT event processing cycle
	glutMainLoop();
}

void setupGLFWwindow(){
	glfwOpenWindowHint(GLFW_FSAA_SAMPLES,4);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR,3);
	glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR,3);
	glfwOpenWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	if( !glfwOpenWindow( width, height, 0,0,0,0, 32,0, GLFW_WINDOW ) ){
		fprintf( stderr, "Failed to open GLFW window\n" );
		glfwTerminate();
	}
	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
	}
	glfwSetWindowTitle( "skyGesture Pre-Alpha GLFW Build v0.001a" );
	glfwEnable(GLFW_STICKY_KEYS);
	//glfwEnable(GLFW_STICKY_MOUSE_BUTTONS);
	//glfwDisable(GLFW_MOUSE_CURSOR);
	do{
		glfwSwapBuffers();
	}
	while(glfwGetKey(GLFW_KEY_ESC)!= GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));
}





int main( int argc, char* args[] ){
	unsigned int i;
	argcHold = argc;
	argsHold = *args;
	string input, buffer, filename;
	ofstream myfile;
	cout << "Welcome to skyGesture Application.\nThis program requires a .cvs file.\nPlease note that it has to be in the same directory as this executable.\n"<<endl;

	short col = 0; // Which column am i in max 4: x, y, z, classification
	string temp; // to hold and devide the lines from the file
	char *tempArr;
	char *pch; // for spliting the 
	// x, y, & z coordinates will be read in from a file and stored in a vector

	cout << "Trying to open file" << endl;
	ifstream file;
	//file.open("C:/Users/Juan/Downloads/lidar.csv"); // need to make this general
	cout << "Name of your .csv file: ";
	cin >> filename;
	filename += ".csv";
	cout << "Filename: " << filename << endl;
	file.open(filename);
	if(file.is_open())
	{
		cout << "File is open" << endl;
		getline(file,temp); // get the header out of the way

		//while(point_count <5)
		while(file.good())
		{
			getline(file,temp);
			//cout << temp << endl;
			tempArr = new char[temp.size()+1];
			tempArr[temp.size()] = 0;
			memcpy(tempArr, temp.c_str(), temp.size());
			//cout << tempArr << endl;
			pch = strtok(tempArr,",");
			col = 0;
			while(pch != NULL)
			{
				switch(col)
				{
				case 0:
					xcor.push_back(atof(pch));
					break;
				case 1:
					ycor.push_back(atof(pch));
					break;
				case 2:
					zcor.push_back(atof(pch));
					break;
				case 3:
					classification.push_back(atoi(pch));
					break;
				default:
					cout << "Don't print this out" << endl;
				}
				pch = strtok(NULL, ",");
				col++;
			}
			point_count++;
		}

		cout << "Read in " << point_count << " points" << endl;
		file.close();
		cout << "File has been closed" << endl;
		/*
		// print out the points
		cout << "X-coor\tY-coor\t\tZ-coor\tClass" << endl;
		for(int i=0; i<classification.size(); i++)
		{
		cout << xcor[i] << '\t' << ycor[i] << '\t' << zcor[i] << '\t' << classification[i] << endl;
		}
		*/
	}
	else
		cout << "Unable to open file" << endl;

	g_vertex_LIDAR = new GLdouble[point_count*3];
	int counter = 0;
	for(int k =0; k<point_count*3-3;k+=3){
		cout<<"Loop is at: " << k << "counter is at: "<<counter<<endl;
		cout<<xcor[counter]<<","<<ycor[counter]<<","<<zcor[counter]<<endl;
		g_vertex_LIDAR[k] =xcor[counter] ;
		g_vertex_LIDAR[k+1] = ycor[counter];
		g_vertex_LIDAR[k+1] = zcor[counter];
		counter++;

	}

	cout << "Type in 'glut' to start using GLUT Library or 'glfw' to use GLFW Library"<< endl;
	do{
		cout << ">>";
		getline(cin, input);
		if(input.compare("glut")==0){

			setupWindow(argc,args);
		}
		if(input.compare("glfw")==0){
			Draw *glfwWindow = new Draw();
			glfwWindow->draw(Draw::test);

			//glfwWindow->drawLIDAR(&g_vertex_LIDAR);
			glfwTerminate();
		}
	} while(input.compare("quit") != 0);
	delete g_vertex_LIDAR;
	return 0;    
}
