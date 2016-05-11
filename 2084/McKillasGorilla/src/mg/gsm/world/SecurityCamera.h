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
	int width, height;
	AnimatedSprite *sprite;
public:
	SecurityCamera(int censorshipTarget, int direction, int width, int height, AnimatedSprite *sprite)
	{
		armed = true;
		dead = false;
		this->censorshipTarget = censorshipTarget;
		this->direction = direction;
		this->width = width;
		this->height = height;
		this->sprite = sprite;
	}
	void setArmed(bool armed) { this->armed = armed; }
	void setDead(bool dead) { this->dead = dead; }
	bool getArmed() { return armed; }
	bool getDead() { return dead; }
	int getCensorshipTarget() { return censorshipTarget; }
	int getDirection() { return direction; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	AnimatedSprite *getSprite() { return sprite; }
};