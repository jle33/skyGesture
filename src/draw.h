#ifndef _DRAW_H
#define DRAW_H
//Standard Library
#include <stdio.h>
#include <stdlib.h>

//OpenGL Headers
#include <GL\glew.h>
#include "GL\glut.h"
#include <GL\glfw.h>
#include <glm\glm.hpp>

class Draw{
public:
	enum Type{
		line,
		triangle,
		square,
		better,
		sky
	};

	Draw();
	~Draw();

	void draw (Type type);
private:
	void setupWindow();
	void setupGL();
	void drawLine();
	void drawTriangle();
	void drawSquare();
	void drawBetterSquare();
	void drawSky();

private:
	//SDL_Surface *window;
	int width;
	int height;
};
#endif