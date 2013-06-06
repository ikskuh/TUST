#ifndef _PROC_CITY_C_
#define _PROC_CITY_C_

#include "math.h"

// ----------------------------------------------------------------------------------------
// Street tool
// ----------------------------------------------------------------------------------------

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