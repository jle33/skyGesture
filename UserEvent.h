#ifndef _USER_EVENT_H
#define _USER_EVENT_H

#include <SDL.h>

class UserEvent{

public:
	UserEvent();
	~UserEvent();

	void processEvent(SDL_Event *event);
protected:
	virtual void onKeyDown(SDL_Event *event ) = 0;
	virtual void onKeyUp(SDL_Event *event) = 0;
	virtual void onQuit(SDL_Event *event) = 0;

};

#endif