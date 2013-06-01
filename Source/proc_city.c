#ifndef _PROC_CITY_C_
#define _PROC_CITY_C_

// ----------------------------------------------------------------------------------------
// Street tool
// ----------------------------------------------------------------------------------------

Street *street_create(int maxPoints, int _streetWidth, int _streetHeight)
{
	Street *street = sys_malloc(sizeof(Street));
	
	street->points = sys_malloc(sizeof(VECTOR*) * maxPoints);
	memset(street->points, 0, sizeof(VECTOR*) * maxPoints);
	
	street->numPoints = maxPoints;
	street->width = _streetWidth; // 320
	street->groundDist = 16;
	street->height = _streetHeight; // 48
	street->slopewidth = 3;
	street->segments = 10;
	
	return street;
}

void street_remove(Street *street)
{
	int i;
	for(i = 0; i < street->numPoints; i++)
	{
		if((street->points)[i] != NULL)
			sys_free((street->points)[i]);
	}
	sys_free(street->points);
	if(street->ent != NULL)
		ent_remove(street->ent);
	sys_free(street);
}

void street_setpos(Street *street, int i, VECTOR *pos)
{
	if(street->numPoints < 0)
		return;
	if(i >= street->numPoints)
		return;
	if((street->points)[i] == NULL)
		(street->points)[i] = sys_malloc(sizeof(VECTOR));
	vec_set((street->points)[i], pos);
}

int street_getpos(Street *street, int i, VECTOR *pos)
{
	if(street->numPoints < 0)
		return 0;
	if(street->numPoints >= i)
		return 0;
	if((street->points)[i] == NULL)
		return 0;
	vec_set(pos, (street->points)[i]);
	return 1;
}

void street_clearpos(Street *street, int i)
{
	if(street->numPoints < 0)
		return;
	if(street->numPoints >= i)
		return;
	if((street->points)[i] != NULL)
		sys_free((street->points)[i]);
	(street->points)[i] = NULL;
}

var street_getground(Street *street, VECTOR *pos, VECTOR *offset, int mirror)
{
	var zStart = 0;
	
	VECTOR left, right;
	vec_set(&left, pos);
	vec_add(&left, offset);
	vec_set(&right, pos);
	vec_sub(&right, offset);
	
	if(mirror < 0)
		vec_set(&left, &right);
	
	if(c_trace(	vector(left.x, left.y, left.z + 1024),
				vector(left.x, left.y, left.z - 1024),
				IGNORE_MODELS | IGNORE_SPRITES | IGNORE_PASSABLE | IGNORE_PASSENTS | USE_POLYGON))
	{
		zStart = maxv(zStart, target.z);
	}
	if(mirror > 0)
	{
		if(c_trace(	vector(right.x, right.y, right.z + 1024),
					vector(right.x, right.y, right.z - 1024),
					IGNORE_MODELS | IGNORE_SPRITES | IGNORE_PASSABLE | IGNORE_PASSENTS | USE_POLYGON))
		{
			zStart = maxv(zStart, target.z);
		}
	}
	return zStart;
}

