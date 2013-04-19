#include <acknex.h>

SOUND *sndThrow = "throw.wav";
SOUND *sndSlice = "slice.wav";

// This action is used for the fruits beeing thrown up.
action fruit()
{
	var speed_x = random(2) - 1;	// Throw a little bit left or right
	var speed_y = 15 + random(5);	// Throw up
	var rotation = random(20) - 10;	// Rotation speed
	while(me)
	{
		// Move our fruit
		my.x += speed_x * time_step;
		my.y += speed_y * time_step;
		
		// Apply gravity
		speed_y -= 1 * time_step;
		
		// Rotate our fruit.
		my.pan += rotation * time_step;
		
		wait(1);
	}
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
	}
}

function main()
{
	video_set(1024, 768, 32, 2);
	level_load(NULL);

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

	throw_fruits();
}