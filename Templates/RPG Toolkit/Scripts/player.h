// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 060610
//
// title: Player Movement code
// class: Movement
// type: Player
// help: Handles the movement of the player
//
// prefix: MOVE_
// idcode: 3
//
// ENDHEADER
// ---------------------------------------------------------------------

// create player struct
typedef struct {
	// player model
	ENTITY *Character;
	
	ANIMATION *anim;
	
	// make player an object to be able to look at
	LOOKAT *MOVE_lookAtEntity;
	
	GRAVITY *MOVE_Gravity;
	
	// make player vulnerable to elipsoid collision, but not push other entity's himself
	LINKED_COLLISION *MOVE_collider;
	
	// variable that holds gameplay mode (cutscene, dialogue, movement etc etc)
	int MOVE_gameplayMode;
	
	// add rest of player needed stuff here
	int MOVE_pressedKey;
	int MOVE_curHeading;
	VECTOR MOVE_playerRot;
} PLAYER;

// variables
PLAYER *MOVE_globalPlayer; // handle to the global player entity

// all function prototypes
void getKeyMapping(PLAYER *tempPlayer);
void getInput(PLAYER *tempChar);

// includes
#include "player.c"

// new definition of previously created struct
PLAYER *init_myPlayer(ENTITY* who) {
	// initialise memory for new item
	PLAYER *myPlayer = sys_nxalloc( sizeof(PLAYER) );
	
	// setup all variables
	myPlayer->Character = who;
	
	// set gameplay mode to movement at spawn
	myPlayer->MOVE_gameplayMode = DEF_GAMEPLAY_MOVEMENT;
	// add gravity to character
	myPlayer->MOVE_Gravity = init_gravity(myPlayer->Character);
	
	// set player as pushable collider
	myPlayer->MOVE_collider = init_elipsoidCollider(myPlayer->Character);
	
	// movement pressing keys
	myPlayer->MOVE_pressedKey = 0;
	myPlayer->MOVE_curHeading = 0;
	vec_set(myPlayer->MOVE_playerRot, nullvector);
	
	// run it's own unique function
	myPlayer->anim = init_myAnimation(myPlayer->Character, myPlayer->Character); // creates animation data	
	myPlayer->MOVE_lookAtEntity = init_lookAtViewer(myPlayer->Character); // run lookat update
	
	RunmyChar(myPlayer); // create player input and movement function
	
	// set this character to the global player as well
	MOVE_globalPlayer = myPlayer;
	
	// return pointer
	return(myPlayer);
}

// get keys input
void getInput(PLAYER *tempChar) {
	VECTOR vecTemp;
	tempChar->MOVE_pressedKey = 0;
	
	var moveUp = key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[0]));
	var moveDown = key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[1]));
	var moveLeft = key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[2]));
	var moveRight = key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[3]));
	
	if(moveUp || moveDown || moveLeft || moveRight) { // if any movement key is pressed...
		vec_set(vecTemp, vector(moveUp - moveDown, moveLeft - moveRight, 0)); // store movement
		vec_to_angle(vecTemp, vecTemp);
		
		tempChar->MOVE_curHeading = vecTemp.x;
		tempChar->MOVE_pressedKey = 1;
	}
}