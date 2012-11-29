#ifndef _DRAW_H
#define DRAW_H

#include "SDL.h"
class Draw{
public:
	enum Type{
		sky
	};
	Draw();
	Draw(int argc, char** argv);
	~Draw();

	void draw (Type type);
private:
	void setupWindow();
	void setupGL();
	//void drawSky(void);

private:
	SDL_Surface *window;
	int width;
	int height;
	int argc;
	char** argv;
};
#endif