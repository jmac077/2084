#pragma once
#include "mg_VS\stdafx.h"

class Teleporter
{
private:
	// THE LOCATION THIS TELEPORTER WILL SEND THE SPRITE TO
	float destX, destY;
	// THE ID OF THE LEVEL SECTION TO SWITCH TO AFTER TELEPORTING
	int targetSection;
public:
	Teleporter(float destX, float destY, int target)
	{
		this->destX = destX;
		this->destY = destY;
		targetSection = target;
	}
	float getDestX() { return destX; }
	float getDestY() { return destY; }
	int getTargetSection() { return targetSection; }
};