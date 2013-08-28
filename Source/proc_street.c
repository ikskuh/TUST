#ifndef _PROC_STREET_C_
#define _PROC_STREET_C_

#include "math.h"
#include "bmap.h" // Needs "BmapGS.dll"
#include "tust.h"

//#define PROC_DEBUG

#define PROC_USE_SHADERS

#define PROC_TEXT_RES 256
#define PROC_INTERSECTION_EXTREMITIES 5
#define STREET_GROUND_DIST 3

// ----------------------------------------------------------------------------------------
// Street tool
// ----------------------------------------------------------------------------------------

Intersection *intersection_create(VECTOR* _pos)
{
	Intersection *newInter = sys_malloc(sizeof(Intersection));
	newInter->incomingConnections = list_create();
	newInter->pos = sys_malloc(sizeof(VECTOR));
	vec_set(newInter->pos, _pos);
	newInter->ent = NULL;
	return newInter;
}


Street *street_create(int _streetWidth, int groundDistance)
{
	Street *street = sys_malloc(sizeof(Street));
	street->points = list_create();
	street->width = _streetWidth; // 320
	street->groundDist = groundDistance;
	return street;
}

void street_remove(Street *street)
{
	// Removes all points from the street and frees the street itself.
	list_clear_content (street->points, sys_free );
	list_delete(street->points);
	if(street->ent != NULL)
		ent_remove(street->ent);
	sys_free(street);
}

void street_addpos(Street *street, VECTOR *pos)
{
	// Create a new VECTOR and copy the source position into it.
	VECTOR *vec = sys_malloc(sizeof(VECTOR));
	vec_set(vec, pos);
	
	// Add vec to the street
	list_add(street->points, vec);
}

var street_getground(Street *street, VECTOR *pos)
{
	if(c_trace(	vector(pos.x, pos.y, pos.z + 1024),
				vector(pos.x, pos.y, pos.z - 1024),
				IGNORE_MODELS | IGNORE_SPRITES | IGNORE_PASSABLE | IGNORE_PASSENTS | USE_POLYGON))
	{
		return target.z;
	}
	return 0;
}

// Creates a new separator. A separator is a pair of two vertices, the left and the right side of the street.
int street_create_separator(DynamicModel *model, D3DVERTEX *left, D3DVERTEX *right)
{
	int index = model->vertexCount;
	
	// Copy vertices into model
	memcpy(&model->vertexBuffer[index + 0], left, sizeof(D3DVERTEX));
	memcpy(&model->vertexBuffer[index + 1], right, sizeof(D3DVERTEX));
	
	model->vertexCount += 2;
	return index;
}

// Creates a new street segment.
// The segment will be built between to separators.
void street_create_segment(DynamicModel *model, int sep1, int sep2)
{
	// Calculate indicies and setup index and vertex buffer
	int index = 3 * model->faceCount;
	
	model->indexBuffer[index + 0] = sep1 + 0;
	model->indexBuffer[index + 1] = sep2 + 0;
	model->indexBuffer[index + 2] = sep1 + 1;
	
	model->indexBuffer[index + 3] = sep1 + 1;
	model->indexBuffer[index + 4] = sep2 + 0;
	model->indexBuffer[index + 5] = sep2 + 1;
	
	model->faceCount += 2;
}

D3DVERTEX *create_vertex(float _x, float _y, float _z, float _nx, float _ny, float _nz, float _uv1x, float _uv1y) {
	D3DVERTEX *newVertex = sys_malloc(sizeof(D3DVERTEX));
	newVertex->x  = _x;    newVertex->y = _y;      newVertex->z = _z;
	newVertex->nx = _nx;   newVertex->ny = _ny;   newVertex->nz = _nz;
	newVertex->u1 = _uv1x; newVertex->v1 = _uv1y;
	return newVertex;
}

float optimal_intersection_rotation(Intersection* _inter) {

	switch(list_get_count(_inter->incomingConnections)) {
		case 0: 
			// Error
		break;
		case 1:
		case 2: // Todo: 2 has also to be rotated!
			IntersectionConnection *ic = (IntersectionConnection*)list_item_at(_inter->incomingConnections, 0);
			return ic->incomingAngle->pan;
		break;
		case 3:
			IntersectionConnection *ic1 = (IntersectionConnection*)list_item_at(_inter->incomingConnections, 0);
			IntersectionConnection *ic2 = (IntersectionConnection*)list_item_at(_inter->incomingConnections, 1);
			IntersectionConnection *ic3 = (IntersectionConnection*)list_item_at(_inter->incomingConnections, 2);
		
			if (ic1->incomingAngle->pan >= ic2->incomingAngle->pan + 180) {
				// Reconnect endings
				int tempId = ic1->id;
				ic1->id = ic2->id;
				ic2->id = ic3->id;
				ic3->id = tempId;
				return ic1->incomingAngle->pan;
			}
			if (ic2->incomingAngle->pan >= ic3->incomingAngle->pan + 180) {
				// Reconnect endings
				int tempId = ic3->id;
				ic3->id = ic2->id;
				ic2->id = ic1->id;
				ic1->id = tempId;					
				return ic2->incomingAngle->pan;
			}
			return ic3->incomingAngle->pan;
		break;
		default:
		
			int j,k,tempPan, resultPan;
			var i;
			var tempPan2;
			VECTOR currentPan, currentPan2;
			float bestPan = list_get_count(_inter->incomingConnections) * 360;
			
			for (j=0; j<list_get_count(_inter->incomingConnections); j++) {
				
				float newPan = 0;
				IntersectionConnection *ic = (IntersectionConnection*)list_item_at(_inter->incomingConnections, j);
				
				
				vec_set(currentPan, ic->incomingAngle->pan);
				tempPan = cycle(currentPan.x, 0, 360);
				
				for(k=0; k<list_get_count(_inter->incomingConnections); k++) {
					
					IntersectionConnection *ic2 = (IntersectionConnection*)list_item_at(_inter->incomingConnections, k);
					
					
					vec_set(currentPan2, ic2->incomingAngle->pan);
					tempPan2 = cycle(currentPan2.x, 0, 360);
					if (tempPan > tempPan2) {
						newPan += tempPan - tempPan2;
					} else {
						newPan += tempPan2 - tempPan;
					}
				}
				
				if (newPan<bestPan) {
					bestPan = newPan;
					resultPan = tempPan2;
				}
			}
			return resultPan;				
		break;
	}
}


void roadnetwork_join_near_intersections(List *_intersections, float _minDist) {
	int i,j,k,l,c;
	BOOL bAllDone = false;
	BOOL bDeleted = false;
	BOOL bAlreadyInList = false;
	ListIterator *it1, *it2;
	do {
		c = list_get_count(_intersections);
		bAllDone = true;
		for(i=0; i<c; i++) {
			for(j=0; j<c; j++) {
				if (j != i) {
					
					Intersection *i1 = list_item_at(_intersections, i);
					Intersection *i2 = list_item_at(_intersections, j);
					
					// Intersections are nearer than _minDist?
					if (vec_dist(i1->pos, i2->pos) <= _minDist) {
						
						// Delete all connections from i1 to i2
						ListIterator *it1 = list_begin_iterate(i1->incomingConnections);
						IntersectionConnection *ic1 = NULL;
						for(ic1 = list_iterate(it1); it1->hasNext; ic1 = list_iterate(it1))
						{
							ListIterator *it2 = list_begin_iterate(i2->incomingConnections);
							IntersectionConnection *ic2 = NULL;
							for(ic2 = list_iterate(it2); it2->hasNext; ic2 = list_iterate(it2)) {
								if (ic1->id == ic2->id) {
									list_remove(i1->incomingConnections, ic1);
									list_remove(i2->incomingConnections, ic2);
								}
							}
							list_end_iterate(it2);
						}
						list_end_iterate(it1);

						
						// Transfer remaining connections to i2
						for(k=0; k<list_get_count(i1->incomingConnections); k++) {
							
							IntersectionConnection *ic1 = list_item_at(i1->incomingConnections, k);
							bAlreadyInList = false;
							
							// ... if it is not yet in list
							for(l=0; l<list_get_count(i2->incomingConnections); l++) {
								IntersectionConnection *ic2 = list_item_at(i2->incomingConnections, l);
								
								if (ic2->id == ic1->id) {
									bAlreadyInList = true;
									break;
								}
							}
							
							if (bAlreadyInList == false) {
								list_add(i2->incomingConnections, ic1);
							}
						}
						// Delete item i1
						list_remove(_intersections, i1);
						
						bDeleted = true;
						bAllDone = false;
						break;
					}
				}
			}
			if (bDeleted == true) {
				bDeleted = false;
				break;
			}
		}
	} while(bAllDone == false);
}

