/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	BugsTextGenerator.cpp

	See BugsTextGenerator.h for a class description.
*/

#include "bugs_VS\stdafx.h"
#include "bugs\BugsApp.h"
#include "bugs\BugsTextGenerator.h"
#include "mg\clock\GameClock.h"
#include "mg\game\Game.h"
#include "mg\game\GameTypes.h"
#include "mg\graphics\GameGraphics.h"
#include "mg\gsm\ai\Bot.h"
#include "mg\gsm\ai\BotRecycler.h"
#include "mg\gsm\sprite\SpriteManager.h"
#include "mg\text\GameText.h"
#include "mg\platforms\Windows\WindowsInput.h"

static const int		LEFT_TEXT_X = 200;
static const int		RIGHT_TEXT_X = 700;
static const int		TEXT_Y = 10;
static const wstring	NEW_LINE_TEXT = L"\n";
static const wstring	MOUSE_COORDS_TEXT = L"Mouse: ";
static const wstring	TARGET_FPS_TEXT = L"Target FPS: ";
static const wstring	SLEEPLESS_FPS_TEXT = L"Sleepless FPS: ";
static const wstring	DELTA_TIME_TEXT = L"Delta Time: ";
static const wstring	BOT_COUNT_TEXT = L"Bot Count: ";
static const wstring	BOT_RECYCLER_TEXT = L"Bot Recycler Stats";
static const wstring	RECYCLABLE_BOTS_TEXT = L"-Recyclable Bots: ";

void BugsTextGenerator::appendBotRecycler()
{
	Game *game = Game::getSingleton();

	wstringstream wss;
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	BotRecycler *botRecycler = spriteManager->getBotRecycler();
	map<wstring, list<Bot *> *> *recyclableBots = botRecycler->getRecyclableBots();
	wss << BOT_RECYCLER_TEXT;
	map<wstring, list<Bot *> *>::iterator it = recyclableBots->begin();
	while (it != recyclableBots->end())
	{
		wstring botType = it->first;
		list<Bot *> *bots = it->second;
		it++;
		wss << NEW_LINE_TEXT;
		wss << L"-" << botType;
		wss << ": ";
		wss << bots->size();
	}
	wss << NEW_LINE_TEXT;
	rightTextToGenerate.append(wss.str());
}

void BugsTextGenerator::appendBotCount()
{
	Game *game = Game::getSingleton();

	wstringstream wss;
	SpriteManager *spriteManager = game->getGSM()->getSpriteManager();
	leftTextToGenerate.append(BOT_COUNT_TEXT);
	wss << spriteManager->getNumberOfBots();
	wss << NEW_LINE_TEXT;
	leftTextToGenerate.append(wss.str());
}

void BugsTextGenerator::appendMouseCoords()
{
	Game *game = Game::getSingleton();

	wstringstream wss;
	WindowsInput *input = (WindowsInput*)game->getInput();
	leftTextToGenerate.append(MOUSE_COORDS_TEXT);
	wss << input->getMousePoint()->x;
	wss << L", ";
	wss << input->getMousePoint()->y;
	wss << NEW_LINE_TEXT;
	leftTextToGenerate.append(wss.str());
}

void BugsTextGenerator::appendClock()
{
	Game *game = Game::getSingleton();
	GameClock *clock = game->getClock();

	wstringstream wss;
	wss << TARGET_FPS_TEXT;
	wss << clock->getTargetFPS();
	wss << NEW_LINE_TEXT;

	wss << SLEEPLESS_FPS_TEXT;
	wss << clock->getSleeplessFPS();
	wss << NEW_LINE_TEXT;
	
	wss << DELTA_TIME_TEXT;
	wss << clock->getDeltaTime();
	wss << NEW_LINE_TEXT;
	leftTextToGenerate.append(wss.str());
}

/*
	startUp - Provides an example of how to render text to our screen every frame,
	allowing for the text to continually change. This method adds a text object
	to the text manager (GameText), which allows it to be updated each frame. This
	method should be called only once for each object.
*/
void BugsTextGenerator::startUp()
{
	// FIRST REGISTER THIS GENERATOR WITH GameText
	Game *game = Game::getSingleton();
	GameText *text = game->getText();
	text->setTextGenerator((TextGenerator*)this);

	// THEN GENERATE THE INITIAL TEXT TO DISPLAY
	appendMouseCoords();
	appendClock();
	appendBotCount();
	appendBotRecycler();

	// AND THEN ADD IT TO THE TEXT MANAGER, SPECIFYING WHERE IT SHOULD BE RENDERED
	text->addRenderText(&leftTextToGenerate, LEFT_TEXT_X, TEXT_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
	text->addRenderText(&rightTextToGenerate, RIGHT_TEXT_X, TEXT_Y, game->getGraphics()->getScreenWidth(), game->getGraphics()->getScreenHeight());
}

/*
	updateText - updates the text. Note that the text must already have been added
	to the text manager via the initText method. Also not that there should be a
	separate TextGenerator for each line of text we wish to render.
*/
void BugsTextGenerator::update()
{
	// THE TEXT MANAGER (GameText) HAS A POINTER TO OUR TEXT, SO WE CAN
	// SIMPLY CLEAR IT OUT OF OLD CHARACTER DATA AND FILL IT IN WITH
	// WHAT WE WANT.
	leftTextToGenerate.clear();
	rightTextToGenerate.clear();
	appendMouseCoords();
	appendClock();
	appendBotCount();
	appendBotRecycler();
}

