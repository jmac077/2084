#pragma once

#include "mg_VS\stdafx.h"
#include "mg\game\Game.h"

struct PathNode
{
	int column;
	int row;
	int G;
	int H;
	PathNode *parentNode;
};

class AnimatedSprite;

class GridPathfinder
{
protected:
	Game *game;
	vector<bool> pathGrid;
	int numColumns;
	int numRows;

public:
	int getNumColumns() { return numColumns; }
	int getNumRows() { return numRows; }
	virtual void	updatePath(AnimatedSprite *sprite)=0;
	virtual float	getColumnCenterX(int column)=0;
	virtual float	getRowCenterY(int row)=0;
	virtual void	mapPath(AnimatedSprite *sprite, float worldX, float worldY)=0;
	virtual void	initGrid()=0;
	virtual bool	isWalkable(int col, int row)=0;
	virtual int		getGridWidth()=0;
	virtual	int		getGridHeight()=0;
};