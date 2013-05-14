#include <acknex.h>
#include <default.c>

// Define skill1 is also named BAD_FRUIT
#define BAD_FRUIT	skill1
#define SPEED_X		skill2
#define SPEED_Y		skill3
#define ROTATION	skill4

SOUND *sndThrow = "throw.wav";
SOUND *sndSlice = "slice.wav";
SOUND *sndBoom = "boom.wav";

FONT *fontCounter = "Arial#64b";

var fruitCount = 0;
var fruitTimer = 1.0;

var minuteTimer = 1;
var secondTimer = 30;

var mouse_x = 0;
var mouse_y = 0;

STRING *strTimerValue = "#6";

PANEL *panelFruitCounter = 
{
	digits( 10, 10, "%3.0f", fontCounter, 1, fruitCount);
	flags = SHOW;
}

PANEL *panelTimeCounter = 
{
	digits( 10, 10, "%s", fontCounter, 1, strTimerValue);
	flags = SHOW;
}

action explosion()
{
	my.tilt = -90;
	my.pan = random(360);
	my.alpha = 100;
	set(me, BRIGHT);
	set(me, TRANSLUCENT);
	wait(1);
	while(my.frame < 13)
	{
		my.frame += 1.5 * time_step;
		wait(1);
	}
	ent_remove(me);
}

action physical_item()
{
	while(me)
	{
		// Move our fruit
		my.x += my.SPEED_X * time_step;
		my.y += my.SPEED_Y * time_step;
		
		// Apply gravity
		my.SPEED_Y -= 1 * time_step;
		
		// Rotate our fruit.
		my.pan += my.ROTATION * time_step;
		
		wait(1);
	}
}

action fruit_drop()
{
	set(me, SHADOW);
	
	// Activate physical properties
	physical_item();
	
	while(me)
	{
		if(my.y < camera.bottom)
		{
			ent_remove(me);	// Remove the entity if it fell out of the screen
			return;			// Leave this action immidiately.
		}
		
		wait(1);
	}
}

action fruit_splatter()
{
	my.tilt = -90;
	set(me, TRANSLUCENT);
	set(me, BRIGHT);
	set(me, LIGHT);
	my.alpha = 100;
	vec_fill(my.scale_x, 0.25);
	wait(-1.0);
	for(my.alpha = 100; my.alpha > 0; my.alpha -= time_step) wait(1);
	ent_remove(me);
}

function fruit_event()
{
	switch(event_type)
	{
		case EVENT_SHOOT:
			// We hit the fruit, remove it.
			wait(1);	// Don't forget to wait one frame in an entity event
			
			if(my.BAD_FRUIT != 0)
			{
				// Create explosion
				snd_play(sndBoom, 100, 0);	// Play a sound for an explosion
				ent_create("explo+13.tga", my.x, explosion);
				
				fruitCount -= 10;			// Remove player points
				
				// Loop through all entities
				for(you = ent_next(NULL); you != NULL; you = ent_next(you))
				{					
					// Add an explosion force to all entities
					VECTOR dir;
					vec_diff(dir, you.x, my.x);		// Get explosion direction
					vec_normalize(dir, 5);			// Adjust explosion strength
					
					you.SPEED_X += dir.x;
					you.SPEED_Y += dir.y;
				}
				you = ent_create("explosion.png", vector(my.x + mouse_x, my.y + mouse_y, -34), fruit_splatter);
				reset(you, BRIGHT);
			}
			else
			{
				snd_play(sndSlice, 100, 0);	// Play a sound for a sliced fruit.
				
				fruitCount += 1;			// Add a fruit to our high score
				fruitTimer -= 0.01;		// Slowly reduce the fruit throwing time
				if(fruitTimer < 0.25)
					fruitTimer = 0.25;	// , but limit it to 0.25.
				
				// Calculate the speed offset for the two new parts
				VECTOR sliceSpeed;
				vec_set(sliceSpeed, vector(2 + random(2), 0, 0));
				vec_rotate(sliceSpeed, my.pan);
				
				if(my.string1 != NULL)
				{
					// Create the first slice part (if any)
					you = ent_create(my.string1, my.x, fruit_drop);
					you.pan = my.pan;
					you.SPEED_X -= sliceSpeed.x;
					you.SPEED_Y -= sliceSpeed.y;
				}
				if(my.string2 != NULL)
				{
					// Create the second slice part (if any)
					you = ent_create(my.string2, my.x, fruit_drop);
					you.pan = my.pan;
					you.SPEED_X += sliceSpeed.x;
					you.SPEED_Y += sliceSpeed.y;
				}
				int splatter = integer(random(2));
				switch(splatter)
				{
					case 0:
						you = ent_create("splatter1.png", vector(my.x + mouse_x, my.y + mouse_y, -34), fruit_splatter);
						break;
					case 1:
						you = ent_create("splatter2.png", vector(my.x + mouse_x, my.y + mouse_y, -34), fruit_splatter);
						break;
				}
				vec_set(you.blue, my.blue);
				vec_to_angle(you.pan, vector(mouse_x, mouse_y, 0));
				you.tilt = -90;
				you.roll = 0;
			}
			ent_remove(me);
			break;
	}
}

