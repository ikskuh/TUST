#ifndef _PROC_CITY_C_
#define _PROC_CITY_C_

#include "math.h"

// ----------------------------------------------------------------------------------------
// Street tool
// ----------------------------------------------------------------------------------------

Intersection *intersection_create(VECTOR* _pos)
{
	Intersection *newInter = sys_malloc(sizeof(Intersection));
	newInter->incomingAngles = list_create();
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

D3DVERTEX *create_vertex(int _x, int _y, int _z, int _nx, int _ny, int _nz, int _uv1x, int _uv1y) {
	D3DVERTEX *newVertex = sys_malloc(sizeof(D3DVERTEX));
	newVertex->x  = _x;    newVertex->y = _y;      newVertex->z = _z;
	newVertex->nx = _nx;   newVertex->ny = _ny;   newVertex->nz = _nz;
	newVertex->u1 = _uv1x; newVertex->v1 = _uv1y;
	return newVertex;
}

ENTITY *build_intersection(Intersection *_intersection, BMAP* _intersectionTexture)
{
	int nIncomingCount = _intersection->incomingStreets;
	DynamicModel *model = dmdl_create();
	model->skin[0] = _intersectionTexture;
	
	/*D3DVERTEX *v1 = sys_malloc(sizeof(D3DVERTEX));
	D3DVERTEX *v2 = sys_malloc(sizeof(D3DVERTEX));
	D3DVERTEX *v3 = sys_malloc(sizeof(D3DVERTEX));
	D3DVERTEX *v4 = sys_malloc(sizeof(D3DVERTEX));*/
	
	
	switch(nIncomingCount) {
		// Should not happen!
		case 0: return NULL; break;
		
		// Street ending
		case 1:
			// y == z
			D3DVERTEX *v1 = create_vertex(_intersection->pos->x + 00, _intersection->pos->y, _intersection->pos->z + 00, 0, 1, 0, 0, 1);
			D3DVERTEX *v2 = create_vertex(_intersection->pos->x + 00, _intersection->pos->y, _intersection->pos->z - 50, 0, 1, 0, 0, 1);
			D3DVERTEX *v3 = create_vertex(_intersection->pos->x + 40, _intersection->pos->y, _intersection->pos->z - 50, 0, 1, 0, 0, 1);
			D3DVERTEX *v4 = create_vertex(_intersection->pos->x + 55, _intersection->pos->y, _intersection->pos->z - 40, 0, 1, 0, 0, 1);
			D3DVERTEX *v5 = create_vertex(_intersection->pos->x + 60, _intersection->pos->y, _intersection->pos->z - 30, 0, 1, 0, 0, 1);
			D3DVERTEX *v6 = create_vertex(_intersection->pos->x + 60, _intersection->pos->y, _intersection->pos->z - 20, 0, 1, 0, 0, 1);
			D3DVERTEX *v7 = create_vertex(_intersection->pos->x + 55, _intersection->pos->y, _intersection->pos->z - 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v8 = create_vertex(_intersection->pos->x + 40, _intersection->pos->y, _intersection->pos->z + 00, 0, 1, 0, 0, 1);
			
			int i1 = dmdl_add_vertex(model, v1);
			int i2 = dmdl_add_vertex(model, v2);
			int i3 = dmdl_add_vertex(model, v3);
			int i4 = dmdl_add_vertex(model, v4);
			int i5 = dmdl_add_vertex(model, v5);
			int i6 = dmdl_add_vertex(model, v6);
			int i7 = dmdl_add_vertex(model, v7);
			int i8 = dmdl_add_vertex(model, v8);
			
			dmdl_connect_vertices(model, i1, i8, i7);
			dmdl_connect_vertices(model, i1, i7, i6);
			dmdl_connect_vertices(model, i1, i6, i5);
			dmdl_connect_vertices(model, i1, i5, i4);
			dmdl_connect_vertices(model, i1, i4, i3);
			dmdl_connect_vertices(model, i1, i3, i2);
		break;
		
		// A simple connection
		case 2:
			D3DVERTEX *v1 = create_vertex(_intersection->pos->x - 30, _intersection->pos->y, _intersection->pos->z - 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v2 = create_vertex(_intersection->pos->x - 10, _intersection->pos->y, _intersection->pos->z - 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v3 = create_vertex(_intersection->pos->x - 30, _intersection->pos->y, _intersection->pos->z + 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v4 = create_vertex(_intersection->pos->x - 10, _intersection->pos->y, _intersection->pos->z + 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v5 = create_vertex(_intersection->pos->x + 10, _intersection->pos->y, _intersection->pos->z - 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v6 = create_vertex(_intersection->pos->x + 10, _intersection->pos->y, _intersection->pos->z + 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v7 = create_vertex(_intersection->pos->x + 30, _intersection->pos->y, _intersection->pos->z - 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v8 = create_vertex(_intersection->pos->x + 30, _intersection->pos->y, _intersection->pos->z + 10, 0, 1, 0, 0, 1);
			
			int i1 = dmdl_add_vertex(model, v1);
			int i2 = dmdl_add_vertex(model, v2);
			int i3 = dmdl_add_vertex(model, v3);
			int i4 = dmdl_add_vertex(model, v4);
			int i5 = dmdl_add_vertex(model, v5);
			int i6 = dmdl_add_vertex(model, v6);
			int i7 = dmdl_add_vertex(model, v7);
			int i8 = dmdl_add_vertex(model, v8);
			
			dmdl_connect_vertices(model, i1, i3, i2);
			dmdl_connect_vertices(model, i2, i3, i4);
			dmdl_connect_vertices(model, i2, i4, i5);
			dmdl_connect_vertices(model, i5, i4, i6);
			dmdl_connect_vertices(model, i7, i5, i6);
			dmdl_connect_vertices(model, i7, i6, i8);		
		break;
		
		// Three incoming streets
		case 3:
			D3DVERTEX *v1  = create_vertex(_intersection->pos->x - 30, _intersection->pos->y, _intersection->pos->z - 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v2  = create_vertex(_intersection->pos->x - 10, _intersection->pos->y, _intersection->pos->z - 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v3  = create_vertex(_intersection->pos->x - 30, _intersection->pos->y, _intersection->pos->z + 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v4  = create_vertex(_intersection->pos->x - 10, _intersection->pos->y, _intersection->pos->z + 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v5  = create_vertex(_intersection->pos->x + 10, _intersection->pos->y, _intersection->pos->z - 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v6  = create_vertex(_intersection->pos->x + 10, _intersection->pos->y, _intersection->pos->z + 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v7  = create_vertex(_intersection->pos->x + 30, _intersection->pos->y, _intersection->pos->z - 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v8  = create_vertex(_intersection->pos->x + 30, _intersection->pos->y, _intersection->pos->z + 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v9  = create_vertex(_intersection->pos->x - 10, _intersection->pos->y, _intersection->pos->z + 30, 0, 1, 0, 0, 1);			
			D3DVERTEX *v10 = create_vertex(_intersection->pos->x + 10, _intersection->pos->y, _intersection->pos->z + 30, 0, 1, 0, 0, 1);			
			
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
			dmdl_connect_vertices(model, i4, i9, i6);
			dmdl_connect_vertices(model, i6, i9, i10);
		break;
		
		// A cross
		case 4:
			D3DVERTEX *v1  = create_vertex(_intersection->pos->x - 30, _intersection->pos->y, _intersection->pos->z - 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v2  = create_vertex(_intersection->pos->x - 10, _intersection->pos->y, _intersection->pos->z - 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v3  = create_vertex(_intersection->pos->x - 30, _intersection->pos->y, _intersection->pos->z + 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v4  = create_vertex(_intersection->pos->x - 10, _intersection->pos->y, _intersection->pos->z + 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v5  = create_vertex(_intersection->pos->x + 10, _intersection->pos->y, _intersection->pos->z - 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v6  = create_vertex(_intersection->pos->x + 10, _intersection->pos->y, _intersection->pos->z + 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v7  = create_vertex(_intersection->pos->x + 30, _intersection->pos->y, _intersection->pos->z - 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v8  = create_vertex(_intersection->pos->x + 30, _intersection->pos->y, _intersection->pos->z + 10, 0, 1, 0, 0, 1);
			D3DVERTEX *v9  = create_vertex(_intersection->pos->x - 10, _intersection->pos->y, _intersection->pos->z + 30, 0, 1, 0, 0, 1);			
			D3DVERTEX *v10 = create_vertex(_intersection->pos->x + 10, _intersection->pos->y, _intersection->pos->z + 30, 0, 1, 0, 0, 1);			
			D3DVERTEX *v11 = create_vertex(_intersection->pos->x - 10, _intersection->pos->y, _intersection->pos->z - 30, 0, 1, 0, 0, 1);
			D3DVERTEX *v12 = create_vertex(_intersection->pos->x + 10, _intersection->pos->y, _intersection->pos->z - 30, 0, 1, 0, 0, 1);
			
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
			D3DVERTEX *v1  = create_vertex(_intersection->pos->x + 00, _intersection->pos->y, _intersection->pos->z + 00, 0, 1, 0, 0, 1);
			D3DVERTEX *v2  = create_vertex(_intersection->pos->x + 00, _intersection->pos->y, _intersection->pos->z - 30, 0, 1, 0, 0, 1);
			D3DVERTEX *v3  = create_vertex(_intersection->pos->x + 20, _intersection->pos->y, _intersection->pos->z - 20, 0, 1, 0, 0, 1);
			D3DVERTEX *v4  = create_vertex(_intersection->pos->x + 30, _intersection->pos->y, _intersection->pos->z + 00, 0, 1, 0, 0, 1);
			D3DVERTEX *v5  = create_vertex(_intersection->pos->x + 20, _intersection->pos->y, _intersection->pos->z + 20, 0, 1, 0, 0, 1);
			D3DVERTEX *v6  = create_vertex(_intersection->pos->x + 00, _intersection->pos->y, _intersection->pos->z + 30, 0, 1, 0, 0, 1);
			D3DVERTEX *v7  = create_vertex(_intersection->pos->x - 20, _intersection->pos->y, _intersection->pos->z + 20, 0, 1, 0, 0, 1);
			D3DVERTEX *v8  = create_vertex(_intersection->pos->x - 30, _intersection->pos->y, _intersection->pos->z + 00, 0, 1, 0, 0, 1);
			D3DVERTEX *v9  = create_vertex(_intersection->pos->x - 20, _intersection->pos->y, _intersection->pos->z - 20, 0, 1, 0, 0, 1);			
			
			int i1  = dmdl_add_vertex(model, v1);
			int i2  = dmdl_add_vertex(model, v2);
			int i3  = dmdl_add_vertex(model, v3);
			int i4  = dmdl_add_vertex(model, v4);
			int i5  = dmdl_add_vertex(model, v5);
			int i6  = dmdl_add_vertex(model, v6);
			int i7  = dmdl_add_vertex(model, v7);
			int i8  = dmdl_add_vertex(model, v8);
			int i9  = dmdl_add_vertex(model, v9);
			
			dmdl_connect_vertices(model, i1, i3, i2);
			dmdl_connect_vertices(model, i1, i4, i3);
			dmdl_connect_vertices(model, i1, i5, i4);
			dmdl_connect_vertices(model, i1, i6, i5);
			dmdl_connect_vertices(model, i1, i7, i6);
			dmdl_connect_vertices(model, i1, i8, i7);
			dmdl_connect_vertices(model, i1, i9, i8);
			dmdl_connect_vertices(model, i1, i2, i9);		
		break;
	}
	
	//printf("Vertices %i Faces: %i", model->vertexCount, model->faceCount);
	ENTITY *ent = dmdl_create_instance(model, vector(0, 0, 0), NULL);
	dmdl_delete(model);
	return ent;	
}


ENTITY *street_build(Street *street, BMAP* _streetTexture)
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
	
	var dist = 0;
	for(dist = 0; dist <= 1; dist += 0.01)
	{
		VECTOR startSegment, endSegment, dir;		

		// Get starting position
		vec_set(&startSegment, math_get_spline(splineData, iPointCount, dist));
		
		// Get a next segment on the street (to calculate the distance)
		if(dist >= 1)
		{
			// Special case: Last segment
			vec_set(&endSegment, math_get_spline(splineData, iPointCount, dist - 0.01));
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
		var zStartLeft = street_getground(street, vector(startSegment.x + offset.x, startSegment.y + offset.y, startSegment.z));
		var zStartRight = street_getground(street, vector(startSegment.x - offset.x, startSegment.y - offset.y, startSegment.z));
		zStartLeft += street->groundDist;
		zStartRight += street->groundDist;
		
		/************************************************************************************************/
		/*	Build Street Surface																		*/
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
		if(dist > 0)
			// Connect the current and the last separator to a segment (surface)
			street_create_segment(model, previousSeparator, separator);
		previousSeparator = separator;
	}
	
	// Connect to loop
	if(isLooped)
		// Link the last to the first segment
		street_create_segment(model, previousSeparator, 0);
	
	// Create the entity
	ENTITY *ent = dmdl_create_instance(model, vector(0, 0, 0), NULL);
	dmdl_save(model, "C:\\Users\\padmalcom\\test.x");
	
	// Free data
	dmdl_delete(model);
	sys_free(splineData);
	
	return ent;
}

#endif