#include <acknex.h>
#include <default.c>
#include "..\\..\\Source\\tust.h"
#include "..\\..\\Source\\cct.h"
#include "..\\..\\Source\\animator.h"
#include "..\\..\\Source\\input.h"

// Global ressources
#define PRAGMA_PATH "..\\..\\Ressources\\Models\\Characters"
#define PRAGMA_PATH "..\\..\\Ressources\\Models\\Buildings"
#define PRAGMA_PATH "..\\..\\Ressources\\Terrains\\"

// RTS ressources
#define PRAGMA_PATH "Resources"

VECTOR spawnPoint;

// Defines the spawn point of our player
action player_spawn()
{
	// Just set up our spawnpoint, then remove the spawnpoint model itself
	vec_set(spawnPoint, my.x);
	ent_remove(me);
}

// Provides controls, camera and weapon actions for our player
action player_func()
{
	// Allow only one player!
	if(proc_status(player_func) > 1)
	{
		error("Only one player can be active at a time!");
		return;
	}
	
	my->z += 8; // Lift player up a little to prevent falling through ground
	
	// Setup the camera
	cam_target(me); // Focus the player
	cam_mouse_speed(25); // Set a fast mouse.
	cam_basic_offset(vector(0, 0, 20)); // We want a camera focus a little bit above the player
	cam_3person_offset(vector(-250, -30, 0)); // We want to camera to be behind 
	cam_set_mode(CAMERA_THIRD_PERSON); // Enable third person camera
	
	// Setup the CCT
	CCT *cct = cct_create(my.x, vector(10, 16, 52));
	
	// Setup the animations system
	Animator *anim = animator_attach(me, 6);
	animator_set(anim, CCT_STANDING, "stand", 5.0, true);
	animator_set(anim, CCT_CROUCHING, "duck", 5.0, true);
	
	animator_set(anim, CCT_CRAWLING, "crawl", 5.0, true);
	animator_set(anim, CCT_WALKING, "walk", 5.0, true);
	animator_set(anim, CCT_RUNNING, "run", 5.0, true);
	
	animator_set(anim, CCT_JUMPING, "jump", 10.0, false);
	
	while(1)
	{
		cam_update(); // Update the camera each frame
		my->pan = camera->pan; // We sync the entity rotation with the camera
		
		// Setup the cct input
		cct_set_input(cct, CCT_FORWARD, input_get("forward"));
		cct_set_input(cct, CCT_SIDEWARD, input_get("sideward"));
		cct_set_input(cct, CCT_JUMP, input_get("jump"));
		cct_set_input(cct, CCT_CRAWL, input_get("crouch"));
		cct_set_input(cct, CCT_SPRINT, input_get("sprint"));
		cct_set_rotation(cct, my->pan);
		cct_update(cct); // Update the character controller
		
		cct_get_position(cct, &my->x); // Get the position of the character
		my.z -= 2; // Move the char a little bit down so we stand on the ground (CCT and Model don't have the same origin)
		
		int cctState = cct_get_state(cct);
		switch(cctState)
		{
			case CCT_JUMPING:
				animator_play(anim, CCT_JUMPING, false); // Switch to the jump animation, don't force any animation reset
				break;
			default:
				animator_blend(anim, cctState, 3.0); // Blend to the fitting animation in 5 ticks
				break;
		}
		animator_update(anim); // Animate our model
		
		wait(1);
	}
}

function main()
{
	fps_max = 60; // Limit fps to 60
	video_set(1024, 768, 32, 2); // Setup some screen params
	cam_init(); // Initialize the camera system
	input_init(); // Initialize the input system
	physX3_open(); // Open PhysX 3
	
	// Setup controls
	input_set_buttons("forward", key_for_str("w"), key_for_str("s"));
	input_set_axis("forward", key_force.y, 1, 0.01);

	input_set_buttons("sideward", key_for_str("a"), key_for_str("d"));
	input_set_axis("sideward", key_force.x, -1, 0.01); // Invert X-Axis to get proper left-right movement

	input_set_button("jump", key_for_str("space"));
	input_set_button("crouch", 29); // ctrl
	input_set_button("sprint", key_for_str("shiftl"));
	
	level_load("basic.wmb"); //Load our level
	ent_create("soldier.mdl", spawnPoint, player_func); // Spawn the player
	
	proc_mode = PROC_EARLY; // This function should be scheduled before any other function.
	while(1)
	{
		input_update();
		wait(1);
	}
}