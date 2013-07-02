#ifndef _PROC_CITY_H_
#define _PROC_CITY_H_

/**
 * \file proc_city.h
 * Contains functions for procedural generation of cities and other content
 */
 
#include <d3d9.h>
#include "DynamicModels.h"
#include "List.h"



// ----------------------------------------------------------------------------------------
// Voronoi
// ----------------------------------------------------------------------------------------

// To be found in "ProceduralGS.dll"

/**
 * Initializes the voronoi function
 */
void vo_init();

/**
 * Frees the voronoi objects
 */
void vo_free();

/**
 * Adds points to generate the voronoi diagram around
 * \param	float	x
 * \param	float y
 */
void vo_add_point(float _x, float _y);

/**
 * Returns a point from the internal list
 * \param	int	index
 * \param	float	Returned x value
 * \param	float	Returned y value
 */
void vo_get_point(int _i, float *_x, float *_y);

/**
 * Returns the number of internal points
 * \return			Number of points
 */
int vo_get_point_count();

/**
 * Executes the calculation function
 * \param	float	Minimal x value of diagram area
 * \param	float	Maximal x value of diagram area
 * \param	float Minimal y value of diagram area
 * \param	float	Maximal y value of diagram area
 * \param	float	Minimal distance between two points to create an edge
 */
void vo_execute(float _minX, float _maxX, float _minY, float _maxY, float _minDist);

/**
 * Returns the number of result edges
 * \return			Number of edges
 */
int vo_get_result_count();

/**
 * Returns one of the result edges
 * \param	int	index
 * \param	float	x value of start point
 * \param	float	y value of start point
 * \param	float xvalue of end point
 * \param	float	y value of end point
 */
void vo_get_result_at(int _i, float *_x1, float *_y1, float *_x2, float *_y2);





// ----------------------------------------------------------------------------------------
// Street tool
// ----------------------------------------------------------------------------------------

typedef struct Street
{
	List *points;
	var width;
	var groundDist;
	ENTITY *ent;
	LPD3DXMESH mesh;
} Street;

typedef struct {
	VECTOR *pos;
	int incomingStreets;
	List *incomingAngles;
} Intersection;


/**
 * Creates a new intersection.
 * \param   _pos            Position of the intersection
 * \return                  The newly created intersection.
 */
Intersection *intersection_create(VECTOR* _pos);


/**
 * Creates a new street without mesh.
 * \param   _streetWidth    The width of the street in quants
 * \param   groundDistance  The distance of the street to the ground.
 * \return                  The newly created street.
 */
Street *street_create(int _streetWidth, int groundDistance);

/**
 * Removes a street object. Could be done after building the mesh of the street.
 * \param   street          The street to delete.
 */
void street_remove(Street *street);

/**
 * Add a position to the street object. It will be added to the end of the street.
 * \param   street          The street where the position should be added.
 * \param   pos             The position to add.
 */
void street_addpos(Street *street, VECTOR *pos);

/**
 * Builds a mesh and ENTITY* from a given street object.
 * \param   street          The street to be built.
 * \param   _streetTexture  The texture of the street. Should be tilable in y direction.
 * \return                  The ENTITY* build from the street.
 */
ENTITY *street_build(Street *street, BMAP* _streetTexture);

#include "proc_city.c"

#endif