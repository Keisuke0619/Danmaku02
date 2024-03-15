#pragma once
#include "EventCallback.h"

namespace Fade
{
	void Start(IEventCallBack* callback, int frames);
	void Release();
	void Update();
	void Draw();
}