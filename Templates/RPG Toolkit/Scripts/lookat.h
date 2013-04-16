// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 210610
//
// title: Code to handle all characters/objects to look at their surrounding
// class: NPC Player Objects
// type: look at
// help: contains code handling bone rotations to look at entity's
// 
// prefix: LOOK_
// idcode: 8
// 
// ENDHEADER
// ---------------------------------------------------------------------

typedef struct LINKED_LOOKAT {
	ENTITY *LOOK_object;
	VECTOR *LOOK_lookAtOffset;
	
	struct LINKED_LOOKAT *LOOK_nextObject;
} LINKED_LOOKAT;

typedef struct LINKED_BONEROT {
	STRING *LOOK_boneRot;
	
	struct LINKED_BONEROT *LOOK_nextBone;
} LINKED_BONEROT;

typedef struct {
	ENTITY* LOOK_object;
	
	LINKED_BONEROT *LOOK_firstBone;
	LINKED_BONEROT *LOOK_lastBone;
	var LOOK_boneAmount;
	var LOOK_maxBoneRotAngle;
	
	var LOOK_smoothSpeed;
	var LOOK_headSpeed;
	
	// entity which is the current lookAt object
	ENTITY *LOOK_lookAtObject;
	
	// make me an entity to look at as well
	LINKED_LOOKAT *LOOK_viewObject;
} LOOKAT;

// prototypes
ENTITY *LOOK_getLookAtObject(ENTITY* who, var viewAngle);
LINKED_BONEROT *init_boneRotString(LOOKAT *lookAtObject, STRING *boneName);
void LOOK_getBoneRotationSetup(LOOKAT *tempLookAt);

// definitions
LINKED_LOOKAT *LOOKAT_firstObject;
LINKED_LOOKAT *LOOKAT_currentObject;

// include
#include "lookat.c"

// functions
LINKED_LOOKAT *init_lookAtObject(ENTITY *object) {
	LINKED_LOOKAT *myLookAtObject = sys_nxalloc(sizeof(LINKED_LOOKAT));
	
	myLookAtObject->LOOK_object = object;
	
	// check and set first object in list
	if(LOOKAT_firstObject == NULL) {
		LOOKAT_firstObject = myLookAtObject;
	}
	
	// check and set objects next object
	if(LOOKAT_currentObject != NULL) {
		LOOKAT_currentObject->LOOK_nextObject = myLookAtObject;
	}
	
	// set current object
	LOOKAT_currentObject = myLookAtObject;
	myLookAtObject->LOOK_nextObject = NULL;
	myLookAtObject->LOOK_lookAtOffset = nullvector;
	
	return(myLookAtObject);
}

LINKED_BONEROT *init_boneRotString(LOOKAT *lookAtObject, STRING *boneName) {
	LINKED_BONEROT *myboneRotObject = sys_nxalloc(sizeof(LINKED_BONEROT));
	
	myboneRotObject->LOOK_boneRot = str_create(boneName);
	
	// check and set first object in list
	if(lookAtObject->LOOK_firstBone == NULL) {
		lookAtObject->LOOK_firstBone = myboneRotObject;
	}
	// check and set objects next object
	if(lookAtObject->LOOK_lastBone != NULL) {
		lookAtObject->LOOK_lastBone->LOOK_nextBone = myboneRotObject;
	}
	
	// set current object
	lookAtObject->LOOK_lastBone = myboneRotObject;
	myboneRotObject->LOOK_nextBone = NULL;
	
	return(myboneRotObject);
}

LOOKAT *init_lookAtViewer(ENTITY* who) {
	LOOKAT *myLookAtEntity = sys_nxalloc(sizeof(LOOKAT));
	
	myLookAtEntity->LOOK_object = who;
	
	// get all setup for bone look at setup
	LOOK_getBoneRotationSetup(myLookAtEntity);
	
	myLookAtEntity->LOOK_smoothSpeed = 4;
	myLookAtEntity->LOOK_headSpeed = 0.2;
	myLookAtEntity->LOOK_lookAtObject = who;
	
	myLookAtEntity->LOOK_viewObject = init_lookAtObject(who);
	
	RunBoneRotateLookAt(myLookAtEntity);
	
	return(myLookAtEntity);
}

void LOOK_getBoneRotationSetup(LOOKAT *tempLookAt) {
	var fhandle_n = file_open_read(tempLookAt->LOOK_object.string1);
	
	STRING *getCurrentData = str_create("");
	STRING *currentBone = str_create("");
	var animStrLength;
	var curBoneStrLength;
	
	// set bones amount to 0
	tempLookAt->LOOK_boneAmount = 0;
	
	// get Character
	file_find(fhandle_n, tempLookAt->LOOK_object.string2);
	
	// get max angle for head to look at
	file_find(fhandle_n, "bone rotation angle: ");
	file_str_read(fhandle_n, getCurrentData); // get max angle
	tempLookAt->LOOK_maxBoneRotAngle = str_to_num(getCurrentData);
	
	// get bones that rotate the head
	file_find(fhandle_n, "bone rotation bones: ");
	file_str_read(fhandle_n, getCurrentData); // get all bone names
	
	file_close(fhandle_n);
	
	while(1) {
		// find new animations string
		animStrLength = str_stri(getCurrentData, " "); // check for the next space seperating the bones
		if(animStrLength == 0) { break; } // if no more bones are found, break this loop.
		
		// process animation into new game animation
		str_cpy(currentBone, getCurrentData); // store animations string
		str_clip(getCurrentData, animStrLength); // remove animation from animations string
		curBoneStrLength = str_len(getCurrentData);
		str_trunc(currentBone, curBoneStrLength+1); // remove all but first animation name (and the extra space)
		
		// add new bone as linked bone list, using the name from 'currentBone'
		init_boneRotString(tempLookAt, currentBone);
		
		tempLookAt->LOOK_boneAmount += 1;
	}
	
	ptr_remove(getCurrentData);
	ptr_remove(currentBone);
}

ENTITY *LOOK_getLookAtObject(ENTITY* who, var viewAngle) {
	LINKED_LOOKAT *myLookatObj;
	
	ENTITY* returnLookatObj = NULL;
	myLookatObj = LOOKAT_firstObject;
	
	var closestDistance = 500000; // set very high initally to find entity's

	VECTOR LOOK_moveAway;
	var LOOK_objectDist;
	
	while(myLookatObj != NULL) {
		// if the next entity is not the same as the looking entity
		if(myLookatObj->LOOK_object != who) {
			// if the distance is closer then the previous closest entity
			if(vec_dist(who.x, myLookatObj->LOOK_object.x) < closestDistance) {
				// and when the object is in view
//				vec_diff(LOOK_moveAway.x, myLookatObj->LOOK_object.x, who.x);
//				vec_to_angle(LOOK_moveAway.x, LOOK_moveAway.x);
//				LOOK_objectDist = ang(LOOK_moveAway.x);
				
//				if(abs(ang(who.pan - LOOK_objectDist)) < viewAngle) {
					// and finally check if the object is close by
//					if(vec_dist(who.x, myLookatObj->LOOK_object.x) < 200) {
						closestDistance = vec_dist(who.x, myLookatObj->LOOK_object.x); // set new closest distance
						returnLookatObj = myLookatObj->LOOK_object; // you are the closest entity to look at now
//					}
//				}
			}
		}
		
		myLookatObj = myLookatObj->LOOK_nextObject;
	}
	
	if(returnLookatObj == NULL) { returnLookatObj = who; }
	return(returnLookatObj);
}