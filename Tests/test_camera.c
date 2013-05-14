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
	ent_create("warlock.mdl", vector(0, 0, 0), character);
}

action character()
{
	cam_init();
	cam_mode(CAMERA_THIRD_PERSON);
	cam_target(me);
	cam_basic_offset(vector(0, 0, 28));
	while(me)
	{
		if(c_trace(my.x, vector(my.x, my.y, my.z - 1000), IGNORE_ME | IGNORE_PASSABLE | USE_POLYGON))
			my.z = target.z + 32;
		
		cam_update();
		
		if(key_w | key_a | key_s | key_d)
		{
			my.pan = camera.pan;
			c_move(me, vector(5 * time_step * (key_w - key_s), 5 * time_step * (key_a - key_d), 0), vector(0, 0, 0), IGNORE_ME | IGNORE_PASSABLE | PASSABLE | GLIDE);
		}
		
		wait(1);
	}
}