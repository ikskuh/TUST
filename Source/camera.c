#include <acknex.h>
#include "camera.h"

ENTITY *camTarget = NULL;
int camMode = CAMERA_FREE;
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
	vec_set(&cPos, &camOffset);
	if(camTarget != NULL)
	{
		vec_rotate(&cPos, &camTarget->pan);
		vec_add(&cPos, &camTarget->x);
	}
	
	switch(camMode)
	{
		case CAMERA_FREE:
			
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
			
			break;
	}
}

void cam_mode(int mode) { camMode = mode; }

void cam_target(ENTITY *ent) { camTarget = ent; }

void cam_basic_offset(VECTOR *vec) { vec_set(&camOffset, vec); }

void cam_mouse_speed(var speed) { camMouseSpeed = speed; }

void cam_1person_limit_min(var angle) { cam1PersonLimitMin = angle; }

void cam_1person_limit_max(var angle) { cam1PersonLimitMax = angle; }

void cam_3person_offset(VECTOR *vec) { vec_set(&cam3PersonOffset, vec); }

void cam_3person_limit_min(var angle) { cam3PersonLimitMin = angle; }

void cam_3person_limit_max(var angle) { cam3PersonLimitMax = angle; }