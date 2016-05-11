/*	
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	GameStateManager.h

	This class manages the static data for the game level being
	used. This means all game backgrounds, which are stored and
	manipulated in WorldLayer objects. A given level can have
	a game background rendered using multiple layers, some which
	can be tiled, others that can be sparse, and others that
	can be isometric.

	This class stores these layers and ensures they are rendered
	in the proper order, low index to high. Layers that need to
	be drawn first (the back-most layer), should be added first.
*/
#pragma once
#include "mg_VS\stdafx.h"
#include "mg\graphics\RenderList.h"
#include "mg\gsm\state\GameState.h"
#include "mg\gsm\sprite\SpriteManager.h"
#include "mg\gsm\state\GameStateMachine.h"
#include "mg\gsm\world\World.h"
#include "mg\gsm\world\WorldLayer.h"
#include "mg\gsm\world\Checkpoint.h"
#include "mg\resources\SoundManager.h"
#include "Box2D.h"

class Game;

static const int NO_LEVEL_LOADED = -1;

class GameStateManager
{
private:
	// THE CURRENT GAME STATE
	GameState currentGameState;

	// THE CURRENT LEVEL IS THE ONE CURRENTLY BEING PLAYED. NOTE THAT IF
	// THE CURRENT LEVEL IS 0, THEN IT MEANS THERE IS NO ACTIVE LEVEL LOADED,
	// LIKE WHEN WE'RE AT THE MAIN MENU
	unsigned int currentLevelIndex;

	// THESE VECTORS STORE THE NAMES OF EACH LEVEL AND THE RELATIVE
	// PATH AND FILE NAME OF EACH DATA INPUT FILE FOR ALL GAME LEVELS. 
	// NOTE THAT WE'LL LOAD THESE VECTORS WITH THIS DATA WHEN THE GAME STARTS UP, 
	// BUT WE'LL ONLY LOAD THE LEVELS INTO THE GAME WORLD AS EACH LEVEL IS PLAYED
	// NOTE THAT THE LEVEL NAME AT INDEX i IN THE FIRST VECTOR CORRESPONDS
	// TO THE PATH AND FILE NAME AT INDEX i IN THE SECOND ONE
	vector<wstring> levelNames;
	vector<wstring> levelDirs;
	vector<wstring> levelFiles;

	// FOR MANAGING STATIC GAME WORLD DATA, i.e. BACKGROUND IMAGES,
	// TILES, PLATFORMS, etc. BASICALLY THINGS THAT ARE NOT ANIMATED
	// AND DO NOT MOVE
	World world;

	// FOR MANAGING DYNAMIC GAME OBJECTS FOR CURRENT LEVEL
	// NOTE THAT WE CALL THE DYNAMIC OBJECTS "SPRITES"
	SpriteManager *spriteManager;

	// THIS IS AI SYSTEM THAT MANAGES GAME STATE TRANSITIONS
	GameStateMachine *gameStateMachine;

	// THE SYSTEM THAT PLAYS SOUNDS AND MUSIC
	SoundManager *soundManager;

	// FOR DOING ALL COLLISION DETECTION AND RESOLUTION
	b2World* myWorld;
	float32 timeStep = 1 / 60.0;      //the length of time passed to simulate (seconds)
	int32 velocityIterations = 8;   //how strongly to correct velocity
	int32 positionIterations = 3;

	// THE BITWISE FLAGS FOR THE CURRENT LEVEL
	int levelFlags = 0;

	// THE FLAG THAT WHEN ADDED TO levelFlags WILL TRIGGER LEVEL COMPLETION
	int levelCompletionFlag;

	// A MAP OF CENSORSHIP TARGETS TO IF THEY ARE CURRENTLY ACTIVE OR NOT
	map<int, bool> censorshipTargets;

public:
	// INLINED ACCESSOR METHODS
	GameState			getCurrentGameState()	{ return currentGameState;				}
	unsigned int		getCurrentLevelIndex()	{ return currentLevelIndex;				}
	unsigned int		getNumLevels()			{ return levelNames.size();				}
	SpriteManager*		getSpriteManager()		{ return spriteManager;					}
	SoundManager*		getSoundManager()		{ return soundManager;					}
	World*				getWorld()				{ return &world;						}
	GameStateMachine*	getGameStateMachine()	{ return gameStateMachine;				}
	wstring				getCurrentLevelName()	{ return levelNames[currentLevelIndex];	}
	b2World*			getB2World()			{ return myWorld;						}
	int					getFlags()				{ return levelFlags;					}
	bool				getCensorship(int c)	{ return censorshipTargets[c];			}

	// INLINED MUTATOR METHODS
	void setGameStateMachine(GameStateMachine *initBotStateManager) { gameStateMachine = initBotStateManager; }
	void setFlag(int flag) { levelFlags = levelFlags | flag; }
	void setLevelCompletionFlag(int flag) { levelCompletionFlag = flag; }
	void setCensorship(int c, bool active) { censorshipTargets[c] = active; }

	// METHODS FOR TESTING THE CURRENT GAME STATE
	bool			isAppActive();
	bool			isAtSplashScreen();
	bool			isGameInProgress();
	bool			isGameLevelLoading();
	bool			isPreGame();
	bool			isWorldRenderable();

	// METHODS FOR TRANSITIONING TO OTHER GAME STATES
	void			goToGame();
	void			goToInGame();
	void			goToLoadLevel();
	void			goToMainMenu();
	void			goToAboutMenu();
	void			goToHelpMenu();
	void			goToPreGame();
	void			goToGameOver();

	// METHODS DEFINED in GameStateManager.cpp
	GameStateManager();
	~GameStateManager();
	void			addGameRenderItemsToRenderList();
	void			addLevel(wstring levelToAddName, wstring levelToAddDir, wstring levelToAddFile);
	unsigned int	getLevelNum(wstring levelName);
	void			loadLevel(unsigned int levelNum);
	void			loadLevel(wstring levelName);
	void			loadCurrentLevel();
	void			loadNextLevel();
	void			unloadCurrentLevel();
	void			update();
	void			startUp();
	void			shutDown();
};