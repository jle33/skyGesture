#include "UserEvent.h"

UserEvent::UserEvent(){

}

UserEvent::~UserEvent(){

}
void UserEvent::processEvent(SDL_Event *event){
	int type = event->type;
	switch (type)
	{
	case SDL_KEYDOWN:
		onKeyDown(event);
		break;
	case SDL_KEYUP:
		onKeyUp(event);
		break;
	case SDL_QUIT:
		onQuit(event);
	}

}

