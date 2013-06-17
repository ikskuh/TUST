
#ifdef CM_TEST01_PANEL

/* ---------------------------------------------------------------------------------------------------- */
TEXT *txtCMTest01 = 
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
var cmTimer2 = 0;
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
		"x.digit=3,camera->x",
		"y.digit=3,camera->y",
		"z.digit=3,camera->z",
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
	sys_exit ( NULL );
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

#endif