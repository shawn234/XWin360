#include "CXBOXController.h"
#include "MouseHandler.h"
#include <Windows.h>
#include <iostream>
#include <math.h>
#include <atlimage.h>
#include "resource.h"

CXBOXController* Player1;
MouseHandler* mouseH;

bool typeMode = false;
bool pressed[16] = {false};
int BUTTON_A = 0; //A Button
int BUTTON_B = 1; //B Button
int BUTTON_X = 2; //X Button
int BUTTON_Y = 3; //Y Button
int BUTTON_LB = 4; //Left Bumper
int BUTTON_RB = 5; //Right Bumper
int BUTTON_BK = 6; //Back
int BUTTON_ST = 7; //Start
int BUTTON_DUP = 8; //D-pad Up
int BUTTON_DDWN = 9; //D-pad Down
int BUTTON_DLFT = 10; //D-pad Left
int BUTTON_DRT = 11; //D-pad Right
int BUTTON_LSTK = 12; //Click Left Stick
int BUTTON_RSTK = 13; //Click Right Stick
int BUTTON_GD = 14; //Guide
int TRIGGER_RT = 15; //Right Trigger

int daisyState = 8;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine, int nCmdShow)
{
	Player1 = new CXBOXController(1);
	mouseH = new MouseHandler();

	//DWin->drawWin();
						LPCTSTR szWindowClass = _T("TransparentClass");

    // Register class
    WNDCLASSEX wcex = {0};

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.lpfnWndProc    = DefWindowProc;
    wcex.hInstance      = hInstance;
    wcex.lpszClassName  = szWindowClass;

    RegisterClassEx(&wcex);

						HWND hWnd = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, szWindowClass, 0, WS_OVERLAPPEDWINDOW,
     CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

CImage img;
img.LoadFromResource(hInstance,MAKEINTRESOURCE(IDB_BITMAP10));
// Get image sizes
int nWidth = img.GetWidth();
int nHeight = img.GetHeight();
// Create memory DC
HDC hdcScreen = GetDC(NULL);
HDC hDC = CreateCompatibleDC(hdcScreen);
// Create memory bitmap
HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, hBmp);
// Draw image to memory bitmap (currently selected in memory DC)
img.Draw(hDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight);

// Call UpdateLayeredWindow
BLENDFUNCTION blend = {0};
blend.BlendOp = AC_SRC_OVER;
blend.SourceConstantAlpha = 255;// half transparent
blend.AlphaFormat = 0;
//blend.AlphaFormat = AC_SRC_ALPHA;
POINT ptLocation = {0, 0};
SIZE szWnd = {nWidth, nHeight};
POINT ptSrc = {0, 0};
UpdateLayeredWindow(hWnd, hdcScreen, &ptLocation, &szWnd, hDC, &ptSrc, 0, &blend, ULW_ALPHA);

