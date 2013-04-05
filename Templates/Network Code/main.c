#include <acknex.h>
#include <default.c>
#include "..\\..\\Source\\tust.h"

// Global ressources
#define PRAGMA_PATH "..\\..\\Ressources\\Models\\Characters"
#define PRAGMA_PATH "..\\..\\Ressources\\Models\\Buildings"
#define PRAGMA_PATH "..\\..\\Ressources\\Terrains\\"

STRING *sessionName = "gs_netsession";
STRING *defaultHost = "127.0.0.1";

// A lot of space for our chat
STRING *chatLog = "#1024";

// Our chat message to send around
STRING *chatMessage = "#256";

// The chat message input
STRING *chatInput = "#256";

void run_server();
void run_client();
void run_game();

// Chat "window"
TEXT *txtChat = 
{
	pos_x = 10;
	pos_y = 10;
	string = (chatLog, chatInput);
	flags = SHOW;
}

// Adds a message to the chat log
void chat_add(STRING *msg)
{
	// Add the received message to the chat log
	str_cat(chatLog, msg);
	str_cat(chatLog, "\n");
}

// We allow the user to enter a name
TEXT *txtName = 
{
	string = player_name;
	font = "Arial#24b";
	flags = CENTER_Y | CENTER_Y;
}

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
	
	// Empty the chat log
	str_cpy(chatLog, "");
	str_cpy(player_name, "Anonymous!");
	
	// Simple decision if user creates a server or a client
	while(1)
	{
		draw_text(str_printf(NULL, "Welcome, %s!\nPress [C] for local client\nPress [S] for hosting a game\nPress [N] for changing the user name", _chr(player_name)), 4, 4, COLOR_RED);
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
		if(key_n)
		{
			txtName.pos_x = screen_size.x / 2;
			txtName.pos_y = screen_size.y / 2;
			set(txtName, SHOW);
			STRING *previous = "#256";
			str_cpy(previous, player_name); // Store the previous player name
			
			var result = inkey(player_name);
			if(result != 13)
				str_cpy(player_name, previous);	// Restore the old name if we cancelled the name input
			
			reset(txtName, SHOW);
		}
		wait(1);
	}
}

// The server event function
void on_server_event(void *data, var sender)
{
	// Just a prototype event handler
	switch(event_type)
	{
		case EVENT_JOIN:
			break;
		case EVENT_LEAVE:
			break;
		case EVENT_STRING:
			if(data == chatMessage)
			{
				// If the server received a chat message, add the name and broadcast it
				STRING *newMessage = "#256";
				str_for_id(newMessage, sender);
				str_cat(newMessage, ": ");
				str_cat(newMessage, chatMessage);
				str_cpy(chatMessage, newMessage);
				
				send_string(chatMessage);
				chat_add(chatMessage);
				
				// Reset the chat message with 256 spaces to receive another message
				STRING *empty = "#256";
				str_cpy(chatMessage, empty);
			}
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
			if(data == chatMessage)
			{
				// If the client received a chat message, add it to the chat log
				chat_add(chatMessage);
				
				// Reset the chat message with 256 spaces to receive another message
				STRING *empty = "#256";
				str_cpy(chatMessage, empty);
			}
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

void chat_message()
{
	STRING *empty = "#256";
	str_cpy(chatInput, empty);
	
	// Input a chat text
	var r = inkey(chatInput);
	if(r == 13)
	{
		if(connection & 1)
		{
			// Send message from server to all
			str_cpy(chatMessage, player_name);
			str_cat(chatMessage, ": ");
			str_cat(chatMessage, chatInput);
			chat_add(chatMessage);
			send_string(chatMessage);
		}
		else
		{
			// Send message from client to server!
			str_cpy(chatMessage, chatInput);
			send_string(chatMessage);
		}
	}
	// Clear the messages
	str_cpy(chatMessage, empty);
	str_cpy(chatInput, empty);
}

void run_game()
{
	// Create player model:
	ENTITY *entPlayer = ent_create("warlock.mdl", vector(random(512)-256, random(512)-256, 0), actPlayer);
	on_enter = chat_message;
	while(1)
	{
		// Draw some server/client information.
		if(connection & 1) // Check if the connection has the server bit set.
			draw_text("Server", screen_size.x - 60, 4, COLOR_RED);
		else
			draw_text("Client", screen_size.x - 60, 4, COLOR_RED);
		wait(1);
	}
}
























