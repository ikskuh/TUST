#include <acknex.h>
#include <default.c>

#include "..\\Source\\effects.h"
#define PRAGMA_PATH "..\\Ressources\\Graphics\\"

TEXT* txtHeadline = {
	pos_x = 10;
	pos_y = 10;
	string(
		"1 - Fountain",
		"2 - Dense Smoke",
		"3 - Star rain",
		"4 - Fire place",
		"5 - Explosion",
		"6 - Complex Smoke"
	);
	flags = SHOW | OUTLINE;
	font = "Arial#24";
}

void main() {
	level_load("..\\Ressources\\Terrains\\small.hmp");
	vec_set(sky_color.blue, vector(0,0,0));
	random_seed(0);
	vec_set(camera.x, vector(-150, 0, 50));
	
	while(1) {
	
		// Press 1 for fountain
		if (key_1) {
			vec_set(camera.x, vector(-150, 100, 50));
			effect(pFountain,maxv(1,40*time_step),vector(0,100,0),vector(0,0,5));
		}
		
		// Press 2 for smoke
		if (key_2) {
			vec_set(camera.x, vector(-150, -100, 50));
			effect(pDenseSmoke,maxv(1,10*time_step),vector(0,-100,0),vector(0,0,3));
		}

		// Press 3 for star rain
		if (key_3) {
			vec_set(camera.x, vector(50, 100, 50));
			effect(pStars,maxv(1,10*time_step),vector(200,100,100),vector(2-random(4),2-random(4),3));
		}

		// Press 4 for fire place
		if (key_4) {
			vec_set(camera.x, vector(50, -100, 50));
			effect(pFlame,maxv(1,20*time_frame),vector(200,-100,0),vector(0,0,3));
			effect(pSmokeFlames,maxv(1,20*time_frame),vector(200,-100,5),vector(0,0,3));
		}
			
		// Press 5 for explosion
		if (key_5) {
			while(key_5) wait(1);
			vec_set(camera.x, vector(-150, -100, 50));
			wait(10);
			eff_explosion(vector(200,-100,5));
		}
			
		// Press 6 for complex smoke
		if (key_6) {
			// Create a dense (50%) smoke cloud for 64 ticks (4 seconds)
			while(key_6) wait(1);
			eff_complexSmoke("smoke.tga", vector(200,-100,5), vector(200, 200, 100), 50, -1);
		}

		wait(1);
	}
}