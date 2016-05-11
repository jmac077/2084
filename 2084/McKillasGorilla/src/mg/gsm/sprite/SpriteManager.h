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

	// LIST OF TELEPORTERS THAT WILL BE RENDERED FOR THEIR SPECIFIC CENSORSHIP PARTS
	list<Teleporter*> teleporters;

	// THE LIST OF WORLD ITEMS (COLLECTIBLES, INTERACTIBLES, ETC.) IN THIS LEVEL
	list<WorldItem*> worldItems;

	// THE BotRecycler MAKES SURE WE DON'T HAVE TO CONSTRUCT BOTS WHENEVER
	// WE NEED TO SPAWN THEM, INSTEAD IT WILL RECYCLE THEM FOR US
	BotRecycler botRecycler;

	// THESE ARE THIS LEVEL'S SPAWNING POOLS
	vector<BotSpawningPool*> spawningPools;

public:
	// NOTHING TO INIT OR DESTROY
	SpriteManager() { 
		player = nullptr;
	}
	~SpriteManager()	{}

	// INLINED ACCESSOR METHODS
	BotRecycler*			getBotRecycler()			{ return &botRecycler;		}
	int						getNumberOfBots()			{ return bots.size();		}
	AnimatedSprite*			getPlayer()					{ return player;			}
	list<WorldItem*>::iterator getWorldItems()			{ return worldItems.begin();}
	list<WorldItem*>::iterator getEndOfWorldItems()		{ return worldItems.end();  }
	list<Bot*>::iterator	getBotsIterator()			{ return bots.begin();		}
	list<Bot*>::iterator	getEndOfBotsIterator()		{ return bots.end();		}
	vector<BotSpawningPool*>*  getSpawningPools()		{ return &spawningPools;	}
	AnimatedSprite*			getTv()						{ 
		list<WorldItem*>::iterator it;
		for (it = worldItems.begin(); it != worldItems.end(); it++) {
			if ((*it)->getSprite()->getSpriteType() == getSpriteType(L"TV")) {
				return (*it)->getSprite();
			}
		}
	}

	// INLINED METHOD FOR ADDING A SPAWNING POOL
	void addSpawningPool(BotSpawningPool *poolToAdd)
	{
		spawningPools.push_back(poolToAdd);
	}
	void setPlayer(AnimatedSprite *initPlayer)
	{
		player = initPlayer;
	}
	void addTeleporter(Teleporter *teleporter)
	{
		teleporters.push_back(teleporter);
	}
	void addWorldItem(WorldItem *item)
	{
		worldItems.push_back(item);
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