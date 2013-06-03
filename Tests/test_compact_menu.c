#include <acknex.h>
#include "..\\Source\\tust.h"

#define PRAGMA_POINTER
#define PRAGMA_PATH "%EXE_DIR%\\templates\\sounds"
#define PRAGMA_PATH "%EXE_DIR%\\templates\\images"


/* ---------------------------------------------------------------------------------------------------- */
TEXT *txtMain = 
{
	string = 
	(
		"main menu.title=0",
		".space=1",
	 	"camera.submenu=txtCamera",
	 	"global lighting.submenu=txtGlobalLighting",
	 	"sky.submenu=txtSky",
		".space=3",
	 	"system.submenu=txtSystem"
		".space=1",
		".line=1",
		".title=0"
	);
}

/* ---------------------------------------------------------------------------------------------------- */
var nCameraDistance = 100;
void evnCameraLocate ()
{
	vec_for_angle ( &camera->x, &camera->pan );
	vec_normalize ( &camera->x, -nCameraDistance );
}

TEXT *txtCamera =
{
	string =
	(
		".line=0",
		"pan.slider=-180,180,1,%.0f,camera->pan,evnCameraLocate",
		"tilt.slider=-90,90,1,%.0f,camera->tilt,evnCameraLocate",
		"roll.slider=-180,180,1,%.0f,camera->roll,evnCameraLocate",
		".space=1",
		"distance.slider=10,200,1,%.0f,nCameraDistance,evnCameraLocate",
		"arc.slider=1,179,1,%.0f,camera->arc",
		"ambient.slider=0,100,1,%.0f,camera->ambient",
		".space=1",
		"background color.submenu=txtBackgroundColor",
		".line=2"
	);
}

/* ---------------------------------------------------------------------------------------------------- */
COLOR colCameraBG;

void evnCameraBGSet ()
{
	camera->bg = pixel_for_vec ( &colCameraBG, 100, 8888 );
}

TEXT *txtBackgroundColor =
{
	string =
	(
		".line=0",
		"blue.slider=0,255,1,%.0f,colCameraBG.blue,evnCameraBGSet",
		"green.slider=0,255,1,%.0f,colCameraBG.green,evnCameraBGSet",
		"red.slider=0,255,1,%.0f,colCameraBG.red,evnCameraBGSet",
		".line=2"
	);
}

/* ---------------------------------------------------------------------------------------------------- */
TEXT *txtGlobalLighting =
{
	string =
	(
		".line=0",
		"sun.submenu=txtSun",
		"ambient.submenu=txtAmbient",
		".line=2"
	);
}

/* ---------------------------------------------------------------------------------------------------- */
TEXT *txtSun =
{
	string =
	(
		".line=0",
		"pan.slider=-180,180,1,%.0f,sun_angle.pan",
		"tilt.slider=-90,90,1,%.0f,sun_angle.tilt",
		".space=1",
		"distance (roll).slider=1,100000,1,%.0f,sun_angle.roll",
		".space=1",
		"blue.slider=0,255,1,%.0f,sun_color.blue",
		"green.slider=0,255,1,%.0f,sun_color.green",
		"red.slider=0,255,1,%.0f,sun_color.red",
		".line=2"
	);
}

/* ---------------------------------------------------------------------------------------------------- */
TEXT *txtAmbient =
{
	string =
	(
		".line=0",
		"blue.slider=0,255,1,%.0f,ambient_color.blue",
		"green.slider=0,255,1,%.0f,ambient_color.green",
		"red.slider=0,255,1,%.0f,ambient_color.red",
		".line=2"
	);
}

/* ---------------------------------------------------------------------------------------------------- */
BMAP *bmpSky = NULL;

TEXT *txtSky =
{
	string =
	(
		".line=0",
		".bmap=bmpSky",
		".line=2"
	);
}

/* ---------------------------------------------------------------------------------------------------- */
SOUND *sndBeep = "beep.wav";

void fncBeep ( void *object )
{
	snd_play ( sndBeep, 50, 0 );
}

void fncExit ( void *object )
{
	cmenu_remove_all ();
	
	sys_exit ( NULL );
}

TEXT *txtSystem =
{
	string =
	(
		".line=0",
	 	"beep.button=fncBeep",
	 	"exit.button=fncExit",
		".line=2"
	);
}

/* ---------------------------------------------------------------------------------------------------- */

void main ()
{
	video_mode = 8;
	wait(2);
	bmap_zbuffer ( bmap_createblack(2048,2048,32) );
	mouse_mode = 4;
	mouse_map = bmap_create ( "arrow_yellow.pcx" );
	vec_fill ( &screen_color, 128 );
	
	level_load ( "" );
	camera->pan = -40;
	camera->tilt = -30;
	evnCameraLocate ();
	vec_set ( &colCameraBG, vector(150,150,150) );
	camera->bg = pixel_for_vec ( &colCameraBG, 100, 8888 );
	
	ENTITY *ent = ent_create ( CUBE_MDL, nullvector, NULL );
	FONT *fntArial = font_create ( "Arial#14" );

	
	// Create a compact menu panel
	// PANEL *cmenu_create ( var pos_x, var pos_y, var size_x, var layer, var flags, TEXT* txtMembers );
	PANEL *myMenu01 = cmenu_create ( "myMenu01", 120, 20, 400, 1, SHOW, "txtMain", fntArial );
	PANEL *myMenu02 = cmenu_create ( "myMenu02", 520, 20, 400, 1, SHOW, "txtMain", fntArial );
	
	cmenu_resize ( myMenu01, 200 );
	
	bmpSky = bmap_create ( "sky_fu_256+6.tga" );
	
	while ( !key_esc )
		wait(1);
	
	bmap_remove ( bmpSky );
	bmap_remove ( mouse_map );
	mouse_map = NULL;
	bmpSky = NULL;
	
	cmenu_remove ( myMenu01 );
	cmenu_remove ( myMenu02 );
	
	sys_exit ( NULL );

}