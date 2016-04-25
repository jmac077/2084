#include "bugs_VS\stdafx.h"
#include "bugs\BugsMouseEventHandler.h"
#include "mg\game\Game.h"
#include "mg\gsm\sprite\SpriteManager.h"
#include "mg\gsm\state\GameStateManager.h"
#include "mg\gui\Viewport.h"

void BugsMouseEventHandler::handleMousePressEvent(int mouseX, int mouseY)
{
	Game *game = Game::getSingleton();
	if (game->getGSM()->isGameInProgress())
	{
		Viewport *viewport = game->getGUI()->getViewport();

		// DETERMINE WHERE ON THE MAP WE HAVE CLICKED
		int worldCoordinateX = mouseX + viewport->getViewportX();
		int worldCoordinateY = mouseY + viewport->getViewportY();

		//build player path
		SpriteManager* spriteManager = game->getGSM()->getSpriteManager();
		AnimatedSprite* player = spriteManager->getPlayer();
		game->getPathFinder()->mapPath(player,worldCoordinateX, worldCoordinateY);
	}
}

void BugsMouseEventHandler::handleMouseMoveEvent(int mouseX, int mouseY)
{
	Game *game = Game::getSingleton();

	// DETERMINE WHAT THE PATHFINDING GRID CELL
	// IS THAT THE MOUSE CURRENTLY IS ABOVE
	if (game->getGSM()->isGameInProgress())
	{
		// IF YOU LIKE YOU COULD DO SOME MOUSE OVER DEBUGGING STUFF HERE
	}
}