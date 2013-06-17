#include <acknex.h>
//#include <default.c>
#include "..\\Source\\compact_menu.h"

#define PRAGMA_POINTER

#define PRAGMA_PATH "%EXE_DIR%\\templates\\sounds"
#define PRAGMA_PATH "%EXE_DIR%\\templates\\images"

var nSlider = 0;
var nDigit = 0;
var *ptrDigit = NULL;
var *ptrSlider = NULL;
SOUND *sndBeep = "beep.wav";

void txtMain_startup ()
{
	ptrDigit = &nDigit;
	ptrSlider = &nSlider;
}

void fncBeep ( void *object )
{
	snd_play ( sndBeep, 50, 0 );
}

var myTimer = 0;
var mytimer_fill ()
{
	return cmdigitValue + ( time_step / 16 );
}

TEXT *txtMain = 
{
	string = 
	(
		".line=2",
	 	".bitmap=bmpSky",
		".line=2",
		"title.title=0",
		"digit (var).digit=3,nDigit",
		"digit (var*).digit=3,ptrDigit",
		"digit (var return).digit=0,myTimer,mytimer_fill",
	 	"button.button=fncBeep"
	 	"rbutton.rbutton=fncBeep"
		"slider (var).slider=-180,180,1,0,nSlider",
		"slider (var*).slider=-180,180,1,0,ptrSlider",
		"digedit (var).digedit=3,nDigit",
		"digedit (var*).digedit=3,ptrDigit",
		"submenu.submenu=txtCMTest01",
		".space=2"
	);
}


void main ()
{
	video_mode = 8;
	fps_max = 60;
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
	
	// Create style for the menues
	// CMMEMBER *myMenuStyle = cmstyle_create ( FONT *font, COLOR *colText, COLOR *colBack, COLOR *colOver )
	FONT *fntTTF = font_create("Arial#16");
	CMSTYLE *myMenuStyle01 = cmstyle_create ( fntTTF, vector(40,40,40), vector(250,250,250), vector(210,210,210) );
	FONT *fntBitmap = font_create("ackfont.pcx");
	CMSTYLE *myMenuStyle02 = cmstyle_create ( fntBitmap, vector(170,170,255), vector(30,20,0), vector(0,0,185) );
	
	// Create a compact menu panel
	// PANEL *cmenu_create ( char *chrMember, var pos_x, var pos_y, var size_x, var layer, var flags, CMSTYLE *style )
	PANEL *myMenu01 = cmenu_create ( "menu name.submenu=txtMain", 110, 20, 200, 1, SHOW, myMenuStyle01 );
	PANEL *myMenu02 = cmenu_create ( "debug & statics.submenu=txtCMDebug", 500, 20, 200, 1, SHOW, myMenuStyle01 );
	
	cmenu_modify ( myMenu02, 120, myMenuStyle02 );
	
	bmpSky = bmap_create ( "sky_fu_256+6.tga" );
	
	while ( !key_esc && !nExit )
		wait(1);
	
	bmap_remove ( bmpSky );
	bmpSky = NULL;
	bmap_remove ( mouse_map );
	mouse_map = NULL;
	cmenu_remove ( myMenu01 );
	sys_free ( myMenuStyle01 ); 
	font_remove ( fntTTF ); 
	cmenu_remove ( myMenu02 );
	sys_free ( myMenuStyle02 );
	font_remove ( fntBitmap ); 
	
	sys_exit ( NULL );

}