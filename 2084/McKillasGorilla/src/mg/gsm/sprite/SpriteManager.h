/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	SpriteManager.h

	This class manages all of the sprites in a given game level. Note
	that the player sprite is also managed by this class.
*/

#pragma once
#include "mg_VS\stdafx.h"
#include "mg\graphics\GameGraphics.h"
#include "mg\gsm\ai\Bot.h"
#include "mg\gsm\ai\BotRecycler.h"
#include "mg\gsm\ai\BotSpawningPool.h"
#include "mg\gsm\sprite\AnimatedSprite.h"
#include "mg\gsm\sprite\AnimatedSpriteType.h"

class SpriteManager
{
private:
	// NOTE THAT MULTIPLE SPRITES MAY SHARE ARTWORK, SO SPRITE TYPES
	// SPECIFIES A TYPE OF SPRITE, OF WHICH THERE MAY BE MANY INSTANCES
	vector<AnimatedSpriteType*> spriteTypes;
	map<wstring, AnimatedSpriteType*> spriteTypesByName;

	// THESE ARE THE BOTS IN THE GAME, LIKE ENEMIES, ROCKETS, OR ANYTHING
	// THAT MOVES AROUND AND IS NOT THE PLAYER
	list<Bot*> bots;

	// AND THIS IS THE PLAYER. AS-IS, WE ONLY ALLOW FOR ONE PLAYER AT A TIME
	AnimatedSprite *player;

	AnimatedSprite *tv;
	// THE BotRecycler MAKES SURE WE DON'T HAVE TO CONSTRUCT BOTS WHENEVER
	// WE NEED TO SPAWN THEM, INSTEAD IT WILL RECYCLE THEM FOR US
	BotRecycler botRecycler;

	// THESE ARE THIS LEVEL'S SPAWNING POOLS
	vector<BotSpawningPool*> spawningPools;

public:
	// NOTHING TO INIT OR DESTROY
	SpriteManager() { 
		player = nullptr;
		tv = nullptr;
	}
	~SpriteManager()	{}

	// INLINED ACCESSOR METHODS
	BotRecycler*			getBotRecycler()		{ return &botRecycler;		}
	int						getNumberOfBots()		{ return bots.size();		}
	AnimatedSprite*			getPlayer()				{ return player;			}
	AnimatedSprite*			getTv() { return tv; }
	list<Bot*>::iterator	getBotsIterator()		{ return bots.begin();		}
	list<Bot*>::iterator	getEndOfBotsIterator()	{ return bots.end();		}
	vector<BotSpawningPool*>*  getSpawningPools()		{ return &spawningPools;		}

	// INLINED METHOD FOR ADDING A SPAWNING POOL
	void addSpawningPool(BotSpawningPool *poolToAdd)
	{
		spawningPools.push_back(poolToAdd);
	}
	void setPlayer(AnimatedSprite *initPlayer)
	{
		player = initPlayer;
	}
	void setTv(AnimatedSprite *initTv)
	{
		tv = initTv;
	}

	// METHODS DEFINED IN SpriteManager.cpp
	void				addBot(Bot *botToAdd);
	void				addSpriteItemsToRenderList();
	unsigned int		addSpriteType(wstring spriteTypeName, AnimatedSpriteType *spriteTypeToAdd);
	void				addSpriteToRenderList(AnimatedSprite *sprite, RenderList *renderList, Viewport *viewport, int offsetX, int offsetY);
	AnimatedSpriteType* getSpriteType(unsigned int typeIndex);
	AnimatedSpriteType* getSpriteType(wstring spriteTypeName);
	Bot*				removeBot(Bot *botToRemove);
	void				unloadSprites();
	void				update();
	int					round(float realNum);
};