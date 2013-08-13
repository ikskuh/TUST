#include <acknex.h>
#include <default.c>
#include <d3d9.h>
#include <MtlFX.c>

#include "..\\Source\\math.h"
#include "..\\Source\\proc_city.h"
#include "..\\Source\\DynamicModels.h"
#include "..\\Source\\tust.h"

//#define PROCEDURAL_DEBUG


void draw_point(int _x, int _y) {
	draw_line(vector(_x-1, _y-1, 0), NULL, 100);
	draw_line(vector(_x+1, _y-1, 0), COLOR_BLUE, 100);
	draw_line(vector(_x-1, _y, 0), NULL, 100);
	draw_line(vector(_x+1, _y, 0), COLOR_BLUE, 100);
	draw_line(vector(_x-1, _y+1, 0), NULL, 100);
	draw_line(vector(_x+1, _y+1, 0), COLOR_BLUE, 100);
}

// Example 1: Draws a simple voronoi diagram on screen
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

// Example 2: Renders a simple street on a terrain
void create_small_streets()
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
	ENTITY *street = street_build(editorStreet, bmapStreetTexture, false, 0.01);
	place_street_on_ground(street, 3);
}

// Example 4: Create intersections
void create_intersections() {
	
	int i=0;
	
	random_seed(0);
	
	level_load("");
	vec_set(camera.x, vector(-11, -500, 228));
	vec_set(camera.pan, vector(87, -28, 0));
	
	Intersection *i1 = intersection_create(vector(-200,0,0));
	//i1->incomingStreets +=5;
	
	for(i=0; i<10; i++) {
		VECTOR* vecNewAngle = sys_malloc(sizeof(VECTOR));
		vec_set(vecNewAngle, vector(360/(i+1), 0, 0));
		IntersectionConnection *ic = sys_malloc(sizeof(IntersectionConnection));
		ic->incomingAngle = vecNewAngle;
		list_add(i1->incomingConnections, ic);
	}
	ENTITY* e1 = build_intersection(i1);

	
	Intersection *i2 = intersection_create(vector(-125,0,0));
	//i2->incomingStreets +=4;
	build_intersection(i2);
	
	Intersection *i3 = intersection_create(vector(-50,0,0));
	//i3->incomingStreets +=3;
	build_intersection(i3);
	
	Intersection *i4 = intersection_create(vector(25,0,0));
	//i4->incomingStreets +=2;
	build_intersection(i4);
	
	Intersection *i5 = intersection_create(vector(100,0,0));
	//i5->incomingStreets +=1;
	build_intersection(i5);
}

void main() {
	video_mode = 8;
	mouse_mode = 4;
	max_entities = 10000;
	random_seed(0);
	
	while(total_frames < 1) wait(1);
	draw_textmode("Arial", 0, 14, 100);
	level_load("");
	
	proc_city_create_skins();	
	
	vec_set(camera.x, vector(-11, -500, 228));
	vec_set(camera.pan, vector(87, -28, 0));
	
	List *points = roadnetwork_from_rectlangle(-1000, -1000, 1000, 1000, 200, 6);
	//List *points = roadnetwork_from_voronoi(50, -1000, -1000, 1000, 1000);
	
	List *intersections = roadnetwork_calculate(points);
	
	// Delete intersections which are too near to each other
	roadnetwork_join_near_intersections(intersections, 100);
	
	roadnetwork_build(intersections, 0, false);
	
	/*while(1) {
		DEBUG_BMAP(bmapStreetTextureNM, 10, 1);
		wait(1);
	}*/
	//draw_voronoi();
	//create_small_streets();
	//create_intersections();
	//create_random_streets();
}