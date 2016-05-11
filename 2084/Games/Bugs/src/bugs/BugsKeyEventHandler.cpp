/*
Author: Richard McKenna
Stony Brook University
Computer Science Department

BugsKeyEventHandler.cpp

See BugsKeyEventHandler.h for a class description.
*/

#include "bugs_VS\stdafx.h"
#include "bugs\BugsApp.h"
#include "bugs\BugsKeyEventHandler.h"
#include "mg\clock\GameClock.h"
#include "mg\game\Game.h"
#include "mg\game\WStringTable.h"
#include "mg\graphics\GameGraphics.h"
#include "mg\gsm\sprite\AnimatedSprite.h"
#include "mg\gsm\state\GameState.h"
#include "mg\gsm\state\GameStateManager.h"
#include "mg\gui\Cursor.h"
#include "mg\gui\GameGUI.h"
#include "mg\input\GameInput.h"

/*
handleKeyEvent - this method handles all keyboard interactions. Note that every frame this method
gets called and it can respond to key interactions in any custom way. Ask the GameInput class for
key states since the last frame, which can allow us to respond to key presses, including when keys
are held down for multiple frames.
*/
void BugsKeyEventHandler::handleKeyEvents()
{
	Game *game = Game::getSingleton();

	// WE CAN QUERY INPUT TO SEE WHAT WAS PRESSED
	GameInput *input = game->getInput();

	// LET'S GET THE PLAYER'S PHYSICAL PROPERTIES, IN CASE WE WANT TO CHANGE THEM
	GameStateManager *gsm = game->getGSM();
	AnimatedSprite *player = gsm->getSpriteManager()->getPlayer();
	Viewport *viewport = game->getGUI()->getViewport();

	// IF THE GAME IS IN PROGRESS
	if (gsm->isGameInProgress())
	{
		if (input->isKeyDownForFirstTime(P_KEY))
		{

		}
		if (input->isKeyDownForFirstTime(T_KEY))
		{

		}
		if (input->isKeyDownForFirstTime(R_KEY))
		{
			viewport->toggleDebugView();
			game->getGraphics()->toggleDebugTextShouldBeRendered();
		}
		if (input->isKeyDownForFirstTime(K_KEY))
		{
			player->killSprite();
		}
		if (player->isDead())
			return;
		// HANDLE MOVEMENT INPUT
		// FIST GET DESIRED VELOCITY
		float xVel = 0, yVel = 0;
		float playerVelocity = 25.0f;
		if (input->isKeyDown(W_KEY)) yVel -= 1;
		if (input->isKeyDown(A_KEY)) xVel -= 1;
		if (input->isKeyDown(S_KEY)) yVel += 1;
		if (input->isKeyDown(D_KEY)) xVel += 1;
		// THEN CALCULATE FINAL VELOCITIES, SCALE BY MAGNITUDE AND VELOCITY SCALAR
		float magnitude = sqrt(xVel*xVel + yVel*yVel);
		if (magnitude > 0) {
			gsm->getSoundManager()->PlaySoundEffectLoop(WALKING_SFX);
			float xVelocity = playerVelocity * xVel / magnitude, yVelocity = playerVelocity * yVel / magnitude;
			player->getB2Body()->SetLinearVelocity(b2Vec2(xVelocity, yVelocity));
			if (xVelocity > 0)
				player->setCurrentState(WALKING_RIGHT);
			else if (xVelocity < 0)
				player->setCurrentState(WALKING_LEFT);
			else if (yVelocity > 0)
				player->setCurrentState(WALKING_DOWN);
			else if (yVelocity < 0)
				player->setCurrentState(WALKING_UP);
		}
		else {
			gsm->getSoundManager()->StopSoundEffect();
			player->getB2Body()->SetLinearVelocity(b2Vec2(0, 0));
			player->setCurrentState(L"IDLE");
		}
	}

	// 0X43 is HEX FOR THE 'C' VIRTUAL KEY
	// THIS CHANGES THE CURSOR IMAGE
	if ((input->isKeyDownForFirstTime(C_KEY))
		&& input->isKeyDown(VK_SHIFT))
	{
		Cursor *cursor = game->getGUI()->getCursor();
		unsigned int id = cursor->getActiveCursorID();
		id++;
		if (id == cursor->getNumCursorIDs())
			id = 0;
		cursor->setActiveCursorID(id);
	}

	// LET'S MESS WITH THE TARGET FRAME RATE IF THE USER PRESSES THE HOME OR END KEYS
	GameClock *clock = game->getClock();
	int fps = clock->getTargetFPS();

	// THIS SPEEDS UP OUR GAME LOOP AND THUS THE GAME, NOTE THAT WE COULD ALTERNATIVELY SCALE
	// DOWN THE GAME LOGIC (LIKE ALL VELOCITIES) AS WE SPEED UP THE GAME. THAT COULD PROVIDE
	// A BETTER PLAYER EXPERIENCE
	if (input->isKeyDown(VK_HOME) && (fps < MAX_FPS))
		clock->setTargetFPS(fps + FPS_INC);

	// THIS SLOWS DOWN OUR GAME LOOP, BUT WILL NOT GO BELOW 5 FRAMES PER SECOND
	else if (input->isKeyDown(VK_END) && (fps > MIN_FPS))
		clock->setTargetFPS(fps - FPS_INC);
}