action inter_info() {
	mouse_range = 10000;
	int i=0;
	while(my.skill1 == NULL) wait(1);
	
	Intersection* i1 = my.skill1;
	while(me) {
		if(mouse_ent == me) {
			ENTITY *entTemp = mouse_ent;
			for(i=0; i<list_get_count(i1->incomingConnections); i++) {
				
				IntersectionConnection *ic = (IntersectionConnection*)list_item_at(i1->incomingConnections, i);
				VECTOR* vecCurrentAng;
				vec_set(vecCurrentAng, ic->incomingAngle->pan);
				
				draw_text("Inc. street:     Pos:", 0, i*20, COLOR_GREEN);
				draw_text(str_for_int(NULL, i), 50, i*20, COLOR_GREEN);
				draw_text(str_for_num(NULL, vecCurrentAng->x), 095, i*20, COLOR_RED);
				draw_text(str_for_num(NULL, vecCurrentAng->y), 155, i*20, COLOR_RED);
				draw_text(str_for_num(NULL, vecCurrentAng->z), 215, i*20, COLOR_RED);
				
				draw_text("Connection Vertices:", 300, i*20, COLOR_GREEN);
				draw_text(str_for_int(NULL, ic->leftVertex), 410, i*20, COLOR_RED);
				draw_text(str_for_int(NULL, ic->rightVertex), 440, i*20, COLOR_RED);	
				
				if (key_r) {
					while(key_r) wait(1);						
					CONTACT* c = ent_getvertex(entTemp, NULL, ic->leftVertex);
					VECTOR* vecVert1 = vector(0,0,0);
					vec_set(vecVert1, vector(c->v->x, c->v->z, c->v->y));
					vec_mul(vecVert1, entTemp.scale_x);
					vec_rotate(vecVert1, entTemp.pan);
					vec_add(vecVert1, entTemp.x);
					printf("1: %.2f %.2f %.2f", (double)vecVert1.x, (double)vecVert1.y, (double)vecVert1.z);
				}				
			}
				
			draw_text("My pan:", 0, (i+3)*20, COLOR_GREEN);
			draw_text(str_for_num(NULL, my.pan), 50, (i+3)*20, COLOR_RED);
			
			draw_text("Src:", 0, (i+4)*20, COLOR_GREEN);
			draw_text(str_for_int(NULL, i1->source), 50, (i+4)*20, COLOR_RED);
			
			draw_text("Pos", 0, (i+5)*20, COLOR_GREEN);
			draw_text(str_for_num(NULL, i1->pos->x), 075, (i+5)*20, COLOR_RED);
			draw_text(str_for_num(NULL, i1->pos->y), 155, (i+5)*20, COLOR_RED);
			draw_text(str_for_num(NULL, i1->pos->z), 235, (i+5)*20, COLOR_RED);
			
			if (key_q) {
				my.pan +=10 * time_step;
			}
			if (key_e) {
				my.pan -=10 * time_step;
			}
		}
		wait(1);
	}
}

int pan_angle_compare(ListData *left, ListData *right) { //and returns 1 if left>right, 0 if left=right and -1 if left<right.
	
	IntersectionConnection *icLeft = (IntersectionConnection*)left;
	IntersectionConnection *icRight = (IntersectionConnection*)right;
	
	if (((ANGLE*)icLeft->incomingAngle)->pan > ((ANGLE*)icRight->incomingAngle)->pan) {
		return -1; // Todo 1
	}
	
	if (((ANGLE*)icLeft->incomingAngle)->pan == ((ANGLE*)icRight->incomingAngle)->pan) {
		return 0;
	}
	
	if (((ANGLE*)icLeft->incomingAngle)->pan < ((ANGLE*)icRight->incomingAngle)->pan) {
		return 1; // Todo -1
	}	
}


