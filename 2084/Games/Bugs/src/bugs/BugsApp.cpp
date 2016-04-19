/*
	Author: Richard McKenna
			Stony Brook University
			Computer Science Department

	BugsApp.cpp

	This is a test game application, a game that demonstrates use of the 
	SideScrollerFramework to make a little scrolling, interactive demo. It
	demonstrates the rendering of images and text as well as responding to
	key presses and button cicks. Students should make their own named game
	applictions using a similar pattern, gradually adding other components,
	like additional gui controls, tiles, sprites, ai, and physics.
 */

#include "bugs_VS\stdafx.h"

// Bugs GAME INCLUDES
#include "bugs\BugsApp.h"
#include "bugs\BugsButtonEventHandler.h"
#include "bugs\BugsKeyEventHandler.h"
#include "bugs\BugsMouseEventHandler.h"
#include "bugs\BugsTextGenerator.h"
#include "bugs\BugsStateMachine.h"

// GAME OBJECT INCLUDES
#include "mg\game\Game.h"
#include "mg\graphics\GameGraphics.h"
#include "mg\gsm\state\GameState.h"
#include "mg\gsm\state\GameStateMachine.h"
#include "mg\gui\Cursor.h"
#include "mg\gui\GameGUI.h"
#include "mg\gui\ScreenGUI.h"
#include "mg\input\GameInput.h"
#include "mg\os\GameOS.h"
#include "mg\text\GameText.h"

// WINDOWS PLATFORM INCLUDES
#include "mg\platforms\Windows\WindowsOS.h"
#include "mg\platforms\Windows\WindowsInput.h"

// DIRECTX INCLUDES
#include "mg\platforms\DirectX\DirectXGraphics.h"
#include "mg\platforms\DirectX\DirectXTextureManager.h"

// FORWARD DECLARATIONS
void initCursor();
void initInGameGUI();
void initMainMenu();
void initSplashScreen();
void initLoadingLevel();
void initViewport();

/*
	WinMain - This is the application's starting point. In this method we will construct a Game object, 
	then initialize all the platform-dependent technologies, then construct all the custom data for our 
	game, and then initialize the Game with	our custom data. We'll then start the game loop.
*/
int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine,
                   int nCmdShow)
{
	// CREATE AND START UP THE ENGINE
	Game *bugsGame = Game::getSingleton();
	bugsGame->startUp();

	// NOW LOAD THE GAME-SPECIFIC STUFF

	// LOAD OUR CUSTOM TEXT GENERATOR, WHICH DETERMINES WHAT TEXT IS DRAWN EACH FRAME
	BugsTextGenerator *bugsTextGenerator = new BugsTextGenerator();
	bugsTextGenerator->startUp();

	// NOW LET'S LOAD THE GUI STUFF
	initViewport();
	initCursor();
	initSplashScreen();
	initMainMenu();
	initInGameGUI();
	initLoadingLevel();

	// SPECIFY WHO WILL HANDLE BUTTON EVENTS
	BugsButtonEventHandler *bugsButtonHandler = new BugsButtonEventHandler();
	GameGUI *gui = bugsGame->getGUI();
	gui->registerButtonEventHandler((ButtonEventHandler*)bugsButtonHandler);

	// SPECIFY WHO WILL HANDLE KEY EVENTS
	BugsKeyEventHandler *bugsKeyHandler = new BugsKeyEventHandler();
	bugsGame->getInput()->registerKeyHandler((KeyEventHandler*)bugsKeyHandler);

	// SPECIFY WHO WILL HANDLE MOUSE EVENTS NOT RELATED TO THE GUI
	BugsMouseEventHandler *bugsMouseHandler = new BugsMouseEventHandler();
	bugsGame->getInput()->registerMouseHandler((MouseEventHandler*)bugsMouseHandler);

	// MAKE THE GAME STATE MACHINE THAT WILL HELP 
	// IMPLEMENT SOME CUSTOM GAME RULES
	BugsStateMachine *bsm = new BugsStateMachine();
	GameStateManager *gsm = bugsGame->getGSM();
	gsm->setGameStateMachine(bsm);

	// START THE GAME LOOP
	bugsGame->runGameLoop();

	// AND RETURN
	return 0;
}

