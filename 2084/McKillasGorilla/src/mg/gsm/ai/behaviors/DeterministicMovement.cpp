#include "mg\gsm\ai\Bot.h"
#include "mg\gsm\ai\behaviors\BotBehavior.h"
#include "mg\gsm\ai\behaviors\DeterministicMovement.h"

void DeterministicMovement::behave(Bot *bot)
{
	Game *game = Game::getSingleton();
	if (bot->getCurrentPathToFollow()->size() <= 0) {
		game->getPathFinder()->mapPath(bot,1095,2024);
	}
}

BotBehavior* DeterministicMovement::clone()
{
	DeterministicMovement *behavior = new DeterministicMovement();
	return behavior;
}

