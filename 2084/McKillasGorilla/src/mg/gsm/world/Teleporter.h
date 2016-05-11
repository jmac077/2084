#pragma once
#include "mg_VS\stdafx.h"
#include "mg\gsm\sprite\AnimatedSprite.h"

class Teleporter
{
private:
	// THE LOCATION THIS TELEPORTER WILL SEND THE SPRITE TO
	float destX, destY;
	// THE ID OF THE LEVEL SECTION TO SWITCH TO AFTER TELEPORTING
	int targetSection;
	// IF THIS TELEPORTER IS JUST FOR THE CENSORHIP TIMED PART, IF NOT, IT WILL ALWAYS BE ACTIVE
	bool censorship;
	// THE CORRESPONDING TARGET OF THE CAMERA THAT ACTIVATES THIS IF censorship == true
	int censorshipTarget;
	// THE ACTUAL SPRITE TO DRAW IF censorship == true
	AnimatedSprite *sprite;
public:
	Teleporter(float destX, float destY, int targetSection, bool censorship, int censorshipTarget, AnimatedSprite *sprite)
	{
		this->destX = destX;
		this->destY = destY;
		this->targetSection = targetSection;
		this->censorship = censorship;
		this->censorshipTarget = censorshipTarget;
		this->sprite = sprite;
	}
	~Teleporter() {
		delete sprite;
	};
	float getDestX() { return destX; }
	float getDestY() { return destY; }
	int getTargetSection() { return targetSection; }
	bool getCensorship() { return censorship; }
	int getCensorshipTarget() { return censorshipTarget; }
	AnimatedSprite *getSprite() { return sprite; }
};