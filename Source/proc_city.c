#ifndef _PROC_CITY_C_
#define _PROC_CITY_C_

#include "math.h"
#include "bmap.h" // Needs "BmapGS.dll"

#define PROC_DEBUG

#define PROC_INTERSECTION_EXTREMITIES 5

// ----------------------------------------------------------------------------------------
// Street tool
// ----------------------------------------------------------------------------------------

Intersection *intersection_create(VECTOR* _pos)
{
	Intersection *newInter = sys_malloc(sizeof(Intersection));
	newInter->incomingConnections = list_create();
	newInter->pos = sys_malloc(sizeof(VECTOR));
	vec_set(newInter->pos, _pos);
	return newInter;
}


Street *street_create(int _streetWidth, int groundDistance)
{
	// Sets up a new street object
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


	switch(_inter->incomingStreets) {
		case 0: 
			// Error
		break;
		case 1:
		case 2: // Todo: 2 has also to be rotated!
			IntersectionConnection *ic = (IntersectionConnection*)list_item_at(_inter->incomingConnections, 0);
			return ic->incomingAngle->pan;
		break;
		case 3:
		case 4:
		
			int j,k,tempPan, resultPan;
			VECTOR currentPan, currentPan2;
			float bestPan = list_get_count(_inter->incomingConnections) * 360;
			
			for (j=0; j<list_get_count(_inter->incomingConnections); j++) {
				
				float newPan = 0;
				IntersectionConnection *ic = (IntersectionConnection*)list_item_at(_inter->incomingConnections, j);
				
				
				vec_set(currentPan, ic->incomingAngle->pan);
				tempPan = currentPan.x;
				
				for(k=0; k<list_get_count(_inter->incomingConnections); k++) {
					
					IntersectionConnection *ic2 = (IntersectionConnection*)list_item_at(_inter->incomingConnections, k);
					
					
					vec_set(currentPan2, ic2->incomingAngle->pan);
					if (tempPan > currentPan2.x) {
						newPan += tempPan - currentPan2.x;
					} else {
						newPan += currentPan2.x - tempPan;
					}
				}
				
				if (newPan<bestPan) {
					bestPan = newPan;
					resultPan = currentPan.x;
				}
			}
			return resultPan;				
		break;
		default:
			IntersectionConnection *ic = (IntersectionConnection*)list_item_at(_inter->incomingConnections, 0);
			return ic->incomingAngle->pan;
		break;
	}



	
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
				
				if (ic->leftVertexPos != NULL) {
					draw_text("Left: ", 480, i*20, COLOR_GREEN);
					draw_text(str_for_int(NULL, my.x+ic->leftVertexPos->x), 520, i*20, COLOR_RED);
					draw_text(str_for_int(NULL, my.y+ic->leftVertexPos->y), 560, i*20, COLOR_RED);
					draw_text(str_for_int(NULL, my.z+ic->leftVertexPos->z), 600, i*20, COLOR_RED);
					
					draw_text("Right: ", 640, i*20, COLOR_GREEN);
					draw_text(str_for_int(NULL, my.x+ic->rightVertexPos->x), 680, i*20, COLOR_RED);
					draw_text(str_for_int(NULL, my.y+ic->rightVertexPos->y), 720, i*20, COLOR_RED);
					draw_text(str_for_int(NULL, my.z+ic->rightVertexPos->z), 760, i*20, COLOR_RED);
					
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
				my.pan -=2 * time_step;
			}
		}
		wait(1);
	}
}

int pan_angle_compare(ListData *left, ListData *right) { //and returns 1 if left>right, 0 if left=right and -1 if left<right.
	
	IntersectionConnection *icLeft = (IntersectionConnection*)left;
	IntersectionConnection *icRight = (IntersectionConnection*)right;
	
	if (((ANGLE*)icLeft->incomingAngle)->pan > ((ANGLE*)icRight->incomingAngle)->pan) {
		return 1;
	}
	
	if (((ANGLE*)icLeft->incomingAngle)->pan == ((ANGLE*)icRight->incomingAngle)->pan) {
		return 0;
	}
	
	if (((ANGLE*)icLeft->incomingAngle)->pan < ((ANGLE*)icRight->incomingAngle)->pan) {
		return -1;
	}	
}

