// GAME OBJECT INCLUDES
#include "mg\game\Game.h"
#include "mg\graphics\GameGraphics.h"
#include "mg\gsm\state\GameState.h"
#include "mg\gui\Cursor.h"
#include "mg\gui\GameGUI.h"
#include "mg\gui\ScreenGUI.h"
#include "mg\input\GameInput.h"
#include "mg\os\GameOS.h"
#include "mg\resources\GameResources.h"
#include "mg\resources\importers\MGLevelImporter.h"
#include "mg\text\GameText.h"

// WINDOWS PLATFORM INCLUDES
#include "mg\platforms\Windows\WindowsOS.h"
#include "mg\platforms\Windows\WindowsInput.h"

// DIRECTX INCLUDES
#include "mg\platforms\DirectX\DirectXGraphics.h"
#include "mg\platforms\DirectX\DirectXTextureManager.h"#include "LevelLoadingTestBed.h"

namespace LevelLoadingTestBed
{
	void runLevelLoadingTest()
	{
		// CREATE AND START UP THE ENGINE
		Game *bugsGame = Game::getSingleton();
		bugsGame->startUp();

		// GET THE LEVEL IMPORTER
		GameResources *resources = bugsGame->getResources();
		MGLevelImporter *importer = (MGLevelImporter*)resources->getLevelImporter();

		// THEN LOAD THE WORLD WITH A LEVEL
		wstring levelDir = L"./data/levels/";
		wstring levelFile = L"Level1.xml";
		cout << "Loading " << levelDir.c_str() << levelFile.c_str() << "\n";
		importer->load(levelDir, levelFile);

		// THEN UNLOAD IT
		cout << "Unloading " << levelDir.c_str() << levelFile.c_str() << "\n";
		importer->unload();

		// THEN LOAD ANOTHER LEVEL
		levelFile = L"Level2.xml";
		cout << "Loading " << levelDir.c_str() << levelFile.c_str() << "\n";
		importer->load(levelDir, levelFile);

		// THEN UNLOAD IT
		cout << "Loading " << levelDir.c_str() << levelFile.c_str() << "\n";
		importer->unload();

		// AND THEN LOAD THE FIRST ONE AGAIN
		levelFile = L"Level1.xml";
		cout << "Loading " << levelDir.c_str() << levelFile.c_str() << "\n";
		importer->load(levelDir, levelFile);
	}
}