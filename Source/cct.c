// include libs:
#include <acknex.h>

// include physX:
#include "..\\Source\\physX3\\ackphysX3.h"
// include cct template:
#include "..\\Source\\cct.h"

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
	bbox.standingSize = size.z;
	bbox.crouchingSize = size.z * 0.6;
	// radius and half-size of the cct:
   var halStandfHeight = bbox.standingSize * 0.5;
   var halCrouchfHeight = bbox.crouchingSize * 0.5;
   var radius = size.x;
	// setup scale vectors:
   vec_set(standScale.x, vector(radius / bbox.max_x, radius / bbox.max_x, halStandfHeight / bbox.max_z));
   vec_set(crouchScale.x, vector(radius / bbox.max_x, radius / bbox.max_x, halCrouchfHeight / bbox.max_z));
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

void cct_set_input(Actor* actor, int state, int value){
	
}

void cct_rotate(Actor* actor, var rotation){
	
}

int cct_get_input(Actor* actor, int state){
	
}

void cct_get_position(Actor* actor, VECTOR* pos){
	
}

var cct_get_rotation(Actor* actor){
	
}

void cct_update(Actor* actor){
	
}