// include libs:
#include <acknex.h>
#include <default.c>

// include physX:
#include "..\\..\\Source\\physX3\\ackphysX3.h"
// include cct template:
#include "..\\..\\Source\\cct.h"

// player's action:
action actHero(){
	// flags:
	set(my, TRANSLUCENT | PASSABLE);
	// create an actior:
	Actor* actor = cct_actor(me, vector(20, 20, 80), vector(0, 0, 0));
	// loop:
	while(1){
		// get input:
		cct_set_input(actor, CCT_WALK, key_w - key_s);
		cct_set_input(actor, CCT_STRAFE, key_a - key_d);
		cct_set_input(actor, CCT_RUN, key_shift);
		cct_set_input(actor, CCT_CRAWL, key_ctrl);
		cct_set_input(actor, CCT_JUMP, key_space);
		// update physics:
		cct_update(actor);
		// get the proper positions:
		cct_get_position(actor, my.x);
		// rotate character:
		cct_rotate(actor, 4 * mouse_force.y * time_step);
		// rotate model:
		my.pan = cct_get_rotation(actor);
		// wait one frame:
		wait(1);
	}
}

// define test level's name:
STRING* levelStr = "map01.wmb";

// main game function:
function main(){
	// disable v-sync:
	d3d_triplebuffer = 1;
	// turn doppler effect OFF:
	doppler_factor = 0;	
	// preload in buffer all created models:
	preload_mode = 3;
	// show all errors:
	warn_level = 6;
	// limit FPS:
	fps_max = 60;
	// init physX:
	physX3_open();
	// load the level:
	freeze_mode = 1;
	level_load(levelStr);
	wait(3);
	freeze_mode = 0;
}