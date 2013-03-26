// include libs:
#include <acknex.h>

// global ressources:
#define PRAGMA_PATH "..\\..\\Ressources\\Models\\Characters"

// include physX:
#include "..\\Source\\physX3\\ackphysX3.h"
// include cct template:
#include "..\\Source\\cct.h"

// this are the models, used for the cct template:
STRING* cctActor_mdl = "capsule.mdl";

/**
* Set up an actor.
* \param	parent	The pointer to the creator entity, which is placed in WED.
* \param	size		The size of the cct bounding ellipsoid (max_x\min_x).
* \param	offset	The cct offset from the creator entity.
*/
Actor* cct_actor(ENTITY* parent, VECTOR* size, VECTOR* offset){
	// vector:
	VECTOR vecTemp;
	VECTOR standScale;
	VECTOR crouchScale;
	// reset vector:
	vec_fill(vecTemp.x, 0);
	vec_fill(standScale.x, 0);
	vec_fill(crouchScale.x, 0);
	// struct pointer:
	Actor* actor = (Actor*)sys_malloc(sizeof(Actor));
	// positions for cct:
	vec_set(vecTemp.x, offset.x);
	vec_rotate(vecTemp.x, parent.pan);
	vec_add(vecTemp.x, parent.x);
	// bbox pointer (cct):
	ENTITY* bbox = ent_create(cctActor_mdl, vecTemp.x, NULL);
	// set parent entity:
	bbox.parent = parent;
	// flags:
	set(bbox, TRANSLUCENT);
	// standing and crouching size's of cct:
	actor.standingSize = size.z;
	actor.crouchingSize = size.z * 0.6;
	// radius and half-size of the cct:
	actor.halfStandingSize = actor.standingSize * 0.5;
	actor.halfCrouchingSize = actor.crouchingSize * 0.5;
	actor.cctRadius = size.x;
	// setup scale vectors:
	vec_set(standScale.x, vector(actor.cctRadius / bbox.max_x, actor.cctRadius / bbox.max_x, actor.halfStandingSize / bbox.max_z));
	vec_set(crouchScale.x, vector(actor.cctRadius / bbox.max_x, actor.cctRadius / bbox.max_x, actor.halfCrouchingSize / bbox.max_z));
	// scale the bbox:
	vec_scale(bbox.scale_x, standScale.x);
	// wait one frame:
	wait(1);
	// set both collusion flags:
	bbox.eflags |= FAT | NARROW;
	// set bounding box to individual values:
	vec_set(bbox.min_x, vector(-size.x, -size.y, -size.z * 0.5)); 
	vec_set(bbox.max_x, vector(size.x, size.y, size.z * 0.5));
	// register the cct:
	pX3ent_settype(bbox, PH_CHAR, PH_CAPSULE);
	// redirect pointer:
	actor.bbox = bbox;
	// return pointer:
	return(actor);
}

/**
* Actor control.
* \param	actor		The pointer to the cct actor.
* \param	state		Current movement state (f.e. CCT_WALK).
* \param	value		
*/
void cct_set_input(Actor* actor, int state, int value){
	
}

/**
* Rotates the actor about a relative angle and limits the rotation speed to Actor::maxRotSpeed.
* \param	actor		The pointer to the cct actor.
* \param	rotation	Rotation input.
*/
void cct_rotate(Actor* actor, var rotation){
	
}

/**
* Returns the value of an actors input state.
* \param	actor		The pointer to the cct actor.
* \param	state		Current movement state (f.e. CCT_WALK).
*/
int cct_get_input(Actor* actor, int state){
	
}

/**
* Gets the position of an actor into the vector.
* \param	actor		The pointer to the cct actor.
* \param	pos		The cct position.
*/
void cct_get_position(Actor* actor, VECTOR* pos){
	
}

/**
* Gets the rotation of an actor.
* \param	actor		The pointer to the cct actor.
*/
var cct_get_rotation(Actor* actor){
	
}

/**
* Updates the actor and performs all moving, jumping etc.
* \param	actor		The pointer to the cct actor.
*/
void cct_update(Actor* actor){
	
}