#include <acknex.h>

SOUND *sndThrow = "throw.wav";
SOUND *sndSlice = "slice.wav";

function fruit_event()
{
	switch(event_type)
	{
		case EVENT_SHOOT:
			// We hit the fruit, remove it.
			wait(1);	// Don't forget to wait one frame in an entity event
			ent_remove(me);
			break;
	}
}

// This action is used for the fruits beeing thrown up.
action fruit()
{
	var speed_x = random(2) - 1;	// Throw a little bit left or right
	var speed_y = 15 + random(5);	// Throw up
	
	// We want the model to rotate in the direction of our throw:
	var rotation = 
		-sign(speed_x) * 			// Get the direction of the rotation
		(5 + random(15));			// Randomize the rotation speed between 5 and 15
	
	my.emask = ENABLE_SHOOT;
	my.event = fruit_event;
	
	while(me)
	{
		// Move our fruit
		my.x += speed_x * time_step;
		my.y += speed_y * time_step;
		
		// Apply gravity
		speed_y -= 1 * time_step;
		
		// Rotate our fruit.
		my.pan += rotation * time_step;
		
		if(my.y < camera.bottom)
		{
			ent_remove(me);	// Remove the entity if it fell out of the screen
			return;			// Leave this action immidiately.
		}
		
		wait(1);
	}
}


// This action is used for the single parts of the sword trail
action sword_trail()
{
	my.tilt = -90;	// Let the sprite look flat to the camera.
	my.alpha = 100; 	// Not translucent, but needed for BRIGHT.
	set(my, BRIGHT);
	set(my, TRANSLUCENT);
	set(my, PASSABLE);
	wait(1);
	while(my.alpha > 0)
	{
		my.alpha -= 25 * time_step;	// Fading speed
		wait(1);
	}
	
	ent_remove(me);
}

// This function creates new fruits to be thrown
function throw_fruits()
{
	while(1)
	{
		wait(-1);	// Wait a second
		snd_play(sndThrow, 100, 0);
		
		// Get a position between left screen border and right screen border
		var pos_x = 0.5 * camera.left + random(0.5 * (camera.right - camera.left));
		
		ent_create(CUBE_MDL, vector(pos_x, camera.bottom, 0), fruit);
		
		// We don't need a wait(1) here because we have a wait(-1) at the top of the loop.
	}
}

// This function creates the mouse sword.
function create_sword()
{
	while(1)
	{
		while(!mouse_left)
		{
			// Do nothing while we don't press the mouse button
			wait(1);
		}
		
		// Store the mouse position in world coordinates.
		VECTOR start;
		vec_set(start, mouse_pos);
		vec_for_screen(start, camera);
		
		while(mouse_left)
		{
			// Get the new mouse position
			VECTOR pos;
			vec_set(pos, mouse_pos);
			vec_for_screen(pos, camera);
		
			// We need to move at list a little bit.
			if(vec_dist(pos, start) > 5)
			{
				ENTITY *entSwordTrail = ent_create("sword.png", vector(0, 0, 0), sword_trail);
			
				entSwordTrail.x = pos.x;
				entSwordTrail.y = pos.y;
				entSwordTrail.z = 32;
				entSwordTrail.scale_x = 0.125;							// Make the sprite smaller
				entSwordTrail.scale_y = vec_dist(pos, start) / 30.0;	// But scale it so that we have a continuous trail
				
				VECTOR dir;
				ANGLE angle;
				
				vec_diff(dir, start, pos);
				vec_to_angle(angle, dir);
			
				entSwordTrail.pan = angle.pan;
		
				// Create a trace with the mouse movement, so we can slice our fruits
				c_trace(vector(start.x, start.y, 0), vector(pos.x, pos.y, 0), IGNORE_PASSABLE | IGNORE_PASSENTS | ACTIVATE_SHOOT);
		
				vec_set(start, pos);
			}
			wait(1);
		}
	}
}

function main()
{
	video_set(1024, 768, 32, 2);
	level_load(NULL);

	// Setup the camera to look "down" on our field so we can use x-y like screen coordinates
	camera.x = 0;
	camera.y = 0;
	camera.z = 100;
	camera.pan = 90;
	camera.tilt = -90;
	set(camera, ISOMETRIC);
	
	// Setup camera area:
	camera.left = -100 * (screen_size.x / screen_size.y);
	camera.right = 100 * (screen_size.x / screen_size.y);
	camera.top = 100;
	camera.bottom = -100;
	
	mouse_mode = 4;

	throw_fruits();
	create_sword();
}