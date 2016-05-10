#pragma once
#include "mg_VS\stdafx.h"
#include <mmsystem.h>
#include <mciapi.h>
#pragma comment(lib, "Winmm.lib")

static const string CENSORED_SONG = "censored";
static const string DETECTED_SONG = "detected";
static const string FREEDOM_TOWER_SONG = "freedomtower";
static const string TABLETOPS_SONG = "tabletops";
static const string THE_DEEP_SONG = "thedeep";
static const string WANDERER_SONG = "wanderer";
static const string FORWARD_CLICK_SFX = "forwardclick";
static const string BACKWARD_CLICK_SFX = "backwardclick";
static const string PICK_UP_ITEM_SFX = "pickupitem";
static const string WALKING_SFX = "walking";
static const string TELEPORT_SFX = "teleport";

class SoundManager
{
private:
	string currentSong;
	string currentSoundEffect;
public:
	SoundManager();
	~SoundManager();
	void PlaySoundEffect(const string soundEffect);
	void PlaySoundEffectLoop(const string soundEffect);
	void StopSoundEffect();
	void PlaySong(const string song);
	void LoadFiles();
	void CloseFiles();
};