SelectObject(hDC, hBmpOld);
DeleteObject(hBmp);
DeleteDC(hDC);
ReleaseDC(NULL, hdcScreen);
	while(true)
	{
		if(Player1->IsConnected())
		{
			XINPUT_STATE playerState = Player1->GetState(); 
			XINPUT_GAMEPAD gamepadState = playerState.Gamepad;
			mouseH->updateMouse(playerState);

			if(typeMode){
				int rawX = gamepadState.sThumbLX;
				int rawY = gamepadState.sThumbLY;
				double angle = 0.0;
				int d = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
				if(abs(rawX)<d){
					rawX = 0;
				}
				if(abs(rawY)<d){
					rawY = 0;
				}
				if(rawX!=0){
					angle = atan(1.0*rawY/rawX);
				}
				else if(rawY!=0){
					angle = 3.14159263/2;
					if(rawY<0){
					    angle = -angle;
					}
				}
				if(rawX<0){
					angle+=3.141592653;
				}
				angle=angle/3.141592653*180;
				INPUT ip;
					ip.type = INPUT_KEYBOARD;
					ip.ki.time = 0;
					ip.ki.dwFlags = 0;
					ip.ki.wScan = 0; 
					ip.ki.dwExtraInfo = 0;
					ip.ki.wVk = VK_BACK;
					if(gamepadState.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER){
						if(!pressed[BUTTON_LB]){
							SendInput(1, &ip, sizeof(INPUT));
							pressed[BUTTON_LB] = true;
						}
					}
					else{
						if(pressed[BUTTON_LB]){
							ip.ki.dwFlags = KEYEVENTF_KEYUP;
							SendInput(1, &ip, sizeof(INPUT));
							ip.ki.dwFlags = 0;
							pressed[BUTTON_LB] = false;
						}
					}
					ip.ki.wVk = VK_SPACE;
					if(gamepadState.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER){
						if(!pressed[BUTTON_RB]){
							SendInput(1, &ip, sizeof(INPUT));
							ip.ki.dwFlags = KEYEVENTF_KEYUP;
							SendInput(1, &ip, sizeof(INPUT));
							pressed[BUTTON_RB] = true;
						}
					}
					else{
						pressed[BUTTON_RB] = false;
					}
				//-----------------------DaisyWheel Sectors------------------
				if(rawX!=0 || rawY != 0){
					INPUT ip;
					ip.type = INPUT_KEYBOARD;
					ip.ki.time = 0;
					ip.ki.dwFlags = KEYEVENTF_UNICODE;
					ip.ki.dwExtraInfo = 0;
					ip.ki.wVk = 0;
					//-----Sector 0
					if(0<=angle && angle<=22.5 || -22.5<angle && angle<=0){
						if(daisyState!=0){
							CImage img;
img.LoadFromResource(hInstance,MAKEINTRESOURCE(IDB_BITMAP2));
// Get image sizes
int nWidth = img.GetWidth();
int nHeight = img.GetHeight();
// Create memory DC
HDC hdcScreen = GetDC(NULL);
HDC hDC = CreateCompatibleDC(hdcScreen);
// Create memory bitmap
HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, hBmp);
// Draw image to memory bitmap (currently selected in memory DC)
img.Draw(hDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight);
UpdateLayeredWindow(hWnd, hdcScreen, &ptLocation, &szWnd, hDC, &ptSrc, 0, &blend, ULW_ALPHA);
SelectObject(hDC, hBmpOld);
DeleteObject(hBmp);
DeleteDC(hDC);
ReleaseDC(NULL, hdcScreen);
							daisyState = 0;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_X){
							ip.ki.wScan = 'i'; 
							if(!pressed[BUTTON_X]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_X] = true;
							}
						}
						else{
							pressed[BUTTON_X] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_Y){
							ip.ki.wScan = 'j'; 
							if(!pressed[BUTTON_Y]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_Y] = true;
							}
						}
						else{
							pressed[BUTTON_Y] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_B){
							ip.ki.wScan = 'k'; 
							if(!pressed[BUTTON_B]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_B] = true;
							}
						}
						else{
							pressed[BUTTON_B] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_A){
							ip.ki.wScan = 'l'; 
							if(!pressed[BUTTON_A]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_A] = true;
							}
						}
						else{
							pressed[BUTTON_A] = false;
						}
					//---------Sector 1
					}else if(22.5<angle && angle<=67.5){
						if(daisyState!=1){
							CImage img;
img.LoadFromResource(hInstance,MAKEINTRESOURCE(IDB_BITMAP3));
// Get image sizes
int nWidth = img.GetWidth();
int nHeight = img.GetHeight();
// Create memory DC
HDC hdcScreen = GetDC(NULL);
HDC hDC = CreateCompatibleDC(hdcScreen);
// Create memory bitmap
HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, hBmp);
// Draw image to memory bitmap (currently selected in memory DC)
img.Draw(hDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight);
UpdateLayeredWindow(hWnd, hdcScreen, &ptLocation, &szWnd, hDC, &ptSrc, 0, &blend, ULW_ALPHA);
SelectObject(hDC, hBmpOld);
DeleteObject(hBmp);
DeleteDC(hDC);
ReleaseDC(NULL, hdcScreen);
							daisyState = 1;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_X){
							ip.ki.wScan = 'e'; 
							if(!pressed[BUTTON_X]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_X] = true;
							}
						}
						else{
							pressed[BUTTON_X] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_Y){
							ip.ki.wScan = 'f'; 
							if(!pressed[BUTTON_Y]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_Y] = true;
							}
						}
						else{
							pressed[BUTTON_Y] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_B){
							ip.ki.wScan = 'g'; 
							if(!pressed[BUTTON_B]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_B] = true;
							}
						}
						else{
							pressed[BUTTON_B] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_A){
							ip.ki.wScan = 'h'; 
							if(!pressed[BUTTON_A]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_A] = true;
							}
						}
						else{
							pressed[BUTTON_A] = false;
						}
					//----------------Sector 2
					}else if(67.5<angle && angle<=112.5){
						if(daisyState!=2){
							CImage img;
img.LoadFromResource(hInstance,MAKEINTRESOURCE(IDB_BITMAP4));
// Get image sizes
int nWidth = img.GetWidth();
int nHeight = img.GetHeight();
// Create memory DC
HDC hdcScreen = GetDC(NULL);
HDC hDC = CreateCompatibleDC(hdcScreen);
// Create memory bitmap
HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, hBmp);
// Draw image to memory bitmap (currently selected in memory DC)
img.Draw(hDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight);
UpdateLayeredWindow(hWnd, hdcScreen, &ptLocation, &szWnd, hDC, &ptSrc, 0, &blend, ULW_ALPHA);
SelectObject(hDC, hBmpOld);
DeleteObject(hBmp);
DeleteDC(hDC);
ReleaseDC(NULL, hdcScreen);
							daisyState = 2;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_X){
							ip.ki.wScan = 'a'; 
							if(!pressed[BUTTON_X]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_X] = true;
							}
						}
						else{
							pressed[BUTTON_X] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_Y){
							ip.ki.wScan = 'b'; 
							if(!pressed[BUTTON_Y]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_Y] = true;
							}
						}
						else{
							pressed[BUTTON_Y] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_B){
							ip.ki.wScan = 'c'; 
							if(!pressed[BUTTON_B]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_B] = true;
							}
						}
						else{
							pressed[BUTTON_B] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_A){
							ip.ki.wScan = 'd'; 
							if(!pressed[BUTTON_A]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_A] = true;
							}
						}
						else{
							pressed[BUTTON_A] = false;
						}
					//-----------Sector 3
					}else if(112.5<angle && angle<=157.5){
						if(daisyState!=3){
							CImage img;
img.LoadFromResource(hInstance,MAKEINTRESOURCE(IDB_BITMAP5));
// Get image sizes
int nWidth = img.GetWidth();
int nHeight = img.GetHeight();
// Create memory DC
HDC hdcScreen = GetDC(NULL);
HDC hDC = CreateCompatibleDC(hdcScreen);
// Create memory bitmap
HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, hBmp);
// Draw image to memory bitmap (currently selected in memory DC)
img.Draw(hDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight);
UpdateLayeredWindow(hWnd, hdcScreen, &ptLocation, &szWnd, hDC, &ptSrc, 0, &blend, ULW_ALPHA);
SelectObject(hDC, hBmpOld);
DeleteObject(hBmp);
DeleteDC(hDC);
ReleaseDC(NULL, hdcScreen);
							daisyState = 3;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_X){
							ip.ki.wScan = ':'; 
							if(!pressed[BUTTON_X]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_X] = true;
							}
						}
						else{
							pressed[BUTTON_X] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_Y){
							ip.ki.wScan = '/'; 
							if(!pressed[BUTTON_Y]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_Y] = true;
							}
						}
						else{
							pressed[BUTTON_Y] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_B){
							ip.ki.wScan = '@'; 
							if(!pressed[BUTTON_B]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_B] = true;
							}
						}
						else{
							pressed[BUTTON_B] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_A){
							ip.ki.wScan = '-'; 
							if(!pressed[BUTTON_A]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_A] = true;
							}
						}
						else{
							pressed[BUTTON_A] = false;
						}
					//----------------Sector 4
					}else if(157.5<angle && angle<=202.5){
						if(daisyState!=4){
							CImage img;
img.LoadFromResource(hInstance,MAKEINTRESOURCE(IDB_BITMAP6));
// Get image sizes
int nWidth = img.GetWidth();
int nHeight = img.GetHeight();
// Create memory DC
HDC hdcScreen = GetDC(NULL);
HDC hDC = CreateCompatibleDC(hdcScreen);
// Create memory bitmap
HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, hBmp);
// Draw image to memory bitmap (currently selected in memory DC)
img.Draw(hDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight);
UpdateLayeredWindow(hWnd, hdcScreen, &ptLocation, &szWnd, hDC, &ptSrc, 0, &blend, ULW_ALPHA);
SelectObject(hDC, hBmpOld);
DeleteObject(hBmp);
DeleteDC(hDC);
ReleaseDC(NULL, hdcScreen);
							daisyState = 4;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_X){
							ip.ki.wScan = 'y'; 
							if(!pressed[BUTTON_X]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_X] = true;
							}
						}
						else{
							pressed[BUTTON_X] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_Y){
							ip.ki.wScan = 'z'; 
							if(!pressed[BUTTON_Y]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_Y] = true;
							}
						}
						else{
							pressed[BUTTON_Y] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_B){
							ip.ki.wScan = ','; 
							if(!pressed[BUTTON_B]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_B] = true;
							}
						}
						else{
							pressed[BUTTON_B] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_A){
							ip.ki.wScan = '.'; 
							if(!pressed[BUTTON_A]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_A] = true;
							}
						}
						else{
							pressed[BUTTON_A] = false;
						}
					//-------------------Sector 5
					}else if(202.5<angle && angle<=247.5){
						if(daisyState!=5){
							CImage img;
img.LoadFromResource(hInstance,MAKEINTRESOURCE(IDB_BITMAP7));
// Get image sizes
int nWidth = img.GetWidth();
int nHeight = img.GetHeight();
// Create memory DC
HDC hdcScreen = GetDC(NULL);
HDC hDC = CreateCompatibleDC(hdcScreen);
// Create memory bitmap
HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, hBmp);
// Draw image to memory bitmap (currently selected in memory DC)
img.Draw(hDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight);
UpdateLayeredWindow(hWnd, hdcScreen, &ptLocation, &szWnd, hDC, &ptSrc, 0, &blend, ULW_ALPHA);
SelectObject(hDC, hBmpOld);
DeleteObject(hBmp);
DeleteDC(hDC);
ReleaseDC(NULL, hdcScreen);
							daisyState = 5;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_X){
							ip.ki.wScan = 'u'; 
							if(!pressed[BUTTON_X]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_X] = true;
							}
						}
						else{
							pressed[BUTTON_X] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_Y){
							ip.ki.wScan = 'v'; 
							if(!pressed[BUTTON_Y]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_Y] = true;
							}
						}
						else{
							pressed[BUTTON_Y] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_B){
							ip.ki.wScan = 'w'; 
							if(!pressed[BUTTON_B]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_B] = true;
							}
						}
						else{
							pressed[BUTTON_B] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_A){
							ip.ki.wScan = 'x'; 
							if(!pressed[BUTTON_A]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_A] = true;
							}
						}
						else{
							pressed[BUTTON_A] = false;
						}
					//-------------------Sector 6
					}else if(247.5<angle && angle<=270 || -90<angle && angle<=-67.5){
						if(daisyState!=6){
							CImage img;
img.LoadFromResource(hInstance,MAKEINTRESOURCE(IDB_BITMAP8));
// Get image sizes
int nWidth = img.GetWidth();
int nHeight = img.GetHeight();
// Create memory DC
HDC hdcScreen = GetDC(NULL);
HDC hDC = CreateCompatibleDC(hdcScreen);
// Create memory bitmap
HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, hBmp);
// Draw image to memory bitmap (currently selected in memory DC)
img.Draw(hDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight);
UpdateLayeredWindow(hWnd, hdcScreen, &ptLocation, &szWnd, hDC, &ptSrc, 0, &blend, ULW_ALPHA);
SelectObject(hDC, hBmpOld);
DeleteObject(hBmp);
DeleteDC(hDC);
ReleaseDC(NULL, hdcScreen);
							daisyState = 6;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_X){
							ip.ki.wScan = 'q'; 
							if(!pressed[BUTTON_X]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_X] = true;
							}
						}
						else{
							pressed[BUTTON_X] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_Y){
							ip.ki.wScan = 'r'; 
							if(!pressed[BUTTON_Y]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_Y] = true;
							}
						}
						else{
							pressed[BUTTON_Y] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_B){
							ip.ki.wScan = 's'; 
							if(!pressed[BUTTON_B]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_B] = true;
							}
						}
						else{
							pressed[BUTTON_B] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_A){
							ip.ki.wScan = 't'; 
							if(!pressed[BUTTON_A]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_A] = true;
							}
						}
						else{
							pressed[BUTTON_A] = false;
						}
					//-------------------Sector 7
					}else if(-67.5<angle && angle<=-22.5){
						if(daisyState!=7){
							CImage img;
img.LoadFromResource(hInstance,MAKEINTRESOURCE(IDB_BITMAP9));
// Get image sizes
int nWidth = img.GetWidth();
int nHeight = img.GetHeight();
// Create memory DC
HDC hdcScreen = GetDC(NULL);
HDC hDC = CreateCompatibleDC(hdcScreen);
// Create memory bitmap
HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, hBmp);
// Draw image to memory bitmap (currently selected in memory DC)
img.Draw(hDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight);
UpdateLayeredWindow(hWnd, hdcScreen, &ptLocation, &szWnd, hDC, &ptSrc, 0, &blend, ULW_ALPHA);
SelectObject(hDC, hBmpOld);
DeleteObject(hBmp);
DeleteDC(hDC);
ReleaseDC(NULL, hdcScreen);
							daisyState = 7;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_X){
							ip.ki.wScan = 'm'; 
							if(!pressed[BUTTON_X]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_X] = true;
							}
						}
						else{
							pressed[BUTTON_X] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_Y){
							ip.ki.wScan = 'n'; 
							if(!pressed[BUTTON_Y]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_Y] = true;
							}
						}
						else{
							pressed[BUTTON_Y] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_B){
							ip.ki.wScan = 'o'; 
							if(!pressed[BUTTON_B]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_B] = true;
							}
						}
						else{
							pressed[BUTTON_B] = false;
						}
						if(gamepadState.wButtons & XINPUT_GAMEPAD_A){
							ip.ki.wScan = 'p'; 
							if(!pressed[BUTTON_A]){
								SendInput(1, &ip, sizeof(INPUT));
								ip.ki.dwFlags |= KEYEVENTF_KEYUP;
								SendInput(1, &ip, sizeof(INPUT));
								pressed[BUTTON_A] = true;
							}
						}
						else{
							pressed[BUTTON_A] = false;
						}
					}
				}
				else{
					if(daisyState!=8){
						if(daisyState!=0){
							CImage img;
img.LoadFromResource(hInstance,MAKEINTRESOURCE(IDB_BITMAP10));
// Get image sizes
int nWidth = img.GetWidth();
int nHeight = img.GetHeight();
// Create memory DC
HDC hdcScreen = GetDC(NULL);
HDC hDC = CreateCompatibleDC(hdcScreen);
// Create memory bitmap
HBITMAP hBmp = CreateCompatibleBitmap(hdcScreen, nWidth, nHeight);
HBITMAP hBmpOld = (HBITMAP)SelectObject(hDC, hBmp);
// Draw image to memory bitmap (currently selected in memory DC)
img.Draw(hDC, 0, 0, nWidth, nHeight, 0, 0, nWidth, nHeight);
UpdateLayeredWindow(hWnd, hdcScreen, &ptLocation, &szWnd, hDC, &ptSrc, 0, &blend, ULW_ALPHA);
SelectObject(hDC, hBmpOld);
DeleteObject(hBmp);
DeleteDC(hDC);
ReleaseDC(NULL, hdcScreen);
							daisyState = 8;
						}
					}
					
					ip.ki.wVk = VK_RETURN;
					if(!pressed[BUTTON_A]){
						if(gamepadState.wButtons & XINPUT_GAMEPAD_A){
							SendInput(1, &ip, sizeof(INPUT));
							ip.ki.dwFlags = KEYEVENTF_KEYUP;
							SendInput(1, &ip, sizeof(INPUT));
							pressed[BUTTON_A] = true;
							typeMode = false;
							//DWin.hide()
							ShowWindow(hWnd,SW_HIDE);
							ShowWindow(hWnd,SW_HIDE);
						}
					}
					else{
						pressed[BUTTON_A] = false;
					}
					if(!pressed[BUTTON_B]){
						if(gamepadState.wButtons & XINPUT_GAMEPAD_B){
							typeMode = false;
							pressed[BUTTON_B] = true;
							//DWin.hide();
							ShowWindow(hWnd,SW_HIDE);
							ShowWindow(hWnd,SW_HIDE);
						}
					}
					else{
						pressed[BUTTON_B] = false;
					}
				}
			}
			else{
				mouseH->updateScroll(playerState);
				if(gamepadState.wButtons & XINPUT_GAMEPAD_Y){
					typeMode = true;
					ShowWindow(hWnd, SW_SHOW);
				}

			    //----------------------Other Buttons for Navigation Mode---------------------------
				//A Button: ENTER
				if(gamepadState.wButtons & XINPUT_GAMEPAD_A)
				{
					if(!pressed[BUTTON_A]) {
						INPUT ip;
						ip.type = INPUT_KEYBOARD;
						ip.ki.time = 0;
						ip.ki.dwFlags = 0;
						ip.ki.wScan = 0; 
						ip.ki.dwExtraInfo = 0;
						ip.ki.wVk = VK_PROCESSKEY;
						SendInput(1, &ip, sizeof(INPUT));
						//Key Up 
						ip.ki.dwFlags = KEYEVENTF_KEYUP;
						SendInput(1, &ip, sizeof(INPUT));
						pressed[BUTTON_A]=true;
					}
				} else {
					pressed[BUTTON_A]=false;
				}
				//B Button: ESC
				if(gamepadState.wButtons & XINPUT_GAMEPAD_B) 
				{
					if(!pressed[BUTTON_B]) {
						INPUT ip;
						ip.type = INPUT_KEYBOARD;
						ip.ki.time = 0;
						ip.ki.dwFlags = 0;
						ip.ki.wScan = 0; 
						ip.ki.dwExtraInfo = 0;
						ip.ki.wVk = VK_ESCAPE;
						SendInput(1, &ip, sizeof(INPUT));
						//Key Up 
						ip.ki.dwFlags = KEYEVENTF_KEYUP;
						SendInput(1, &ip, sizeof(INPUT));
						pressed[BUTTON_B]=true;
					}
				} else {
					pressed[BUTTON_B]=false;
				}
		

				//X Button: RIGHT CLICK
				if(gamepadState.wButtons & XINPUT_GAMEPAD_X) 
				{
					if(!pressed[BUTTON_X]) {
						INPUT ip;
						ip.type = INPUT_MOUSE;
						ip.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
						ip.mi.mouseData = 0;
						SendInput(1,&ip,sizeof(INPUT));
						pressed[BUTTON_X]=true;
					}
				} else {
					if(pressed[BUTTON_X]){
						INPUT ip;
						ip.type = INPUT_MOUSE;
						ip.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
						ip.mi.mouseData = 0;
						SendInput(1,&ip,sizeof(INPUT));
						pressed[BUTTON_X]=false;
					}
				}
				//Right Shoulder Button: ALT + TAB
				if(gamepadState.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER){
					if(!pressed[BUTTON_RB]){
						//Key Down
						INPUT ip;
						ip.type = INPUT_KEYBOARD;
						ip.ki.time = 0;
						ip.ki.dwFlags = 0;
						ip.ki.wScan = 0; 
						ip.ki.dwExtraInfo = 0;
						ip.ki.wVk = VK_MENU;
						SendInput(1, &ip, sizeof(INPUT));
						ip.ki.wVk = VK_TAB;
						SendInput(1, &ip, sizeof(INPUT));
						//Key Up 
						ip.ki.dwFlags = KEYEVENTF_KEYUP;
						SendInput(1, &ip, sizeof(INPUT));
						ip.ki.wVk = VK_MENU;
						SendInput(1, &ip, sizeof(INPUT));
						pressed[BUTTON_RB]=true;
					}
				}
				else{
					pressed[BUTTON_RB]=false;
				}
			}

				//Left Shoulder Button: ALT + SHIFT + TAB
				if(gamepadState.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER){
					if(!pressed[BUTTON_LB]){
						//std::cout << "LB Pressed\n";
						//Key Down
						INPUT ip;
						ip.type = INPUT_KEYBOARD;
						ip.ki.time = 0;
						ip.ki.dwFlags = 0;
						ip.ki.wScan = 0; 
						ip.ki.dwExtraInfo = 0;
						ip.ki.wVk = VK_MENU;
						SendInput(1, &ip, sizeof(INPUT));
						ip.ki.wVk = VK_SHIFT;
						SendInput(1, &ip, sizeof(INPUT));
						ip.ki.wVk = VK_TAB;
						SendInput(1, &ip, sizeof(INPUT));
						//Key Up 
						ip.ki.dwFlags = KEYEVENTF_KEYUP;
						SendInput(1, &ip, sizeof(INPUT));
						ip.ki.wVk = VK_SHIFT;
						SendInput(1, &ip, sizeof(INPUT));
						ip.ki.wVk = VK_MENU;
						SendInput(1, &ip, sizeof(INPUT));
						pressed[BUTTON_LB]=true;
					}
				}
				else{
					pressed[BUTTON_LB]=false;
				}
			}
			//Keys that should work in both Navigation and Keyboard mode

			//Dpad: Arrow Key
			if(gamepadState.wButtons & XINPUT_GAMEPAD_DPAD_LEFT){
				if(!pressed[BUTTON_DLFT]){
					INPUT ip;
					ip.type = INPUT_KEYBOARD;
					ip.ki.time = 0;
					ip.ki.dwFlags = 0;
					ip.ki.wScan = 0; 
					ip.ki.wVk = VK_LEFT;
					ip.ki.dwExtraInfo = 0;
					SendInput(1, &ip, sizeof(INPUT));
					ip.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &ip, sizeof(INPUT));
					pressed[BUTTON_DLFT]=true;
				}
			}
			else{
				pressed[BUTTON_DLFT]=false;
			}
			if(gamepadState.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT){
				if(!pressed[BUTTON_DRT]){
					INPUT ip;
					ip.type = INPUT_KEYBOARD;
					ip.ki.time = 0;
					ip.ki.dwFlags = 0;
					ip.ki.wScan = 0; 
					ip.ki.wVk = VK_RIGHT;
					ip.ki.dwExtraInfo = 0;
					SendInput(1, &ip, sizeof(INPUT));
					ip.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &ip, sizeof(INPUT));
					pressed[BUTTON_DRT]=true;
				}
			}
			else{
				pressed[BUTTON_DRT]=false;
			}
			if(gamepadState.wButtons & XINPUT_GAMEPAD_DPAD_UP){
				if(!pressed[BUTTON_DUP]){
					INPUT ip;
					ip.type = INPUT_KEYBOARD;
					ip.ki.time = 0;
					ip.ki.dwFlags = 0;
					ip.ki.wScan = 0; 
					ip.ki.wVk = VK_UP;
					ip.ki.dwExtraInfo = 0;
					SendInput(1, &ip, sizeof(INPUT));
					ip.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &ip, sizeof(INPUT));
					pressed[BUTTON_DUP]=true;
				}
			}
			else{
				pressed[BUTTON_DUP]=false;
			}
			if(gamepadState.wButtons & XINPUT_GAMEPAD_DPAD_DOWN){
				if(!pressed[BUTTON_DDWN]){
					INPUT ip;
					ip.type = INPUT_KEYBOARD;
					ip.ki.time = 0;
					ip.ki.dwFlags = 0;
					ip.ki.wScan = 0; 
					ip.ki.wVk = VK_DOWN;
					ip.ki.dwExtraInfo = 0;
					SendInput(1, &ip, sizeof(INPUT));
					ip.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &ip, sizeof(INPUT));
					pressed[BUTTON_DDWN]=true;
				}
			}
			else{
				pressed[BUTTON_DDWN]=false;
			}
			//Start Button: Windows Key
			if(gamepadState.wButtons & XINPUT_GAMEPAD_START){
				if(!pressed[BUTTON_ST]){
					//std::cout << "START\n";
					INPUT ip;
					ip.type = INPUT_KEYBOARD;
					ip.ki.time = 0;
					ip.ki.dwFlags = 0;
					ip.ki.wScan = 0; 
					ip.ki.wVk = VK_LWIN;
					ip.ki.dwExtraInfo = 0;
					SendInput(1, &ip, sizeof(INPUT));
					ip.ki.dwFlags = KEYEVENTF_KEYUP;
					SendInput(1, &ip, sizeof(INPUT));
					pressed[BUTTON_ST]=true;
				}
			}
			else{
				pressed[BUTTON_ST]=false;
			}
			if(gamepadState.wButtons & XINPUT_GAMEPAD_BACK)
			{
				break;
			}
		}
		else{
			//std::cout << "\n\tERROR! PLAYER 1 - XBOX 360 Controller Not Found!\n";
			//std::cout << "Press Any Key To Exit.";
			//std::cin.get();
			break;
		}
		Sleep(16);
	}

	delete(Player1);
	return( 0 );
}

