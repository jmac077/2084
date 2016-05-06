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
		
		// THE LEVEL NODE HAS 4 CHILD NODES TO LOAD

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

		//init player sprite
		AnimatedSprite* player = new AnimatedSprite();
		player->setSpriteType(spriteManager->getSpriteType(L"Guy"));
		player->setAlpha(255);
		player->setCurrentState(L"WALKING_DOWN");
		player->setRotationInRadians(0);
		spriteManager->setPlayer(player);
		b2BodyDef bodyDef;
		bodyDef.type = b2_dynamicBody;
		bodyDef.position.Set(57.0f,51.0f);
		bodyDef.fixedRotation = true;
		b2Body* body = gsm->getB2World()->CreateBody(&bodyDef);
		b2PolygonShape dynamicBox;
		dynamicBox.SetAsBox(.8125f,2.5625f);
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		body->CreateFixture(&fixtureDef);
		player->setB2Body(body);
		body->SetUserData(player);
		player->setCollisionBehavior(player->getTeleportPlayer());

		//TV
		AnimatedSprite* tv = new AnimatedSprite();
		tv->setSpriteType(spriteManager->getSpriteType(L"TV"));
		tv->setAlpha(255);
		tv->setCurrentState(L"IDLE");
		tv->setRotationInRadians(0);
		spriteManager->setTv(tv);
		bodyDef.type = b2_kinematicBody;
		bodyDef.position.Set(58.0f, 38.0f);
		bodyDef.fixedRotation = true;
		body = gsm->getB2World()->CreateBody(&bodyDef);
		dynamicBox.SetAsBox(4.0f, 4.0f);
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		body->CreateFixture(&fixtureDef);
		tv->setB2Body(body);
		
		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(58.0f,5.0f);
		body = gsm->getB2World()->CreateBody(&bodyDef);
		dynamicBox.SetAsBox(6.0f, 1.0f);
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.isSensor = true;
		body->CreateFixture(&fixtureDef);
		body->SetUserData((void*)1);

		bodyDef.type = b2_staticBody;
		bodyDef.position.Set(82.0f, 193.0f);
		body = gsm->getB2World()->CreateBody(&bodyDef);
		dynamicBox.SetAsBox(8.0f, 1.0f);
		fixtureDef.shape = &dynamicBox;
		fixtureDef.density = 1.0f;
		fixtureDef.friction = 0.0f;
		fixtureDef.isSensor = true;
		body->CreateFixture(&fixtureDef);
		body->SetUserData((void*)2);

		// level_bot_types
		TiXmlElement *botTypesList = levelSpriteTypes->NextSiblingElement();
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
			dynamicBox.SetAsBox(.2f, .2f);
			b2FixtureDef fixtureDef;
			fixtureDef.shape = &dynamicBox;
			fixtureDef.density = 1.0f;
			fixtureDef.friction = 0.3f;
			body->CreateFixture(&fixtureDef);
			botToSpawn->setB2Body(body);
			//body->SetUserData(botToSpawn);
			body->SetLinearVelocity(b2Vec2(initVx, initVy));


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