#ifndef _SYSTEM_H_
#define _SYSTEM_H_

/**
 * \file system.h
 * \Function and variables for the game system
 *
 * Provides basic functionality for resolution, sounds, game settings, ...
 */

var vGameDifficulty			= 30;
var vGameBlood					= 1;
var vGameShowDialogs			= 1;
var vGameShowHints			= 1;

var vGameResolution			= 7;
var vGameAntiAlias			= 0;
var vGameBrightness			= 80;
var vGameShaders				= 0;
var vGameShadows				= 0;
var vGameDetails				= 2;

var vGameMusicVolume			= 50;
var vGameSpeechVolume		= 50;
var vGameEffectsVolume		= 50;

var vGameMouseSensitivity	= 3;

#include "system.c"

#endif