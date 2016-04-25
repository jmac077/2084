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
		b2Filter filter  = spriteManager->getTv()->getB2Body()->GetFixtureList()->GetFilterData();
		filter.categoryBits = 0x0000;
		spriteManager->getTv()->getB2Body()->GetFixtureList()->SetFilterData(filter);
		spriteManager->getTv()->getB2Body()->SetLinearVelocity(b2Vec2(-2.0f,0.0f));
		if ((spriteManager->getTv()->getB2Body()->GetPosition().x - 50.0f) < .0333f) {
			spriteManager->getTv()->getB2Body()->SetTransform(b2Vec2(50.0f,38.0f),0.0f);
			spriteManager->getTv()->getB2Body()->SetLinearVelocity(b2Vec2(0.0f,0.0f));
		}
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