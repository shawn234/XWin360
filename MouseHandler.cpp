#include "MouseHandler.h"
#include <math.h>
#include <iostream>

int SENSITIVITY = 75; //value between 0 and 10.
int SCROLL_SENSITIVITY = 50; //value between 0 and 10
int maxDelta = 10; //set to max mouse delta (when right thumbstick pushed all the way) 
int maxScrollDelta = 10; //set to max scroll delta (when left thumbstick pushed all the way)
int minTriggerDelta = 1;
bool triggerPressed[1] = {false};
MouseHandler::MouseHandler(){
	maxDelta = ceil(1.0*maxDelta*(1.0*SENSITIVITY/10));//These two sensitivity functions are a bit shitty, should fix later
	maxScrollDelta = ceil(1.0*maxScrollDelta*(1.0*SCROLL_SENSITIVITY/10));
	std::cout <<"maxDelta:"<<maxDelta<<"\n";
}

void MouseHandler::updateScroll(XINPUT_STATE playerState){
		//-------------Scrolling on Left Stick-------------
	//TODO: Scrolling sometimes turns the screen off. Don't know why, some dumb shit happening there...
	int rawSX = playerState.Gamepad.sThumbLX;
	int rawSY = playerState.Gamepad.sThumbLY;
	int ld = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	double lx,ly;
	if(abs(rawSX)<ld){
		lx=0;	//Dead zone
	}
	else{
		lx = 1.0*maxScrollDelta*rawSX/32768;
	}
	if(abs(rawSY)<ld){
		ly=0;	//Dead zone
	}
	else{
		ly = 1.0*maxScrollDelta*rawSY/32768;
	}
	if(ly!=0){ //Vertical Scrolling
		INPUT ip;
		ip.type = INPUT_MOUSE;
		ip.mi.dwFlags = MOUSEEVENTF_WHEEL;
		ip.mi.mouseData = ly;
		SendInput(1,&ip,sizeof(INPUT));
	}
	if(lx!=0){ //Horizontal Scrolling
		INPUT ip;
		ip.type = INPUT_MOUSE;
		ip.mi.dwFlags = MOUSEEVENTF_HWHEEL;
		ip.mi.mouseData = lx;
		SendInput(1,&ip,sizeof(INPUT));
	}
}

void MouseHandler::updateMouse(XINPUT_STATE playerState){
	//-------------Mouse Cursor on Right Stick---------------
	//TODO: Only convert back to int in event send to prevent precision loss
    POINT pt;
	int d=XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	int rawX=playerState.Gamepad.sThumbRX; //Range: -32768 to 32767
	int rawY=playerState.Gamepad.sThumbRY; //Range: -32768 to 32767
	int throttle = playerState.Gamepad.bLeftTrigger; //Range: 0 to 255
	if(throttle<XINPUT_GAMEPAD_TRIGGER_THRESHOLD){
		throttle = 0;
	}
	int x,y;
	if(abs(rawX)<d){
		x=0;	//Dead Zone
	}
	else{
		x = 1.0*maxDelta*(1.0*rawX/32768);
	}	
	if(abs(rawY)<d){
		y=0;	//Dead Zone
	}
	else{
		y = 1.0*maxDelta*(1.0*rawY/32768);
	}
	GetCursorPos(&pt);
	//Left Trigger Throttle
	double focus= max(1.0*(255-throttle)/255,1.0*minTriggerDelta/maxDelta);
	pt.x+=ceil(1.0*x*focus);
	pt.y-=ceil(1.0*y*focus);
	SetCursorPos(pt.x, pt.y);

	//------------Click/Hold for Right Trigger--------------
	bool click = false;
	if(playerState.Gamepad.bRightTrigger>XINPUT_GAMEPAD_TRIGGER_THRESHOLD){
		click = true;
	}
	if(click) 
	{
		if(!triggerPressed[1]) {
			INPUT ip;
			ip.type = INPUT_MOUSE;
			ip.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			ip.mi.mouseData = 0;
			SendInput(1,&ip,sizeof(INPUT));
			triggerPressed[1]=true;
		}
	} else {
		if(triggerPressed[1]){
			INPUT ip;
			ip.type = INPUT_MOUSE;
			ip.mi.dwFlags = MOUSEEVENTF_LEFTUP;
			ip.mi.mouseData = 0;
			SendInput(1,&ip,sizeof(INPUT));
			triggerPressed[1]=false;
		}
	}

}