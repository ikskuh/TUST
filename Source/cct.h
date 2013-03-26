#ifndef _CCT_H_
	#define _CCT_H_

	/**
	* \file cct.h
	* fs.h provides helper functions related to the character controller template.
	*/
	
	// The Actor struct:
	typedef struct Actor {
		
		var speed;
		var runningFactor;				// How much faster do we run?
		var strafeFactor;					// How much faster do we strafe?
		var crawlFactor;					// How much faster do we crawl?
		var jumpStrength;					// How high do we jump?
		var maxRotSpeed;					// Max. character rotation speed.
		
		var standingSize;					// Full standing size of the cct
		var halfStandingSize;			// Half standing size of the cct
		var crouchingSize;				// Full crouching size of the cct
		var halfCrouchingSize;			// Half crouching size of the cct
		var cctRadius;						// The radius of the cct
		var doStandup;						// Cct state switch! 0 - if standing; 1 - if crouching;
		
		int flags;							// Explained below
		
		// internal use:
		ENTITY* bbox;						// The physic bbox
		VECTOR offset;						// Offset of the chars origin
		VECTOR size;						// Size of the model
	} Actor;

	#define CCT_WALK						1			// Walk forward (>0), stand (=0), walk backwards (<0)
	#define CCT_STRAFE					2			// Strafe left (>0), stand (=0), strafe right (<0)
	#define CCT_RUN						3			// Actor runs (!=0), actor walks (=0)
	#define CCT_CRAWL						4			//	Actor crawls (!=0), actor stands (=0)
	#define CCT_JUMP						5			// Actor starts jumping if (!=0)

	#define ACTOR_CRAWL_RUN				(1<<0)	// Allows running in crawl mode
	#define ACTOR_NO_JUMP				(1<<1)	// Disables jumping
	#define ACTOR_ALLOW_REJUMP			(1<<2)	// Actor jumps again when landing on the ground
	#define ACTOR_ALLOW_MULTIJUMP		(1<<3)	// Jumping has no ground check
	#define ACTOR_NO_AIRCONTROL		(1<<4)	// Actor can't change speed in air
	
	
	/**
	* Set up an actor.
	* \param	parent	The pointer to the creator entity, which is placed in WED.
	* \param	size		The size of the cct bounding ellipsoid (max_x\min_x).
	* \param	offset	The cct offset from the creator entity.
	*/
	Actor* cct_actor(ENTITY* parent, VECTOR* size, VECTOR* offset);
	
	/**
	* Removes all actor entitys and quits the actors update loop.
	* \param	actor		The pointer to the cct actor.
	*/
	void cct_remove(Actor *actor);
	
	/**
	* Actor control.
	* \param	actor		The pointer to the cct actor.
	* \param	state		Current movement state (f.e. CCT_WALK).
	* \param	value		
	*/
	void cct_set_input(Actor* actor, int state, int value);

	/**
	* Rotates the actor about a relative angle and limits the rotation speed to Actor::maxRotSpeed.
	* \param	actor		The pointer to the cct actor.
	* \param	rotation	Rotation input.
	*/
	void cct_rotate(Actor* actor, var rotation);

	/**
	* Returns the value of an actors input state.
	* \param	actor		The pointer to the cct actor.
	* \param	state		Current movement state (f.e. CCT_WALK).
	*/
	int cct_get_input(Actor* actor, int state);

	/**
	* Gets the position of an actor into the vector.
	* \param	actor		The pointer to the cct actor.
	* \param	pos		The cct position.
	*/
	void cct_get_position(Actor* actor, VECTOR* pos);

	/**
	* Gets the rotation of an actor.
	* \param	actor		The pointer to the cct actor.
	*/
	var cct_get_rotation(Actor* actor);

	/**
	* Updates the actor and performs all moving, jumping etc.
	* \param	actor		The pointer to the cct actor.
	*/
	void cct_update(Actor* actor);
	
	#include "cct.c"
#endif