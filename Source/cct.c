// include cct template:
#include "cct.h"

// check for the collisions of the cct (f.e. if we are trying to stand up from crawling):
function cct_check_collision(ENTITY* actor, var cct_max_x, var cct_min_x, var cct_max_z, var cct_min_z)
{
	// vectors:
	VECTOR vStart, vEnd;
	// free vectors:
	vec_fill(vStart.x, 0);
	vec_fill(vEnd.x, 0);
	// get the radius:
	var vRadius = (cct_max_x + cct_min_x) * 0.5;
	// defile start position to check:
	vec_set(vStart, vector(0, 0, cct_max_z));
	vec_add(vStart, actor.x);
	// defile end position to check:
	vec_set(vEnd, vector(0, 0, cct_min_z));		
	vec_add(vEnd, actor.x);
	// check for collusion:
	if(pX3_capsulecheckany(vStart, vEnd, vRadius, 0, 0, NULL, PX_IGNORE_ME))
	{
		// return 1:
		return 1;
	}
	// return 0:
	return 0;
}

// linear friction of movement (should be below 1):
var cctGroundFriction = 0.8;
// gravity force:
var cctGravity = -5.5;
// jumping switch:


void cct_go_crawl(CCT *cct)
{
	// if we press ctrl:
	if(cct->input[CCT_CRAWL])
	{
		// if we need to change the size:
		if(cct->physBody->scale_z > cct->crawlSize.z)
		{
			// change the visual capsule size:
			vec_set(&cct->physBody->scale_x, &cct->crawlSize);
			// change the size of the cct to crawl:
			pX3ent_resizechar(cct->physBody, 0);
			// update bbox:
			c_setminmax(cct->physBody);
		}
		// we are crawling:
		cct->crawlOn = 1;
	}
	else
	{
		if(cct->crawlOn)
		{
			// and there is no celling above the head (20 - are min/max_x and 40 - are min/max_z values):
			if(cct_check_collision(cct->physBody, 20, 20, 40, 40) == 0)
			{
				cct->crawlOn = 0;
			}
		}
		if(!cct->crawlOn)
		{
			// if we need to change the size:
			if(cct->physBody->scale_z < cct->standSize.z)
			{
				// change the visual capsule size:
				vec_set(&cct->physBody->scale_x, &cct->standSize);
				// change the size of the cct to crawl:
				pX3ent_resizechar(cct->physBody, 40);
				// update bbox:
				c_setminmax(cct->physBody);
				// we are not crawling:
				cct->crawlOn = 0;
			}			
		}
	}
}

void cct_set_input(CCT *cct, int inputID, var value)
{
	if(cct == NULL) return;
	if(inputID < 0 || inputID > 4) return;
	cct->input[inputID] = value;
}

void cct_enable_physbody(CCT *cct)
{
	// wait one frame:
	wait(1);
	// set both collision flags:
	cct->physBody->eflags |= FAT | NARROW;
	// set bounding box to individual values:
	vec_set(cct->physBody->min_x, vector(-cct->boundingBox.x, -cct->boundingBox.y, -cct->boundingBox.z * 0.5)); 
	vec_set(cct->physBody->max_x, vector(cct->boundingBox.x, cct->boundingBox.y, cct->boundingBox.z * 0.5));
	// register the cct:
	pX3ent_settype(cct->physBody, PH_CHAR, PH_CAPSULE);
	// set group id:
	pX3ent_setgroup(cct->physBody, 3);
	cct->physBody->group = 3;
}

CCT *cct_create(VECTOR *spawnPoint, VECTOR *boundingBox)
{
	CCT *cct = sys_nxalloc(sizeof(CCT));
	
	cct->crawlSpeed = 0.3;
	cct->walkSpeed = 0.6;
	cct->runSpeed = 1.0;
	cct->baseSpeed = 15;
	cct->jumpForce = 25;
	
	vec_set(&cct->boundingBox, boundingBox);
	
	cct->physBody = ent_create(CUBE_MDL, spawnPoint, NULL);
	cct->physBody->flags = INVISIBLE | PASSABLE;
	
	// standing and crouching size's of cct:
	var cct_radius = cct->boundingBox.x;
	// setup scale vectors:
	vec_set(&cct->standSize, vector(cct_radius / cct->physBody->max_x, cct_radius / cct->physBody->max_x, 0.5 * cct->boundingBox.z / cct->physBody->max_z));
	vec_set(&cct->crawlSize, vector(cct_radius / cct->physBody->max_x, cct_radius / cct->physBody->max_x, 0.5 * 0.6 * cct->boundingBox.z / cct->physBody->max_z));
	// scale the bbox:
	vec_set(cct->physBody->scale_x, &cct->standSize);
	
	cct_enable_physbody(cct);
	
	return cct;
}