ENTITY *build_intersection(Intersection *_intersection)
{
	//int nIncomingCount = _intersection->incomingStreets;
	int nIncomingCount = list_get_count(_intersection->incomingConnections);
	DynamicModel *model = dmdl_create();
	
	#ifdef PROC_DEBUG
		if (bmapStreetIntersection1 == NULL) {
			printf("Intersection skins are not initialized - consider calling 'proc_city_create_skins' first!");
		}
	#endif
	
	// Rotate intersections according the first incoming street
	float fOptimalPan = 0;
	if (list_get_count(_intersection->incomingConnections) > 0) {
		
		// Sort incoming angles
		list_sort(_intersection->incomingConnections, pan_angle_compare);	
			
		fOptimalPan = optimal_intersection_rotation(_intersection);
	}	
	
	switch(nIncomingCount) {
		// Should not happen!
		case 0: return NULL; break;
		
		// Street ending
		case 1:
		
			model->skin[0] = bmapStreetIntersection1;
			model->skin[1] = bmapStreetIntersectionNM1;
			// y == z
			/*D3DVERTEX *v1 = create_vertex(0 - 10,  0, 0 - 10,   0, 1, 0,   0,    0);
			D3DVERTEX *v2 = create_vertex(0 - 10,  0, 0 + 10,   0, 1, 0,   0,    1);
			D3DVERTEX *v3 = create_vertex(0 + 2.5, 0, 0 + 10,   0, 1, 0,   0.66, 1);
			D3DVERTEX *v4 = create_vertex(0 + 7.5, 0, 0 + 2.5,  0, 1, 0,   1,    0.66);
			D3DVERTEX *v5 = create_vertex(0 + 7.5, 0, 0 - 2.5,  0, 1, 0,   1,    0.33);
			D3DVERTEX *v6 = create_vertex(0 + 2.5, 0, 0 - 10,   0, 1, 0,   0.66, 0);*/
			D3DVERTEX *v1 = create_vertex(0 - 10,  0, 0 - 10,   0, 1, 0,   1.0 / 3,            1.0 / 3);
			D3DVERTEX *v2 = create_vertex(0 - 10,  0, 0 + 10,   0, 1, 0,   1.0 / 3,            2.0 / 3);
			D3DVERTEX *v3 = create_vertex(0 + 2.5, 0, 0 + 10,   0, 1, 0,   2.0 / 3 - 1.0 / 18, 2.0 / 3);
			D3DVERTEX *v4 = create_vertex(0 + 7.5, 0, 0 + 2.5,  0, 1, 0,   2.0 / 3,            2.0 / 3 - 1.0 / 9); 
			D3DVERTEX *v5 = create_vertex(0 + 7.5, 0, 0 - 2.5,  0, 1, 0,   2.0 / 3,            1.0 / 3 + 1.0 / 9);
			D3DVERTEX *v6 = create_vertex(0 + 2.5, 0, 0 - 10,   0, 1, 0,   2.0 / 3 - 1.0 / 18, 1.0 / 3);
			
			
			// Store the middle of the end that should be connected to a street
			if (list_get_count(_intersection->incomingConnections) > 0) {
				IntersectionConnection *ic = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, 0);
				ic->pos = sys_malloc(sizeof(VECTOR));
				vec_set(ic->pos, vector(-10,0,0));
				ic->leftVertex = 1;
				ic->rightVertex = 2;
			}	
			
			int i1 = dmdl_add_vertex(model, v1);
			int i2 = dmdl_add_vertex(model, v2);
			int i3 = dmdl_add_vertex(model, v3);
			int i4 = dmdl_add_vertex(model, v4);
			int i5 = dmdl_add_vertex(model, v5);
			int i6 = dmdl_add_vertex(model, v6);
		
			dmdl_connect_vertices(model, i1, i2, i5);
			dmdl_connect_vertices(model, i5, i2, i4);
			dmdl_connect_vertices(model, i4, i2, i3);
			dmdl_connect_vertices(model, i1, i5, i6);
			
		break;
		
		// A simple connection
		case 2:
		
			// Calculate the angle difference
			if (list_get_count(_intersection->incomingConnections) > 0) {
				
				
				// Set the right skin
				IntersectionConnection *ic1 = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, 0);
				IntersectionConnection *ic2 = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, 1);
				
				ic1->pos            = sys_malloc(sizeof(VECTOR));
				ic2->pos            = sys_malloc(sizeof(VECTOR));
				vec_set(ic1->pos, vector(-10,0,0));
				ic1->leftVertex  = 1;
				ic1->rightVertex = 2;				
				
				float difference;
				if (ic1->incomingAngle->pan >= ic2->incomingAngle->pan) {
					difference = ic1->incomingAngle->pan - ic2->incomingAngle->pan;
				} else {
					difference = ic2->incomingAngle->pan - ic1->incomingAngle->pan;
				}
				
				if ((difference <= 315) && (difference > 225)) {
					model->skin[0] = bmapStreetIntersection2_2;
					model->skin[1] = bmapStreetIntersectionNM2_2;
					vec_set(ic2->pos, vector(0,0,-10));
					ic2->leftVertex = 3;
					ic2->rightVertex = 1;				
				}	
				if ((difference <= 225) && (difference > 135)) {
					model->skin[0] = bmapStreetIntersection2_1;
					model->skin[1] = bmapStreetIntersectionNM2_1;
					vec_set(ic2->pos, vector(10,0,0));
					ic2->leftVertex = 4;
					ic2->rightVertex = 3;					
				}								
				if ((difference <= 135) && (difference > 45)) {
					model->skin[0] = bmapStreetIntersection2_3;
					model->skin[1] = bmapStreetIntersectionNM2_3;
					vec_set(ic2->pos, vector(0,0,10));
					ic2->leftVertex = 2;
					ic2->rightVertex = 4;
				}				
			} else {
				model->skin[0] = bmapStreetIntersection2_1;
				model->skin[1] = bmapStreetIntersectionNM2_1;
			}
			
			
			D3DVERTEX *v1 = create_vertex(-10,                                 0, -10, 0, 1, 0, 1.0 / 3, 1.0 / 3);
			D3DVERTEX *v2 = create_vertex(-10,                                 0,  10, 0, 1, 0, 1.0 / 3, 2.0 / 3);
			D3DVERTEX *v3 = create_vertex( 10,                                 0, -10, 0, 1, 0, 2.0 / 3, 1.0 / 3);
			D3DVERTEX *v4 = create_vertex( 10,                                 0,  10, 0, 1, 0, 2.0 / 3, 2.0 / 3);
		
			int i1 = dmdl_add_vertex(model, v1);
			int i2 = dmdl_add_vertex(model, v2);
			int i3 = dmdl_add_vertex(model, v3);
			int i4 = dmdl_add_vertex(model, v4);
			
			dmdl_connect_vertices(model, i1, i2, i3);
			dmdl_connect_vertices(model, i3, i2, i4);
		break;
		
		// Three incoming streets
		case 3:
		
			// Set the connection points
			if (list_get_count(_intersection->incomingConnections) > 0) {
				
				
				// Set the right skin
				IntersectionConnection *ic1 = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, 0);
				IntersectionConnection *ic2 = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, 1);
				IntersectionConnection *ic3 = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, 2);
				
				ic1->pos            = sys_malloc(sizeof(VECTOR));
				ic2->pos            = sys_malloc(sizeof(VECTOR));
				ic3->pos            = sys_malloc(sizeof(VECTOR));
				vec_set(ic1->pos, vector(30,0,0));
				ic1->leftVertex  = 8;
				ic1->rightVertex = 7;
				vec_set(ic2->pos, vector(0,0,-30));
				ic2->leftVertex  = 10;
				ic2->rightVertex = 9;				
				vec_set(ic3->pos, vector(-30,0,0));
				ic3->leftVertex  = 1;
				ic3->rightVertex = 3;				
				
			}
			model->skin[0] = bmapStreetIntersection3;
			model->skin[1] = bmapStreetIntersectionNM3;
			
			D3DVERTEX *v1  = create_vertex(0 - 30, 0, 0 - 10, 0, 1, 0, 0,       1.0 / 3);
			D3DVERTEX *v2  = create_vertex(0 - 10, 0, 0 - 10, 0, 1, 0, 1.0 / 3, 1.0 / 3);
			D3DVERTEX *v3  = create_vertex(0 - 30, 0, 0 + 10, 0, 1, 0, 0,       2.0 / 3);
			D3DVERTEX *v4  = create_vertex(0 - 10, 0, 0 + 10, 0, 1, 0, 1.0 / 3, 2.0 / 3);
			D3DVERTEX *v5  = create_vertex(0 + 10, 0, 0 - 10, 0, 1, 0, 2.0 / 3, 1.0 / 3);
			D3DVERTEX *v6  = create_vertex(0 + 10, 0, 0 + 10, 0, 1, 0, 2.0 / 3, 2.0 / 3);
			D3DVERTEX *v7  = create_vertex(0 + 30, 0, 0 - 10, 0, 1, 0, 1,       1.0 / 3);
			D3DVERTEX *v8  = create_vertex(0 + 30, 0, 0 + 10, 0, 1, 0, 1,       2.0 / 3);
			D3DVERTEX *v9  = create_vertex(0 - 10, 0, 0 - 30, 0, 1, 0, 1.0 / 3, 0);			
			D3DVERTEX *v10 = create_vertex(0 + 10, 0, 0 - 30, 0, 1, 0, 2.0 / 3, 0);			
			
			int i1  = dmdl_add_vertex(model, v1);
			int i2  = dmdl_add_vertex(model, v2);
			int i3  = dmdl_add_vertex(model, v3);
			int i4  = dmdl_add_vertex(model, v4);
			int i5  = dmdl_add_vertex(model, v5);
			int i6  = dmdl_add_vertex(model, v6);
			int i7  = dmdl_add_vertex(model, v7);
			int i8  = dmdl_add_vertex(model, v8);
			int i9  = dmdl_add_vertex(model, v9);
			int i10 = dmdl_add_vertex(model, v10);
			
			dmdl_connect_vertices(model, i1, i3, i2);
			dmdl_connect_vertices(model, i2, i3, i4);
			dmdl_connect_vertices(model, i2, i4, i5);
			dmdl_connect_vertices(model, i5, i4, i6);
			dmdl_connect_vertices(model, i7, i5, i6);
			dmdl_connect_vertices(model, i7, i6, i8);
			dmdl_connect_vertices(model, i2, i5, i10);
			dmdl_connect_vertices(model, i9, i2, i10);
		break;
		
		// A cross
		case 4:
			// Set the connection points
			if (list_get_count(_intersection->incomingConnections) > 0) {
				
				IntersectionConnection *ic1 = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, 0);
				IntersectionConnection *ic2 = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, 1);
				IntersectionConnection *ic3 = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, 2);
				IntersectionConnection *ic4 = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, 3);
				
				ic1->pos            = sys_malloc(sizeof(VECTOR));
				ic2->pos            = sys_malloc(sizeof(VECTOR));
				ic3->pos            = sys_malloc(sizeof(VECTOR));
				ic4->pos            = sys_malloc(sizeof(VECTOR));
				vec_set(ic4->pos, vector(-30,0,0));
				ic4->leftVertex  = 1;
				ic4->rightVertex = 3;
				vec_set(ic3->pos, vector(0,0,-30));
				ic3->leftVertex  = 12;
				ic3->rightVertex = 11;				
				vec_set(ic2->pos, vector(30,0,0)); // Todo left right may be wrong
				ic2->leftVertex  = 8;
				ic2->rightVertex = 7;
				vec_set(ic1->pos, vector(0,0,30));
				ic1->leftVertex  = 9;
				ic1->rightVertex = 10;								
			}		
			model->skin[0] = bmapStreetIntersection4;
			model->skin[1] = bmapStreetIntersectionNM4;
			
			D3DVERTEX *v1  = create_vertex(0 - 30, 0, 0 - 10, 0, 1, 0, 0,       1.0 / 3);
			D3DVERTEX *v2  = create_vertex(0 - 10, 0, 0 - 10, 0, 1, 0, 1.0 / 3, 1.0 / 3);
			D3DVERTEX *v3  = create_vertex(0 - 30, 0, 0 + 10, 0, 1, 0, 0,       2.0 / 3);
			D3DVERTEX *v4  = create_vertex(0 - 10, 0, 0 + 10, 0, 1, 0, 1.0 / 3, 2.0 / 3);
			D3DVERTEX *v5  = create_vertex(0 + 10, 0, 0 - 10, 0, 1, 0, 2.0 / 3, 1.0 / 3);
			D3DVERTEX *v6  = create_vertex(0 + 10, 0, 0 + 10, 0, 1, 0, 2.0 / 3, 2.0 / 3);
			D3DVERTEX *v7  = create_vertex(0 + 30, 0, 0 - 10, 0, 1, 0, 1,       1.0 / 3);
			D3DVERTEX *v8  = create_vertex(0 + 30, 0, 0 + 10, 0, 1, 0, 1,       2.0 / 3);
			D3DVERTEX *v9  = create_vertex(0 - 10, 0, 0 + 30, 0, 1, 0, 1.0 / 3, 1);			
			D3DVERTEX *v10 = create_vertex(0 + 10, 0, 0 + 30, 0, 1, 0, 2.0 / 3, 1);			
			D3DVERTEX *v11 = create_vertex(0 - 10, 0, 0 - 30, 0, 1, 0, 1.0 / 3, 0);
			D3DVERTEX *v12 = create_vertex(0 + 10, 0, 0 - 30, 0, 1, 0, 2.0 / 3, 0);
			
			int i1  = dmdl_add_vertex(model, v1);
			int i2  = dmdl_add_vertex(model, v2);
			int i3  = dmdl_add_vertex(model, v3);
			int i4  = dmdl_add_vertex(model, v4);
			int i5  = dmdl_add_vertex(model, v5);
			int i6  = dmdl_add_vertex(model, v6);
			int i7  = dmdl_add_vertex(model, v7);
			int i8  = dmdl_add_vertex(model, v8);
			int i9  = dmdl_add_vertex(model, v9);
			int i10 = dmdl_add_vertex(model, v10);
			int i11 = dmdl_add_vertex(model, v11);
			int i12 = dmdl_add_vertex(model, v12);
			
			dmdl_connect_vertices(model, i1, i3, i2);
			dmdl_connect_vertices(model, i2, i3, i4);
			dmdl_connect_vertices(model, i2, i4, i5);
			dmdl_connect_vertices(model, i5, i4, i6);
			dmdl_connect_vertices(model, i7, i5, i6);
			dmdl_connect_vertices(model, i7, i6, i8);
			dmdl_connect_vertices(model, i4, i9, i6);
			dmdl_connect_vertices(model, i6, i9, i10);
			dmdl_connect_vertices(model, i12, i11, i2);
			dmdl_connect_vertices(model, i12, i2, i5);
		break;
		
		// A circle
		default:
		
			model->skin[0] = bmapStreetIntersection5;
			
			// Set the connection points
			var vNumConnections = list_get_count(_intersection->incomingConnections);
			if (vNumConnections > 0) {
				
				// Calculate regular polygon radius
				var polyRadius = 20 / sinv(360 / vNumConnections);				
				
				// Create vertex in the middle
				D3DVERTEX *v0  = create_vertex(0, 0, 0, 0, 1, 0, 0.5, 0.5);
				int iMiddle  = dmdl_add_vertex(model, v0);
				
				// Create vertex on top
				var vNewX = sin(0) * polyRadius;
				var vNewY = cos(0) * polyRadius;
				var vFirstNewX = vNewX;
				var vFirstNewY = vNewY;
				var vNewUVX = 0.5 + (1 / (2*polyRadius) * vNewX);
				var vNewUVY = 0.5 + (1 / (2*polyRadius) * vNewY);
				D3DVERTEX *v1  = create_vertex(vNewX, 0, vNewY, 0, 1, 0, vNewUVX, vNewUVY);
				int i1  = dmdl_add_vertex(model, v1);							

				// Needed to calculate connection positions
				var vOldX = vNewX;
				var vOldY = vNewY;
				
				int i;
				int iNewVertex = 0;
				int iLastVertex = i1;
				//int iPreLastVertex = 0;
				
				for(i=1; i<vNumConnections; i++) {
					IntersectionConnection *ic1 = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, i-1);
					ic1->pos            = sys_malloc(sizeof(VECTOR));
					
					// Calculate new vertex position
					double fAngle = i / vNumConnections * 2 * PI;
					vNewX = sin(fAngle) * polyRadius;
					vNewY = cos(fAngle) * polyRadius;
					vNewUVX = 0.5 + (1 / (2*polyRadius) * vNewX);
					vNewUVY = 0.5 + (1 / (2*polyRadius) * vNewY);
					D3DVERTEX *v2  = create_vertex(vNewX, 0, vNewY, 0, 1, 0, vNewUVX,   vNewUVY);
					iNewVertex  = dmdl_add_vertex(model, v2);
					
					vec_lerp(ic1->pos, vector(vNewX, 0, vNewY), vector(vOldX, 0, vOldY), 0.5);
					ic1->leftVertex  = iLastVertex+1;
					ic1->rightVertex = iNewVertex+1;
					
					vOldX = vNewX;
					vOldY = vNewY;
					
					dmdl_connect_vertices(model, iMiddle, iLastVertex, iNewVertex);
					iLastVertex = iNewVertex;
				}
				
				// Closing vertex
				dmdl_connect_vertices(model, iMiddle, iNewVertex, i1);
				IntersectionConnection *ic2 = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, vNumConnections-1);
				ic2->pos = sys_malloc(sizeof(VECTOR));
				vec_lerp(ic2->pos, vector(vOldX, 0, vOldY), vector(vFirstNewX, 0, vFirstNewY), 0.5);
				ic2->leftVertex  = iLastVertex+1;
				ic2->rightVertex = i1+1;
			}
				
			model->skin[0] = bmapStreetIntersection5;	
			model->skin[1] = bmapStreetIntersectionNM5;
		break;
	}
	
	ENTITY *ent = dmdl_create_instance(model, vector(_intersection->pos->x, _intersection->pos->z, _intersection->pos->y), inter_info);
	#ifdef PROC_USE_SHADERS
		ent.material = mtl_specBump;
	#endif
	
	ent->skill1 = _intersection;
	_intersection->ent = ent;
	
	ent->pan = fOptimalPan;
	
	dmdl_delete(model);
	
	return ent;
}

