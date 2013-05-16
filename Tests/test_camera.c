#include <acknex.h>
#include <default.c>
#include "..\Source\tust.h"

#define PRAGMA_PATH "..\Ressources\Models\Characters\"
#define PRAGMA_PATH "..\Ressources\Terrains\"

action character();

function main()
{
	fps_max = 60;
	level_load("small.hmp");
	
	// Generate camera positions to test observation camera
	observationCameras = list_create();
	list_add(observationCameras, vec_create(level_ent.min_x, level_ent.min_y, level_ent.max_z + 200));
	list_add(observationCameras, vec_create(level_ent.max_x, level_ent.min_y, level_ent.max_z + 200));
	list_add(observationCameras, vec_create(level_ent.min_x, level_ent.max_y, level_ent.max_z + 200));
	list_add(observationCameras, vec_create(level_ent.max_x, level_ent.max_y, level_ent.max_z + 200));
	vec_set(&camera->x, ((VECTOR*)list_item_at(observationCameras, 0))->x);
	
	ent_create("warlock.mdl", vector(0, 0, 0), character);
}

action character()
{
	cam_init();
	cam_mode(CAMERA_FREE);
	//cam_mode(CAMERA_FIRST_PERSON);
	//cam_mode(CAMERA_THIRD_PERSON);
	//cam_mode(CAMERA_OBSERVATION);
	
	
	cam_target(me);
	cam_basic_offset(vector(0, 0, 28));
	while(me)
	{
		if(c_trace(my.x, vector(my.x, my.y, my.z - 1000), IGNORE_ME | IGNORE_PASSABLE | USE_POLYGON))
			my.z = target.z + 32;
		
		cam_update();
		
		if(key_w | key_a | key_s | key_d)
		{
			switch (camMode) {
				case CAMERA_THIRD_PERSON:
				case CAMERA_FIRST_PERSON:
				case CAMERA_FREE:
					my.pan = camera.pan;
				break;
				case CAMERA_OBSERVATION:
					my.pan = my.pan + (key_a - key_d) * 5 * time_step;
				break;
			}
			
			c_move(me, vector(5 * time_step * (key_w - key_s), 5 * time_step * (key_a - key_d), 0), vector(0, 0, 0), IGNORE_ME | IGNORE_PASSABLE | PASSABLE | GLIDE);
		}
		
		wait(1);
	}
}