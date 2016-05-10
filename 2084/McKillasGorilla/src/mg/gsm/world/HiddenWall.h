#pragma once
#include "mg_VS\stdafx.h"

class HiddenWall
{
private:
	int x;
	int y;
	int width;
	int height;
public:
	HiddenWall(int x, int y, int width, int height)
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
};