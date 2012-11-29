#include <stdlib.h>
#include "GL\glut.h"
#include "draw.h"


Draw::Draw(): window(NULL),width(600),height(400){
	setupWindow();
	setupGL();
}
Draw::Draw(int argc, char** argv): window(NULL),width(600),height(400){
	this->argc = argc;
	this->argv = argv;
	setupWindow();
	setupGL();
}

Draw::~Draw(){
	SDL_Quit();
}

void Draw::draw(Type type){
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

	glPushMatrix();
	glOrtho(0, width, height , 0, -1.0, 1.0);
	switch (type){
	case sky:
		drawSky();

	}
	glPopMatrix();
	SDL_GL_SwapBuffers();
}

/*void Draw::setupWindow(){
	if(SDL_Init(SDL_INIT_EVERYTHING))
		exit(1);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,8);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,32);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER,1);
	SDL_WM_SetCaption( "skyGesture Pre-Alpha Build v0.001a", NULL );
	if((window = SDL_SetVideoMode(width,height,32,SDL_OPENGL)) == NULL)
		exit(1);
}*/


void Draw::setupWindow(){
	//GLUT Window Initialization
	glutInit(&(this->argc),this->argv);
	glutInitDisplayMode(GLUT_RGB|GLUT_DEPTH|GLUT_DOUBLE|GLUT_ALPHA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(this->width,this->height);
	glutCreateWindow("skyGesture Pre-Alpha Build v0.001a");

	//register callback
	glutDisplayFunc(drawSky);
	//enter GLUT event processing cycle
	glutMainLoop();
}

void Draw::setupGL(){
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glClearColor(0.0, 0.0, 0.0, 1.0);
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glClear(GL_COLOR_BUFFER_BIT);
	glLoadIdentity();

}


