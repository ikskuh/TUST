#include <acknex.h>
#include "..\\Source\\compact_menu.h"

#define PRAGMA_POINTER

#define PRAGMA_PATH "%EXE_DIR%\\templates\\sounds"
#define PRAGMA_PATH "%EXE_DIR%\\templates\\images"


/* ---------------------------------------------------------------------------------------------------- */
var nSlider = 0;
var nDigit = 0;
SOUND *sndBeep = "beep.wav";

void fncBeep ( void *object )
{
	snd_play ( sndBeep, 50, 0 );
}

TEXT *txtMain = 
{
	string = 
	(
		".line=2",
	 	".bitmap=bmpSky",
		".line=2",
		"title.title=0",
		"digit.digit=3,nDigit",
	 	"button.button=fncBeep"
	 	"rbutton.rbutton=fncBeep"
		"slider.slider=-180,180,1,0,nSlider",
		"digedit.digedit=3,nDigit",
		"submenu.submenu=txtGlobals",
		".space=2"
	);
}

/* ---------------------------------------------------------------------------------------------------- */
TEXT *txtGlobals = 
{
	string = 
	(
		".line=0"
	 	"camera.submenu=txtCamera",
	 	"lighting.submenu=txtGlobalLighting",
	 	"sky.submenu=txtSky",
		".space=3",
	 	"system.submenu=txtSystem"
		".line=2"
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
		"x.digit=3,camera->x,evnCameraLocate",
		"y.digit=3,camera->y,evnCameraLocate",
		"z.digit=3,camera->z,evnCameraLocate",
		".space=1",
		"pan.slider=-180,180,1,0,camera->pan,evnCameraLocate",
		"tilt.slider=-90,90,1,0,camera->tilt,evnCameraLocate",
		"roll.slider=-180,180,1,0,camera->roll,evnCameraLocate",
		".space=1",
		"distance.digedit=3,nCameraDistance,evnCameraLocate",
		"arc.slider=1,179,1,0,camera->arc",
		"ambient.slider=-100,100,1,0,camera->ambient",
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
		"blue.slider=0,255,1,0,colCameraBG.blue,evnCameraBGSet",
		"green.slider=0,255,1,0,colCameraBG.green,evnCameraBGSet",
		"red.slider=0,255,1,0,colCameraBG.red,evnCameraBGSet",
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
		"angles.title=0",
		"pan.slider=-180,180,1,0,sun_angle.pan",
		"tilt.slider=-90,90,1,0,sun_angle.tilt",
		".space=1",
		"distance (roll).digedit=0,sun_angle.roll",
		"color.title=0",
		"blue.slider=0,255,1,0,sun_color.blue",
		"green.slider=0,255,1,0,sun_color.green",
		"red.slider=0,255,1,0,sun_color.red",
		".line=2"
	);
}

/* ---------------------------------------------------------------------------------------------------- */
TEXT *txtAmbient =
{
	string =
	(
		".line=0",
		"blue.slider=0,255,1,0,ambient_color.blue",
		"green.slider=0,255,1,0,ambient_color.green",
		"red.slider=0,255,1,0,ambient_color.red",
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
		".bitmap=bmpSky",
		".line=2"
	);
}

/* ---------------------------------------------------------------------------------------------------- */
var nExit = 0;
void fncExit ( void *object )
{
	nExit = 1;
}

TEXT *txtSystem =
{
	string =
	(
		".line=0",
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
	
	FONT *fntTTF = font_create("Arial#16");
	FONT *fntBitmap = font_create("ackfont.pcx");
	// Create style for the menues
	// CMMEMBER *myMenuStyle = cmstyle_create ( FONT *font, COLOR *colText, COLOR *colBack, COLOR *colOver )
	CMSTYLE *myMenuStyle01 = cmstyle_create ( fntTTF, vector(40,40,40), vector(250,250,250), vector(210,210,210) );
	CMSTYLE *myMenuStyle02 = cmstyle_create ( fntBitmap, vector(170,170,255), vector(30,20,0), vector(0,0,185) );
	
	// Create a compact menu panel
	// PANEL *cmenu_create ( char *chrMember, var pos_x, var pos_y, var size_x, var layer, var flags, CMSTYLE *style )
	PANEL *myMenu01 = cmenu_create ( "menu members.submenu=txtMain", 100, 20, 200, 1, SHOW, myMenuStyle01 );
	PANEL *myMenu02 = cmenu_create ( "global variables.submenu=txtGlobals", 500, 20, 200, 1, SHOW, myMenuStyle01 );
	
	cmenu_modify ( myMenu02, 150, myMenuStyle02 );
	
	bmpSky = bmap_create ( "sky_fu_256+6.tga" );
	
	while ( !key_esc && !nExit )
		wait(1);
	
	bmap_remove ( bmpSky );
	bmap_remove ( mouse_map );
	mouse_map = NULL;
	bmpSky = NULL;
	cmenu_remove ( myMenu01 );
	cmenu_remove ( myMenu02 );
	sys_free ( myMenuStyle01 ); 
	sys_free ( myMenuStyle02 );
	font_remove ( fntTTF ); 
	font_remove ( fntBitmap ); 
	
	sys_exit ( NULL );

}