// This action is used for the fruits beeing thrown up.
action fruit()
{
	my.SPEED_X = random(2) - 1;	// Throw a little bit left or right
	my.SPEED_Y = 15 + random(5);	// Throw up
	
	// We want the model to rotate in the direction of our throw:
	my.ROTATION = 
		-sign(my.SPEED_X) * 			// Get the direction of the rotation
		(5 + random(15));			// Randomize the rotation speed between 5 and 15
	
	my.emask = ENABLE_SHOOT;
	my.event = fruit_event;
	
	set(me, SHADOW);
	
	// Activate physical properties
	physical_item();
	
	while(me)
	{
		if(my.y < camera.bottom)
		{
			if(my.BAD_FRUIT == 0)
			{
				// A fruit drops out of the screen bottom, reduce points:
				fruitCount -= 1;
			}
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
		wait(-fruitTimer);	// Wait a second
		snd_play(sndThrow, 100, 0);
		
		// Get a position between left screen border and right screen border
		var pos_x = 0.75 * camera.left + random(0.75 * (camera.right - camera.left));
		
		// Select a model and create it.
		int iModel = integer(random(4));
		switch(iModel)
		{
			case 0:
				you = ent_create(CUBE_MDL, vector(pos_x, camera.bottom, 0), fruit);
				vec_set(you.blue, vector(255, 255, 255));
				break;
			case 1:
				you = ent_create("nectarine.mdl", vector(pos_x, camera.bottom, 0), fruit);
				you.string1 = "nectarine_right.mdl";
				you.string2 = "nectarine_left.mdl";
				vec_set(you.blue, vector(0, 96, 255));
				break;
			case 2:
				you = ent_create("banana.mdl", vector(pos_x, camera.bottom, 0), fruit);
				you.string1 = "banana_right.mdl";
				you.string2 = "banana_left.mdl";
				vec_set(you.blue, vector(21, 198, 255));
				break;
			case 3:
				// Create a bomb
				you = ent_create(SPHERE_MDL, vector(pos_x, camera.bottom, 0), fruit);
				set(you, UNLIT);	// Make it dark
				you.BAD_FRUIT = 1;	// Mark it as a bad fruit / bomb
				break;
		}

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
				
				mouse_x = pos.x - start.x;
				mouse_y = pos.y - start.y;
				
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

function game_timer()
{
	minuteTimer = 1;
	secondTimer = 30;
	while(minuteTimer > 0 || secondTimer > 0)
	{
		wait(-1);	// Wait a second
		secondTimer -= 1;
		if(secondTimer <= 0)
		{
			minuteTimer -= 1;
			secondTimer = 60;
		}
	}
}

function main()
{
	video_set(1024, 768, 32, 2);
	shadow_stencil = 2;
	
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
	
	sun_angle.pan = 30;
	sun_angle.tilt = 80;

	throw_fruits();
	create_sword();
	game_timer();
	
	ENTITY *background = ent_create("Background.jpg", vector(0, 0, -35), NULL);
	background.tilt = -90;
	background.pan = 90;
	set(background, LIGHT);	// Brighten it up
	
	// Adjust the background to the screen resolution so we have a 
	background.scale_x = 1.1 * (camera.right - camera.left) / (background.max_x - background.min_x);
	background.scale_y = 1.1 * (camera.top - camera.bottom) / (background.max_y - background.min_y) * (screen_size.x / screen_size.y);
	
	// Adjust the timer panel
	panelTimeCounter.pos_x = screen_size.x - 200;
	
	while(1)
	{
		str_printf(strTimerValue, "%2d:%02d", (int)minuteTimer, (int)secondTimer);
		wait(1);
	}
}












