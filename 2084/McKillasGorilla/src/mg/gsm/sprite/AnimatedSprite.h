/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	AnimatedSprite.h

	This class represents a sprite that can can
	be used to animate a game character or object.
*/

#pragma once
#include "mg_VS\stdafx.h"
#include "mg\gsm\sprite\AnimatedSpriteType.h"
#include "mg\gui\Viewport.h"
#include "mg\gsm\ai\pathfinding\OrthographicGridPathfinder.h"
#include "mg\gsm\world\Teleporter.h"
#include "Box2D.h"

// ANIMATION STATES
static const wstring WALKING_DOWN = L"WALKING_DOWN";
static const wstring WALKING_RIGHT = L"WALKING_RIGHT";
static const wstring WALKING_UP = L"WALKING_UP";
static const wstring WALKING_LEFT = L"WALKING_LEFT";

class GameStateManager;

class AnimatedSprite
{
protected:
	// SPRITE TYPE FOR THIS SPRITE. THE SPRITE TYPE IS THE ID
	// OF AN AnimatedSpriteType OBJECT AS STORED IN THE SpriteManager
	AnimatedSpriteType *spriteType;

	// TRANSPARENCY/OPACITY
	int alpha;

	// THE "current" STATE DICTATES WHICH ANIMATION SEQUENCE 
	// IS CURRENTLY IN USE, BUT IT MAP ALSO BE USED TO HELP
	// WITH OTHER GAME ACTIVITIES, LIKE PHYSICS
	wstring currentState;

	// THE INDEX OF THE CURRENT FRAME IN THE ANIMATION SEQUENCE
	// NOTE THAT WE WILL COUNT BY 2s FOR THIS SINCE THE VECTOR
	// THAT STORES THIS DATA HAS (ID,DURATION) TUPLES
	unsigned int frameIndex;

	// USED TO ITERATE THROUGH THE CURRENT ANIMATION SEQUENCE
	unsigned int animationCounter;

	// USED TO RENDER A ROTATED SPRITE, NOT INVOLVED IN PHYSICS
	float rotationInRadians;

	// HELPS US KEEP TRACK OF WHEN TO REMOVE IT
	bool markedForRemoval;

	list<PathNode> *currentPathToFollow;

	list<PathNode>::iterator currentPathNode;

	b2Body* myBody;

	bool m_contacting = false;

	void (AnimatedSprite::*collisionBehavior)() = NULL;

	// THE TELEPORT INFO FOR WHEN THIS SPRITE IS TELEPORTED
	Teleporter *teleportTarget;
public:
	// INLINED ACCESSOR METHODS
	int					getAlpha()				{ return alpha;					}
	wstring				getCurrentState()		{ return currentState;			}
	unsigned int		getFrameIndex()			{ return frameIndex;			}
	float				getRotationInRadians()	{ return rotationInRadians;		}
	AnimatedSpriteType*	getSpriteType()			{ return spriteType;			}
	bool				isMarkedForRemoval() { return markedForRemoval; }
	b2Body*				getB2Body()				{ return myBody; }
	void				(AnimatedSprite::*getTeleportPlayer())(){ return &AnimatedSprite::teleportPlayer; }

	// INLINED MUTATOR METHODS
	void setAlpha(int initAlpha)
	{	alpha = initAlpha;						}
	void setRotationInRadians(float initRotation)
	{	rotationInRadians = initRotation;		}
	void setSpriteType(AnimatedSpriteType *initSpriteType)
	{	spriteType = initSpriteType;			}
	void markForRemoval()
	{
		markedForRemoval = true;
	}
	void setB2Body(b2Body* initBody) {
		myBody = initBody;
	}
	void setCollisionBehavior(void (AnimatedSprite::*input)()) {
		collisionBehavior = input;
	}
	void setTeleportTarget(Teleporter *tel) {
		teleportTarget = tel;
	}

	// METHODS DEFINED IN AnimatedSprite.cpp
	AnimatedSprite();
	virtual ~AnimatedSprite();
	void changeFrame();
	unsigned int getCurrentImageID();
	void setCurrentState(wstring newState);
	void updateSprite();
	list<PathNode>* getCurrentPathToFollow() { return currentPathToFollow;  }
	list<PathNode>::iterator getCurrentPathNode() { return currentPathNode;  }
	void advanceCurrentPathNode() { currentPathNode++;  }
	bool hasReachedDestination() { return currentPathNode == currentPathToFollow->end(); }
	void clearPath() { currentPathToFollow->clear();  }
	void resetCurrentPathNode() { currentPathNode = currentPathToFollow->begin(); }
	void startContact();
	void endContact();
	void teleportPlayer();
};