/*
	initCursor - initializes a simple little cursor for the gui.
*/
void initCursor()
{
	Game *game = Game::getSingleton();
	GameGraphics *graphics = game->getGraphics();
	TextureManager *guiTextureManager = graphics->getGUITextureManager();

	// SETUP THE CURSOR
	vector<unsigned int> *imageIDs = new vector<unsigned int>();
	int imageID;

	// - LOAD THE RED ANT CURSOR IMAGE
	imageID = guiTextureManager->loadTexture(RED_ANT_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - LOAD THE BLACK ANT CURSOR IMAGE
	imageID = guiTextureManager->loadTexture(BLACK_ANT_CURSOR_PATH);
	imageIDs->push_back(imageID);

	// - NOW BUILD AND LOAD THE CURSOR
	Cursor *cursor = new Cursor();
	cursor->initCursor(imageIDs,
		*(imageIDs->begin()),
		0,
		0,
		0,
		255,
		48,
		48);
	GameGUI *gui = game->getGUI();
	gui->setCursor(cursor);
}

/*
	initInGameGUI - initializes the game's in-game gui.
*/
void initInGameGUI()
{
	Game *game = Game::getSingleton();
	GameGraphics *graphics = game->getGraphics();
	TextureManager *guiTextureManager = graphics->getGUITextureManager();

	// NOW ADD THE IN-GAME GUI
	ScreenGUI *inGameGUI = new ScreenGUI();

	unsigned int imageID = guiTextureManager->loadTexture(IN_GAME_TOOLBAR_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->width = 1366;
	imageToAdd->height = 64;
	imageToAdd->x = 0;
	imageToAdd->y = 0;
	imageToAdd->z = 0;
	imageToAdd->imageID = imageID;
	inGameGUI->addOverlayImage(imageToAdd);

	unsigned int normalTextureID = guiTextureManager->loadTexture(QUIT_IMAGE_PATH);
	unsigned int mouseOverTextureID = guiTextureManager->loadTexture(QUIT_IMAGE_MO_PATH);
	
	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		0,
		0,
		0,
		255,
		200,
		100,
		false,
		QUIT_COMMAND);
	inGameGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	GameGUI *gui = game->getGUI();
	gui->addScreenGUI(GS_GAME_IN_PROGRESS, inGameGUI);
}

/*
	initMainMenu - initializes the game's main menu gui.
*/
void initMainMenu()
{
	Game *game = Game::getSingleton();
	GameGraphics *graphics = game->getGraphics();
	TextureManager *guiTextureManager = graphics->getGUITextureManager();

	// NOW LET'S LOAD A MAIN MENU GUI SCREEN
	ScreenGUI *mainMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(MAIN_MENU_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->width = 1500;
	imageToAdd->height = 1120;
	imageToAdd->x = (graphics->getScreenWidth() / 2) - (imageToAdd->width / 2);
	imageToAdd->y = (graphics->getScreenHeight() / 2) - (imageToAdd->height / 2);
	imageToAdd->z = 0;
	imageToAdd->imageID = imageID;
	mainMenuGUI->addOverlayImage(imageToAdd);

	// AND LET'S ADD AN EXIT BUTTON
	Button *buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	int normalTextureID = guiTextureManager->loadTexture(EXIT_IMAGE_PATH);
	int mouseOverTextureID = guiTextureManager->loadTexture(EXIT_IMAGE_MO_PATH);
	int buttonWidth = 200;
	int buttonHeight = 100;
	int buttonY = (graphics->getScreenHeight() * 70 / 100);
	int buttonPadding = 15;

	// - INIT THE EXIT BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		((graphics->getScreenWidth() / 5) - buttonWidth) / 2 + 4 * (graphics->getScreenWidth() / 5),
		buttonY,
		0,
		255,
		buttonWidth,
		buttonHeight,
		false,
		EXIT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD AN LOAD BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture(LOAD_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(LOAD_IMAGE_MO_PATH);

	// - INIT THE LOAD BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		((graphics->getScreenWidth() / 5) - buttonWidth) / 2 + (graphics->getScreenWidth() / 5),
		buttonY,
		0,
		255,
		buttonWidth,
		buttonHeight,
		false,
		EXIT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD AN ABOUT BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture(ABOUT_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(ABOUT_IMAGE_MO_PATH);

	// - INIT THE ABOUT BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		((graphics->getScreenWidth() / 5) - buttonWidth) / 2 + 2 * (graphics->getScreenWidth() / 5),
		buttonY,
		0,
		255,
		buttonWidth,
		buttonHeight,
		false,
		EXIT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD AN HELP BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs
	normalTextureID = guiTextureManager->loadTexture(HELP_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(HELP_IMAGE_MO_PATH);

	// - INIT THE HELP BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		((graphics->getScreenWidth() / 5) - buttonWidth) / 2 + 3 * (graphics->getScreenWidth() / 5),
		buttonY,
		0,
		255,
		buttonWidth,
		buttonHeight,
		false,
		EXIT_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD A START BUTTON
	buttonToAdd = new Button();

	// - GET THE BUTTON COMMAND AND IMAGE IDs

	normalTextureID = guiTextureManager->loadTexture(START_IMAGE_PATH);
	mouseOverTextureID = guiTextureManager->loadTexture(START_IMAGE_MO_PATH);

	// - INIT THE START BUTTON
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		((graphics->getScreenWidth() / 5) - buttonWidth) / 2,
		buttonY,
		0,
		255,
		buttonWidth,
		buttonHeight,
		false,
		START_COMMAND);

	// AND NOW LOAD IT INTO A ScreenGUI
	mainMenuGUI->addButton(buttonToAdd);

	// AND LET'S ADD OUR SCREENS
	GameGUI *gui = game->getGUI();
	gui->addScreenGUI(GS_MAIN_MENU, mainMenuGUI);
}


void initLoadingLevel()
{
	Game *game = Game::getSingleton();
	GameGraphics *graphics = game->getGraphics();
	TextureManager *guiTextureManager = graphics->getGUITextureManager();

	// ADD A LOADING SCREEN GUI
	ScreenGUI *loadingLevelGUI = new ScreenGUI();

	// INIT THE QUIT BUTTON
	// NOW LET'S LOAD A MAIN MENU GUI SCREEN
	ScreenGUI *mainMenuGUI = new ScreenGUI();
	unsigned int imageID = guiTextureManager->loadTexture(LOADING_LEVEL_PATH);
	OverlayImage *imageToAdd = new OverlayImage();
	imageToAdd->alpha = 255;
	imageToAdd->width = 1500;
	imageToAdd->height = 1120;
	imageToAdd->x = (graphics->getScreenWidth() / 2) - (imageToAdd->width / 2);
	imageToAdd->y = (graphics->getScreenHeight() / 2) - (imageToAdd->height / 2);
	imageToAdd->z = 0;
	imageToAdd->imageID = imageID;
	loadingLevelGUI->addOverlayImage(imageToAdd);

	// AND REGISTER IT WITH THE GUI
	GameGUI *gui = game->getGUI();
	gui->addScreenGUI(GS_PRE_GAME, loadingLevelGUI);
	gui->addScreenGUI(GS_LOADING_LEVEL, loadingLevelGUI);

}

/*
	initSplashScreen - initializes the game's splash screen gui.
*/
void initSplashScreen()
{
	Game *game = Game::getSingleton();
	GameGraphics *graphics = game->getGraphics();
	TextureManager *guiTextureManager = graphics->getGUITextureManager();

	// NOW, FIRST LET'S ADD A SPLASH SCREEN GUI
	ScreenGUI *splashScreenGUI = new ScreenGUI();

	// WE'LL ONLY HAVE ONE IMAGE FOR OUR GIANT BUTTON
	unsigned int normalTextureID = guiTextureManager->loadTexture(SPLASH_SCREEN_PATH);
	unsigned int mouseOverTextureID = normalTextureID;

	int imageWidth = 1500;
	int imageHeight = 1120;

	// INIT THE QUIT BUTTON
	Button *buttonToAdd = new Button();
	buttonToAdd->initButton(normalTextureID,
		mouseOverTextureID,
		(game->getGraphics()->getScreenWidth() / 2) - (imageWidth / 2),
		(game->getGraphics()->getScreenHeight() / 2) - (imageHeight / 2),
		0,
		255,
		imageWidth,
		imageHeight,
		false,
		GO_TO_MM_COMMAND);
	splashScreenGUI->addButton(buttonToAdd);

	// AND REGISTER IT WITH THE GUI
	GameGUI *gui = game->getGUI();
	gui->addScreenGUI(GS_SPLASH_SCREEN, splashScreenGUI);
}

/*
	initViewport - initializes the game's viewport.
*/
void initViewport()
{
	Game *game = Game::getSingleton();
	GameGUI *gui = game->getGUI();
	GameGraphics *graphics = game->getGraphics();

	// AND NOW SPECIFY THE VIEWPORT SIZE AND LOCATION. NOTE THAT IN THIS EXAMPLE,
	// WE ARE PUTTING A TOOLBAR WITH A BUTTON ACCROSS THE NORTH OF THE APPLICATION.
	// THAT TOOLBAR HAS A HEIGHT OF 64 PIXELS, SO WE'LL MAKE THAT THE OFFSET FOR
	// THE VIEWPORT IN THE Y AXIS
	Viewport *viewport = gui->getViewport();

	int viewportWidth = graphics->getScreenWidth() - VIEWPORT_OFFSET_X;
	int viewportHeight = graphics->getScreenHeight() - VIEWPORT_OFFSET_Y;
	viewport->setViewportWidth(viewportWidth);
	viewport->setViewportHeight(viewportHeight);
	viewport->setToggleOffsetY(TOGGLE_OFFSET_Y);
}