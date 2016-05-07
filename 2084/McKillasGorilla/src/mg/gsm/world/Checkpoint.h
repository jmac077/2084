#pragma once
#include "mg_VS\stdafx.h"

class Checkpoint
{
private:
	int x;
	int y;
	int width;
	int height;
	int playerX;
	int playerY;
public:
	Checkpoint(int x, int y, int width, int height)
	{
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}
	int getX() { return x; }
	int getY() { return y; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	int getPlayerX() { return playerX; }
	int getPlayerY() { return playerY; }
	void setPlayerPos(int px, int py) { playerX = px; playerY = py; }
};