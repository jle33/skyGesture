#include "draw.h"

Draw::Draw():width(800),height(600){
	if(!glfwInit()){
		fprintf (stderr, "Failed to initialize GLFW\n");
		exit(-1);
	}
	
	setupWindow();
	setupGL();
}

Draw::~Draw(){
	
}

void Draw::draw(Type type){
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();
	glOrtho(0, width, height , 0, -1.0, 1.0);
	switch (type){
	case line:
		drawLine();
		break;
	case triangle:
		drawTriangle();
		break;
	case square:
		drawSquare();
		break;
	case better:
		drawBetterSquare();
		break;
	case sky:
		drawSky();

	}
	glPopMatrix();
	glfwSwapBuffers();
}

void Draw::setupWindow(){
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
	glfwEnable(GLFW_STICKY_MOUSE_BUTTONS);
	glfwDisable(GLFW_MOUSE_CURSOR);
	do{
		glfwSwapBuffers();
	}
	while(glfwGetKey(GLFW_KEY_ESC)!= GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED));
	glfwTerminate();
}

void Draw::setupGL(){
	//glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

}
void Draw::drawLine(){
	glBegin(GL_LINES);
	glColor3f(1.0,0.0,0.0);
	glVertex2f(10.0,10.0);
	glVertex2f(50.0,50.0);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(0.0,1.0,0.0);
	glVertex2f(100.0,250.0);
	glVertex2f(450.0,500.0);
	glEnd();
}
void Draw::drawTriangle(){
	glBegin(GL_TRIANGLES);
	glColor3f(1.0,0.0,0.0);
	glVertex2f(200.0,10.0);

	glColor3f(0.0,1.0,0.0);
	glVertex2f(350.0,250.0);
	
	glColor3f(0.0,0.0,1.0);
	glVertex2f(180.0,180.0);
	glEnd();
}
void Draw::drawSquare(){
	glBegin(GL_QUADS);


}
void Draw::drawBetterSquare(){

}
void Draw::drawSky(){
	float x[150];
	float y[150];
	srand(54654);

	glBegin(GL_POINTS);
	glColor3f(0.3,0.7,0.9);
	for (int i = 0; i < 150; i++){
		x[i] = rand()%width;
		y[i] = rand()%height;

		glVertex2f(x[i],y[i]);
	}
	glEnd();
}

