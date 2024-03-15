#pragma once

enum EEventID
{
	EVENT_FINISH_EVENT_CAMERA,
	EVENT_CLOSE_PAUSE_WINDOW,
	EVENT_FADE_DONE,
};

class IEventCallBack
{
public:
	virtual void CallBack(int eventID) = 0;
};