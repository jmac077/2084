#pragma once
#include "mg_VS\stdafx.h"

class Checkpoint
{
private:
	// LOCATION OF PLAYER WHEN CHECKPOINT WAS HIT
	int playerX, playerY;
	// THE LEVEL SECTION THAT THIS CHECKPOINT IS IN
	int levelSection;
	// IF THIS CHECKPOINT HAS BEEN HIT
	bool activated;
public:
	Checkpoint(int levelSection)
	{
		this->levelSection = levelSection;
		activated = false;
	}
	int getPlayerX() { return playerX; }
	int getPlayerY() { return playerY; }
	int getLevelSection() { return levelSection; }
	bool getActivated() { return activated; }
	void setPlayerPos(int px, int py) { playerX = px; playerY = py; activated = true; }
};