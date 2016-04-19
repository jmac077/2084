#include "stdafx.h"

// THESE ARE THE TEST BEDS
#include "LevelLoadingTestBed.h"

/*
WinMain - This is the application's starting point. In this method we will construct a Game object,
then initialize all the platform-dependent technologies, then construct all the custom data for our
game, and then initialize the Game with	our custom data. We'll then start the game loop.
*/
void main(int argc, char *argv[])
{
	// TEST LEVEL LOADING AND UNLOADING
	LevelLoadingTestBed::runLevelLoadingTest();
}