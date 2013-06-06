#include <acknex.h>
#include <default.c>
#include <d3d9.h>

#include "..\\Source\\proc_city.h"
#include "..\\Source\\DynamicModels.h"


void draw_point(int _x, int _y) {
	draw_line(vector(_x-1, _y-1, 0), NULL, 100);
	draw_line(vector(_x+1, _y-1, 0), COLOR_BLUE, 100);
	draw_line(vector(_x-1, _y, 0), NULL, 100);
	draw_line(vector(_x+1, _y, 0), COLOR_BLUE, 100);
	draw_line(vector(_x-1, _y+1, 0), NULL, 100);
	draw_line(vector(_x+1, _y+1, 0), COLOR_BLUE, 100);
}

void draw_voronoi() {
	int i=0;
		
	level_load("");
	vec_set(sky_color.blue, vector(0,0,0));
	random_seed(0);
	
	vo_init();
	
	for(i=0; i<40; i++) {
		vo_add_point(integer(random(screen_size.x)), integer(random(screen_size.y)));
	}
	
	vo_execute(0, screen_size.x, 0, screen_size.y, 3);
	int nResults = vo_get_result_count();
	int nPoints = vo_get_point_count();
	
	float x1, y1, x2, y2;
	while(!key_esc) {
		
		// Draw diagram
		for(i=0; i<nResults; i++) {
			vo_get_result_at(i, &x1, &y1, &x2, &y2);
			
			draw_line(vector(x1, y1, 0), NULL, 100);
			draw_line(vector(x2, y2, 0), COLOR_RED, 100);
			
		}
		
		for(i=0; i<nPoints; i++) {
			vo_get_point(i, &x1, &y1);
			draw_point(x1, y1);
		}
		wait(1);
	}
	vo_free();	
}

void create_random_streets()
{
	level_load("");
	vec_set(camera.x, vector(15, 1014, 698));
	vec_set(camera.pan, vector(270, -35, 0));
	
	ENTITY* entTerrain = ent_create("..\\Ressources\\Terrains\\helymap1.hmp", vector(0,0,0), NULL);
	BMAP* bmapStreetTexture = bmap_create("..\\Ressources\\Graphics\\street.tga");
	
	
	Street *editorStreet = street_create(25, 5);

	// Add street positions	
	street_addpos(editorStreet, vector(100,200,0));
	street_addpos(editorStreet, vector(200,200,0));
	street_addpos(editorStreet, vector(300,100,0));
	street_addpos(editorStreet, vector(300,0,0));
	street_addpos(editorStreet, vector(300,-100,0));
	street_addpos(editorStreet, vector(100,-100,0));
	street_addpos(editorStreet, vector(-100,-100,0));
	street_addpos(editorStreet, vector(-200,-200,0));
	street_addpos(editorStreet, vector(-100,200,0));
	street_addpos(editorStreet, vector(-350,200,0));
	street_addpos(editorStreet, vector(100,200,0));

	// "Draw" streets
	ENTITY *street = street_build(editorStreet, bmapStreetTexture);
}

void main() {
	//draw_voronoi();
	create_random_streets();
}