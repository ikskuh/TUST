// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 160510
//
// title: Main Code
// class: Main
// type: Main
// help: Handles the whole Toolkit Template (all includes etc go from here)
//
// prefix: RPG_
// idcode: 1
//
// ENDHEADER
// ---------------------------------------------------------------------

#include <acknex.h> // include for Pure Mode
#include <default.c> // default key functions

// Paths...
#define PRAGMA_PATH "./Levels";
#define PRAGMA_PATH "./Models";
#define PRAGMA_PATH "./Scripts";

// include all scripts here; safeguarded most to prevent multiple of the same systems running
// Basic variables setup and defintions
#include "defines.h"
/*
// Level changing code
#include "level.h"
*/
// key Setup
#include "input.h"

// Inventory
#include "inventory.h"

// include the code for Animating all characters in the world (blended)
#include "animation.h"

// Dialogue system
#include "dialogue.h"

// Gravity
#include "gravity.h"

// Collision
#include "collision.h"  

// Object's / characters to look at
#include "lookat.h"

// include the player Movement (basic camera oriented movement)
#include "player.h"

// NPC's
#include "npc.h"

// Camera
#include "camera.h"

// Triggers
#include "trigger.h"

// Music - Maybe through custom events though?!

// Battle

/*
// include the Menu (testing menu, user will need to update this himself to fit game's purpose)
#include <menu.c>
*/
// Actions (that can be applied via WED)
#include "actions.c"

// startup function
void main() {
	fps_max = 60;
	fps_min = 30;

	// set resolution of game (4:3 size, 1024x768)
	video_switch(8, 0, 8);
	random_seed(0); // always get a complete random
	
	// Engine variables settings
	// set anti aliasing
	shadow_stencil = 0;
	// set shadow type
	d3d_antialias = 9;
	
	// make the following a nice solution later on that is to be called off of a menu :)
	
	// get key setup before running the game
	getGlobalKeymapping();
	CAM_LastUsedCam = str_create("");
	
	str_cpy(delimit_str, "#"); // set delimiter to another character then "," so you CAN use comma in your dialogues
	
	// load test level
	level_load("testLevel.wmb");
	
	// inventory testing
//	createInventoryTest();
}