// Places each vertex of an entity's mesh on the ground (and adds a given distance)
void place_street_on_ground(ENTITY* _street, int _dist) {
	
	var nVertexCount = ent_status(_street, 0);
		
	var i, j;
	VECTOR vecTemp;
	CONTACT* c;
	for (i=1; i<=nVertexCount; i++) {
		
		c = ent_getvertex(_street, NULL, i);
		
		if (c != NULL) {
			
			vec_set(vecTemp, vector(c.v.x, c.v.z, c.v.y));
			vec_mul(vecTemp, vector(_street.scale_x, _street.scale_y, _street.scale_z));
			vec_rotate(vecTemp, _street.pan);
			vec_add(vecTemp, _street.x);
			if(c_trace(	vecTemp, vector(vecTemp.x, vecTemp.y, vecTemp.z - 1024),
						IGNORE_MODELS | IGNORE_SPRITES | IGNORE_PASSABLE | IGNORE_PASSENTS | USE_POLYGON | IGNORE_ME))
			{
				c.v.y = target.z + _dist - _street.z;
				ent_setvertex(_street, c, i);
			}
		}		
	}
}

ENTITY *street_build(Street *street, BMAP* _streetTexture, BOOL _placeOnGround, float _details) {
	street_build_ext(street, _streetTexture, NULL, _placeOnGround, _details, NULL, NULL, NULL, NULL);
}

// Start and end segments are no longer spline aligned!
ENTITY *street_build_ext(Street *street, BMAP* _streetTexture, BMAP* _streetNormalMap, BOOL _placeOnGround, float _details, VECTOR* _v1_1, VECTOR* _v1_2, VECTOR* _v2_1, VECTOR* _v2_2)
{
	int iPointCount = list_get_count(street->points);
	
	// Create array with space for all spline points
	VECTOR *splineData = sys_malloc(sizeof(VECTOR) * iPointCount);
	
	VECTOR *value;
	
	// Copy the list into the splineData array.
	ListIterator *it = list_begin_iterate(street->points);
	int i;
	for(value = list_iterate(it), i = 0; it->hasNext; value = list_iterate(it))
	{
		vec_set(splineData[i], value);
		i++;
	}
	list_end_iterate(it);
	
	DynamicModel *model = dmdl_create();
	model->skin[0] = _streetTexture;
	
	// Appy normal map
	if (_streetNormalMap != NULL) {
		model->skin[1] = _streetNormalMap;
	}
	
	DYNAMIC_QUAD quad;
	
	D3DVERTEX left;
	D3DVERTEX right;
	
	var isLooped = vec_dist(&splineData[0], &splineData[iPointCount-1]) < 0.1;
	
	int previousSeparator = 0;
	
	//VECTOR vecStartPosition;
	
	var dist = 0;
	for(dist = 0; dist <= 1.0; dist += _details)
	{
		VECTOR startSegment, endSegment, dir;		

		// Save beginning to create the entity at the end
		if (dist == 0) {
			vec_set(&startSegment, &splineData[0]);
			//vec_set(vecStartPosition, startSegment);
			
			#ifdef PROC_DEBUG
				ENTITY* entB = ent_create(CUBE_MDL, startSegment, NULL); set(entB, LIGHT); entB.lightrange = 255; vec_set(entB.blue, vector(0,255,0));
			#endif
		}
		else
		{
			// Get starting position
			vec_set(&startSegment, math_get_spline(splineData, iPointCount, dist));
		}
		
		// Get a next segment on the street (to calculate the distance)
		if(dist >= 0.99)
		{
			// Special case: Last segment
			vec_set(&endSegment, &splineData[iPointCount-1]);
			
			#ifdef PROC_DEBUG
				ENTITY* entA = ent_create(CUBE_MDL, endSegment, NULL); set(entA, LIGHT); entA.lightrange = 255; vec_set(entA.blue, vector(0,0,255));
			#endif
			
			//vec_set(&endSegment, math_get_spline(splineData, iPointCount, dist - 0.01));
			vec_diff(&dir, &startSegment, &endSegment);
		}
		else
		{
			vec_set(&endSegment, math_get_spline(splineData, iPointCount, dist + 0.01));
			vec_diff(&dir, &endSegment, &startSegment);
		}
		
		// Calculate direction
		dir.z = 0;
		vec_to_angle(&dir, &dir);
		
		// Create offset for left/right street side
		VECTOR offset;
		vec_set(&offset, vector(0, 0.5 * street->width, 0));
		vec_rotate(&offset, &dir);

		// Get left and right street height
		var zStartLeft = startSegment.z;
		var zStartRight = startSegment.z;
		if (_placeOnGround == true) {
			zStartLeft = street_getground(street, vector(startSegment.x + offset.x, startSegment.y + offset.y, startSegment.z));
			zStartRight = street_getground(street, vector(startSegment.x - offset.x, startSegment.y - offset.y, startSegment.z));
		}
		zStartLeft += street->groundDist;
		zStartRight += street->groundDist;
		
		/************************************************************************************************/
		/*	Build Street Surface                                                                         */
		/************************************************************************************************/
		
		// Setup left vertex
		// First element and _v1_1 defined?
		if ((dist == 0) && (_v1_1 != NULL)) {
			left.x = _v1_1.x;
			left.y = _v1_1.z;
			left.z = _v1_1.y;
		} else {
			// Last element and _v2_1 defined?
			if ((dist > 0.98) && (_v2_1 != NULL)) {
				left.x = _v2_2.x;
				left.y = _v2_2.z;
				left.z = _v2_2.y;
			} else {
				left.x = startSegment.x + offset.x;
				left.y = zStartLeft;
				left.z = startSegment.y + offset.y;	
			}
		}
		left.nx = 0; left.ny = 1; left.nz = 0;
		//left.v1 = 15 * dist;
		//left.u1 = 0;
		left.u1 = 6 * dist;
		left.v1 = 1.0 / 3;
		
		// Setup right vertex
		if ((dist == 0) && (_v1_2 != NULL)) {
			right.x = _v1_2.x;
			right.y = _v1_2.z;
			right.z = _v1_2.y;			
		} else {
		
			if ((dist > 0.98) && (_v2_2 != NULL)) {
				right.x = _v2_1.x;
				right.y = _v2_1.z;
				right.z = _v2_1.y;
			} else {
				right.x = startSegment.x - offset.x;
				right.y = zStartRight;
				right.z = startSegment.y - offset.y;
			}
		}
		right.nx = 0; right.ny = 1; right.nz = 0;
		//right.v1 = 15 * dist;
		//right.u1 = 1;		
		right.u1 = 6 * dist;
		right.v1 = 2.0 / 3;
		
		// Create separator for this part
		int separator = street_create_separator(model, &left, &right);
		
		// Fix to be able to close loops
		if((dist > 0) && (dist < 0.99)) {
			// Connect the current and the last separator to a segment (surface)
			street_create_segment(model, previousSeparator, separator);
		}
		
		// Fix to be able to close loops	
		if (dist < 0.99) {
			previousSeparator = separator;
		}
	}
	
	// Connect to loop
	if(isLooped) {
		// Link the last to the first segment
		street_create_segment(model, previousSeparator, 0);
	}
	
	// Create the entity
	ENTITY *ent = dmdl_create_instance(model, vector(0, 0, 0), NULL);
	//ENTITY *ent = dmdl_create_instance(model, vecStartPosition, NULL);
	
	// Free data
	dmdl_delete(model);
	sys_free(splineData);
	
	return ent;
}

