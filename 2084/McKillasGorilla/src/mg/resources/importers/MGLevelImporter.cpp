#include "mg_VS\stdafx.h"
#include "mg\game\Game.h"
#include "mg\graphics\GameGraphics.h"
#include "mg\gsm\ai\behaviors\BotBehavior.h"
#include "mg\gsm\ai\behaviors\DeterministicMovement.h"
#include "mg\gsm\ai\behaviors\RandomIntervalMovement.h"
#include "mg\gsm\ai\behaviors\RandomSpawn.h"
#include "mg\gsm\ai\behaviors\RecycledDeath.h"
#include "mg\gsm\ai\behaviors\RespawnedDeath.h"
#include "mg\gsm\ai\BotRecycler.h"
#include "mg\gsm\ai\BotSpawningPool.h"
#include "mg\gsm\sprite\AnimatedSprite.h"
#include "mg\gsm\sprite\SpriteManager.h"
#include "mg\gsm\state\GameStateManager.h"
#include "mg\resources\GameResources.h"
#include "mg\resources\importers\MGLevelImporter.h"
#include "mg\resources\importers\PoseurSpriteTypeImporter.h"
#include "mg\resources\importers\TMXMapImporter.h"
#include "mg\text\GameText.h"
#include "tinyxml\tinystr.h";
#include "tinyxml\tinyxml.h";
#include "Box2D.h"
#include "mg\gsm\world\HiddenWall.h"

