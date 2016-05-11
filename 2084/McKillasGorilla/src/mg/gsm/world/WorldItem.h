#pragma once
#include "mg_VS\stdafx.h"
#include "mg\gsm\sprite\AnimatedSprite.h"

class WorldItem
{
private:
	// THE ACTUAL SPRITE TO RENDER
	AnimatedSprite *sprite;
	// THE WORLD FLAG TO TRIGGER WHEN THIS ITEM IS TRIGGERED, 0 IF NONE
	int flag;
	// IF THIS ITEM IS A COLLECTIBLE (WILL DISAPPEAR WHEN YOU INTERACT WITH IT)
	bool collectible;
	// TRUE IF THIS ITEM HASN'T BEEN TRIGGERED YET
	bool active;
public:
	WorldItem(AnimatedSprite *sprite, int flag, bool collectible)
	{
		this->sprite = sprite;
		this->flag = flag;
		this->collectible = collectible;
		active = true;
	}
	~WorldItem() {
		delete sprite;
	};
	AnimatedSprite *getSprite() { return sprite; }
	int getFlag() { return flag; }
	bool getCollectible() { return collectible; }
	bool getActive() { return active; }
	void setActive(bool active) { this->active = active; }
};