List *roadnetwork_from_rectlangle(int _minX, int _minY, int _maxX, int _maxY, int _dist, int _rndDeleteFactor) {
	int i,j,k;
	
	List *points = list_create();
	
	for(i=_minX; i<=_maxX; i +=_dist) {
		for(j=_minY; j<_maxY; j +=_dist) {
			
			// Skip this part if rnd greater than k
			k = integer(random(10));
			if (k > _rndDeleteFactor) continue;
			
			// Horizontal streets
			float *fCurrentMinX_H = sys_malloc(sizeof(float));
			float *fCurrentMinY_H = sys_malloc(sizeof(float));
			float *fCurrentMaxX_H = sys_malloc(sizeof(float));
			float *fCurrentMaxY_H = sys_malloc(sizeof(float));
			
			*fCurrentMinX_H = j;
			*fCurrentMaxX_H = j + _dist;
			*fCurrentMinY_H = i;
			*fCurrentMaxY_H = i;
			list_add(points, fCurrentMinX_H); list_add(points, fCurrentMinY_H); list_add(points, fCurrentMaxX_H); list_add(points, fCurrentMaxY_H);
		}
	}
	
	for(i=_minX; i<=_maxX; i +=_dist) {
		for(j=_minY; j<_maxY; j +=_dist) {
		
			// Skip this part if rnd greater than k
			k = integer(random(10));
			if (k > _rndDeleteFactor) continue;
			
			// Vertical streets
			float *fCurrentMinX_V = sys_malloc(sizeof(float));
			float *fCurrentMinY_V = sys_malloc(sizeof(float));
			float *fCurrentMaxX_V = sys_malloc(sizeof(float));
			float *fCurrentMaxY_V = sys_malloc(sizeof(float));
			
			*fCurrentMinX_V = i;
			*fCurrentMaxX_V = i;			
			*fCurrentMinY_V = j;
			*fCurrentMaxY_V = j + _dist;
			list_add(points, fCurrentMinX_V); list_add(points, fCurrentMinY_V); list_add(points, fCurrentMaxX_V); list_add(points, fCurrentMaxY_V);
		}
	}
	return points;	
}

List* roadnetwork_calculate(List *_points) {
	int i,j;
	BOOL bFoundOne = false;
	BOOL bFoundTwo = false;
	float *x1, *y1, *x2, *y2;
	Intersection *tempInter = NULL;
	//int nIncomingStreets = 0;	
	
	//List *points = list_create();
	
	List *intersections = list_create();
	i = 0;
	int nPointCount = list_get_count(_points);
	while(i<nPointCount) {
		
		//nIncomingStreets = 0;
		x1 = list_item_at(_points, i);
		y1 = list_item_at(_points, i+1);
		x2 = list_item_at(_points, i+2);
		y2 = list_item_at(_points, i+3);
		
		// There could be roads with the start==end; ignore them!
		if ((float_cmp(*x1,*x2) == 0) && (float_cmp(*y1,*y2) == 0)) {
			i +=4;
			continue;
		}
		
		bFoundOne = false;
		bFoundTwo = false;	
			
		
		// We are checking lines, so check start and end point separately
		// Start point
		for (j=0; j<list_get_count(intersections); j++) {
		
			Intersection *tempInter = list_item_at(intersections, j);
			
			// Found an intersection at a known point (1st end)
			if ((float_cmp(tempInter->pos->x,*x1) == 0) && (float_cmp(tempInter->pos->z,*y1) == 0)) {
				bFoundOne = true;
				VECTOR* vecNewAngle = sys_malloc(sizeof(VECTOR));
				VECTOR* vecTemp2 = vector(0,0,0);
				vec_set(vecTemp2, vector(*x1,*y1,0));
				vec_sub(vecTemp2, vector(*x2,*y2,0));
				vec_to_angle(vecNewAngle, vecTemp2);
				ang_normalize(vecNewAngle);
				
				IntersectionConnection *ic = sys_malloc(sizeof(IntersectionConnection));
				ic->incomingAngle = vecNewAngle;
				ic->id = i;
				ic->isConnected = 0;
				
				list_add(tempInter->incomingConnections, ic);
			}
		}
		
		if (bFoundOne == false) {
			Intersection *newInter = intersection_create(vector(*x1, 0, *y1));
			//newInter->incomingStreets = 1;
			newInter->source = 1;
			
			VECTOR* vecNewAngle = sys_malloc(sizeof(VECTOR));
			VECTOR* vecTemp2 = vector(0,0,0);
			vec_set(vecTemp2, vector(*x1,*y1,0));
			vec_sub(vecTemp2, vector(*x2,*y2,0));
			vec_to_angle(vecNewAngle, vecTemp2);
			ang_normalize(vecNewAngle);

			IntersectionConnection *ic = sys_malloc(sizeof(IntersectionConnection));
			ic->incomingAngle = vecNewAngle;
			ic->id = i;
			ic->isConnected = 0;
			
			list_add(newInter->incomingConnections, ic);
			
			list_add(intersections, newInter);
		}
	
		// End point
		for (j=0; j<list_get_count(intersections); j++) {
		
			Intersection *tempInter = list_item_at(intersections, j);
			if ((float_cmp(tempInter->pos->x,*x2) == 0) && (float_cmp(tempInter->pos->z,*y2) == 0)) {
				bFoundTwo = true;
				//tempInter->incomingStreets +=1;
				VECTOR* vecNewAngle = sys_malloc(sizeof(VECTOR));
				VECTOR* vecTemp2 = vector(0,0,0);
				vec_set(vecTemp2, vector(*x2,*y2,0));
				vec_sub(vecTemp2, vector(*x1,*y1,0));
				vec_to_angle(vecNewAngle, vecTemp2);
				ang_normalize(vecNewAngle);
				
				IntersectionConnection *ic = sys_malloc(sizeof(IntersectionConnection));
				ic->incomingAngle = vecNewAngle;
				ic->id = i;
				ic->isConnected = 0;
				
				list_add(tempInter->incomingConnections, ic);
			}
		}
			
		if (bFoundTwo == false) {
			Intersection *newInter = intersection_create(vector(*x2, 0, *y2));
			//newInter->incomingStreets = 1;
			newInter->source = 2;
			
			VECTOR* vecNewAngle = sys_malloc(sizeof(VECTOR));
			VECTOR* vecTemp2 = vector(0,0,0);
			vec_set(vecTemp2, vector(*x2,*y2,0));
			vec_sub(vecTemp2, vector(*x1,*y1,0));
			vec_to_angle(vecNewAngle, vecTemp2);
			ang_normalize(vecNewAngle);

			IntersectionConnection *ic = sys_malloc(sizeof(IntersectionConnection));
			ic->incomingAngle = vecNewAngle;
			ic->id = i;
			ic->isConnected = 0;
			
			list_add(newInter->incomingConnections, ic);

			list_add(intersections, newInter);
		}
		
		i +=4;
	}
	return intersections;	
}

List *roadnetwork_from_voronoi(int _pointCount, int _minX, int _minY, int _maxX, int _maxY) {
	
	int i;
	List *points = list_create();
	
	// Generate voronoi
	vo_init();
	for(i=0; i<_pointCount; i++) {
		vo_add_point(_minX + integer(random(2 * _maxX)), _minY + integer(random(2 * _maxY)));
	}
	vo_execute(_minX, _maxX, _minY, _maxY, 100);
	int nResults = vo_get_result_count();
	
	// Read streets
	for(i=0; i<nResults; i++) {
		float *x1 = sys_malloc(sizeof(float));
		float *y1 = sys_malloc(sizeof(float));
		float *x2 = sys_malloc(sizeof(float));
		float *y2 = sys_malloc(sizeof(float));			
		vo_get_result_at(i, x1, y1, x2, y2);		
		list_add(points, x1); list_add(points, y1); list_add(points, x2); list_add(points, y2);
	}
	vo_free();
	return points;
}

