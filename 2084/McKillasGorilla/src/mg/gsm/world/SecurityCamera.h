#pragma once
#include "mg_VS\stdafx.h"
#include "mg\gsm\sprite\AnimatedSprite.h"

class SecurityCamera
{
private:
	bool dead;
	int censorshipTarget;
	int direction;
	int xOffset, yOffset;
	int time;
	AnimatedSprite *sprite;
public:
	SecurityCamera(int time, int censorshipTarget, int direction, int xOffset, int yOffset, AnimatedSprite *sprite)
	{
		dead = false;
		this->time = time;
		this->censorshipTarget = censorshipTarget;
		this->direction = direction;
		this->xOffset = xOffset;
		this->yOffset = yOffset;
		this->sprite = sprite;
	}
	void setDead(bool dead) { this->dead = dead; }
	bool getDead() { return dead; }
	int getTime() { return time; }
	int getCensorshipTarget() { return censorshipTarget; }
	int getDirection() { return direction; }
	int getXOffset() { return xOffset; }
	int getYOffset() { return yOffset; }
	AnimatedSprite *getSprite() { return sprite; }
};