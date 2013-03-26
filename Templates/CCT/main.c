// include libs:
#include <acknex.h>
#include <default.c>

// include physX:
#include "ackphysX3.h" 

STRING* visualActor_mdl = "soldier.mdl";
STRING* cctActor_mdl = "capsule.mdl";

typedef struct Actor {
	var speed;
	var runningFactor;	// How much faster do we run?
	var strafeFactor;		// How much faster do we strafe?
	var crawlFactor;		// How much faster do we crawl?
	var jumpStrength;		// How high do we jump?
	var maxRotSpeed;		// Max. character rotation speed.
	
	int flags;				// Explained below
	
	// internal use:
	ENTITY* bbox;			// The physic bbox
	VECTOR offset;			// Offset of the chars origin
	VECTOR size;			// Size of the model
} Actor;

#define standingSize 					skill20
#define crouchingSize 					skill21
#define controllerRadius 				skill22
#define doStandup 						skill23

#define CCT_WALK							1			// Walk forward (>0), stand (=0), walk backwards (<0)
#define CCT_STRAFE						2			// Strafe left (>0), stand (=0), strafe right (<0)
#define CCT_RUN							3			// Actor runs (!=0), actor walks (=0)
#define CCT_CRAWL							4			//	Actor crawls (!=0), actor stands (=0)
#define CCT_JUMP							5			// Actor starts jumping if (!=0)

#define ACTOR_CRAWL_RUN					FLAG1
#define ACTOR_NO_JUMP					FLAG2
#define ACTOR_ALLOW_REJUMP				FLAG3
#define ACTOR_ALLOW_MULTIJUMP			FLAG4
#define ACTOR_NO_AIRCONTROL			FLAG5

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

// player's action:
action actHero(){
	set(my, TRANSLUCENT | PASSABLE);
	// create an actior:
	Actor* actor = cct_actor(me, vector(20, 20, 80), vector(0, 0, 0));
	// loop:
	while(1){
		// get input:
		cct_set_input(actor, CCT_WALK, key_w - key_s);
		cct_set_input(actor, CCT_STRAFE, key_a - key_d);
		cct_set_input(actor, CCT_RUN, key_shift);
		cct_set_input(actor, CCT_CRAWL, key_ctrl);
		cct_set_input(actor, CCT_JUMP, key_space);
		// update physics:
		cct_update(actor);
		// get the proper positions:
		cct_get_position(actor, my.x);
		// rotate character:
		cct_rotate(actor, 4 * mouse_force.y * time_step);
		// rotate model:
		my.pan = cct_get_rotation(actor);
		// wait one frame:
		wait(1);
	}
}

// define test level's name:
STRING* levelStr = "map01.wmb";

// main game function:
function main(){
	// disable v-sync:
	d3d_triplebuffer = 1;
	// turn doppler effect OFF:
	doppler_factor = 0;	
	// preload in buffer all created models:
	preload_mode = 3;
	// show all errors:
	warn_level = 6;
	// limit FPS:
	fps_max = 60;
	// init physX:
	physX3_open();
	// load the level:
	freeze_mode = 1;
	level_load(levelStr);
	wait(3);
	freeze_mode = 0;
}