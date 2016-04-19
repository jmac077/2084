#include "mg_VS\stdafx.h"
#include "BugsStateMachine.h"
#include "mg\game\Game.h"
#include "mg\gsm\state\GameStateManager.h"

void BugsStateMachine::update()
{
	Game *game = Game::getSingleton();
	GameStateManager *gsm = game->getGSM();
	SpriteManager *spriteManager = gsm->getSpriteManager();

	// IS THE GAME GOING ON?
	if (gsm->isGameInProgress())
	{
	}
	else if (gsm->isPreGame())
	{
		gsm->goToLoadLevel();
	}
	else if (gsm->isGameLevelLoading())
	{
		// NOW WE NEED TO TRANSITION TO THE NEXT LEVEL
		// BUT NOTE WE'RE HARD CODING THIS IN THAT WE
		// KNOW HOW MANY LEVELS THERE ARE, WHICH ISN'T IDEAL
		if ((gsm->getCurrentLevelIndex() == 0) ||
			(gsm->getCurrentLevelIndex() == NO_LEVEL_LOADED))
		{
			// WE'LL GO ONTO LEVEL INDEX 1
			gsm->loadNextLevel();
			gsm->goToGame();
		}
		else
		{
			// WE ONLY HAVE 2 LEVELS SO WHEN WE FINISH
			// THE SECOND ONE JUST GO BACK TO THE MAIN MENU
			gsm->goToMainMenu();
		}
	}
}