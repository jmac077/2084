/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	BugsButtonEventHandler.cpp

	See BugsButtonEventHandler.h for a class description.
*/

#include "bugs_VS\stdafx.h"
#include "bugs\BugsApp.h"
#include "bugs\BugsButtonEventHandler.h"
#include "mg\game\Game.h"
#include "mg\gsm\state\GameStateManager.h"

void BugsButtonEventHandler::handleButtonEvents(	wstring command)
{
	Game *game = Game::getSingleton();

	// THE USER PRESSED THE Exit BUTTON ON THE MAIN MENU,
	// SO LET'S SHUTDOWN THE ENTIRE APPLICATION
	if (command.compare(EXIT_COMMAND) == 0)
	{
		game->exitApp();
	}
	// THE USER PRESSED THE MOUSE BUTTON ON THE SPLASH
	// SCREEN, SO LET'S GO TO THE MAIN MENU
	else if (command.compare(GO_TO_MM_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		gsm->goToMainMenu();
	}
	else if (command.compare(ABOUT_SCREEN_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		gsm->goToAboutMenu();
	}
	else if (command.compare(HELP_SCREEN_COMMAND) == 0)
	{
		GameStateManager *gsm = game->getGSM();
		gsm->goToHelpMenu();
	}
	// THE USER PRESSED THE Start BUTTON ON THE MAIN MENU,
	// SO LET'S START THE GAME FROM THE FIRST LEVEL
	else if (command.compare(START_COMMAND) == 0)
	{
		game->startGame();
	}
	else if (command.compare(RETRY_COMMAND) == 0)
	{
		game->getGSM()->getSpriteManager()->getPlayer()->respawnAtLastCheckpoint();
		game->getGSM()->goToInGame();
	}
	// THE USER PRESSED THE Quit BUTTON ON THE IN-GAME MENU,
	// SO LET'S UNLOAD THE LEVEL AND RETURN TO THE MAIN MENU
	else if (command.compare(QUIT_COMMAND) == 0)
	{
		game->quitGame();
	}
}