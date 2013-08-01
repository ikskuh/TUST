#include <acknex.h>
#include <default.c>
#include "..\\..\\Source\\tust.h"
#include "..\\..\\Source\\cct.h"
#include "..\\..\\Source\\hitbox.h"
#include "..\\..\\Source\\attachment.h"

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
	
	ENTITY *box;
	CharacterShape *shape = charshape_attach(me);
	box = charshape_add(shape, "cylinderShape.mdl", 53, 57, 36, 39); box->scale_y = box->scale_z = 0.5;
	box = charshape_add(shape, "cylinderShape.mdl", 273, 291, 312, 313); box->scale_y = box->scale_z = 0.5;
	box = charshape_add(shape, "cylinderShape.mdl", 273, 291, 53, 57); box->scale_y = box->scale_z = 0.6;
	box = charshape_add(shape, "cylinderShape.mdl", 156, 73, 72, 131); box->scale_y = box->scale_z = 0.75;
	box = charshape_add(shape, "cylinderShape.mdl", 156, 290, 303, 197); box->scale_y = box->scale_z = 0.75;
	box = charshape_add(shape, "cylinderShape.mdl", 303, 197, 217, 265); box->scale_y = box->scale_z = 0.75;
	box = charshape_add(shape, "cylinderShape.mdl", 72, 131, 116, 71); box->scale_y = box->scale_z = 0.75;
	box = charshape_add(shape, "cylinderShape.mdl", 85, 126, 133, 64); box->scale_y = box->scale_z = 0.4;
	box = charshape_add(shape, "cylinderShape.mdl", 278, 209, 272, 232); box->scale_y = box->scale_z = 0.4;
	box = charshape_add(shape, "cylinderShape.mdl", 159, 184, 177, 188); box->scale_y = box->scale_z = 1.2;
	box = charshape_add(shape, "cylinderShape.mdl", 177, 188, 183, 183); box->scale_y = box->scale_z = 0.6;
	set(me, PASSABLE); // We can make our character passable, we have bounding boxes for it.
	
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
		
		charshape_update(shape);
		
		wait(1);
	}
}

/*
// Init
int swordDrawn = 0;
ENTITY *sword = ent_create("katana.mdl", vector(0, 0, 0), NULL); // Create a katana
vec_fill(&sword->scale_x, 0.05);
//attach_entity(sword, me, vector(-6, -4, 24), vector(0, 0, 20));
//attach_entity_to_vertex(sword, me, 158, vector(-2, -4, 4), vector(0, -5, 15));

// Loop
if(input_hit("draw"))
	swordDrawn = !swordDrawn;

if(swordDrawn) // Attach sword to hand
	attach_entity_to_vertices(sword, me, 7, 8, vector(0, 0, 0), vector(0, -90, 0));
else // Attach sword to backpack
	attach_entity_to_vertices(sword, me, 158, 157, vector(-2, -4, 4), vector(0, 90, 0));
*/

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
	input_set_button("draw", key_for_str("f"));
	
	level_load("basic.wmb"); //Load our level
	ent_create("soldier.mdl", spawnPoint, player_func); // Spawn the player
	
	proc_mode = PROC_EARLY; // This function should be scheduled before any other function.
	while(1)
	{
		input_update();
		wait(1);
	}
}