ENTITY *street_build(Street *street, ENTITY* _terrain, BMAP* _streetTexture)
{
	int i, j, iPointCount;
	VECTOR center;
	
	//gamelog("street_build: Count items...");
	
	vec_set(&center, vector(0, 0, 0));
	iPointCount = 0;
	
	for(i = 0; i < street->numPoints; i++)
	{
		if((street->points)[i] != NULL)
		{
			vec_add(&center, (street->points)[i]);
			iPointCount++;
		}
	}
	// Calculate street center
	vec_scale(&center, 1.0 / iPointCount);
	
	VECTOR *spline = sys_malloc(sizeof(VECTOR) * iPointCount);
	int pos = 0;
	for(i = 0; i < street->numPoints; i++)
	{
		if((street->points)[i] != NULL)
		{
			vec_set(&spline[pos], (street->points)[i]);
			pos++;
		}
	}
	
	DynamicModel *model = dmdl_create();
	model->skin[0] = _streetTexture;
	
	DynamicModel *modelSlope = dmdl_create();
	modelSlope->skin[0] = ent_getskin(_terrain, 1);
	modelSlope->skin[1] = ent_getskin(_terrain, 2);
	
	DYNAMIC_QUAD quad;
	
	VECTOR previousLeft;
	VECTOR previousRight;
	
	var distance = 0;
	for (i = 0; i < (iPointCount - 1); i++)
	{
		VECTOR startPart, endPart;
		vec_set(&startPart, &spline[i]);
		vec_set(&endPart, &spline[i + 1]);
		var j;
		for(j = 0; j < street->segments; j++)
		{
			VECTOR startSegment, endSegment, dir;
			vec_lerp(&startSegment, &startPart, &endPart, j / street->segments);
			vec_lerp(&endSegment, &startPart, &endPart, (j + 1) / street->segments);
		
			var deltaDist = vec_dist(&startSegment, &endSegment);
		
			vec_diff(&dir, &endSegment, &startSegment);
			dir.z = 0;
			vec_to_angle(&dir, &dir);
			
			VECTOR offset;
			vec_set(&offset, vector(0, 0.5 * street->width, 0));
			vec_rotate(&offset, &dir);
			
			VECTOR startOffset, endOffset;
			vec_set(&startOffset, &offset);
			vec_set(&endOffset, &offset);
		
///			0 --- 1
///			|   / |
///			| /   |
///			2 --- 3

			var zStart = street_getground(street, &startSegment, &offset, 1);
			var zEnd = street_getground(street, &endSegment, &offset, 1);
			
			zStart += street->groundDist;
			zEnd += street->groundDist;

			// Lower start of street and end of street
			if(i == 0 && j == 0)
			{
				zStart -= 1.5 * street->height;
			}
			if(i == (iPointCount - 2) && j == (street->segments - 1))
			{
				zEnd -= 1.5 * street->height;
			}
			
			/************************************************************************************************/
			/*	Build Street Surface																		*/
			/************************************************************************************************/
			
			// We got a start element AND not the first element
			if(j == 0 && i > 0 && 0)
			{
				quad.v[0].x = previousLeft.x;
				quad.v[0].y = previousLeft.z;
				quad.v[0].z = previousLeft.y;
			}
			else
			{
				quad.v[0].x = startSegment.x + startOffset.x;
				quad.v[0].y = zStart;
				quad.v[0].z = startSegment.y + startOffset.y;
			}
			quad.v[0].nx = 0; quad.v[0].ny = 1; quad.v[0].nz = 0;
			quad.v[0].u1 = 0;
			quad.v[0].v1 = 0.001 * (distance);
			
			if(j == 0 && i > 0 && 0)
			{
				quad.v[0].x = previousRight.x;
				quad.v[0].y = previousRight.z;
				quad.v[0].z = previousRight.y;
			}
			else
			{
				quad.v[1].x = startSegment.x - startOffset.x;
				quad.v[1].y = zStart;
				quad.v[1].z = startSegment.y - startOffset.y;
			}
			quad.v[1].nx = 0; quad.v[1].ny = 1; quad.v[1].nz = 0;
			quad.v[1].u1 = 1;
			quad.v[1].v1 = 0.001 * (distance);

			quad.v[2].x = endSegment.x + endOffset.x;
			quad.v[2].y = zEnd;
			quad.v[2].z = endSegment.y + endOffset.y;
			quad.v[2].nx = 0; quad.v[2].ny = 1; quad.v[2].nz = 0;
			quad.v[2].u1 = 0;
			quad.v[2].v1 = 0.001 * (distance + deltaDist);
			
			quad.v[3].x = endSegment.x - endOffset.x;
			quad.v[3].y = zEnd;
			quad.v[3].z = endSegment.y - endOffset.y;
			quad.v[3].nx = 0; quad.v[3].ny = 1; quad.v[3].nz = 0;
			quad.v[3].u1 = 1;
			quad.v[3].v1 = 0.001 * (distance + deltaDist);
			
			previousLeft.x = quad.v[2].x;
			previousLeft.y = quad.v[2].z;
			previousLeft.z = quad.v[2].y;
			
			previousRight.x = quad.v[3].x;
			previousRight.y = quad.v[3].z;
			previousRight.z = quad.v[3].y;
		
			dmdl_add_quad(model, &quad);
			
			/************************************************************************************************/
			/*	Build Street Slop																			*/
			/************************************************************************************************/
			
			VECTOR sideOffset;
			vec_set(&sideOffset, &startOffset);
			vec_scale(&sideOffset, street->slopewidth);
			
			quad.v[0].x = startSegment.x + sideOffset.x;
			quad.v[0].y = street_getground(street, &startSegment, &sideOffset, 0) - street->height;
			quad.v[0].z = startSegment.y + sideOffset.y;
			quad.v[0].nx = 0; quad.v[0].ny = 1; quad.v[0].nz = 0;
			
			quad.v[1].x = startSegment.x + startOffset.x;
			quad.v[1].y = zStart;
			quad.v[1].z = startSegment.y + startOffset.y;
			quad.v[1].nx = 0; quad.v[1].ny = 1; quad.v[1].nz = 0;
			
			vec_set(&sideOffset, &endOffset);
			vec_scale(&sideOffset, street->slopewidth);

			quad.v[2].x = endSegment.x + sideOffset.x;
			quad.v[2].y = street_getground(street, &endSegment, &sideOffset, 0) - street->height;
			quad.v[2].z = endSegment.y + sideOffset.y;
			quad.v[2].nx = 0; quad.v[2].ny = 1; quad.v[2].nz = 0;
			
			quad.v[3].x = endSegment.x + endOffset.x;
			quad.v[3].y = zEnd;
			quad.v[3].z = endSegment.y + endOffset.y;
			quad.v[3].nx = 0; quad.v[3].ny = 1; quad.v[3].nz = 0;
		
			dmdl_add_quad(modelSlope, &quad);
			
			
			vec_set(&sideOffset, &startOffset);
			vec_scale(&sideOffset, street->slopewidth);
			
			quad.v[0].x = startSegment.x - startOffset.x;
			quad.v[0].y = zStart;
			quad.v[0].z = startSegment.y - startOffset.y;
			quad.v[0].nx = 0; quad.v[0].ny = 1; quad.v[0].nz = 0;
			
			quad.v[1].x = startSegment.x - sideOffset.x;
			quad.v[1].y = street_getground(street, &startSegment, &sideOffset, -1) - street->height;
			quad.v[1].z = startSegment.y - sideOffset.y;
			quad.v[1].nx = 0; quad.v[1].ny = 1; quad.v[1].nz = 0;
			
			vec_set(&sideOffset, &endOffset);
			vec_scale(&sideOffset, street->slopewidth);

			quad.v[2].x = endSegment.x - endOffset.x;
			quad.v[2].y = zEnd;
			quad.v[2].z = endSegment.y - endOffset.y;
			quad.v[2].nx = 0; quad.v[2].ny = 1; quad.v[2].nz = 0;
			
			quad.v[3].x = endSegment.x - sideOffset.x;
			quad.v[3].y = street_getground(street, &endSegment, &sideOffset, -1) - street->height;
			quad.v[3].z = endSegment.y - sideOffset.y;
			quad.v[3].nx = 0; quad.v[3].ny = 1; quad.v[3].nz = 0;
		
			dmdl_add_quad(modelSlope, &quad);
			
			distance += deltaDist;
		}
	}
	
	ENTITY *ent = dmdl_create_instance(model, vector(0, 0, 0), NULL);
	//ENTITY *entSlope = dmdl_create_instance(modelSlope, vector(0, 0, 0), NULL);
	
	dmdl_delete(model);
	//dmdl_delete(modelSlope);
	
	wait(1);
	c_updatehull(ent, 1);
	//c_updatehull(entSlope, 1);
	wait(1);
	pXent_settype(ent, PH_STATIC, PH_POLY);
	//pXent_settype(entSlope, PH_STATIC, PH_POLY);
	
	//entSlope->material = mtl_terrain;
}

void street_debug(Street *street)
{
	int i;
	while(1)
	{
		int found = 0;
		for(i = 0; i < street->numPoints; i++)
		{
			if((street->points)[i] != NULL)
			{
				if(found == 0)
				{
					draw_line3d((street->points)[i], NULL, 100);
					draw_line3d((street->points)[i], COLOR_RED, 100);
				}
				else
				{
					draw_line3d((street->points)[i], COLOR_RED, 100);
				}
				found = 1;
			}
		}
		wait(1);
	}
}

#endif