ENTITY *build_intersection(Intersection *_intersection)
{
	int nIncomingCount = _intersection->incomingStreets;
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
			// y == z
			D3DVERTEX *v1 = create_vertex(0 - 10,  0, 0 - 10,   0, 1, 0,   0,    0);
			D3DVERTEX *v2 = create_vertex(0 - 10,  0, 0 + 10,   0, 1, 0,   0,    1);
			D3DVERTEX *v3 = create_vertex(0 + 2.5, 0, 0 + 10,   0, 1, 0,   0.66, 1);
			D3DVERTEX *v4 = create_vertex(0 + 7.5, 0, 0 + 2.5,  0, 1, 0,   1,    0.66);
			D3DVERTEX *v5 = create_vertex(0 + 7.5, 0, 0 - 2.5,  0, 1, 0,   1,    0.33);
			D3DVERTEX *v6 = create_vertex(0 + 2.5, 0, 0 - 10,   0, 1, 0,   0.66, 0);
			
			
			// Store the middle of the end that should be connected to a street
			if (list_get_count(_intersection->incomingConnections) > 0) {
				IntersectionConnection *ic = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, 0);
				ic->pos = sys_malloc(sizeof(VECTOR));
				ic->leftVertexPos   = sys_malloc(sizeof(VECTOR));
				ic->rightVertexPos  = sys_malloc(sizeof(VECTOR));
				vec_set(ic->pos, vector(-10,0,0));
				vec_set(ic->leftVertexPos, vector(-10,0,-10));
				vec_set(ic->rightVertexPos, vector(-10,0,10));
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
			
			//dmdl_save(model, "inter1.x");
			//bmap_save(bmapStreetIntersection1, "test.bmp");
		break;
		
		// A simple connection
		case 2:
		
			// Calculate the angle difference
			if (list_get_count(_intersection->incomingConnections) > 0) {
				
				
				// Set the right skin
				IntersectionConnection *ic1 = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, 0);
				IntersectionConnection *ic2 = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, 1);
				
				ic1->pos            = sys_malloc(sizeof(VECTOR));
				ic1->leftVertexPos  = sys_malloc(sizeof(VECTOR));
				ic1->rightVertexPos = sys_malloc(sizeof(VECTOR));
				ic2->pos            = sys_malloc(sizeof(VECTOR));
				ic2->leftVertexPos  = sys_malloc(sizeof(VECTOR));
				ic2->rightVertexPos = sys_malloc(sizeof(VECTOR));				
				vec_set(ic1->pos, vector(-10,0,0));
				ic1->leftVertex  = 1;
				ic1->rightVertex = 2;				
				
				float difference;
				if (ic1->incomingAngle->pan >= ic2->incomingAngle->pan) {
					difference = ic1->incomingAngle->pan - ic2->incomingAngle->pan;
				} else {
					difference = ic2->incomingAngle->pan - ic1->incomingAngle->pan;
				}
				
				if (difference < 270) {
					model->skin[0] = bmapStreetIntersection2_2;
					vec_set(ic2->pos, vector(0,0,10));
					vec_set(ic2->leftVertexPos, vector(-10,0,10));
					vec_set(ic2->rightVertexPos, vector(10,0,10));
					ic2->leftVertex = 2;
					ic2->rightVertex = 4;
				}
				if (difference < 180) {
					model->skin[0] = bmapStreetIntersection2_1;
					vec_set(ic2->pos, vector(10,0,0));
					vec_set(ic2->leftVertexPos, vector(10,0,10));
					vec_set(ic2->rightVertexPos, vector(10,0,-10));					
					ic2->leftVertex = 4;
					ic2->rightVertex = 3;					
				}	
				if (difference < 90) {
					model->skin[0] = bmapStreetIntersection2_3;
					vec_set(ic2->pos, vector(0,0,-10));
					vec_set(ic2->leftVertexPos, vector(10,0,-10));
					vec_set(ic2->rightVertexPos, vector(-10,0,-10));										
					ic2->leftVertex = 3;
					ic2->rightVertex = 1;				
				}
				
			} else {
				model->skin[0] = bmapStreetIntersection2_1;
			}
			
			
			D3DVERTEX *v1 = create_vertex(-10,                                 0, -10, 0, 1, 0, 0.33, 0.33);
			D3DVERTEX *v2 = create_vertex(-10,                                 0,  10, 0, 1, 0, 0.33, 0.66);
			D3DVERTEX *v3 = create_vertex( 10,                                 0, -10, 0, 1, 0, 0.66, 0.33);
			D3DVERTEX *v4 = create_vertex( 10,                                 0,  10, 0, 1, 0, 0.66, 0.66);
		
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
				ic1->leftVertexPos  = sys_malloc(sizeof(VECTOR));
				ic1->rightVertexPos = sys_malloc(sizeof(VECTOR));
				ic2->pos            = sys_malloc(sizeof(VECTOR));
				ic2->leftVertexPos  = sys_malloc(sizeof(VECTOR));
				ic2->rightVertexPos = sys_malloc(sizeof(VECTOR));
				ic3->pos            = sys_malloc(sizeof(VECTOR));
				ic3->leftVertexPos  = sys_malloc(sizeof(VECTOR));
				ic3->rightVertexPos = sys_malloc(sizeof(VECTOR));							
				vec_set(ic1->pos, vector(-10,0,0));
				ic1->leftVertex  = 1;
				ic1->rightVertex = 2;
				
				// Todo: FIX
				vec_set(ic2->pos, vector(-10,0,0));
				ic2->leftVertex  = 1;
				ic2->rightVertex = 2;				
				vec_set(ic3->pos, vector(-10,0,0));
				ic3->leftVertex  = 1;
				ic3->rightVertex = 2;				
			}
			model->skin[0] = bmapStreetIntersection3;
			
			D3DVERTEX *v1  = create_vertex(0 - 30, 0, 0 - 10, 0, 1, 0, 0,    0.33);
			D3DVERTEX *v2  = create_vertex(0 - 10, 0, 0 - 10, 0, 1, 0, 0.33, 0.33);
			D3DVERTEX *v3  = create_vertex(0 - 30, 0, 0 + 10, 0, 1, 0, 0,    0.66);
			D3DVERTEX *v4  = create_vertex(0 - 10, 0, 0 + 10, 0, 1, 0, 0.33, 0.66);
			D3DVERTEX *v5  = create_vertex(0 + 10, 0, 0 - 10, 0, 1, 0, 0.66, 0.33);
			D3DVERTEX *v6  = create_vertex(0 + 10, 0, 0 + 10, 0, 1, 0, 0.66, 0.66);
			D3DVERTEX *v7  = create_vertex(0 + 30, 0, 0 - 10, 0, 1, 0, 1,    0.33);
			D3DVERTEX *v8  = create_vertex(0 + 30, 0, 0 + 10, 0, 1, 0, 1,    0.66);
			D3DVERTEX *v9  = create_vertex(0 - 10, 0, 0 - 30, 0, 1, 0, 0.33, 0);			
			D3DVERTEX *v10 = create_vertex(0 + 10, 0, 0 - 30, 0, 1, 0, 0.66, 0);			
			
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
				ic1->leftVertexPos  = sys_malloc(sizeof(VECTOR));
				ic1->rightVertexPos = sys_malloc(sizeof(VECTOR));
				ic2->pos            = sys_malloc(sizeof(VECTOR));
				ic2->leftVertexPos  = sys_malloc(sizeof(VECTOR));
				ic2->rightVertexPos = sys_malloc(sizeof(VECTOR));
				ic3->pos            = sys_malloc(sizeof(VECTOR));
				ic3->leftVertexPos  = sys_malloc(sizeof(VECTOR));
				ic3->rightVertexPos = sys_malloc(sizeof(VECTOR));
				ic4->pos            = sys_malloc(sizeof(VECTOR));
				ic4->leftVertexPos  = sys_malloc(sizeof(VECTOR));
				ic4->rightVertexPos = sys_malloc(sizeof(VECTOR));												
				vec_set(ic1->pos, vector(-10,0,0));
				ic1->leftVertex  = 1;
				ic1->rightVertex = 2;
				
				// Todo: FIX
				vec_set(ic2->pos, vector(-10,0,0));
				ic2->leftVertex  = 1;
				ic2->rightVertex = 2;				
				vec_set(ic3->pos, vector(-10,0,0));
				ic3->leftVertex  = 1;
				ic3->rightVertex = 2;
				vec_set(ic4->pos, vector(-10,0,0));
				ic4->leftVertex  = 1;
				ic4->rightVertex = 2;								
			}		
			model->skin[0] = bmapStreetIntersection4;
			
			D3DVERTEX *v1  = create_vertex(0 - 30, 0, 0 - 10, 0, 1, 0, 0,    0.33);
			D3DVERTEX *v2  = create_vertex(0 - 10, 0, 0 - 10, 0, 1, 0, 0.33, 0.33);
			D3DVERTEX *v3  = create_vertex(0 - 30, 0, 0 + 10, 0, 1, 0, 0,    0.66);
			D3DVERTEX *v4  = create_vertex(0 - 10, 0, 0 + 10, 0, 1, 0, 0.33, 0.66);
			D3DVERTEX *v5  = create_vertex(0 + 10, 0, 0 - 10, 0, 1, 0, 0.66, 0.33);
			D3DVERTEX *v6  = create_vertex(0 + 10, 0, 0 + 10, 0, 1, 0, 0.66, 0.66);
			D3DVERTEX *v7  = create_vertex(0 + 30, 0, 0 - 10, 0, 1, 0, 1,    0.33);
			D3DVERTEX *v8  = create_vertex(0 + 30, 0, 0 + 10, 0, 1, 0, 1,    0.66);
			D3DVERTEX *v9  = create_vertex(0 - 10, 0, 0 + 30, 0, 1, 0, 0.33, 1);			
			D3DVERTEX *v10 = create_vertex(0 + 10, 0, 0 + 30, 0, 1, 0, 0.66, 1);			
			D3DVERTEX *v11 = create_vertex(0 - 10, 0, 0 - 30, 0, 1, 0, 0.33, 0);
			D3DVERTEX *v12 = create_vertex(0 + 10, 0, 0 - 30, 0, 1, 0, 0.66, 0);
			
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
				var vNewUVX = 0.5 + (1 / (2*polyRadius) * vNewX);
				var vNewUVY = 0.5 + (1 / (2*polyRadius) * vNewY);
				D3DVERTEX *v1  = create_vertex(vNewX, 0, vNewY, 0, 1, 0, vNewUVX, vNewUVY);
				int i1  = dmdl_add_vertex(model, v1);							
				
				int i;
				int iNewVertex = 0;
				int iLastVertex = i1;
				
				for(i=1; i<vNumConnections; i++) {
					IntersectionConnection *ic1 = (IntersectionConnection*)list_item_at(_intersection->incomingConnections, i-1);
					ic1->pos            = sys_malloc(sizeof(VECTOR));
					ic1->leftVertexPos  = sys_malloc(sizeof(VECTOR));
					ic1->rightVertexPos = sys_malloc(sizeof(VECTOR));
					
					// Calculate new vertex position
					double fAngle = i / vNumConnections * 2 * PI;
					vNewX = sin(fAngle) * polyRadius;
					vNewY = cos(fAngle) * polyRadius;
					vNewUVX = 0.5 + (1 / (2*polyRadius) * vNewX);
					vNewUVY = 0.5 + (1 / (2*polyRadius) * vNewY);
					D3DVERTEX *v2  = create_vertex(vNewX, 0, vNewY, 0, 1, 0, vNewUVX,   vNewUVY);
					iNewVertex  = dmdl_add_vertex(model, v2);
					
					vec_set(ic1->pos, vector(vNewX,0,vNewY));
					ic1->leftVertex  = iLastVertex;
					ic1->rightVertex = iNewVertex;
					
					dmdl_connect_vertices(model, iMiddle, iLastVertex, iNewVertex);
					
					iLastVertex = iNewVertex;
				}
				
				// Closing vertex
				dmdl_connect_vertices(model, iMiddle, iLastVertex, i1);
			}
				
			model->skin[0] = bmapStreetIntersection5;	
		break;
	}
	
	ENTITY *ent = dmdl_create_instance(model, vector(_intersection->pos->x, _intersection->pos->z, _intersection->pos->y), inter_info);
	
	ent->skill1 = _intersection;
	
	ent->pan = fOptimalPan;
	
	dmdl_delete(model);
	return ent;	
}

