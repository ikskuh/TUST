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
	ENTITY *street = street_build(editorStreet, bmapStreetTexture);
}


#define VO_MIN_X -1000
#define VO_MAX_X 1000
#define VO_MIN_Y -1000
#define VO_MAX_Y 1000

typedef struct {
	VECTOR *pos;
	int incomingStreets;
	List *incomingAngles;
} Intersection;

void create_random_streets()
{
	int i, j;
	
	level_load("");
	vec_set(camera.x, vector(15, 1014, 698));
	vec_set(camera.pan, vector(270, -35, 0));
	
	//ENTITY* entTerrain = ent_create("..\\Ressources\\Terrains\\helymap1.hmp", vector(0,0,0), NULL);
	BMAP* bmapStreetTexture = bmap_create("..\\Ressources\\Graphics\\street.tga");

	// Create a road network using voronoi diagrams
	random_seed(0);
	vo_init();
	for(i=0; i<6; i++) {
		vo_add_point(VO_MIN_X + integer(random(2 * VO_MAX_X)), VO_MIN_Y + integer(random(2 * VO_MAX_Y)));
	}
	vo_execute(VO_MIN_X, VO_MAX_X, VO_MIN_Y, VO_MAX_Y, 3);
	int nResults = vo_get_result_count();
	printf("Lines: %i", nResults);
	
	// Create a list of streets
	List *streets = list_create();
	
	// Positions that have already been identified as intersections
	// can be ignored
	List *intersections = list_create(); // Type: Intersection
	
	// A temporary street that is filled so long with points until
	// an intersection is reached
	Street *currentStreet = street_create(25, 5); // Type: Street
	
	
	float x1, y1, x2, y2, x3, y3, x4, y4;
	int nIncomingStreets = 0;
	
	BOOL bFoundOne = false;
	BOOL bFoundTwo = false;
	
	TEXT* txtIntersections = txt_create(300, 10);
	set(txtIntersections, SHOW);
		
	for(i=0; i<nResults; i++) {
		nIncomingStreets = 0;
		vo_get_result_at(i, &x1, &y1, &x2, &y2);
		
		bFoundOne = false;
		bFoundTwo = false;
		for (j=0; j<list_get_count(intersections); j++) {
		
			Intersection *tempInter = list_item_at(intersections, j);
			
			// Found an intersection at a known point (1st end)
			
			if ((floatv(tempInter->pos->x) == x1) && (floatv(tempInter->pos->y) == y1) && (bFoundOne == false)) {
				bFoundOne = true;
				tempInter->incomingStreets +=1;
				VECTOR* vecNewAngle = sys_malloc(sizeof(VECTOR));
				vec_set(vecNewAngle, vector(0,0,0));
				list_add(tempInter->incomingAngles, vecNewAngle);
				list_add(intersections, tempInter);
			}
			
			// Found an intersection at a known point (2nd end)
			if ((floatv(tempInter->pos->x) == x2) && (floatv(tempInter->pos->y) == y2) && (bFoundTwo == false)) {
				bFoundTwo = true;
				tempInter->incomingStreets +=1;
				VECTOR* vecNewAngle = sys_malloc(sizeof(VECTOR));
				vec_set(vecNewAngle, vector(0,0,0));
				list_add(tempInter->incomingAngles, vecNewAngle);
				list_add(intersections, tempInter);		
			}
		}
		
		if (bFoundOne == false) {
			Intersection *newInter = sys_malloc(sizeof(Intersection));
			newInter->incomingAngles = list_create();
			newInter->pos = sys_malloc(sizeof(VECTOR));
			newInter->pos->x = x1;
			newInter->pos->y = y1;
			newInter->incomingStreets = 1;
			list_add(intersections, newInter);
			str_cpy((txtIntersections.pstring)[list_get_count(intersections)], str_for_num(NULL, newInter->pos->x));
			str_cat((txtIntersections.pstring)[list_get_count(intersections)], ",");
			str_cat((txtIntersections.pstring)[list_get_count(intersections)], str_for_num(NULL, newInter->pos->y));
		}
		
		if (bFoundTwo == false) {
			Intersection *newInter = sys_malloc(sizeof(Intersection));
			newInter->incomingAngles = list_create();
			newInter->pos = sys_malloc(sizeof(VECTOR));
			newInter->pos->x = x2;
			newInter->pos->y = y2;
			newInter->incomingStreets = 1;
			list_add(intersections, newInter);
			str_cpy((txtIntersections.pstring)[list_get_count(intersections)], str_for_num(NULL, newInter->pos->x));
			str_cat((txtIntersections.pstring)[list_get_count(intersections)], ",");
			str_cat((txtIntersections.pstring)[list_get_count(intersections)], str_for_num(NULL, newInter->pos->y));
		}
	}
	
	int count = list_get_count(intersections);
	printf("Count %i", count);
		
		
		while(!key_esc) {
		
		// Draw diagram
		for(i=0; i<nResults; i++) {
			vo_get_result_at(i, &x1, &y1, &x2, &y2);
			
			draw_line3d(vector(x1, y1, 0), NULL, 100);
			draw_line3d(vector(x2, y2, 0), COLOR_RED, 100);
			
		}
		
		/*for(i=0; i<nPoints; i++) {
			vo_get_point(i, &x1, &y1);
			draw_point(x1, y1);
		}*/
		wait(1);
	}
	

	vo_free();		

	// "Draw" all streets
	
	/*ListIterator *it = list_begin_iterate(streets);
	Street* tempStreet;
	for(tempStreet = list_iterate(it); it->hasNext; tempStreet = list_iterate(it))
	{
		street_build(tempStreet, bmapStreetTexture);
	} */
}

void main() {
	//draw_voronoi();
	create_random_streets();
	//create_small_streets();
}