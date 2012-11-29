#include <stdlib.h>
#include "GL\glut.h"
#include "draw.h"

Draw::Draw(): window(NULL),width(600),height(400){
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
	SDL_GL_SwapBuffers();
}

void Draw::setupWindow(){
	if(SDL_Init(SDL_INIT_VIDEO))
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

