/****************************************************************************
*                                                                           *
*  OpenNI 1.x Alpha                                                         *
*  Copyright (C) 2011 PrimeSense Ltd.                                       *
*                                                                           *
*  This file is part of OpenNI.                                             *
*                                                                           *
*  OpenNI is free software: you can redistribute it and/or modify           *
*  it under the terms of the GNU Lesser General Public License as published *
*  by the Free Software Foundation, either version 3 of the License, or     *
*  (at your option) any later version.                                      *
*                                                                           *
*  OpenNI is distributed in the hope that it will be useful,                *
*  but WITHOUT ANY WARRANTY; without even the implied warranty of           *
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the             *
*  GNU Lesser General Public License for more details.                      *
*                                                                           *
*  You should have received a copy of the GNU Lesser General Public License *
*  along with OpenNI. If not, see <http://www.gnu.org/licenses/>.           *
*                                                                           *
****************************************************************************/
//---------------------------------------------------------------------------
// Includes
//---------------------------------------------------------------------------
#include "NiHandTracker.h"
#include <cassert>
#include <Windows.h>
#include <WinUser.h>
#include <iostream>
using namespace std;


using namespace xn;


//---------------------------------------------------------------------------
// Defines
//---------------------------------------------------------------------------
#define LENGTHOF(arr)			(sizeof(arr)/sizeof(arr[0]))
#define FOR_ALL(arr, action)	{for(int i = 0; i < LENGTHOF(arr); ++i){action(arr[i])}}

#define ADD_GESTURE(name)		{if(m_GestureGenerator.AddGesture(name, NULL) != XN_STATUS_OK){printf("Unable to add gesture"); exit(1);}}
#define REMOVE_GESTURE(name)	{if(m_GestureGenerator.RemoveGesture(name) != XN_STATUS_OK){printf("Unable to remove gesture"); exit(1);}}

#define ADD_ALL_GESTURES		FOR_ALL(cGestures, ADD_GESTURE)
#define REMOVE_ALL_GESTURES		FOR_ALL(cGestures, REMOVE_GESTURE)


//---------------------------------------------------------------------------
// Consts
//---------------------------------------------------------------------------
// Gestures to track
static const char			cClickStr[] = "Click";
static const char			cWaveStr[] = "Wave";
static const char* const	cGestures[] =
{
	cClickStr,
	cWaveStr
};

bool HandDetect = false;
bool RightClicked = false;
bool LeftClicked = false;

bool handID_1 = false;
bool handID_2 = false;

int Hands = 0;
int MaxHands = 2;
int ForceMaxId = 2;
long sent = 3.5;

//---------------------------------------------------------------------------
// Statics
//---------------------------------------------------------------------------
XnListT<HandTracker*>	HandTracker::sm_Instances;



//-------------------------
//Mouse
//-------------------------

void LeftHold()
{
	INPUT Input={0};	// Create our input, 0 for mouse, 1 for keyboard, 2 for hardware
	Input.type = INPUT_MOUSE;	// Let input know we are using the mouse.
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;	// We are setting left mouse button down.
	SendInput( 1, &Input, sizeof(INPUT) );	// Send the input.
	ZeroMemory(&Input,sizeof(INPUT));	//Clear memory
}

void LeftRelease()
{
	INPUT Input={0};							
	Input.type = INPUT_MOUSE;									
	Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;	// setting left mouse button up.
	SendInput( 1, &Input, sizeof(INPUT) );								
}
void RightHold()
{
	INPUT Input = {0};
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput( 1, &Input, sizeof(INPUT));
	ZeroMemory(&Input, sizeof(INPUT));
}

void RightRelease()
{
	INPUT Input = {0};
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags  = MOUSEEVENTF_RIGHTUP;
	SendInput( 1, &Input, sizeof(INPUT) );
}

void LeftClick()
{  
	INPUT Input={0};													
	Input.type = INPUT_MOUSE;									
	Input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;							
	SendInput( 1, &Input, sizeof(INPUT) );								

	ZeroMemory(&Input,sizeof(INPUT));									
	Input.type = INPUT_MOUSE;									
	Input.mi.dwFlags  = MOUSEEVENTF_LEFTUP;								
	SendInput( 1, &Input, sizeof(INPUT) );								
}


void RightClick()
{
	INPUT Input = {0};
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
	SendInput( 1, &Input, sizeof(INPUT));

	ZeroMemory(&Input, sizeof(INPUT));
	Input.type = INPUT_MOUSE;
	Input.mi.dwFlags  = MOUSEEVENTF_RIGHTUP;
	SendInput( 1, &Input, sizeof(INPUT) );
}

