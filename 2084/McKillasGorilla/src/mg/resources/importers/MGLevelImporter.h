#pragma once

#include "mg_VS\stdafx.h"
#include "mg\resources\MGImporter.h"
#include "xmlfi\XMLFileImporter.h"

// ELEMNETS
static const string	MG_LEVEL_ELEMENT = "level";
static const string	MG_LEVEL_MAP_ELEMENT = "level_map";
static const string MG_LEVEL_SECTIONS_ELEMENT = "level_sections";
static const string MG_LEVEL_SECTION_ELEMENT = "level_section";
static const string MG_TELEPORTS_ELEMENT = "teleports";
static const string MG_TELEPORT_ELEMENT = "teleport";
static const string MG_CHECKPOINTS_ELEMENT = "checkpoints";
static const string MG_CHECKPOINT_ELEMENT = "checkpoint";
static const string MG_LEVEL_SPRITE_TYPES_ELEMENT = "level_sprite_types";
static const string MG_BOTS_LIST_ELEMENT = "bots_list";
static const string MG_BOT_ELEMENT = "bot";
static const string MG_SPAWNING_POOLS_ELEMENT = "spawning_pools";
static const string MG_SPAWNING_POOL_ELEMENT = "spawning_pool";

// ATTRIBUTES
static const string MG_LEVEL_NAME_ATT = "level_name";
static const string	MG_MAP_DIR_ATT = "map_dir";
static const string MG_MAP_FILE_ATT = "map_file";
static const string MG_SPRITE_TYPES_DIR_ATT = "sprite_types_dir";
static const string MG_SPRITE_TYPES_FILE_ATT = "sprite_types_file";
static const string MG_SPRITE_TYPE_ATT = "sprite_type";
static const string MG_BOT_TYPE_ATT = "type";
static const string MG_INIT_X_ATT = "init_x";
static const string MG_INIT_Y_ATT = "init_y";
static const string MG_INIT_VX_ATT = "init_vx";
static const string MG_INIT_VY_ATT = "init_vy";
static const string MG_INIT_BOT_STATE_ATT = "init_bot_state";
static const string MG_INIT_SPRITE_STATE_ATT = "init_sprite_state";
static const string MG_X_ATT = "x";
static const string MG_Y_ATT = "y";
static const string MG_DEST_X = "dest_x";
static const string MG_DEST_Y = "dest_y";
static const string MG_WIDTH_ATT = "width";
static const string MG_HEIGHT_ATT = "height";
static const string MG_INTERVAL_TIME_ATT = "interval_time";
static const string MG_INTERVAL_TYPE_ATT = "interval_type";
static const string MG_MAX_VELOCITY = "max_velocity";
static const string MG_ID_ATT = "id";
static const string MG_TARGET_SECTION_ATT = "target_section";
static const string MG_CENSORSHIP_ATT = "censorship";
static const string MG_CENSORSHIP_TARGET_ATT = "censorship_target";
static const string MG_FLAG_ATT = "flag";
static const string MG_COLLECTIBLE_ATT = "collectible";
static const string MG_DIRECTION_ATT = "direction";
static const string MG_WP_1_X_ATT = "wp1_x";
static const string MG_WP_1_Y_ATT = "wp1_y";
static const string MG_WP_2_X_ATT = "wp2_x";
static const string MG_WP_2_Y_ATT = "wp2_y";

// VALUES
static const string MG_DETERMINISTIC_VALUE = "deterministic";
static const string MG_RANDOM_INTERVAL_VALUE = "random_interval";
static const string MG_SCHEDULED_VALUE = "scheduled";
static const string MG_WALKING_VALUE = "WALKING";
static const string MG_RECYCLE_VALUE = "recycle";
static const string MG_RESPAWN_VALUE = "respawn";

class MGLevelImporter : public MGImporter
{
private:
	XMLFileImporter xmlReader;

public:
	bool load(wstring levelFileDir, wstring levelFile);
	bool unload();
};