bool MGLevelImporter::load(wstring levelFileDir, wstring levelFile)
{
	// FOR PRINTING DEBUGGING TEXT
	Game *game = Game::getSingleton();
	GameStateManager *gsm = game->getGSM();
	GameText *text = game->getText();

	// LOAD THE XML DOC
	const char *charPath = xmlReader.newCharArrayFromWstring(levelFileDir + levelFile);
	TiXmlDocument doc(charPath);
	bool loadOkay = doc.LoadFile();
	if (loadOkay)
	{
		TiXmlElement *level = doc.FirstChildElement();
		
		// WHAT'S THE NAME OF THE LEVEL?
		const char* levelName = level->Attribute(MG_LEVEL_NAME_ATT.c_str());
		string textToWrite(levelName);
		wstring wTextToWrite(textToWrite.begin(), textToWrite.end());
		text->writeDebugOutput(L"Level Name: " + wTextToWrite);
		
		int completionFlag = xmlReader.extractIntAtt(level, MG_COMPLETION_FLAG_ATT);
		gsm->setLevelCompletionFlag(completionFlag);

		// level_map
		TiXmlElement *levelMap = level->FirstChildElement();

		// level_map DIR
		const char* mapDir = levelMap->Attribute(MG_MAP_DIR_ATT.c_str());
		string dir(mapDir);
		wstring wDir(dir.begin(), dir.end());
		text->writeDebugOutput(L"Map Dir: " + wDir);

		// level_map FILE
		const char* mapFile = levelMap->Attribute(MG_MAP_FILE_ATT.c_str());
		string file(mapFile);
		wstring wFile(file.begin(), file.end());
		text->writeDebugOutput(L"Map File: " + wFile);

		// NOW LOAD THE MAP
		GameResources *resources = game->getResources();
		TMXMapImporter *mapImporter = (TMXMapImporter*)resources->getMapImporter();
		mapImporter->load(wDir, wFile);

		// level_sprite_types
		TiXmlElement *levelSpriteTypes = levelMap->NextSiblingElement();

		// sprite_types DIR
		const char* spriteTypesDir = levelSpriteTypes->Attribute(MG_SPRITE_TYPES_DIR_ATT.c_str());
		dir = string(spriteTypesDir);
		wDir = wstring(dir.begin(), dir.end());
		text->writeDebugOutput(L"Sprite Types Dir: " + wDir);

		// sprite_types FILE
		const char* spriteTypesFile = levelSpriteTypes->Attribute(MG_SPRITE_TYPES_FILE_ATT.c_str());
		file = string(spriteTypesFile);
		wFile = wstring(file.begin(), file.end());
		text->writeDebugOutput(L"Sprite Types File: " + wFile);

		// AND LOAD THE SPRITE TYPES
		PoseurSpriteTypeImporter *spriteTypesImporter = (PoseurSpriteTypeImporter*)resources->getSpriteTypeImporter();
		spriteTypesImporter->load(wDir, wFile);

		// NOW THAT WE KNOW THE SPRITE TYPES LET'S SETUP
		// THE BotRecycler SO THAT WE CAN EASILY
		// MAKE AnimatedSprites AND SPAWN WITHOUT PENALTY
		SpriteManager *spriteManager = gsm->getSpriteManager();
		BotRecycler *botRecycler = spriteManager->getBotRecycler();

		// player_position
		TiXmlElement *playerPosition = levelSpriteTypes->NextSiblingElement();
		int playerX = xmlReader.extractIntAtt(playerPosition, MG_X_ATT);
		int playerY = xmlReader.extractIntAtt(playerPosition, MG_Y_ATT);

		// Relevant box2d objects to be reused
		b2BodyDef bodyDef;
		b2Body* body;
		b2PolygonShape dynamicBox;
		b2FixtureDef fixtureDef;

		//init player sprite
		AnimatedSprite* player = new AnimatedSprite();
		player->setSpriteType(spriteManager->getSpriteType(L"Guy"));
		player->setAlpha(255);
		player->setCurrentState(L"WALKING_DOWN");
		player->setRotationInRadians(0);
		spriteManager->setPlayer(player);
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(playerX, playerY);
		bodyDef.fixedRotation = true;
		body = gsm->getB2World()->CreateBody(&bodyDef);
		dynamicBox.SetAsBox(.8125f, .8125f);
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.isSensor = false;
		body->CreateFixture(&fixtureDef);
		player->setB2Body(body);
		body->SetUserData(player);
		player->setCollisionBehavior(player->getCollisionHandler());

		// level_sections
		TiXmlElement *levelSections = playerPosition->NextSiblingElement();
		TiXmlElement *levelSection = levelSections->FirstChildElement();
		while (levelSection != nullptr)
		{
			int id = xmlReader.extractIntAtt(levelSection, MG_ID_ATT);
			int x = xmlReader.extractIntAtt(levelSection, MG_X_ATT);
			int y = xmlReader.extractIntAtt(levelSection, MG_Y_ATT);
			int width = xmlReader.extractIntAtt(levelSection, MG_WIDTH_ATT);
			if (width == -1)
				width = gsm->getWorld()->getWorldWidth() - x;
			int height = xmlReader.extractIntAtt(levelSection, MG_HEIGHT_ATT);
			if (height == -1)
				height = gsm->getWorld()->getWorldHeight() - y;

			LevelSection *section = new LevelSection(x, y, width, height);
			gsm->getWorld()->addLevelSection(id, section);

			levelSection = levelSection->NextSiblingElement();
		}

		// teleports
		TiXmlElement *teleports = levelSections->NextSiblingElement();
		TiXmlElement *teleport = teleports->FirstChildElement();
		while (teleport != nullptr)
		{
			int x = xmlReader.extractIntAtt(teleport, MG_X_ATT);
			int y = xmlReader.extractIntAtt(teleport, MG_Y_ATT);
			int width = xmlReader.extractIntAtt(teleport, MG_WIDTH_ATT);
			int height = xmlReader.extractIntAtt(teleport, MG_HEIGHT_ATT);
			int destX = xmlReader.extractIntAtt(teleport, MG_DEST_X);
			int destY = xmlReader.extractIntAtt(teleport, MG_DEST_Y);
			int targetSection = xmlReader.extractIntAtt(teleport, MG_TARGET_SECTION_ATT);
			bool censorship = xmlReader.extractBoolAtt(teleport, MG_CENSORSHIP_ATT);
			int censorshipTarget = xmlReader.extractIntAtt(teleport, MG_CENSORSHIP_TARGET_ATT);

			AnimatedSprite *teleporterSprite = NULL;

			// IF TELEPORTER IS USED FOR CENSORSHIP PART, CREATE ANIMATED SPRITE
			if (censorship)
			{
				// CREATE ANIMATED SPRITE FOR TELEPORTER
				teleporterSprite = new AnimatedSprite();
				teleporterSprite->setSpriteType(spriteManager->getSpriteType(L"StaticColumn"));
				teleporterSprite->setAlpha(255);
				teleporterSprite->setCurrentState(L"IDLE");
				teleporterSprite->setRotationInRadians(0);

				gsm->setCensorship(censorshipTarget, false);
			}

			Teleporter *teleporter = new Teleporter(destX, destY, targetSection, censorship, censorshipTarget, teleporterSprite);

			// CREATE TELEPORTER ZONE AS BOX2D SENSOR BOX
			bodyDef.type = b2_staticBody;
			bodyDef.position.Set(x, y);
			body = gsm->getB2World()->CreateBody(&bodyDef);
			dynamicBox.SetAsBox(width, height);
			fixtureDef.shape = &dynamicBox;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.0f;
			fixtureDef.isSensor = true;
			body->CreateFixture(&fixtureDef);
			body->SetUserData(new CollidableZone(teleporter, TeleporterFlag));

			if (censorship) {
				teleporterSprite->setB2Body(body);
				spriteManager->addTeleporter(teleporter);
			}

			teleport = teleport->NextSiblingElement();
		}

		// checkpoints
		TiXmlElement *checkpoints = teleports->NextSiblingElement();
		TiXmlElement *checkpoint = checkpoints->FirstChildElement();
		while (checkpoint != nullptr)
		{
			int x = xmlReader.extractIntAtt(checkpoint, MG_X_ATT);
			int y = xmlReader.extractIntAtt(checkpoint, MG_Y_ATT);
			int width = xmlReader.extractIntAtt(checkpoint, MG_WIDTH_ATT);
			int height = xmlReader.extractIntAtt(checkpoint, MG_HEIGHT_ATT);
			int levelSection = xmlReader.extractIntAtt(checkpoint, MG_TARGET_SECTION_ATT);
			Checkpoint *check = new Checkpoint(levelSection);

			// CREATE TELEPORTER ZONE AS BOX2D SENSOR BOX
			bodyDef.type = b2_staticBody;
			bodyDef.position.Set(x, y);
			body = gsm->getB2World()->CreateBody(&bodyDef);
			dynamicBox.SetAsBox(width, height);
			fixtureDef.shape = &dynamicBox;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.0f;
			fixtureDef.isSensor = true;
			body->CreateFixture(&fixtureDef);
			body->SetUserData(new CollidableZone(check, CheckpointFlag));
			
			checkpoint = checkpoint->NextSiblingElement();
		}

		// world_items
		TiXmlElement *worldItems = checkpoints->NextSiblingElement();
		TiXmlElement *worldItem = worldItems->FirstChildElement();
		while (worldItem != nullptr)
		{
			// GET ITEM ATTRIBUTES
			int x = xmlReader.extractIntAtt(worldItem, MG_X_ATT);
			int y = xmlReader.extractIntAtt(worldItem, MG_Y_ATT);
			int width = xmlReader.extractIntAtt(worldItem, MG_WIDTH_ATT);
			int height = xmlReader.extractIntAtt(worldItem, MG_HEIGHT_ATT);
			int flag = xmlReader.extractIntAtt(worldItem, MG_FLAG_ATT);
			bool collectible = xmlReader.extractBoolAtt(worldItem, MG_COLLECTIBLE_ATT);

			const char* itemType = xmlReader.extractCharAtt(worldItem, MG_BOT_TYPE_ATT);
			string strItemType(itemType);
			wstring wItemType(strItemType.begin(), strItemType.end());
			text->writeDebugOutput(L"Item Type: " + wItemType);

			// CREATE ANIMATED SPRITE FOR ITEM
			AnimatedSprite *itemSprite = new AnimatedSprite();
			itemSprite->setSpriteType(spriteManager->getSpriteType(wItemType));
			itemSprite->setAlpha(255);
			itemSprite->setCurrentState(L"IDLE");
			itemSprite->setRotationInRadians(0);
			WorldItem *item = new WorldItem(itemSprite, flag, collectible);

			// CREATE WORLD ITEM ZONE AS BOX2D SENSOR BOX
			bodyDef.type = b2_kinematicBody;
			bodyDef.position.Set(x, y);
			bodyDef.fixedRotation = true;
			body = gsm->getB2World()->CreateBody(&bodyDef);
			dynamicBox.SetAsBox(width, height);
			fixtureDef.shape = &dynamicBox;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.0f;
			fixtureDef.isSensor = true;
			body->CreateFixture(&fixtureDef);
			body->SetUserData(new CollidableZone(item, WorldItemFlag));
			itemSprite->setB2Body(body);

			spriteManager->addWorldItem(item);

			worldItem = worldItem->NextSiblingElement();
		}

		TiXmlElement *securityCameras = worldItems->NextSiblingElement();
		TiXmlElement *securityCamera = securityCameras->FirstChildElement();
		while (securityCamera != nullptr)
		{
			// GET CAM ATTRIBUTES
			int time = xmlReader.extractIntAtt(securityCamera, MG_TIME_ATT);
			int x = xmlReader.extractIntAtt(securityCamera, MG_X_ATT);
			int y = xmlReader.extractIntAtt(securityCamera, MG_Y_ATT);
			int drawX = xmlReader.extractIntAtt(securityCamera, MG_DRAW_X_ATT);
			int drawY = xmlReader.extractIntAtt(securityCamera, MG_DRAW_Y_ATT);
			int width = xmlReader.extractIntAtt(securityCamera, MG_WIDTH_ATT);
			int height = xmlReader.extractIntAtt(securityCamera, MG_HEIGHT_ATT);
			int censorshipTarget = xmlReader.extractIntAtt(securityCamera, MG_CENSORSHIP_TARGET_ATT);
			int direction = xmlReader.extractIntAtt(securityCamera, MG_DIRECTION_ATT);

			// CREATE ANIMATED SPRITE FOR CAMERA
			AnimatedSprite *camSprite = new AnimatedSprite();
			camSprite->setSpriteType(spriteManager->getSpriteType(L"SecurityCamera"));
			camSprite->setAlpha(255);
			if (direction > 0)
				camSprite->setCurrentState(L"IDLE_R");
			else
				camSprite->setCurrentState(L"IDLE_L");
			camSprite->setRotationInRadians(0);
			SecurityCamera *cam = new SecurityCamera(time, censorshipTarget, direction, drawX-x, drawY-y, camSprite);

			// CREATE CAMERA DETECTION ZONE AS BOX2D SENSOR BOX
			bodyDef.type = b2_kinematicBody;
			// x,y REPRESENTS CENTER OF DETECTION ZONE
			// CAMERA WILL BE DRAWN IN TOP LEFT OR RIGHT CORNER OF ZONE, DEPENDING ON direction
			bodyDef.position.Set(x,y);
			bodyDef.fixedRotation = true;
			body = gsm->getB2World()->CreateBody(&bodyDef);
			dynamicBox.SetAsBox(width, height);
			fixtureDef.shape = &dynamicBox;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.0f;
			fixtureDef.isSensor = true;
			body->CreateFixture(&fixtureDef);
			body->SetUserData(new CollidableZone(cam, SecurityCameraFlag));
			camSprite->setB2Body(body);

			spriteManager->addSecurityCamera(cam);

			securityCamera = securityCamera->NextSiblingElement();
		}

		TiXmlElement *hiddenWalls = securityCameras->NextSiblingElement();
		TiXmlElement *hiddenWall = hiddenWalls->FirstChildElement();
		while (hiddenWall != nullptr)
		{
			int x = xmlReader.extractIntAtt(hiddenWall, MG_X_ATT);
			int y = xmlReader.extractIntAtt(hiddenWall, MG_Y_ATT);
			int width = xmlReader.extractIntAtt(hiddenWall, MG_WIDTH_ATT);
			int height = xmlReader.extractIntAtt(hiddenWall, MG_HEIGHT_ATT);

			bodyDef.type = b2_staticBody;
			bodyDef.position.Set(x, y);
			body = gsm->getB2World()->CreateBody(&bodyDef);
			dynamicBox.SetAsBox(width, height);
			fixtureDef.shape = &dynamicBox;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.0f;
			fixtureDef.isSensor = false;
			body->CreateFixture(&fixtureDef);

			hiddenWall = hiddenWall->NextSiblingElement();
		}

		// level_bot_types
		TiXmlElement *botTypesList = hiddenWalls->NextSiblingElement();
		TiXmlElement *botType = botTypesList->FirstChildElement();
		while (botType != nullptr)
		{
			// FIRST GET THE BOT TYPE
			const char* botTypeName = xmlReader.extractCharAtt(botType, MG_BOT_TYPE_ATT);
			string strBotTypeName(botTypeName);
			wstring wBotTypeName(strBotTypeName.begin(), strBotTypeName.end());
			text->writeDebugOutput(L"Bot Type: " + wBotTypeName);

			// THEN GET THE SPRITE TYPE
			const char* spriteType = botType->Attribute(MG_SPRITE_TYPE_ATT.c_str());
			string strSpriteType(spriteType);
			wstring wSpriteType(strSpriteType.begin(), strSpriteType.end());
			text->writeDebugOutput(L"Bot's Sprite Type: " + wSpriteType);

			const char* movingBehavior = botType->Attribute(MG_MOVING_BEHAVIOR.c_str());
			string strMovingBehavior(movingBehavior);
			wstring wStrMovingBehavior(strMovingBehavior.begin(), strMovingBehavior.end());
			text->writeDebugOutput(L"bot moving behavior: " + wStrMovingBehavior);

			const char* dyingBehavior = botType->Attribute(MG_DYING_BEHAVIOR.c_str());
			string strDyingBehavior(dyingBehavior);
			wstring wStrDyingBehavior(strDyingBehavior.begin(), strDyingBehavior.end());
			text->writeDebugOutput(L"bot dying behavior: " + wStrDyingBehavior);

			const char* spawningBehavior = botType->Attribute(MG_SPAWNING_BEHAVIOR.c_str());
			string strSpawningBehavior(spawningBehavior);
			wstring wStrSpawningBehavior(strSpawningBehavior.begin(), strSpawningBehavior.end());
			text->writeDebugOutput(L"bot spawning behavior: " + wStrSpawningBehavior);

			// MAKE THE FIRST BOT FOR THIS TYPE
			Bot *firstBot = new Bot();
			AnimatedSpriteType *botSpriteType = spriteManager->getSpriteType(wSpriteType);
			firstBot->setSpriteType(botSpriteType);
			firstBot->setAlpha(255);
			firstBot->setBotState(DEAD);
			firstBot->setType(wBotTypeName);
			
			// WHICH MOVEMENT BEHAVIOR?
			if (strMovingBehavior.compare(MG_MOVING_DETERMINISTIC_BEHAVIOR) == 0)
			{
				firstBot->setBehavior(firstBot->getBotStateForString(MG_MOVING_BEHAVIOR), new DeterministicMovement());
			}
			else if (strMovingBehavior.compare(MG_MOVING_RANDOM_INTERVAL_BEHAVIOR) == 0)
			{
				firstBot->setBehavior(firstBot->getBotStateForString(MG_MOVING_BEHAVIOR), new RandomIntervalMovement());
			}

			// AND WHICH DEATH BEHAVIOR?
			if (strDyingBehavior.compare(MG_DYING_RECYCLE_BEHAVIOR) == 0)
			{
				firstBot->setBehavior(firstBot->getBotStateForString(MG_DYING_BEHAVIOR), new RecycledDeath());
			}
			else if (strDyingBehavior.compare(MG_DYING_RESPAWN_BEHAVIOR) == 0)
			{
				firstBot->setBehavior(firstBot->getBotStateForString(MG_DYING_BEHAVIOR), new RespawnedDeath());
			}

			// AND WHICH SPAWN BEHAVIOR?
			if (strSpawningBehavior.compare(MG_SPAWNING_RANDOM_BEHAVIOR) == 0)
			{
				int maxVelocity = xmlReader.extractIntAtt(botType, MG_MAX_VELOCITY);
				RandomSpawn *randomSpawn = new RandomSpawn();
				randomSpawn->setVelocity(maxVelocity);
				firstBot->setBehavior(firstBot->getBotStateForString(MG_SPAWNING_BEHAVIOR), randomSpawn);
			}
			// TELL THE BOT RECYCLER ABOUT THIS BOT TYPE
			botRecycler->registerBotType(wBotTypeName, firstBot);

			// ON TO THE NEXT BOT TYPE
			botType = botType->NextSiblingElement();
		}

		// NOW FOR INITIAL SPRITES
		TiXmlElement *botsList = botTypesList->NextSiblingElement();
		TiXmlElement *bot = botsList->FirstChildElement();
		while (bot != nullptr)
		{
			int initX = xmlReader.extractIntAtt(bot, MG_INIT_X_ATT);
			int initY = xmlReader.extractIntAtt(bot, MG_INIT_Y_ATT);
			int initVx = xmlReader.extractIntAtt(bot, MG_INIT_VX_ATT);
			int initVy = xmlReader.extractIntAtt(bot, MG_INIT_VY_ATT);
			int wp1X = xmlReader.extractIntAtt(bot, MG_WP_1_X_ATT);
			int wp1Y = xmlReader.extractIntAtt(bot, MG_WP_1_Y_ATT);
			int wp2X = xmlReader.extractIntAtt(bot, MG_WP_2_X_ATT);
			int wp2Y = xmlReader.extractIntAtt(bot, MG_WP_2_Y_ATT);
			wstring debugText = L"Bot x, y, vX, vY: ";
			wstringstream wss;
			wss << initX; wss << L", "; wss << initY; wss << initVx; wss << L","; wss << initVy;
			debugText += wss.str();
			text->writeDebugOutput(debugText);

			const char* initialBotState = xmlReader.extractCharAtt(bot, MG_INIT_BOT_STATE_ATT);
			string strInitialBotState(initialBotState);
			wstring wInitialBotState(strInitialBotState.begin(), strInitialBotState.end());
			text->writeDebugOutput(L"init bot state: " + wInitialBotState);

			const char* initSpriteState = xmlReader.extractCharAtt(bot, MG_INIT_SPRITE_STATE_ATT);
			string strInitialSpriteState(initSpriteState);
			wstring wInitialSpriteState(strInitialSpriteState.begin(), strInitialSpriteState.end());
			text->writeDebugOutput(L"init sprite state: " + wInitialSpriteState);

			// NOW GET THE BOT TYPE
			const char* botTypeName = xmlReader.extractCharAtt(bot, MG_BOT_TYPE_ATT);
			string strBotTypeName(botTypeName);
			wstring wBotTypeName(strBotTypeName.begin(), strBotTypeName.end());
			text->writeDebugOutput(L"Bot Type: " + wBotTypeName);

			// GET A BOT
			Bot* botToSpawn = botRecycler->retrieveBot(wBotTypeName);

			// INIT THE BOT WITH THE DATA WE'VE PULLED OUT
			BotState botState = botToSpawn->getBotStateForString(initialBotState);
			botToSpawn->setBotState(botState);
			botToSpawn->setCurrentState(wInitialSpriteState);
			botToSpawn->setRotationInRadians(0.0f);
			b2BodyDef bodyDef;
			bodyDef.type = b2_dynamicBody;
			bodyDef.position.Set(initX, initY);
			bodyDef.fixedRotation = true;
			b2Body* body = gsm->getB2World()->CreateBody(&bodyDef);
			b2PolygonShape dynamicBox;
			dynamicBox.SetAsBox(2.0f, 2.0f);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicBox;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.3f;
			fixtureDef.isSensor = true;
			body->CreateFixture(&fixtureDef);
			botToSpawn->setB2Body(body);
			body->SetLinearVelocity(b2Vec2(initVx, initVy));
			vector<float> *wayPoint = new vector<float>(0);
			//to be loaded from xml
			wayPoint->push_back(wp1X);
			wayPoint->push_back(wp1Y);
			botToSpawn->addBotWayPoint(*wayPoint);
			wayPoint = new vector<float>(0);
			//to be loaded from xml
			wayPoint->push_back(wp2X);
			wayPoint->push_back(wp2Y);
			botToSpawn->addBotWayPoint(*wayPoint);

			// AND GIVE IT TO THE SPRITE MANAGER
			spriteManager->addBot(botToSpawn);

			// ONTO THE NEXT BOT
			bot = bot->NextSiblingElement();
		}

		// spawning_pools
		TiXmlElement *spawningPoolsList = botsList->NextSiblingElement();
		TiXmlElement *spawningPool = spawningPoolsList->FirstChildElement();
		while (spawningPool != nullptr)
		{
			int initX = xmlReader.extractIntAtt(spawningPool, MG_X_ATT);
			int initY = xmlReader.extractIntAtt(spawningPool, MG_Y_ATT);

			// GET THE INTERVAL TYPE
			const char* intervalTypeChar = xmlReader.extractCharAtt(spawningPool, MG_INTERVAL_TYPE_ATT);
			string strIntervalType(intervalTypeChar);

			// NOW GET THE BOT TYPE
			const char* botTypeName = xmlReader.extractCharAtt(spawningPool, MG_BOT_TYPE_ATT);
			string strBotTypeName(botTypeName);
			wstring wBotTypeName(strBotTypeName.begin(), strBotTypeName.end());
			text->writeDebugOutput(L"Bot Type: " + wBotTypeName);

			// AND THE MAX INTERVAL
			int maxInterval = xmlReader.extractIntAtt(spawningPool, MG_INTERVAL_TIME_ATT);
			BotSpawningPool *pool = new BotSpawningPool(strIntervalType, wBotTypeName, initX, initY, maxInterval);

			// GIVE THE SPAWNING POOL TO THE SPRITE MANAGER
			spriteManager->addSpawningPool(pool);

			// ONTO THE NEXT SPAWNING POOL
			spawningPool = spawningPool->NextSiblingElement();
		}
	}
	return true;
}


bool MGLevelImporter::unload()
{
	Game *game = Game::getSingleton();
	GameStateManager *gsm = game->getGSM();
	gsm->unloadCurrentLevel();
	GameGraphics *graphics = game->getGraphics();
	graphics->clearWorldTextures();
	return true;
}