#include "CXBOXController.h"
class MouseHandler 
{
private:
	//CXBOXController Player;
public:
	MouseHandler();
	void updateMouse(XINPUT_STATE playerState);
	void updateScroll(XINPUT_STATE playerState);
};