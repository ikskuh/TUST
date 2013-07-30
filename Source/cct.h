#ifndef _CCT_H_
#define _CCT_H_

#include "tust.h"
#include "physX3\\ackphysX3.h"

/**
 * \file cct.h
 * Physics based player movement
 */

#define CCT_FORWARD 0
#define CCT_SIDEWARD 1
#define CCT_JUMP 2
#define CCT_CRAWL 3
#define CCT_SPRINT 4

typedef struct CCT
{
	// Movement speeds
	var crawlSpeed;
	var walkSpeed;
	var runSpeed;
	
	// Input values
	var input[5];
	var rotation;
	
	VECTOR boundingBox;

	VECTOR dist;
	VECTOR force;
	
	VECTOR standSize;
	VECTOR crawlSize;
	var jump_time;
	var crawlTimer;
	var crawlOn;
	var goCrawlSpeed;
	
	// Physic body
	ENTITY *physBody;
} CCT;

CCT *cct_create(VECTOR *spawnPoint, VECTOR *boundingBox);

void cct_set_input(CCT *cct, int inputID, var value);

void cct_set_rotation(CCT *cct, var rotation);

void cct_update(CCT *cct);

void cct_get_position(CCT *cct, VECTOR *result);

// include cct template:
#include "cct.c"
	
#endif