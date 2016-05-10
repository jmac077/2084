#include "SoundManager.h"

SoundManager::SoundManager()
{
	currentSong = "";
	currentSoundEffect = "";
}

SoundManager::~SoundManager()
{
	
}

void SoundManager::LoadFiles()
{
	// LOAD ALL MUSIC AND SOUND EFFECT FILES
	int error;
	string openCommand = "open data\\music\\censored.mp3 type mpegvideo alias " + CENSORED_SONG;
	error = mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "open data\\music\\detected.mp3 type mpegvideo alias " + DETECTED_SONG;
	error = mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "open data\\music\\freedomtower.mp3 type mpegvideo alias " + FREEDOM_TOWER_SONG;
	error = mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "open data\\music\\tabletops.mp3 type mpegvideo alias " + TABLETOPS_SONG;
	error = mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "open data\\music\\thedeep.mp3 type mpegvideo alias " + THE_DEEP_SONG;
	error = mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "open data\\music\\wanderer.mp3 type mpegvideo alias " + WANDERER_SONG;
	error = mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "open data\\sfx\\walking.mp3 type mpegvideo alias " + WALKING_SFX;
	error = mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "open data\\sfx\\teleport.mp3 type mpegvideo alias " + TELEPORT_SFX;
	error = mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "open data\\sfx\\pickupitem.mp3 type mpegvideo alias " + PICK_UP_ITEM_SFX;
	error = mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
}

void SoundManager::CloseFiles()
{
	// CLOSE ALL MUSIC AND SOUND EFFECT FILES
	string openCommand = "close " + CENSORED_SONG;
	mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "close " + DETECTED_SONG;
	mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "close " + FREEDOM_TOWER_SONG;
	mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "close" + TABLETOPS_SONG;
	mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "close " + THE_DEEP_SONG;
	mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "close " + WANDERER_SONG;
	mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "close " + WALKING_SFX;
	mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "close " + TELEPORT_SFX;
	mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
	openCommand = "close " + PICK_UP_ITEM_SFX;
	mciSendString(wstring(openCommand.begin(), openCommand.end()).c_str(), NULL, 0, NULL);
}

void SoundManager::PlaySoundEffect(const string soundEffect)
{
	int error;
	string command;
	command = "play " + soundEffect + " from 0";
	error = mciSendString(wstring(command.begin(), command.end()).c_str(), NULL, 0, NULL);
}

void SoundManager::PlaySoundEffectLoop(const string soundEffect)
{
	int error;
	string command;
	if (currentSoundEffect == soundEffect)
		return;
	// STOP CURRENT SONG
	if (currentSoundEffect != "") {
		command = "stop " + currentSoundEffect;
		error = mciSendString(wstring(command.begin(), command.end()).c_str(), NULL, 0, NULL);
	}
	// PLAY NEW SOUND EFFECT ON REPEAT
	currentSoundEffect = soundEffect;
	command = "play " + soundEffect + " from 0 repeat";
	error = mciSendString(wstring(command.begin(), command.end()).c_str(), NULL, 0, NULL);
}

void SoundManager::StopSoundEffect()
{
	int error;
	string command;
	command = "stop " + currentSoundEffect;
	error = mciSendString(wstring(command.begin(), command.end()).c_str(), NULL, 0, NULL);
	currentSoundEffect = "";
}

void SoundManager::PlaySong(const string song)
{
	int error;
	string command;
	if (currentSong == song)
		return;
	// STOP CURRENT SONG
	if (currentSong != "") {
		command = "stop " + currentSong;
		error = mciSendString(wstring(command.begin(), command.end()).c_str(), NULL, 0, NULL);
	}
	// PLAY NEW SONG ON REPEAT
	currentSong = song;
	command = "play " + song + " from 0 repeat";
	error = mciSendString(wstring(command.begin(), command.end()).c_str(), NULL, 0, NULL);
}