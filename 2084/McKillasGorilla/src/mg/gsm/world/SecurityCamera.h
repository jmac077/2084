#pragma once
#include "mg_VS\stdafx.h"
#include "mg\gsm\sprite\AnimatedSprite.h"

class SecurityCamera
{
private:
	bool armed;
	bool dead;
	int censorshipTarget;
	int direction;
	int xOffset, yOffset;
	AnimatedSprite *sprite;
public:
	SecurityCamera(int censorshipTarget, int direction, int xOffset, int yOffset, AnimatedSprite *sprite)
	{
		armed = true;
		dead = false;
		this->censorshipTarget = censorshipTarget;
		this->direction = direction;
		this->xOffset = xOffset;
		this->yOffset = yOffset;
		this->sprite = sprite;
	}
	void setArmed(bool armed) { this->armed = armed; }
	void setDead(bool dead) { this->dead = dead; }
	bool getArmed() { return armed; }
	bool getDead() { return dead; }
	int getCensorshipTarget() { return censorshipTarget; }
	int getDirection() { return direction; }
	int getXOffset() { return xOffset; }
	int getYOffset() { return yOffset; }
	AnimatedSprite *getSprite() { return sprite; }
};