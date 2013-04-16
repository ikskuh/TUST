// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 070910
//
// title: Code that handles the elipsoid collision with entity's
// class: elipsoid collision
// type: elipsoid collision
// help: contains code handling all the elipsoid collision between entity's
// 
// prefix: COL_
// idcode: 14
// 
// ENDHEADER
// ---------------------------------------------------------------------

typedef struct LINKED_COLLISION {
	// the entity
	ENTITY *COL_collider;
	
	struct LINKED_COLLISION *COL_collisionNext;
} LINKED_COLLISION;

typedef struct {
	// closest enity to collide with
	ENTITY *COL_collisionEnt;
	var COL_CollisionDist;

	LINKED_COLLISION *COL_collision;
} COLLISION;

// variables
var COL_defaultElipsoidWidth = 40;

// prototypes
LINKED_COLLISION *init_elipsoidCollider(ENTITY* who);
COLLISION *init_elipsoidCollision(ENTITY* who);
ENTITY *COL_getClosestCollider(ENTITY* who);

// definitions
LINKED_COLLISION *COL_FirstObject;
LINKED_COLLISION *COL_LastObject;

// includes
#include "collision.c"

LINKED_COLLISION *init_elipsoidCollider(ENTITY* who) {
	LINKED_COLLISION *myCollisionEntity = sys_nxalloc(sizeof(LINKED_COLLISION));
	
	// check and set first object in list
	if(COL_FirstObject == NULL) {
		COL_FirstObject = myCollisionEntity;
	}
	
	if(COL_LastObject != NULL) {
		COL_LastObject->COL_collisionNext = myCollisionEntity;
	}
	
	// set collider to "who"
	myCollisionEntity->COL_collider = who;
	COL_LastObject = myCollisionEntity;
	
	return(myCollisionEntity);
}

COLLISION *init_elipsoidCollision(ENTITY* who, var elipsoidWidth) {
	COLLISION *myCollisionEntity = sys_nxalloc(sizeof(COLLISION));
	
	myCollisionEntity->COL_collisionEnt = COL_getClosestCollider(who);
	if(elipsoidWidth == 0) {
		myCollisionEntity->COL_CollisionDist = COL_defaultElipsoidWidth;
	} else {
		myCollisionEntity->COL_CollisionDist = elipsoidWidth;
	}
	
	
	myCollisionEntity->COL_collision = init_elipsoidCollider(who);
	
	// run function
	RunElipsoidCollision(myCollisionEntity);
	
	return(myCollisionEntity);
}

ENTITY *COL_getClosestCollider(ENTITY* who) {
	LINKED_COLLISION *myColliderObj;
	
	ENTITY* returnColliderObj = NULL;
	myColliderObj = COL_FirstObject;
	
	var closestDistance = 500000; // set very high initally to find entity's
	var COL_objectDist;
	
	while(myColliderObj != NULL) {
		// if the next entity is not the same as the looking entity
		if(myColliderObj->COL_collider != who) {
			// if the distance is closer then the previous closest entity
			if(vec_dist(who.x, myColliderObj->COL_collider.x) < closestDistance) {
				closestDistance = vec_dist(who.x, myColliderObj->COL_collider.x); // set new closest distance
				returnColliderObj = myColliderObj->COL_collider; // you are the closest entity to look at now
			}
		}
		
		myColliderObj = myColliderObj->COL_collisionNext;
	}
	
	if(returnColliderObj == NULL) { returnColliderObj = who; }
	return(returnColliderObj);
}