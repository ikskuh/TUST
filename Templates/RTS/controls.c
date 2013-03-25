#ifndef _CONTROLS_C_
#define _CONTROLS_C_

#include "unit.c"

BMAP *controlButtonNormal = "controlbutton_normal.tga";
BMAP *controlButtonOver = "controlbutton_over.tga";

BMAP *controlItemBarracks = "item_barracks.tga";

PANEL *controlsPanel = 
{
	bmap = "controlpanel.tga";
	layer = 10;
	flags = SHOW;
}

PANEL *controlsPanelNoUnits = 
{
	button( 16, 16, controlButtonOver, controlButtonNormal, controlButtonOver, barracks_place, NULL, NULL);
	window( 16, 16, 96, 96, controlItemBarracks, NULL, NULL);
	layer = 11;
	flags = SHOW;
}

void controls_start()
{
	camera.tilt = -45;
	VECTOR cameraSpot;
	cameraSpot.x = 0;
	cameraSpot.y = 0;
	cameraSpot.z = 0;
	var cameraDist = 1000;
	
	while(1)
	{
		// GUI Update
		controlsPanel.pos_y = screen_size.y - 128;
		controlsPanel.size_x = screen_size.x;
		
		controlsPanelNoUnits.pos_x = controlsPanel.pos_x;
		controlsPanelNoUnits.pos_y = controlsPanel.pos_y;
		
		// Camera movement
		camera.pan += 0.5 * mouse_right * mickey.x;
		camera.tilt = clamp(camera.tilt - 0.5 * mouse_right * mickey.y, -90, -20);
		cameraDist = clamp(cameraDist - mickey.z / 1.2, 200, 2000);
		
		VECTOR offset;
		offset.x = -4 * mouse_middle * mickey.y;
		offset.y = -4 * mouse_middle * mickey.x;
		offset.z = 0;
		
		vec_rotate(offset, vector(camera.pan, 0, 0));
		vec_add(cameraSpot.x, offset);
		
		if(c_trace(vector(cameraSpot.x, cameraSpot.y, cameraSpot.z + 1000), vector(cameraSpot.x, cameraSpot.y, cameraSpot.z - 1000), IGNORE_MODELS | IGNORE_SPRITES | USE_POLYGON))
		{
			cameraSpot.z = target.z;
		}
		
		vec_set(camera.x, vector(-cameraDist, 0, 0));
		vec_rotate(camera.x, camera.pan);
		vec_add(camera.x, cameraSpot);
		wait(1);
	}
}

#endif // #ifndef _CONTROLS_C_