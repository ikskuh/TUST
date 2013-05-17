#ifndef _CAMERA_H_
#define _CAMERA_H_

/**
 * \file camera.h
 * Different types of cameras.
 */

/**
 * Free camera mode. Use WASD and mouse to fly through the level.
 */
#define CAMERA_FREE			0

/**
 * First person camera mode. Mouse rotates the camera around a given model offset.
 */
#define CAMERA_FIRST_PERSON	1

/**
 * Third person camera mode. Mouse rotates the camera with a given offset around a given model offset.
 */
#define CAMERA_THIRD_PERSON	2

/**
 * 
 */
#define CAMERA_OBSERVATION	3


/**
 * Initializes the camera system.
 */
void cam_init(void);

/**
 * Updates the camera system.
 */
void cam_update(void);

/**
 * Sets the camera mode.
 */
void cam_set_mode(int mode);

/**
 * Gets the camera mode.
 */
int cam_get_mode(void);

/**
 * Sets the current camera target.
 */
void cam_target(ENTITY *ent);

/**
 * Sets the basic camera offset. This offset is used for every camera mode as the target position.
 */
void cam_basic_offset(VECTOR *vec);

/**
 * Sets the camera flight speed.
 */
void cam_speed(var speed);

/**
 * Sets the camera mouse speed.
 */
void cam_mouse_speed(var speed);

/**
 * Sets the first person angle limit. This limit is used for the camera tilt.
 */
void cam_1person_limit_min(ANGLE *angle);

/**
 * Sets the first person angle limit. This limit is used for the camera tilt.
 */
void cam_1person_limit_max(ANGLE *angle);

/**
 * Sets the third person offset. This offset is used in the third person camera.
 */
void cam_3person_offset(VECTOR *vec);

/**
 * Sets the third person angle limit. This limit is used for the camera tilt.
 */
void cam_3person_limit_min(ANGLE *angle);

/**
 * Sets the third person angle limit. This limit is used for the camera tilt.
 */
void cam_3person_limit_max(ANGLE *angle);

#include "camera.c"
#endif