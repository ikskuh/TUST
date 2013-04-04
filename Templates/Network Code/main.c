#include <acknex.h>
#include <default.c>
#include "..\\..\\Source\\tust.h"

// Global ressources
#define PRAGMA_PATH "..\\..\\Ressources\\Models\\Characters"
#define PRAGMA_PATH "..\\..\\Ressources\\Models\\Buildings"
#define PRAGMA_PATH "..\\..\\Ressources\\Terrains\\"

STRING *sessionName = "gs_netsession";
STRING *defaultHost = "127.0.0.1";

void run_server();
void run_client();
void run_game();

function main()
{
	video_set(800, 600, 32, 2);
	fps_max = 60;
	
	// Initialize global network settings
	dplay_port = 2300;			// Default port
	dplay_entrate = 0.5;		// 2 Updates per tick
	dplay_pingrate = 8;			// Two ping messages per second
	dplay_localfunction = 2;	// Actions run on server and client
	dplay_smooth = 0;			// No smoothing
	dplay_timeout = 80;			// 5 Seconds until disconnect message
	
	// Simple decision if user creates a server or a client
	while(1)
	{
		draw_text("Press [C] for local client\nPress [S] for hosting a game", 4, 4, COLOR_RED);
		if(key_c)
		{
			run_client();
			return;
		}
		if(key_s)
		{
			run_server();
			return;
		}
		wait(1);
	}
}

// The server event function
void on_server_event(void *data)
{
	// Just a prototype event handler
	switch(event_type)
	{
		case EVENT_JOIN:
			break;
		case EVENT_LEAVE:
			break;
		case EVENT_STRING:
			break;
		case EVENT_VAR:
			break;
		case EVENT_DATA:
			break;
	}
}

// Starts a server
void run_server()
{
	// Open a session
	session_open(sessionName);
	
	// Load the server level
	level_load("small.hmp");
	
	// Start the game main loop
	run_game();
}

// The client event function
void on_client_event(void *data)
{
	switch(event_type)
	{
		case EVENT_JOIN:
			// We joined a server, now load the level the server currently has.
			level_load(data);
			
			// Start the game main loop
			run_game();
			break;
		case EVENT_LEAVE:
			// Gets called if we get kicked or lose the connection to the server
			break;
		case EVENT_STRING:
			break;
		case EVENT_VAR:
			break;
		case EVENT_DATA:
			break;
	}
}

void run_client()
{
	// Starts a session and connects to the default host
	session_connect(sessionName, defaultHost);
}

// Our players action
void actPlayer()
{
	// Enable network synchronisation
	net_entsync(me);
	
	// Wait for our entity to get a valid handle
	while(my.client_id < 0) wait(1);
	if(my.client_id == dplay_id)
	{
		// We created the entity, so we control it.
		while(me)
		{
			// Camera and player movement
			camera.pan -= 0.5 * mickey.x;
			camera.tilt = clamp(camera.tilt - 0.5 * mickey.y, -80, 80);
			my.pan = camera.pan;
			
			VECTOR dir;
			dir.x = key_w - key_s;
			dir.y = key_a - key_d;
			dir.z = 0;
			vec_normalize(dir, 5 * time_step);
			vec_rotate(dir, vector(my.pan, 0, 0));
			vec_add(my.x, dir);
			
			// Place model on ground
			if(c_trace(my.x, vector(my.x, my.y, my.z - 1000), IGNORE_ME | USE_POLYGON | IGNORE_PASSABLE | IGNORE_PASSENTS))
			{
				my.z = target.z + 32;
			}
			
			vec_set(camera.x, my.x);
			camera.z += 32;
			
			// We don't need any network code in this action, it's all done by net_entsync.
			
			wait(1);
		}
	}
}

void run_game()
{
	// Create player model:
	ENTITY *entPlayer = ent_create("warlock.mdl", vector(random(512)-256, random(512)-256, 0), actPlayer);
	while(1)
	{
		// Draw some server/client information.
		if(connection & 1) // Check if the connection has the server bit set.
			draw_text("Server", 4, 4, COLOR_RED);
		else
			draw_text("Client", 4, 4, COLOR_RED);
		wait(1);
	}
}
























