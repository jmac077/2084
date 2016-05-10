#pragma once
#include "bugs_VS\stdafx.h"

/*
BugsApp.h

This file contains setup data for making the Bugs game. Note that nearly
all of this data would better serve us if loaded from files.
*/

// HERE ARE THE GAME PROPERTIES WE ARE LOADING FROM FROM THE SETUP FILE
static const int		VIEWPORT_OFFSET_X = 0;
static const int		VIEWPORT_OFFSET_Y = 100;
static const int		TOGGLE_OFFSET_Y = 160;

// CONSTANTS FOR ARTWORK - NOTE, THIS SHOULD REALLY BE LOADED FROM A FILE
static const wstring	BLACK_ANT_CURSOR_PATH = L"data/gui/cursors/black_ant_cursor.png";
static const wstring	RED_ANT_CURSOR_PATH = L"data/gui/cursors/red_ant_cursor.png";
static const wstring	SPLASH_SCREEN_PATH = L"data/gui/overlays/bugs_splash_screen_overlay.png";
static const wstring	ABOUT_SCREEN_PATH = L"data/gui/overlays/bugs_about_screen_overlay.png";
static const wstring	HELP_SCREEN_PATH = L"data/gui/overlays/bugs_help_screen_overlay.png";
static const wstring	MAIN_MENU_PATH = L"data/gui/overlays/bugs_main_menu_overlay.png";
static const wstring	IN_GAME_TOOLBAR_PATH = L"data/gui/overlays/bugs_in_game_toolbar2.png";
static const wstring	LOADING_LEVEL_PATH = L"data/gui/overlays/bugs_loading_level_overlay.png";
static const wstring	START_IMAGE_PATH = L"data/gui/buttons/start_game.png";
static const wstring	START_IMAGE_MO_PATH = L"data/gui/buttons/start_game_mo.png";
static const wstring	EXIT_IMAGE_PATH = L"data/gui/buttons/exit_game.png";
static const wstring	EXIT_IMAGE_MO_PATH = L"data/gui/buttons/exit_game_mo.png";
static const wstring	QUIT_IMAGE_PATH = L"data/gui/buttons/quit.png";
static const wstring	QUIT_IMAGE_MO_PATH = L"data/gui/buttons/quit_mo.png";
static const wstring	LOAD_IMAGE_PATH = L"data/gui/buttons/load_game.png";
static const wstring	LOAD_IMAGE_MO_PATH = L"data/gui/buttons/load_game_mo.png";
static const wstring	HELP_IMAGE_PATH = L"data/gui/buttons/help_game.png";
static const wstring	HELP_IMAGE_MO_PATH = L"data/gui/buttons/help_game_mo.png";
static const wstring	ABOUT_IMAGE_PATH = L"data/gui/buttons/about_game.png";
static const wstring	ABOUT_IMAGE_MO_PATH = L"data/gui/buttons/about_game_mo.png";

// FOR OUR SPRITE
static const wstring	SPRITE_TYPES_DIR = L"data/sprite_types/";
static const wstring	SPRITE_TYPES_LIST = SPRITE_TYPES_DIR + L"sprite_type_list.xml";

// FOR THE GAME WORLD
static const float		MAX_VIEWPORT_AXIS_VELOCITY = 40.0f;
static const int		MIN_FPS = 5;
static const int		MAX_FPS = 100;
static const int		FPS_INC = 1;
static const wstring	IDLE(L"IDLE");