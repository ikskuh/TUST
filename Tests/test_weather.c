#include <acknex.h>
#include <default.c>
#include <mtlView.c>
#include "..\\Source\\weather.h"


MATERIAL* matGodRay = {
	effect = "godrays.fx";
}

TEXT* txtDescription = {
	flags = SHOW | OUTLINE;
	pos_x = 10;
	pos_y = 10;
	font = "Arial#14";
	string("1 - Rain",
			 "2 - Snow",
			 "3 - Sun",
			 "4 - Storm",
			 "5 - Day",
			 "6 - Night",
			 "7 - Morning",
			 "8 - Evening",
			 "9 - Dynamic day night",
			 "q - Static day night");
	layer = 1;
}

void main() {
	video_mode = 10;
	level_load("..\\Ressources\\Terrains\\small.hmp");
	
	//ENTITY* entTest = ent_create(SPHERE_MDL, vector(100,0,0), NULL);
	//entTest.material = matGodRay;
	random_seed(0);
	sky_init();
	
	on_1 = weather_rain;
	on_2 = weather_snow;
	on_3 = weather_sun;
	on_4 = weather_thunder;
	on_5 = weather_day;
	on_6 = weather_night;
	on_7 = weather_morning;
	on_8 = weather_evening;
	on_9 = weather_daynight_dynamic;
	on_q = weather_daynight_static;	
	
	on_space = weather_toggle;
	
	if (d3d_shaderversion >= 3) {
		pp_set(camera, matGodRay);
	}
	
	//printf("%.3f", (double)d3d_shaderversion);
	
}