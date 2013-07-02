#include <acknex.h>
#include <default.c>
#include <d3d9.h>

#include "..\\Source\\proc_city.h"
#include "..\\Source\\DynamicModels.h"

//#define DEBUG


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
	ENTITY *street = street_build(editorStreet, bmapStreetTexture);
}


#define VO_MIN_X -1000
#define VO_MAX_X 1000
#define VO_MIN_Y -1000
#define VO_MAX_Y 1000

// Example 3: Draws a complex road network based on a voronoi diagram
void create_random_streets()
{
	int i, j;
	
	level_load("");
	vec_set(camera.x, vector(41, 2328, 1658));
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
	//printf("Lines: %i", nResults);
	
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
	txtIntersections.pos_y = 100;
	
	TEXT* txtStreets = txt_create(300, 10);
	set(txtStreets, SHOW);
	txtStreets.pos_y = 100;	
	txtStreets.pos_x = 300;	
		
	for(i=0; i<nResults; i++) {
		nIncomingStreets = 0;
		vo_get_result_at(i, &x1, &y1, &x2, &y2);
		
		// There could be roads with the start==end; ignore them!
		if ((x1 == x2) && (y1 == y2)) continue;
		
		#ifdef DEBUG
			str_cpy((txtStreets.pstring)[i], "From ");
			str_cat((txtStreets.pstring)[i], str_for_float(NULL, x1));
			str_cat((txtStreets.pstring)[i], ",");
			str_cat((txtStreets.pstring)[i], str_for_float(NULL, y1));
			str_cat((txtStreets.pstring)[i], " to ");
			str_cat((txtStreets.pstring)[i], str_for_float(NULL, x2));
			str_cat((txtStreets.pstring)[i], ",");
			str_cat((txtStreets.pstring)[i], str_for_float(NULL, y2));
		#endif
		
		bFoundOne = false;
		bFoundTwo = false;
		
		// We are checking lines, so check start and end point separately
		// Start point
		for (j=0; j<list_get_count(intersections); j++) {
		
			Intersection *tempInter = list_item_at(intersections, j);
			
			// Found an intersection at a known point (1st end)
			if (vec_dist(vector(tempInter->pos->x, tempInter->pos->y, 0), vector(x1, y1, 0)) == 0) {
				bFoundOne = true;
				tempInter->incomingStreets +=1;
				VECTOR* vecNewAngle = sys_malloc(sizeof(VECTOR));
				vec_set(vecNewAngle, vector(0,0,0));
				list_add(tempInter->incomingAngles, vecNewAngle);
			}
		}
		
		if (bFoundOne == false) {
			Intersection *newInter = intersection_create(vector(x1, y1, 0));
			newInter->incomingStreets = 1;
			list_add(intersections, newInter);
			ent_create(CUBE_MDL, vector(x1, y1, 0), NULL);
			#ifdef DEBUG
				str_cpy((txtIntersections.pstring)[list_get_count(intersections)], str_for_num(NULL, newInter->pos->x));
				str_cat((txtIntersections.pstring)[list_get_count(intersections)], ",");
				str_cat((txtIntersections.pstring)[list_get_count(intersections)], str_for_num(NULL, newInter->pos->y));
			#endif
		}

		// End point
		for (j=0; j<list_get_count(intersections); j++) {
		
			Intersection *tempInter = list_item_at(intersections, j);
			if (vec_dist(vector(tempInter->pos->x, tempInter->pos->y, 0), vector(x2, y2, 0)) == 0) {
				bFoundTwo = true;
				tempInter->incomingStreets +=1;
				VECTOR* vecNewAngle = sys_malloc(sizeof(VECTOR));
				vec_set(vecNewAngle, vector(0,0,0));
				list_add(tempInter->incomingAngles, vecNewAngle);
			}
		}
		
		if (bFoundTwo == false) {
			Intersection *newInter = intersection_create(vector(x2, y2, 0));
			newInter->incomingStreets = 1;
			list_add(intersections, newInter);
			ent_create(CUBE_MDL, vector(x2, y2, 0), NULL);
			#ifdef DEBUG
				str_cpy((txtIntersections.pstring)[list_get_count(intersections)], str_for_num(NULL, newInter->pos->x));
				str_cat((txtIntersections.pstring)[list_get_count(intersections)], ",");
				str_cat((txtIntersections.pstring)[list_get_count(intersections)], str_for_num(NULL, newInter->pos->y));
			#endif
		}
	}
	
	int count = list_get_count(intersections);
	printf("Intersections found: %i", count);
		
	while(!key_esc) {
		// Draw diagram
		for(i=0; i<nResults; i++) {
			vo_get_result_at(i, &x1, &y1, &x2, &y2);
			draw_line3d(vector(x1, y1, 0), NULL, 100);
			draw_line3d(vector(x2, y2, 0), COLOR_RED, 100);
			
		}
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

// Example 4: Create intersections
void create_intersections() {
	level_load("");
	vec_set(camera.x, vector(-11, -500, 228));
	vec_set(camera.pan, vector(87, -28, 0));
	BMAP* bmapStreetTexture = bmap_create("..\\Ressources\\Graphics\\street.tga");
	
	
	Intersection *i1 = intersection_create(vector(-200,0,0));
	i1->incomingStreets +=5;
	build_intersection(i1, bmapStreetTexture);
	
	Intersection *i2 = intersection_create(vector(-100,0,0));
	i2->incomingStreets +=4;
	build_intersection(i2, bmapStreetTexture);
	
	Intersection *i3 = intersection_create(vector(0,0,0));
	i3->incomingStreets +=3;
	build_intersection(i3, bmapStreetTexture);
	
	Intersection *i4 = intersection_create(vector(100,0,0));
	i4->incomingStreets +=2;
	build_intersection(i4, bmapStreetTexture);
	
	Intersection *i5 = intersection_create(vector(200,0,0));
	i5->incomingStreets +=1;
	build_intersection(i5, bmapStreetTexture);
}

void main() {
	//draw_voronoi();
	//create_random_streets();
	//create_small_streets();
	create_intersections();
}