void roadnetwork_build(List *_intersections, int _zPosition, BOOL _placeOnGround) {
	
	int i,j,k,l;
	
	int count = list_get_count(_intersections);
	for(i=0; i<list_get_count(_intersections); i++) {
		Intersection* tempInter = list_item_at(_intersections, i);
		tempInter->pos->y = _zPosition;
		
		ENTITY* entInter = build_intersection(tempInter);
		if (_placeOnGround) {
			place_street_on_ground(entInter, STREET_GROUND_DIST);
		}
	}
	
	// Build streets between intersections
	VECTOR vecTemp5, vecTemp6;
	VECTOR vx1, vx2, vx3, vx4;
	for(i=0; i<list_get_count(_intersections); i++) {
		Intersection* tempInter = list_item_at(_intersections, i);
		
		for(j=0; j<list_get_count(tempInter->incomingConnections); j++) {
			
			IntersectionConnection *tempCon = (IntersectionConnection*)list_item_at(tempInter->incomingConnections, j);
			
			if(tempCon->isConnected == 0) {
				
				for(k=0; k<list_get_count(_intersections); k++) {
					Intersection* tempInter2 = list_item_at(_intersections, k);
					
					if (tempInter2 != tempInter) {
					
						for(l=0; l<list_get_count(tempInter2->incomingConnections); l++) {
							
							IntersectionConnection *tempCon2 = (IntersectionConnection*)list_item_at(tempInter2->incomingConnections, l);
							
							if ((tempCon2->isConnected == 0) && (tempCon->id == tempCon2->id)) {
								
								// Create Street (width=20, grounddist=3)
								Street *s1 = NULL;
								if (_placeOnGround) {
									s1 = street_create(20, STREET_GROUND_DIST);
								} else {
									s1 = street_create(20, 0);
								}
								
								// Add street positions	
								VECTOR* vecTemp1 = sys_malloc(sizeof(VECTOR));
								vec_set(vecTemp1, vector(tempInter->pos->x, tempInter->pos->z, tempInter->pos->y));
								vec_set(vecTemp5, vector(tempCon->pos->x, tempCon->pos->z, tempCon->pos->y));
								vec_mul(vecTemp5, vector(tempInter->ent->scale_x, tempInter->ent->scale_y, tempInter->ent->scale_z));
								vec_rotate(vecTemp5, tempInter->ent->pan);
								vec_add(vecTemp1, vecTemp5);
								
								VECTOR* vecTemp2 = sys_malloc(sizeof(VECTOR));
								vec_set(vecTemp2, vector(tempInter2->pos->x, tempInter2->pos->z, tempInter2->pos->y));
								vec_set(vecTemp6, vector(tempCon2->pos->x, tempCon2->pos->z, tempCon2->pos->y));
								vec_mul(vecTemp6, vector(tempInter2->ent->scale_x, tempInter2->ent->scale_y, tempInter2->ent->scale_z));
								vec_rotate(vecTemp6, tempInter2->ent->pan);
								vec_add(vecTemp2, vecTemp6);
								
								// 3 and 4 have to be an extension to the intersection endings
								VECTOR* vecTemp3 = sys_malloc(sizeof(VECTOR));
								vec_diff(vecTemp3, vecTemp2, vector(tempInter2->pos->x, tempInter2->pos->z, tempInter2->pos->y));
								vec_scale(vecTemp3, 4);
								vec_add(vecTemp3, vector(tempInter2->pos->x, tempInter2->pos->z, tempInter2->pos->y));
								
								VECTOR* vecTemp4 = sys_malloc(sizeof(VECTOR));
								vec_diff(vecTemp4, vecTemp1, vector(tempInter->pos->x, tempInter->pos->z, tempInter->pos->y));
								vec_scale(vecTemp4, 4);
								vec_add(vecTemp4, vector(tempInter->pos->x, tempInter->pos->z, tempInter->pos->y));
								
								street_addpos(s1, vecTemp2);
								street_addpos(s1, vecTemp3);
								street_addpos(s1, vecTemp4);
								street_addpos(s1, vecTemp1);
								
								// Calculate intersection <-> street connections
								vec_for_ent_ext(&vx1, tempInter->ent, tempCon->leftVertex);
								vec_for_ent_ext(&vx2, tempInter->ent, tempCon->rightVertex);
								vec_for_ent_ext(&vx3, tempInter2->ent, tempCon2->leftVertex);
								vec_for_ent_ext(&vx4, tempInter2->ent, tempCon2->rightVertex);
								
								// "Draw" streets
								ENTITY *street = street_build_ext(s1, bmapStreetTexture, bmapStreetTextureNM, _placeOnGround, 0.01, vx3, vx4, vx1, vx2);
								
								#ifdef PROC_USE_SHADERS
									street.material = mtl_specBump;
								#endif
								
								//Mark connections as "connected"
								tempCon2->isConnected = 1;
								tempCon->isConnected = 1;
							}
						}
					}
				}
			}
		}
	}
}


// ----------------------------------------------------------------------------------------
// Street tool - Create textures
// ----------------------------------------------------------------------------------------

