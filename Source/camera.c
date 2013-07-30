#include <acknex.h>
#include "camera.h"

List *observationCameras = NULL;

ENTITY *camTarget = NULL;
int camMode = CAMERA_FREE;
var camSpeed = 10;
var camMouseSpeed = 10;
VECTOR camOffset;
VECTOR camFreePos;
ANGLE camFreeAngle;
ANGLE cam1PersonAngle;
var cam1PersonLimitMin;
var cam1PersonLimitMax;

ANGLE cam3PersonAngle;
var cam3PersonLimitMin;
var cam3PersonLimitMax;
VECTOR cam3PersonOffset;

VECTOR camObservationPos;

void cam_init()
{
	cam1PersonLimitMin = -80;
	cam1PersonLimitMax = 80;
	
	cam3PersonLimitMin = -60;
	cam3PersonLimitMax = 5;
	
	cam3PersonOffset.x = -250;
	cam3PersonOffset.y = 0;
	cam3PersonOffset.z = 0;
}

void cam_update(void)
{
	VECTOR cPos;
	int i;
	vec_set(&cPos, &camOffset);
	if(camTarget != NULL)
	{
		camFreeAngle.pan -= mouse_force.x * camMouseSpeed * time_step;
		camFreeAngle.tilt += mouse_force.y * camMouseSpeed * time_step;
		
		VECTOR dir;
		vec_set(&dir, vector(key_w - key_s, key_a - key_d, key_q - key_r));
		vec_scale(&dir, camSpeed * time_step);
		vec_rotate(&dir, &camFreeAngle);
		vec_add(camFreePos, &dir);
		
		vec_rotate(&cPos, &camTarget->pan);
		vec_add(&cPos, &camTarget->x);
	}
	
	switch(camMode)
	{
		case CAMERA_FREE:
			
			vec_set(&camera->x, &camFreePos);
			vec_set(&camera->pan, &camFreeAngle);
			
			break;
		case CAMERA_FIRST_PERSON:
			cam1PersonAngle.pan -= mouse_force.x * camMouseSpeed * time_step;
			cam1PersonAngle.tilt += mouse_force.y * camMouseSpeed * time_step;
			
			cam1PersonAngle.tilt = clamp(cam1PersonAngle.tilt, cam1PersonLimitMin, cam1PersonLimitMax);
			
			vec_set(&camera->x, &cPos);
			vec_set(&camera->pan, &cam1PersonAngle);
			break;
		case CAMERA_THIRD_PERSON:
			cam3PersonAngle.pan -= mouse_force.x * camMouseSpeed * time_step;
			cam3PersonAngle.tilt += mouse_force.y * camMouseSpeed * time_step;
			
			cam3PersonAngle.tilt = clamp(cam3PersonAngle.tilt, cam3PersonLimitMin, cam3PersonLimitMax);
			
			vec_set(&camera->x, &cam3PersonOffset);
			vec_rotate(&camera->x, &cam3PersonAngle);
			vec_add(&camera->x, &cPos);
			vec_set(&camera->pan, &cam3PersonAngle);
			
			break;
		case CAMERA_OBSERVATION:
			
			// Any obersation cameras defined?
			if (observationCameras != NULL) {
				
				// Is there any camera that is nearer to the player than the current one?
				for (i=0; i<observationCameras.count; i++) {
					
					// Can the player be seen from any cam?
					if (c_trace(((VECTOR*)list_item_at(observationCameras, i))->x,&cPos,IGNORE_ME|IGNORE_PASSENTS|IGNORE_PASSABLE|IGNORE_SPRITES|USE_BOX) == 0) {
					
						// Is the distance from the player to the new cam smaller than the distance
						// from the current cam to the player?
						if (vec_dist(&camera->x,&cPos) > vec_dist(((VECTOR*)list_item_at(observationCameras, i))->x, &cPos)) {
							
							// If yes, change camera position
							vec_set(&camera->x,	((VECTOR*)list_item_at(observationCameras, i))->x);
						}
					}
				}
			}
			
			// Camera always looks at player
			vec_set(&camObservationPos, &cPos.x);
			vec_sub(&camObservationPos, &camera->x);
			vec_to_angle(&camera->pan, &camObservationPos);
		
			break;
	}
}

void cam_set_mode(int mode) { camMode = mode; }
int cam_get_mode() { return camMode; }

void cam_target(ENTITY *ent) { camTarget = ent; }

void cam_basic_offset(VECTOR *vec) { vec_set(&camOffset, vec); }

void cam_speed(var speed) { camSpeed = speed; }

void cam_mouse_speed(var speed) { camMouseSpeed = speed; }

void cam_1person_limit_min(var angle) { cam1PersonLimitMin = angle; }

void cam_1person_limit_max(var angle) { cam1PersonLimitMax = angle; }

void cam_3person_offset(VECTOR *vec) { vec_set(&cam3PersonOffset, vec); }

void cam_3person_limit_min(var angle) { cam3PersonLimitMin = angle; }

void cam_3person_limit_max(var angle) { cam3PersonLimitMax = angle; }