// Places each vertex of an entity's mesh on the ground (and adds a given distance)
void place_street_on_ground(ENTITY* _street, int _dist) {
	
	var nVertexCount = ent_status(_street, 0);
	var i;
	VECTOR* vecTemp;
	CONTACT* c;
	for (i=1; i<=nVertexCount; i++) {
		c = ent_getvertex(_street, NULL, i);
		if (c != NULL) {
			if(c_trace(	vector(c.v.x, c.v.z, c.v.y + 1024),
						vector(c.v.x, c.v.z, c.v.y - 1024),
						IGNORE_MODELS | IGNORE_SPRITES | IGNORE_PASSABLE | IGNORE_PASSENTS | USE_POLYGON))
			{
				c.v.y = target.z+_dist;
			}
			ent_setvertex(_street, c, i);
		}
	}
}

// Start and end segments are no longer spline aligned!
ENTITY *street_build(Street *street, BMAP* _streetTexture, BOOL _placeOnGround)
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
	
	DYNAMIC_QUAD quad;
	
	D3DVERTEX left;
	D3DVERTEX right;
	
	var isLooped = vec_dist(&splineData[0], &splineData[iPointCount-1]) < 0.1;
	
	int previousSeparator = 0;
	
	//VECTOR vecStartPosition;
	
	var dist = 0;
	for(dist = 0; dist <= 1.0; dist += 0.01)
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
		left.x = startSegment.x + offset.x;
		left.y = zStartLeft;
		left.z = startSegment.y + offset.y;
		left.nx = 0; left.ny = 1; left.nz = 0;
		left.u1 = 0;
		left.v1 = 15 * dist;
		
		// Setup right vertex
		right.x = startSegment.x - offset.x;
		right.y = zStartRight;
		right.z = startSegment.y - offset.y;
		right.nx = 0; right.ny = 1; right.nz = 0;
		right.u1 = 1;
		right.v1 = 15 * dist;
		
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
	dmdl_save(model, "C:\\Users\\padmalcom\\test.x");
	
	// Free data
	dmdl_delete(model);
	sys_free(splineData);
	
	return ent;
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
	
	bmapStreetIntersection1 = bmap_draw_triangle_filled(bmapStreetIntersection1, 
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
	);
	
	bmapStreetIntersection1 = bmap_draw_line(bmapStreetIntersection1,
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
	);
	
	
	
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
}

#endif