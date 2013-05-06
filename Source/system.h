#ifndef _SYSTEM_H_
#define _SYSTEM_H_

/**
 * \file system.h
 * \Function and variables for the game system
 *
 * Provides basic functionality for resolution, sounds, game settings, ...
 */

// Option variables 
var vInitialShaderVersion	= d3d_shaderversion;
var vGameDifficulty			= 1; // 1: Easy, 2: Medium, 3: Hard
var vGameBlood					= 1;
var vGameShowDialogs			= 1;
var vGameShowHints			= 1;
var vGameResolution			= 7;
var vGameDetails				= 2; // 1: Low, 2: Medium, 3: High
var vGameMusicVolume			= 50;
var vGameSpeechVolume		= 50;
var vGameEffectsVolume		= 50;
var vGameMouseSensitivity	= 3;

// Curret key status
int nUp, nDown, nLeft, nRight, nRun, nCrouch, nInteract, nChangeWeaponUp, nChangeWeaponDown, nCharacterMenu, nQuests, nMap, nFire1, nFire2, nJump;
int nESC, n1, n2, n3, n4; // Cannot be changed

// Key code
int nUpKey, nDownKey, nLeftKey, nRightKey, nRunKey, nCrouchKey, nInteractKey, nChangeWeaponUpKey, nChangeWeaponDownKey, nCharacterMenuKey, nQuestsKey, nMapKey, nFire1Key, nFire2Key, nJumpKey;
int nESCKey, n1Key, n2Key, n3Key, n4Key;

int nInGame						= 1; // Is the game running or are we in the start menu?


/**
 * Initializes all keys
 */
void sys_keys_init();
 
/**
 * Game system closed
 */
void sys_close();

/**
 * Check each frame if keys are pressed
 */
void sys_keys_check();


#include "system.c"

#endif