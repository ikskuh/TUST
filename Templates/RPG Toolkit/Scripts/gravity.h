// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 270810
//
// title: Code to handle gravity
// class: Gravity
// type: Gravity
// help: contains code handling all the gravity for entity's
// 
// prefix: GRAV_
// idcode: 11
// 
// ENDHEADER
// ---------------------------------------------------------------------

typedef struct {
	ENTITY *gravityEnt;
	VECTOR GRAV_vFeet;
	
	var GRAV_fallSpeed;
	var GRAV_speedDown;
	var GRAV_distDown;
} GRAVITY;

var GRAV_gravity = 15;

// includes
#include "gravity.c"

GRAVITY* init_gravity(ENTITY* entGravity) {
	// initialise memory for new item
	GRAVITY* myGravity = sys_nxalloc( sizeof(GRAVITY) );
	
	myGravity->gravityEnt = entGravity;
	vec_for_min(myGravity->GRAV_vFeet, myGravity->gravityEnt);
	
	// set gravity speed
	if(myGravity->gravityEnt.skill2 != 0) {
		myGravity->GRAV_fallSpeed = myGravity->gravityEnt.skill2;
	} else { // else set default gravity speed
		myGravity->GRAV_fallSpeed = GRAV_gravity;
	}
	
	myGravity->GRAV_speedDown = 0;
	myGravity->GRAV_distDown = 0;
	
	return(myGravity);
}