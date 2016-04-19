#pragma once

#include "mg\game\Game.h"
#include "mg\input\MouseEventHandler.h"

class BugsMouseEventHandler : public MouseEventHandler
{
public:
	BugsMouseEventHandler() {}
	~BugsMouseEventHandler() {}
	void handleMousePressEvent(int mouseX, int mouseY);
	void handleMouseMoveEvent(int mouseX, int mouseY);
};