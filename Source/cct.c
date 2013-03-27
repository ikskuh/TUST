// include libs:
#include <acknex.h>

// global ressources:
#define PRAGMA_PATH "..\\..\\Ressources\\Models\\Characters"

// include physX:
#include "physX3\\ackphysX3.h"
// include cct template:
#include "cct.h"

// this are the models, used for the cct template:
STRING* cctActor_mdl = "capsule.mdl";

// Set up an actor:
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

// Removes all actor entitys and quits the actors update loop:
void cct_remove(Actor *actor){
	
}

// Actor control:
void cct_set_input(Actor* actor, int state, int value){
	
}

// Rotates the actor about a relative angle and limits the rotation speed to Actor::maxRotSpeed:
void cct_rotate(Actor* actor, var rotation){
	
}

// Returns the value of an actors input state:
int cct_get_input(Actor* actor, int state){
	
}

// Gets the position of an actor into the vector:
void cct_get_position(Actor* actor, VECTOR* pos){
	
}

// Gets the rotation of an actor:
var cct_get_rotation(Actor* actor){
	
}

// Updates the actor and performs all moving, jumping etc:
void cct_update(Actor* actor){
	
}

// Gets the extents of an actor:
VECTOR* cct_getextents(Actor* actor, VECTOR* pos){
	// radius, radius, height:
	pX3ent_getcctextents(actor, pos.x);
	// return vector:
	return(pos.x);
}