// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 210610
//
// title: NPC code for controlling characters to interact with (Non Player Controlled)
// class: NPC's
// type: NPC
// help: Contains all code for NPC handling
// 
// prefix: NPC_
// idcode: 6
// 
// ENDHEADER
// ---------------------------------------------------------------------

// create npc struct
typedef struct {
	// player model
	ENTITY *Character;
	
	ANIMATION *anim;
	// make NPC an object to be able to look at
	LOOKAT *NPC_lookAtEntity;
	// add gravity to the NPC
	GRAVITY *NPC_Gravity;
	// add a dialogue to this NPC
	DS *NPC_Dialogue;
	// add an elipsoid collider that pushes others away
	COLLISION *NPC_collision;
	
	// add dialog here...
} NPC;

// includes
#include "npc.c"

// new definition of previously created struct
NPC *init_myNPC() {
	// initialise memory for new item
	NPC *myNPC = sys_nxalloc( sizeof(NPC) );
	
	// setup all variables
	myNPC->Character = my;
	
	// add gravity to character
	myNPC->NPC_Gravity = init_gravity(myNPC->Character);
	
	// run it's own unique function
	myNPC->anim = init_myAnimation(myNPC->Character, myNPC->Character); // creates animation data
	myNPC->NPC_lookAtEntity = init_lookAtViewer(myNPC->Character); // run lookat update
	myNPC->NPC_Dialogue = DS_getNPCData(myNPC->Character);
	myNPC->NPC_collision = init_elipsoidCollision(myNPC->Character, myNPC->Character.skill3); // set collision with skinwidth of character's skill3 (set in WED)
	
	// return pointer
	return(myNPC);
}