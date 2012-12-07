#ifndef _DRAW_H
#define DRAW_H
//Windows Library
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>
#include <psapi.h>

//Standard Library
#include <math.h>
#include <iostream>
#include <fstream>
#include <string>
#include <istream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

//OpenGL Headers
#include <GL\glew.h>
#include "GL\glut.h"
#include <GL\glfw.h>
#include <glm\glm.hpp>

using namespace std;
using namespace glm;

class Draw{
public:
	enum Type{
		pot,
		sky,
		test

	};

	Draw();
	~Draw();

	void draw (Type type);
	void drawLIDAR(GLdouble *data[]);
private:
	void setupWindow();
	void setupGL();
	void drawPot();
	void drawSky();
	void drawTest();
	
private:
	int width;
	int height;
};
#endif