void proc_city_create_skins() {
	
	COLOR* colStreet       = vector(123,123,123);
	COLOR* colStreetMarker = vector(255, 255, 255);
	int i;
	
	// -----------------------------------
	// Intersection 1 - street ending
	// -----------------------------------
	bmapStreetIntersection1 = bmap_createblack(PROC_TEXT_RES, PROC_TEXT_RES, 32);
	
	/*bmapStreetIntersection1 = bmap_draw_triangle_filled(bmapStreetIntersection1, 
		0,             0,
		0,             PROC_TEXT_RES,
		PROC_TEXT_RES, PROC_TEXT_RES / 3,
		colStreet, 100
	);
	
	bmapStreetIntersection1 = bmap_draw_triangle_filled(bmapStreetIntersection1,
		0,                       0,
		(PROC_TEXT_RES / 3) * 2, 0,
		PROC_TEXT_RES,           PROC_TEXT_RES / 3,
		colStreet, 100
	);
	
	bmapStreetIntersection1 = bmap_draw_triangle_filled(bmapStreetIntersection1,
		0,             PROC_TEXT_RES,
		PROC_TEXT_RES, (PROC_TEXT_RES / 3) * 2,
		PROC_TEXT_RES, PROC_TEXT_RES / 3,
		colStreet, 100
	);
	
	bmapStreetIntersection1 = bmap_draw_triangle_filled(bmapStreetIntersection1,
		0,                       PROC_TEXT_RES,
		PROC_TEXT_RES,           (PROC_TEXT_RES / 3) * 2,
		(PROC_TEXT_RES / 3) * 2, PROC_TEXT_RES,
		colStreet, 100
	);*/
	
	bmapStreetIntersection1 = bmap_draw_rectangle_filled(bmapStreetIntersection1,
		0,       0,
		PROC_TEXT_RES, PROC_TEXT_RES,
		colStreet, 100
	);
	
	bmapStreetIntersection1 = bmap_draw_line(bmapStreetIntersection1,
		(PROC_TEXT_RES / 3),                               (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		((PROC_TEXT_RES / 3) * 2) - (PROC_TEXT_RES / 16),  (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	
	bmapStreetIntersection1 = bmap_draw_line(bmapStreetIntersection1,
		((PROC_TEXT_RES / 3) * 2) - (PROC_TEXT_RES / 16),  (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		((PROC_TEXT_RES / 3) * 2) - (PROC_TEXT_RES / 51),  (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 9) + (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	
	bmapStreetIntersection1 = bmap_draw_line(bmapStreetIntersection1,
		((PROC_TEXT_RES / 3) * 2) - (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 9) + (PROC_TEXT_RES / 51),
		((PROC_TEXT_RES / 3) * 2) - (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 9)*2 - (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	
	bmapStreetIntersection1 = bmap_draw_line(bmapStreetIntersection1,
		((PROC_TEXT_RES / 3) * 2) - (PROC_TEXT_RES / 51),  (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 9)*2 - (PROC_TEXT_RES / 51),
		((PROC_TEXT_RES / 3) * 2) - (PROC_TEXT_RES / 16),  (PROC_TEXT_RES / 3)*2 - (PROC_TEXT_RES / 51),
	colStreetMarker, 100
	);
	
	bmapStreetIntersection1 = bmap_draw_line(bmapStreetIntersection1,
		((PROC_TEXT_RES / 3) * 2) - (PROC_TEXT_RES / 16),  (PROC_TEXT_RES / 3)*2 - (PROC_TEXT_RES / 51),
		(PROC_TEXT_RES / 3),                               (PROC_TEXT_RES / 3)*2 - (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);		
	
	/*bmapStreetIntersection1 = bmap_draw_line(bmapStreetIntersection1,
		0,                                                PROC_TEXT_RES / 51,
		((PROC_TEXT_RES / 3) * 2) - (PROC_TEXT_RES / 51), PROC_TEXT_RES / 51,
		colStreetMarker, 100
	);
	
	bmapStreetIntersection1 = bmap_draw_line(bmapStreetIntersection1,
		((PROC_TEXT_RES / 3) * 2) - (PROC_TEXT_RES / 51), PROC_TEXT_RES / 51,
		PROC_TEXT_RES - (PROC_TEXT_RES / 51),             (PROC_TEXT_RES / 3),
		colStreetMarker, 100
	);
	
	bmapStreetIntersection1 = bmap_draw_line(bmapStreetIntersection1,
		PROC_TEXT_RES - (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3),
		PROC_TEXT_RES - (PROC_TEXT_RES / 51), ((PROC_TEXT_RES / 3) * 2),
		colStreetMarker, 100
	);
	
	bmapStreetIntersection1 = bmap_draw_line(bmapStreetIntersection1,
		PROC_TEXT_RES - (PROC_TEXT_RES / 51), ((PROC_TEXT_RES / 3) * 2),
		((PROC_TEXT_RES / 3) * 2),            PROC_TEXT_RES - (PROC_TEXT_RES / 51),
	colStreetMarker, 100
	);
	
	bmapStreetIntersection1 = bmap_draw_line(bmapStreetIntersection1,
		((PROC_TEXT_RES / 3) * 2), PROC_TEXT_RES - (PROC_TEXT_RES / 51),
		0,                         PROC_TEXT_RES - (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);*/
	
	
	
	// -----------------------------------
	// Intersection 2 - street connection
	// -----------------------------------
	
	// Version 1 - straight
	bmapStreetIntersection2_1 = bmap_createblack(PROC_TEXT_RES, PROC_TEXT_RES, 32);
	
	bmapStreetIntersection2_1 = bmap_draw_rectangle_filled(bmapStreetIntersection2_1,
		PROC_TEXT_RES / 3,       PROC_TEXT_RES / 3,
		(PROC_TEXT_RES / 3) * 2, (PROC_TEXT_RES / 3) * 2,
		colStreet, 100
	);
	
	// Lines at the border
	bmapStreetIntersection2_1 = bmap_draw_line(bmapStreetIntersection2_1,
		0,             (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		PROC_TEXT_RES, (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	bmapStreetIntersection2_1 = bmap_draw_line(bmapStreetIntersection2_1,
		0,             (PROC_TEXT_RES / 3) * 2 - (PROC_TEXT_RES / 51),
		PROC_TEXT_RES, (PROC_TEXT_RES / 3) * 2 - (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	
	// Lines in the middle
	i = PROC_TEXT_RES / 36;
	while(i < PROC_TEXT_RES) {
		bmapStreetIntersection2_1 = bmap_draw_line(bmapStreetIntersection2_1,
			i,                      PROC_TEXT_RES / 2,
			i+(PROC_TEXT_RES / 17), PROC_TEXT_RES / 2,
			colStreetMarker, 100
		);
		i +=PROC_TEXT_RES / 10.2;
	}
	
	
	
	// Version 2 - left
	bmapStreetIntersection2_2 = bmap_createblack(PROC_TEXT_RES, PROC_TEXT_RES, 32);
	
	bmapStreetIntersection2_2 = bmap_draw_rectangle_filled(bmapStreetIntersection2_2,
		PROC_TEXT_RES / 3,       PROC_TEXT_RES / 3,
		(PROC_TEXT_RES / 3) * 2, (PROC_TEXT_RES / 3) * 2,
		colStreet, 100
	);
	
	// Lines at the border
	bmapStreetIntersection2_2 = bmap_draw_line(bmapStreetIntersection2_2,
		(PROC_TEXT_RES / 3),                        (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		(PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	bmapStreetIntersection2_2 = bmap_draw_line(bmapStreetIntersection2_2,
		(PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		(PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3),
		colStreetMarker, 100
	);
	
	bmapStreetIntersection2_2 = bmap_draw_line(bmapStreetIntersection2_2,
		(PROC_TEXT_RES / 3),                          (PROC_TEXT_RES / 3)*2 - (PROC_TEXT_RES / 51),
		(PROC_TEXT_RES / 3)*2 - (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3)*2 - (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	bmapStreetIntersection2_2 = bmap_draw_line(bmapStreetIntersection2_2,
		(PROC_TEXT_RES / 3)*2 - (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3)*2 - (PROC_TEXT_RES / 51),
		(PROC_TEXT_RES / 3)*2 - (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3),
		colStreetMarker, 100
	);	
	
	// Lines in the middle
	i = (PROC_TEXT_RES / 3);
	while(i < (PROC_TEXT_RES / 2)) {
		bmapStreetIntersection2_2 = bmap_draw_line(bmapStreetIntersection2_2,
			i,                      PROC_TEXT_RES / 2,
			i+(PROC_TEXT_RES / 17), PROC_TEXT_RES / 2,
			colStreetMarker, 100
		);
		
		bmapStreetIntersection2_2 = bmap_draw_line(bmapStreetIntersection2_2,
			PROC_TEXT_RES / 2, i,
			PROC_TEXT_RES / 2, i+(PROC_TEXT_RES / 17),
			colStreetMarker, 100
		);
				
		i +=PROC_TEXT_RES / 10.2;
	}
	
	
	
	
	
	
	// Version 3 - right
	bmapStreetIntersection2_3 = bmap_createblack(PROC_TEXT_RES, PROC_TEXT_RES, 32);
	
	bmapStreetIntersection2_3 = bmap_draw_rectangle_filled(bmapStreetIntersection2_3,
		PROC_TEXT_RES / 3,       PROC_TEXT_RES / 3,
		(PROC_TEXT_RES / 3) * 2, (PROC_TEXT_RES / 3) * 2,
		colStreet, 100
	);
	
	// Lines at the border
	bmapStreetIntersection2_3 = bmap_draw_line(bmapStreetIntersection2_3,
		(PROC_TEXT_RES / 3),                        (PROC_TEXT_RES / 3)*2 - (PROC_TEXT_RES / 51),
		(PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3)*2 - (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	bmapStreetIntersection2_3 = bmap_draw_line(bmapStreetIntersection2_3,
		(PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3)*2 - (PROC_TEXT_RES / 51),
		(PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3)*2,
		colStreetMarker, 100
	);
	
	bmapStreetIntersection2_3 = bmap_draw_line(bmapStreetIntersection2_3,
		(PROC_TEXT_RES / 3),                          (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		(PROC_TEXT_RES / 3)*2 - (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	bmapStreetIntersection2_3 = bmap_draw_line(bmapStreetIntersection2_3,
		(PROC_TEXT_RES / 3)*2 - (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		(PROC_TEXT_RES / 3)*2 - (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3)*2,
		colStreetMarker, 100
	);	
	
	// Lines in the middle
	i = (PROC_TEXT_RES / 3);
	while(i < (PROC_TEXT_RES / 2)) {
		bmapStreetIntersection2_3 = bmap_draw_line(bmapStreetIntersection2_3,
			i,                      PROC_TEXT_RES / 2,
			i+(PROC_TEXT_RES / 17), PROC_TEXT_RES / 2,
			colStreetMarker, 100
		);
		i +=PROC_TEXT_RES / 10.2;
	}	
	
	i = (PROC_TEXT_RES / 3)*2;
	while(i > (PROC_TEXT_RES / 2)) {
		bmapStreetIntersection2_3 = bmap_draw_line(bmapStreetIntersection2_3,
			PROC_TEXT_RES / 2, i,
			PROC_TEXT_RES / 2, i  - (PROC_TEXT_RES / 17),
			colStreetMarker, 100
		);
		i -=PROC_TEXT_RES / 10.2;
	}
	


	// -----------------------------------
	// Intersection 3 - Three streets
	// -----------------------------------
	bmapStreetIntersection3 = bmap_createblack(PROC_TEXT_RES, PROC_TEXT_RES, 32);
	bmapStreetIntersection3 = bmap_draw_rectangle_filled(bmapStreetIntersection3,
		0,             PROC_TEXT_RES / 3,
		PROC_TEXT_RES, (PROC_TEXT_RES / 3) * 2,
		colStreet, 100
	);
	bmapStreetIntersection3 = bmap_draw_rectangle_filled(bmapStreetIntersection3,
		PROC_TEXT_RES / 3,       0,
		(PROC_TEXT_RES / 3) * 2, PROC_TEXT_RES / 3,
		colStreet, 100
	);
	
	// Draw lines at the border
	bmapStreetIntersection3 = bmap_draw_line(bmapStreetIntersection3,
		0,                                           (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		(PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),  (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	
	bmapStreetIntersection3 = bmap_draw_line(bmapStreetIntersection3,
		(PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		(PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51), 0,
		colStreetMarker, 100
	);
	
	bmapStreetIntersection3 = bmap_draw_line(bmapStreetIntersection3,
		((PROC_TEXT_RES / 3) * 2) - (PROC_TEXT_RES / 51), 0,
		((PROC_TEXT_RES / 3) * 2) - (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	
	bmapStreetIntersection3 = bmap_draw_line(bmapStreetIntersection3,
		((PROC_TEXT_RES / 3) * 2) - (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		PROC_TEXT_RES,                                    (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	
	bmapStreetIntersection3 = bmap_draw_line(bmapStreetIntersection3,
		0,             ((PROC_TEXT_RES / 3) * 2) - (PROC_TEXT_RES / 51),
		PROC_TEXT_RES, ((PROC_TEXT_RES / 3) * 2) - (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	
	// Draw lines in the middle
	i = PROC_TEXT_RES / 36;
	while(i < (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51)) {
		
		// Horizontal
		bmapStreetIntersection3 = bmap_draw_line(bmapStreetIntersection3,
			i, PROC_TEXT_RES / 2,
			i+(PROC_TEXT_RES / 17), PROC_TEXT_RES / 2,
			colStreetMarker, 100
		);
		
		// Horizontal
		bmapStreetIntersection3 = bmap_draw_line(bmapStreetIntersection3,
			PROC_TEXT_RES / 2, i,
			PROC_TEXT_RES / 2, i+(PROC_TEXT_RES / 17),
			colStreetMarker, 100
		);
		
		// Vertical
		bmapStreetIntersection3 = bmap_draw_line(bmapStreetIntersection3,
			i+PROC_TEXT_RES / 1.677, PROC_TEXT_RES / 2,
			i+PROC_TEXT_RES / 1.677+(PROC_TEXT_RES / 17), PROC_TEXT_RES / 2,
			colStreetMarker, 100
		);	
		i +=PROC_TEXT_RES / 10.2;
	}
	
	// -----------------------------------
	// Intersection 4 - Cross
	// -----------------------------------
	bmapStreetIntersection4 = bmap_createblack(PROC_TEXT_RES, PROC_TEXT_RES, 32);
	
	bmapStreetIntersection4 = bmap_draw_rectangle_filled(bmapStreetIntersection4,
		0,             PROC_TEXT_RES / 3,
		PROC_TEXT_RES, (PROC_TEXT_RES / 3) * 2,
		colStreet, 100
	);
	
	bmapStreetIntersection4 = bmap_draw_rectangle_filled(bmapStreetIntersection4,
		PROC_TEXT_RES / 3,       0,
		(PROC_TEXT_RES / 3) * 2, PROC_TEXT_RES / 3,
		colStreet, 100
	);
	
	bmapStreetIntersection4 = bmap_draw_rectangle_filled(bmapStreetIntersection4,
		PROC_TEXT_RES / 3, (PROC_TEXT_RES / 3) * 2 + 1,
		(PROC_TEXT_RES / 3) * 2, PROC_TEXT_RES,
		colStreet, 100
	);
	
	// Draw lines at the border
	bmapStreetIntersection4 = bmap_draw_line(bmapStreetIntersection4,
		0,                                           (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		(PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),  (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	
	bmapStreetIntersection4 = bmap_draw_line(bmapStreetIntersection4,
		(PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		(PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51), 0,
		colStreetMarker, 100
	);
	
	bmapStreetIntersection4 = bmap_draw_line(bmapStreetIntersection4,
		((PROC_TEXT_RES / 3) *2) - (PROC_TEXT_RES / 51), 0,
		((PROC_TEXT_RES / 3) *2) - (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	
	bmapStreetIntersection4 = bmap_draw_line(bmapStreetIntersection4,
		((PROC_TEXT_RES / 3) *2) - (PROC_TEXT_RES / 51), (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		PROC_TEXT_RES, (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	
	
	// Lines at the border
	bmapStreetIntersection4 = bmap_draw_line(bmapStreetIntersection4,
		0,                                          ((PROC_TEXT_RES / 3) *2) - (PROC_TEXT_RES / 51),
		(PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51), ((PROC_TEXT_RES / 3) *2) - (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	
	bmapStreetIntersection4 = bmap_draw_line(bmapStreetIntersection4,
		(PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51), ((PROC_TEXT_RES / 3) *2) - (PROC_TEXT_RES / 51),
		(PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51), PROC_TEXT_RES,
		colStreetMarker, 100
	);
	
	bmapStreetIntersection4 = bmap_draw_line(bmapStreetIntersection4,
		((PROC_TEXT_RES / 3) *2) - (PROC_TEXT_RES / 51), PROC_TEXT_RES,
		((PROC_TEXT_RES / 3) *2) - (PROC_TEXT_RES / 51), ((PROC_TEXT_RES / 3) *2) - (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	
	bmapStreetIntersection4 = bmap_draw_line(bmapStreetIntersection4,
		((PROC_TEXT_RES / 3) *2) - (PROC_TEXT_RES / 51), ((PROC_TEXT_RES / 3) *2) - (PROC_TEXT_RES / 51),
		PROC_TEXT_RES, ((PROC_TEXT_RES / 3) *2) - (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
		
	// Draw lines in the middle
	i = 7;
	while(i < (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51)) {
		
		bmapStreetIntersection4 = bmap_draw_line(bmapStreetIntersection4,
			i,                      PROC_TEXT_RES / 2,
			i+(PROC_TEXT_RES / 17), PROC_TEXT_RES / 2,
			colStreetMarker, 100
		); // Horizontal
		
		bmapStreetIntersection4 = bmap_draw_line(bmapStreetIntersection4,
			PROC_TEXT_RES / 2, i,
			PROC_TEXT_RES / 2, i+(PROC_TEXT_RES / 17),
			colStreetMarker, 100
		); // Vertical
		
		bmapStreetIntersection4 = bmap_draw_line(bmapStreetIntersection4,
			(PROC_TEXT_RES / 1.677)+i,                      PROC_TEXT_RES / 2,
			(PROC_TEXT_RES / 1.677)+i+(PROC_TEXT_RES / 17), PROC_TEXT_RES / 2,
			colStreetMarker, 100
		);
		
		bmapStreetIntersection4 = bmap_draw_line(bmapStreetIntersection4,
			PROC_TEXT_RES / 2, (PROC_TEXT_RES / 1.677)+i,
			PROC_TEXT_RES / 2, (PROC_TEXT_RES / 1.677)+i+(PROC_TEXT_RES / 17),
			colStreetMarker, 100
		);
		
		i +=PROC_TEXT_RES / 10.2;
	}
	
	// -----------------------------------
	// Intersection 5 - Circle
	// -----------------------------------
	bmapStreetIntersection5 = bmap_createblack(PROC_TEXT_RES, PROC_TEXT_RES, 32);
	bmapStreetIntersection5 = bmap_draw_rectangle_filled(bmapStreetIntersection5, 0, 0, PROC_TEXT_RES, PROC_TEXT_RES, colStreet, 100);
	
	// Outer circle
	bmapStreetIntersection5 = bmap_draw_circle(bmapStreetIntersection5, PROC_TEXT_RES / 2, PROC_TEXT_RES / 2, (PROC_TEXT_RES / 2) - 30, colStreetMarker, 100);
	
	// Inner circle
	bmapStreetIntersection5 = bmap_draw_circle(bmapStreetIntersection5, PROC_TEXT_RES / 2, PROC_TEXT_RES / 2, 20, colStreetMarker, 100);
	
	
	// -----------------------------------
	// Common street texture
	// -----------------------------------	
	bmapStreetTexture = bmap_createblack(PROC_TEXT_RES, PROC_TEXT_RES, 32);
	
	bmapStreetTexture = bmap_draw_rectangle_filled(bmapStreetTexture,
		0,             PROC_TEXT_RES / 3,
		PROC_TEXT_RES, (PROC_TEXT_RES / 3) * 2,
		colStreet, 100
	);
	
	// Lines at the border
	bmapStreetTexture = bmap_draw_line(bmapStreetTexture,
		0,             (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		PROC_TEXT_RES, (PROC_TEXT_RES / 3) + (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	bmapStreetTexture = bmap_draw_line(bmapStreetTexture,
		0,             (PROC_TEXT_RES / 3) * 2 - (PROC_TEXT_RES / 51),
		PROC_TEXT_RES, (PROC_TEXT_RES / 3) * 2 - (PROC_TEXT_RES / 51),
		colStreetMarker, 100
	);
	
	// Lines in the middle
	i = PROC_TEXT_RES / 36;
	while(i < PROC_TEXT_RES) {
		bmapStreetTexture = bmap_draw_line(bmapStreetTexture,
			i,                      PROC_TEXT_RES / 2,
			i+(PROC_TEXT_RES / 17), PROC_TEXT_RES / 2,
			colStreetMarker, 100
		);
		i +=PROC_TEXT_RES / 10.2;
	}
	
	
	// -----------------------------------
	// Normal maps
	// -----------------------------------	
	
	bmapStreetIntersectionNM1   = bmap_createblack(bmap_width(bmapStreetIntersection1),   bmap_height(bmapStreetIntersection1), 32);
	bmapStreetIntersectionNM2_1 = bmap_createblack(bmap_width(bmapStreetIntersection2_1), bmap_height(bmapStreetIntersection2_1), 32);
	bmapStreetIntersectionNM2_2 = bmap_createblack(bmap_width(bmapStreetIntersection2_2), bmap_height(bmapStreetIntersection2_2), 32);
	bmapStreetIntersectionNM2_3 = bmap_createblack(bmap_width(bmapStreetIntersection2_3), bmap_height(bmapStreetIntersection2_3), 32);
	bmapStreetIntersectionNM3   = bmap_createblack(bmap_width(bmapStreetIntersection3),   bmap_height(bmapStreetIntersection3), 32);
	bmapStreetIntersectionNM4   = bmap_createblack(bmap_width(bmapStreetIntersection4),   bmap_height(bmapStreetIntersection4), 32);
	bmapStreetIntersectionNM5   = bmap_createblack(bmap_width(bmapStreetIntersection5),   bmap_height(bmapStreetIntersection5), 32);
	bmapStreetTextureNM         = bmap_createblack(bmap_width(bmapStreetTexture),         bmap_height(bmapStreetTexture), 32);
	
	
	bmap_blit(bmapStreetIntersectionNM1,   bmapStreetIntersection1,   NULL, NULL);
	bmap_blit(bmapStreetIntersectionNM2_1, bmapStreetIntersection2_1, NULL, NULL);
	bmap_blit(bmapStreetIntersectionNM2_2, bmapStreetIntersection2_2, NULL, NULL);
	bmap_blit(bmapStreetIntersectionNM2_3, bmapStreetIntersection2_3, NULL, NULL);
	bmap_blit(bmapStreetIntersectionNM3,   bmapStreetIntersection3,   NULL, NULL);
	bmap_blit(bmapStreetIntersectionNM4,   bmapStreetIntersection4,   NULL, NULL);
	bmap_blit(bmapStreetIntersectionNM5,   bmapStreetIntersection5,   NULL, NULL);
	bmap_blit(bmapStreetTextureNM,         bmapStreetTexture,         NULL, NULL);
	
	bmap_to_normals(bmapStreetIntersectionNM1, 100);
	bmap_to_normals(bmapStreetIntersectionNM2_1, 100);
	bmap_to_normals(bmapStreetIntersectionNM2_2, 100);
	bmap_to_normals(bmapStreetIntersectionNM2_3, 100);
	bmap_to_normals(bmapStreetIntersectionNM3, 100);
	bmap_to_normals(bmapStreetIntersectionNM4, 100);
	bmap_to_normals(bmapStreetIntersectionNM5, 100);
	bmap_to_normals(bmapStreetTextureNM, 100);
}

#endif