#include <acknex.h>
#include <default.c>

#include "..\\Source\\tust.h"

/* ----------------------------------------------------------------------------------------- */

VECTOR vtemp;
STRING *stemp = "";

ENTITY *entGlobal;
#define index		skill1
#define clock		skill2
#define speed		skill3

// Sphere state machine functions
void stEnt3 ( STMACHINE *stm );
void stEnt2 ( STMACHINE *stm );
void stEnt1 ( STMACHINE *stm );
void stEnt0 ( STMACHINE *stm );

// Panel state machine functions
void stPanBig ( STMACHINE *stm );
void stPanWane ( STMACHINE *stm );
void stPanGrow ( STMACHINE *stm );
void stPanOff ( STMACHINE *stm );

/* ----------------------------------------------------------------------------------------- */

void pan_remove_custom ( PANEL *panel )
{
	bmap_remove ( panel->bmap );
	pan_remove ( panel );
}

void panels_create ( int count )
{
	int i = 0;
	for ( ; i<count; i++ )
	{
		PANEL *panel = pan_create ( "flags=SHOW;", 1 );
		panel->bmap = bmap_createblack ( 256, 64, 24 );
		panel->pos_x = 0;
		panel->pos_y = screen_size.y - 128 - (i*70);
		panel->skill_x = (STRING*)str_create ( "point me" );
		pan_setstring ( panel, 0, 10, 20, font_create("Arial#24"), (STRING*)panel->skill_x );
		
		// Add a new state machine for the new panel
		stm_add ( panel, pan_remove_custom, stPanOff, 0, NULL );
	}
}

void ent_remove_custom ( ENTITY *ent )
{
	stm_delete_index ( stm_index_ptr(ent->index) );
	ent_remove ( ent );
}

void spheres_create ( int count )
{
	int i;
	for ( i=0; i<count; i++ )
	{
		vec_set ( &vtemp, vector(random(200)-100,random(200)-100,0) );
		ENTITY *ent = ent_create ( SPHERE_MDL, &vtemp, NULL );
		ent->clock = total_ticks + 10;
		stm_create_index ( ent->index );
		entGlobal = ent;
		
		// Add a new state machine for the new entity
		stm_add ( ent, ent_remove_custom, stEnt0, 0, stm_index_ptr(ent->index) );
	}
}

function main ()
{
	video_mode = 12;
	mouse_mode = 4;
	on_esc = NULL;
	fps_max = 60;
	wait(1);
	
	level_load ( "" );
	camera->x = -200;
	camera->z = 100;
	camera->tilt = -30;
	
	// Start automation
	stm_open ();
	
	panels_create ( 6 );
	
	while ( !key_esc )
	{
		wait(1);
	}
	
	// Stop automation
	stm_close ();
	wait(1); // stm_close needs to wait 1 frame before exiting :(
	
	sys_exit ( NULL );
}

/* ----------------------------------------------------------------------------------------- */

void stEnt3 ( STMACHINE *stm )
{
	me = stm_me ( stm );
	vec_scale ( &my->scale_x, 1+(time_step*0.6) );
	my->alpha *= 1-(time_step*0.4);
	if ( total_ticks < my->clock )
		return;
	stm_stop ( stm );
}

void stEnt2 ( STMACHINE *stm )
{
	me = stm_me ( stm );
	my->speed += time_step;
	my->z -= my->speed * time_step;
	if ( my->z < -50 )
	{
		my->speed *= -0.75;
		my->z = -50;
	}
	if ( total_ticks < my->clock )
		return;
	stm_set_state ( stm, stEnt3, 3 );
	my->clock += 4;
}

void stEnt1 ( STMACHINE *stm )
{
	me = stm_me ( stm );
	my->z -= my->speed * time_step;
	if ( total_ticks < my->clock )
		return;
	stm_set_state ( stm, stEnt2, 2 );
	my->clock += 20+random(30);
}

void stEnt0 ( STMACHINE *stm )
{
	me = stm_me ( stm );
	you = stm_me ( stm_by_index ( stm_index(entGlobal->index) ) );
	if ( you == me )
		vec_scale ( &my->scale_x, 1+(time_step*0.05) );
	else
		vec_scale ( &my->scale_x, 1-(time_step*0.05) );
	if ( total_ticks < my->clock )
		return;
	stm_set_state ( stm, stEnt1, 1 );
	my->speed = random(1)-1.3;
	my->clock += 10+random(20);
	set ( me, TRANSLUCENT );
}

/* ----------------------------------------------------------------------------------------- */

void stPanBig ( STMACHINE *stm )
{
	PANEL *panMe = stm_me ( stm );
	if ( mouse_left )
		spheres_create ( 6 );
	if ( mouse_panel == panMe )
		return;
	stm_set_state ( stm, stPanWane, 3 );
	str_cpy ( (STRING*)panMe->skill_x, "wanning" );
}

void stPanWane ( STMACHINE *stm )
{
	PANEL *panMe = stm_me ( stm );
	if ( mouse_panel == panMe )
	{
		stm_set_state ( stm, stPanGrow, 1 );
		str_cpy ( (STRING*)panMe->skill_x, "growing" );
		return;
	}
	panMe->scale_x -= time_step * 0.2;
	if ( panMe->scale_x > 1 )
		return;
	panMe->scale_x = 1;
	stm_set_state ( stm, stPanOff, 0 );
	str_cpy ( (STRING*)panMe->skill_x, "point me" );
}

void stPanGrow ( STMACHINE *stm )
{
	PANEL *panMe = stm_me ( stm );
	if ( mouse_panel != panMe )
	{
		stm_set_state ( stm, stPanWane, 3 );
		str_cpy ( (STRING*)panMe->skill_x, "wanning" );
		return;
	}
	panMe->scale_x += time_step * 0.2;
	if ( panMe->scale_x < 2 )
		return;
	panMe->scale_x = 2;
	stm_set_state ( stm, stPanBig, 2 );
	str_cpy ( (STRING*)panMe->skill_x, "click on me" );
}

void stPanOff ( STMACHINE *stm )
{
	PANEL *panMe = stm_me ( stm );
	if ( mouse_panel != panMe )
		return;
	stm_set_state ( stm, stPanGrow, 1 );
	str_cpy ( (STRING*)panMe->skill_x, "growing" );
}