// Returns the mouses current position, m holds x and y - May need to calculate where the mouse should  move based on sensor?
POINT GetMousePosition()
{
	static POINT m;
	POINT mouse;
	GetCursorPos(&mouse);
	m.x = mouse.x;
	m.y = mouse.y;
	return m;
}

// Sets the mouses position.
void SetMousePosition(POINT mp)
{
	long fScreenWidth = GetSystemMetrics( SM_CXSCREEN ) - 1; 
	long fScreenHeight = GetSystemMetrics( SM_CYSCREEN ) - 1; 

	// http://msdn.microsoft.com/en-us/library/ms646260(VS.85).aspx
	// If MOUSEEVENTF_ABSOLUTE value is specified, dx and dy contain normalized absolute coordinates between 0 and 65,535.
	// The event procedure maps these coordinates onto the display surface.
	// Coordinate (0,0) maps onto the upper-left corner of the display surface, (65535,65535) maps onto the lower-right corner.
	float fx = mp.x * ( 65535.0f / fScreenWidth  );
	float fy = mp.y * ( 65535.0f / fScreenHeight );		  

	INPUT Input = { 0 };			
	Input.type = INPUT_MOUSE;

	Input.mi.dwFlags = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;

	//Storing x, y coordinates into mouse input
	Input.mi.dx	= (long)fx;
	Input.mi.dy	= (long)fy;
	//Sends it to input to be processed
	SendInput(1,&Input,sizeof(INPUT));
}

//---------------------------------------------------------------------------
// Hooks
//---------------------------------------------------------------------------
void XN_CALLBACK_TYPE HandTracker::Gesture_Recognized(	xn::GestureGenerator&	/*generator*/, 
														const XnChar*			strGesture, 
														const XnPoint3D*		/*pIDPosition*/, 
														const XnPoint3D*		pEndPosition, 
														void*					pCookie)
{
	printf("Gesture recognized: %s\n", strGesture);

	HandTracker*	pThis = static_cast<HandTracker*>(pCookie);
	if(sm_Instances.Find(pThis) == sm_Instances.End())
	{
		printf("Dead HandTracker: skipped!\n");
		return;
	}

	pThis->m_HandsGenerator.StartTracking(*pEndPosition);

	if((strcmp(strGesture, "Click") == 0) && (HandDetect == true)){
		if(RightClicked == false){
			RightHold();
			RightClicked = true;
			printf("Right Click Hold\n");
		}
		else{
			RightRelease();
			RightClicked = false;
			printf("Right Click Release\n");
		}
	}
	if((strcmp(strGesture, "Wave") == 0) && (HandDetect == true)){
		if(LeftClicked == false){
			LeftHold();
			LeftClicked = true;
			printf("Left Click Hold\n");
		}
		else{
			LeftRelease();
			LeftClicked = false;
			printf("Left Click Release\n");
		}
	}

}

void XN_CALLBACK_TYPE HandTracker::Hand_Create(	xn::HandsGenerator& /*generator*/, 
												XnUserID			nId, 
												const XnPoint3D*	pPosition, 
												XnFloat				/*fTime*/, 
												void*				pCookie)
{
	printf("New Hand: %d @ (%f,%f,%f)\n", nId, pPosition->X, pPosition->Y, pPosition->Z);
	Hands++;
	handID_1 = true;
	if(Hands == MaxHands){
		HandDetect = true;
		handID_2 = true;
	}
	HandTracker*	pThis = static_cast<HandTracker*>(pCookie);
	if(sm_Instances.Find(pThis) == sm_Instances.End())
	{
		printf("Dead HandTracker: skipped!\n");
		return;
	}

	pThis->m_History[nId].Push(*pPosition);
}

