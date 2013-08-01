#ifndef _CCT_H_
#define _CCT_H_

#include "tust.h"
#include "physX3\\ackphysX3.h"

/**
 * \file cct.h
 * Physics based player movement
 */

/**
 * Enum value for cct_set_input. Defines the forward component.
 */
#define CCT_FORWARD 0

/**
 * Enum value for cct_set_input. Defines the sideward component.
 */
#define CCT_SIDEWARD 1

/**
 * Enum value for cct_set_input. Defines the jump component.
 */
#define CCT_JUMP 2

/**
 * Enum value for cct_set_input. Defines the crawl component.
 */
#define CCT_CRAWL 3

/**
 * Enum value for cct_set_input. Defines the sprint component.
 */
#define CCT_SPRINT 4

/**
 * Enum value for cct_get_state. The character is standing.
 */
#define CCT_STANDING 0

/**
 * Enum value for cct_get_state. The character is crouching.
 */
#define CCT_CROUCHING 1

/**
 * Enum value for cct_get_state. The character is crawling.
 */
#define CCT_CRAWLING 2

/**
 * Enum value for cct_get_state. The character is walking.
 */
#define CCT_WALKING 3

/**
 * Enum value for cct_get_state. The character is running.
 */
#define CCT_RUNNING 4

/**
 * Enum value for cct_get_state. The character is jumping.
 */
#define CCT_JUMPING 5

/**
 * The character controller.
 */
typedef struct CCT
{
	/**
	 * The crawl speed of the character. Gets multiplied with baseSpeed.
	 */ 
	var crawlSpeed;
	
	/**
	 * The walk speed of the character. Gets multiplied with baseSpeed.
	 */ 
	var walkSpeed;
	
	/**
	 * The run speed of the character. Gets multiplied with baseSpeed.
	 */ 
	var runSpeed;
	
	/**
	 * The base speed of the character.
	 */ 
	var baseSpeed;
	
	/**
	 * The jump force of the character.
	 */ 
	var jumpForce;
	
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
	
	// Physic body
	ENTITY *physBody;
	
	int state;
} CCT;

/**
 * Creates a new character controller.
 * \param  spawnPoint  The creation point of the cct.
 * \param  boundingBox The bounding box of the cct. Defines the size of the capsule.
 * \return             The newly created cct.
 */
CCT *cct_create(VECTOR *spawnPoint, VECTOR *boundingBox);

/**
 * Sets an input value of the cct. This allows you to control it.
 * \param cct     The character controller to be modified.
 * \param inputID One of the following: \ref CCT_FORWARD, \ref CCT_SIDEWARD, \ref CCT_JUMP, \ref CCT_CRAWL, \ref CCT_SPRINT
 * \param value   The new input value for the cct.
 */
void cct_set_input(CCT *cct, int inputID, var value);

/**
 * Sets the rotation of the cct.
 * \param cct      The character controller to be modified.
 * \param rotation The new rotation. This is an absolute angle.
 */
void cct_set_rotation(CCT *cct, var rotation);

/**
 * Updates an character controller and perforces all movement.
 * \param cct      The character controller to be updated.
 */
void cct_update(CCT *cct);

/**
 * Gets the center of the character controller.
 * \param cct    The character controller.
 * \param result A pointer to the resulting position.
 */
void cct_get_position(CCT *cct, VECTOR *result);

/**
 * Gets the state of the character controller.
 * \param  cct The character controller.
 * \return     One of the following: \ref CCT_STANDING, \ref CCT_CROUCHING, \ref CCT_CRAWLING, \ref CCT_WALKING, \ref CCT_RUNNING, \ref CCT_JUMPING
 */
int cct_get_state(CCT *cct);

// include cct template:
#include "cct.c"
	
#endif
























