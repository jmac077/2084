#include "mg\gsm\ai\Bot.h"
#include "mg\gsm\ai\behaviors\BotBehavior.h"
#include "mg\gsm\ai\behaviors\DeterministicMovement.h"

void DeterministicMovement::behave(Bot *bot)
{
	Game *game = Game::getSingleton();
	if (bot->getCurrentDest() != bot->getBotPath().end()) {
		game->getPathFinder()->mapPath(bot, (*bot->getCurrentDest())[0], (*bot->getCurrentDest())[1]);
		bot->nextWayPoint();
	}
	else {
		bot->getCurrentDest() = bot->getBotPath().begin();
		game->getPathFinder()->mapPath(bot, (*bot->getCurrentDest())[0], (*bot->getCurrentDest())[1]);
	}
}

BotBehavior* DeterministicMovement::clone()
{
	DeterministicMovement *behavior = new DeterministicMovement();
	return behavior;
}