void XN_CALLBACK_TYPE HandTracker::Hand_Update(	xn::HandsGenerator& /*generator*/, 
												XnUserID			nId, 
												const XnPoint3D*	pPosition, 
												XnFloat				/*fTime*/, 
												void*				pCookie)
{
	HandTracker*	pThis = static_cast<HandTracker*>(pCookie);

	if(sm_Instances.Find(pThis) == sm_Instances.End())
	{
		printf("Dead HandTracker: skipped!\n");
		return;
	}

	// Add to this user's hands history
	TrailHistory::Iterator it = pThis->m_History.Find(nId);
	if (it == pThis->m_History.End())
	{
		printf("Dead hand update: skipped!\n");
		return;
	}

	it->Value().Push(*pPosition);

	if(HandDetect == false){
	//Mouse---------------------
		long fScreenWidth = GetSystemMetrics( SM_CXSCREEN ) - 1; 
		long fScreenHeight = GetSystemMetrics( SM_CYSCREEN ) - 1; 
		float fx = pPosition->X * ( 65535.0f / fScreenWidth  );
		float fy = pPosition->Y * ( 65535.0f / fScreenHeight );		  
		
		INPUT Input = { 0 };			
		Input.type = INPUT_MOUSE;
		INPUT old = {0};
		INPUT older = {0};
		INPUT news = {0};
		INPUT mouse = {0};
		//some mouse smoothing - not that great at all
		
		news.mi.dx = (long)fx * sent;
		news.mi.dy = -(long)fy * sent;
		
		old.mi.dx = news.mi.dx;
		old.mi.dy = news.mi.dy;
		if ((abs(news.mi.dx - old.mi.dx) == abs(old.mi.dx - older.mi.dx)) && (abs(news.mi.dy - old.mi.dy) == abs(old.mi.dy - older.mi.dy)))
		{
			 mouse.mi.dx = news.mi.dx;
			 mouse.mi.dy = news.mi.dy;
		}
		else
		{
			 mouse.mi.dx = old.mi.dx;
			 mouse.mi.dy = old.mi.dy;
		}
			
			older.mi.dx = old.mi.dx;
			older.mi.dy = old.mi.dy;
			old.mi.dx = news.mi.dx;
			old.mi.dy = news.mi.dy;
		//some end
		



		Input.mi.dwFlags = MOUSEEVENTF_MOVE|MOUSEEVENTF_ABSOLUTE;
				
			Input.mi.dx = (mouse.mi.dx + old.mi.dx)/2;
			Input.mi.dy = (mouse.mi.dy + old.mi.dy)/2;
			//Input.mi.dx = mouse.mi.dx;
			//Input.mi.dy = mouse.mi.dy;
			//Input.mi.dx = (long)fx * 3;
			//Input.mi.dy	= -(long)fy * 3;
		

		cout<< "x" << Input.mi.dx << "\ty" << Input.mi.dy << endl;
		//This sleep timer, just slows down the mouse cursor, it went too fast so ya
		//Sleep(2);
		SendInput(1,&Input,sizeof(INPUT));
	//Mouse------------------------
	}
}

void XN_CALLBACK_TYPE HandTracker::Hand_Destroy(	xn::HandsGenerator& /*generator*/, 
													XnUserID			nId, 
													XnFloat				/*fTime*/, 
													void*				pCookie)
{
	printf("Lost Hand: %d\n", nId);
	
	Hands--;

	if(Hands < MaxHands){
		HandDetect = false;
		handID_2 = false;
	}
	HandTracker*	pThis = static_cast<HandTracker*>(pCookie);
	if(sm_Instances.Find(pThis) == sm_Instances.End())
	{
		printf("Dead HandTracker: skipped!\n");
		return;
	}

	// Remove this user from hands history
	pThis->m_History.Remove(nId);
}


//---------------------------------------------------------------------------
// Method Definitions
//---------------------------------------------------------------------------
HandTracker::HandTracker(xn::Context& context) : m_rContext(context)
{
	// Track all living instances (to protect against calling dead pointers in the Gesture/Hand Generator hooks)
	XnStatus rc = sm_Instances.AddLast(this);
	if (rc != XN_STATUS_OK)
	{
		printf("Unable to add NiHandTracker instance to the list.");
		exit(1);
	}
}

HandTracker::~HandTracker()
{
	// Remove the current instance from living instances list
	XnListT<HandTracker*>::ConstIterator it = sm_Instances.Find(this);
	assert(it != sm_Instances.End());
	sm_Instances.Remove(it);
}

XnStatus HandTracker::Init()
{            
	XnStatus			rc;
	XnCallbackHandle	chandle;

	// Create generators
	rc = m_GestureGenerator.Create(m_rContext);
	if (rc != XN_STATUS_OK)
	{
		printf("Unable to create GestureGenerator.");
		return rc;
	}

	rc = m_HandsGenerator.Create(m_rContext);
	if (rc != XN_STATUS_OK)
	{
		printf("Unable to create HandsGenerator.");
		return rc;
	}

	// Register callbacks
	// Using this as cookie
	rc = m_GestureGenerator.RegisterGestureCallbacks(Gesture_Recognized, Gesture_Process, this, chandle);
	if (rc != XN_STATUS_OK)
	{
		printf("Unable to register gesture callbacks.");
		return rc;
	}

	rc = m_HandsGenerator.RegisterHandCallbacks(Hand_Create, Hand_Update, Hand_Destroy, this, chandle);
	if (rc != XN_STATUS_OK)
	{
		printf("Unable to register hand callbacks.");
		return rc;
	}

	return XN_STATUS_OK;
}

XnStatus HandTracker::Run()
{
	//ADD_ALL_GESTURES;

	XnStatus	rc = m_rContext.StartGeneratingAll();
	if (rc != XN_STATUS_OK)
	{
		printf("Unable to start generating.");
		return rc;
	}

	ADD_ALL_GESTURES;

	return XN_STATUS_OK;
}
