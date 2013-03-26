#ifndef _CCT_H_
	#define _CCT_H_

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
	
	
	
	
	#include "cct.c"
#endif