void cct_update(CCT *cct)
{
	// handle crawling:
	cct_go_crawl(cct);
	
	// get input:
	cct->force.x = cct->baseSpeed * cct->input[CCT_FORWARD];
	cct->force.y = cct->baseSpeed * cct->input[CCT_SIDEWARD];
	// if pressed space key (and switch is set to - zero):
	if(cct->input[CCT_JUMP] && cct->jump_time == 0)
	{
		// and if we are on the ground:
		if(pX3ent_ischargrounded(cct->physBody))
		{
			// then jump:
			cct->force.z = cct->jumpForce;
			// jumping switch on:
			cct->jump_time = 1;
			cct->state = CCT_JUMPING; // Enter jumping state
		}
	}
	// if we release the space key:
	if(!cct->input[CCT_JUMP])
	{
		// if we need to reset the jump switch:
		if(cct->jump_time)
		{
			// reset it:
			cct->jump_time = 0; 
		}
	}
	
	// If we are jumping and standing on the ground and don't go upwards,
	if(cct->state == CCT_JUMPING && pX3ent_ischargrounded(cct->physBody) && cct->force.z <= 0)
	{
		// then reset to standing, because we landed.
		cct->state = CCT_STANDING;
	}
	
	// if movement speed is more that allowed movement speed:
	if(vec_length(vector(cct->force.x, cct->force.y, 0)) > 15)
	{
		// lower it till defined movement speed (depending on state machine):
		var len = sqrt(cct->force.x * cct->force.x + cct->force.y * cct->force.y);				
		cct->force.x *= 15 / len;
		cct->force.y *= 15 / len;
	}
	// if we are crawling:
	if(cct->crawlOn)
	{
		// then crawl:
		cct->force.x *= cct->crawlSpeed;
		cct->force.y *= cct->crawlSpeed;
	}
	else
	{
		// if we press the shift key:
		if(cct->input[CCT_SPRINT])
		{
			// walk, if not pressing shift key:
			cct->force.x *= cct->runSpeed;
			cct->force.y *= cct->runSpeed;
		}
		else
		{
			// then run:
			cct->force.x *= cct->walkSpeed;
			cct->force.y *= cct->walkSpeed;
		}
	}
	
	vec_rotate(&cct->force, vector(cct->rotation, 0, 0));
	
	// accelerate forces:
	accelerate(&cct->dist.x, cct->force.x * time_step, cctGroundFriction);
	accelerate(&cct->dist.y, cct->force.y * time_step, cctGroundFriction);
	// reset relative Z movement:
	&cct->dist.z = 0;
	
	// move cct:
	int moveFlag = pX3ent_move(cct->physBody, &cct->dist, vector(0, 0, cct->force.z * time_step));
	
	// gravity (if we are in air):
	if(!pX3ent_ischargrounded(cct->physBody))
	{
		// accelerate downwards:
		cct->force.z += cctGravity * time_step;
		// limit acceleration:
		cct->force.z = maxv(cct->force.z, -100);
		// if cct collided with celling:
		if(moveFlag & PX_CCT_COLLISION_UP)
		{
			// reset jumping:
			cct->force.z = cctGravity;
		}
	}
	else
	{
		// we are on ground:
		cct->force.z = cctGravity;
	}
	
	// Switch state if we are NOT jumping
	if(cct->state != CCT_JUMPING)
	{
		if(vec_length(&cct->dist) < 0.5)
		{
			if(cct->crawlOn)
				cct->state = CCT_CROUCHING;
			else
				cct->state = CCT_STANDING;
		}
		else
		{
			if(cct->crawlOn)
				cct->state = CCT_CRAWLING;
			else if(cct->input[CCT_SPRINT])
				cct->state = CCT_RUNNING;
			else
				cct->state = CCT_WALKING;
		}
	}
}

void cct_get_position(CCT *cct, VECTOR *position)
{
	if(cct == NULL || position == NULL) return;
	vec_set(position, &cct->physBody->x);
}

void cct_set_rotation(CCT *cct, var rotation)
{
	if(cct == NULL) return;
	cct->rotation = rotation;
}

int cct_get_state(CCT *cct)
{
	if(cct == NULL) return -1;
	return cct->state;
}