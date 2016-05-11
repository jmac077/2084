#pragma once
#include "mg_VS\stdafx.h"

enum CollidableZoneType
{
	TeleporterFlag,
	CheckpointFlag,
	WorldItemFlag,
	SecurityCameraFlag,
	HiddenWallFlag
};

class CollidableZone
{
private:
	// THE TYPE OF ZONE THIS IS
	CollidableZoneType type;
	// POINTER TO THE ACTUAL OBJECT
	void *zone;
public:
	CollidableZone(void *obj, CollidableZoneType t)
	{
		zone = obj;
		type = t;
	}
	CollidableZoneType getType() { return type; }
	void *getZone() { return zone; }
};