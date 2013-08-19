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
// Road networks
// ----------------------------------------------------------------------------------------

// Color maps
BMAP* bmapStreetIntersection1     = NULL;
BMAP* bmapStreetIntersection2_1   = NULL;
BMAP* bmapStreetIntersection2_2   = NULL;
BMAP* bmapStreetIntersection2_3   = NULL;
BMAP* bmapStreetIntersection3     = NULL;
BMAP* bmapStreetIntersection4     = NULL;
BMAP* bmapStreetIntersection5     = NULL;
BMAP* bmapStreetTexture           = NULL;

// Normal maps
BMAP* bmapStreetIntersectionNM1   = NULL;
BMAP* bmapStreetIntersectionNM2_1 = NULL;
BMAP* bmapStreetIntersectionNM2_2 = NULL;
BMAP* bmapStreetIntersectionNM2_3 = NULL;
BMAP* bmapStreetIntersectionNM3   = NULL;
BMAP* bmapStreetIntersectionNM4   = NULL;
BMAP* bmapStreetIntersectionNM5   = NULL;
BMAP* bmapStreetTextureNM         = NULL;


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
	//int incomingStreets;
	List *incomingConnections; // Typ: IntersectionConnection
	int source;
	ENTITY* ent; // The built entity
} Intersection;

typedef struct {
	ANGLE *incomingAngle;
	VECTOR *pos;
	int leftVertex;
	int rightVertex;
	int id;
	int isConnected;
	float angle;
} IntersectionConnection;


/**
 * Builds a physical roadnetwork from a set of intersections
 * \param   _intersections  Intersection list
 * \param   _zPosition      Initial z position fo all created entities
 * \param   _placeOnGround  Place the complete road network on ground (if any)
 */
void roadnetwork_build(List *_intersections, int _zPosition, BOOL _placeOnGround);

/**
 * Calculates the intersections of a roadnetwork and the intersection points
 * \param   _points         Single points of street beginning and street endings
 * \return                  Pointer to a list of intersections
 */
List* roadnetwork_calculate(List *_points);

/**
 * Generate a random road network using voronoi
 * \param   _pointCount     Number of points for the voronoi elements.
 * \param   _minX           Min x value of the network borders.
 * \param   _minY           Min y value of the network borders.
 * \param   _maxX           Max x value of the network borders.
 * \param   _maxY           Max y value of the network borders.
 * \return                  Pointer to a list of float points
 */
List *roadnetwork_from_voronoi(int _pointCount, int _minX, int _minY, int _maxX, int _maxY);


/**
 * Generate a random road network using a rectangular algorithm
 * \param   _minX              Min x value of the network borders.
 * \param   _minY              Min y value of the network borders.
 * \param   _maxX              Max x value of the network borders.
 * \param   _maxY              Max y value of the network borders.
 * \param   _dist              Distance of the squares
 * \param   _rndDeleteFactor   The lower this value, the bigger the possibility that parts get deleted. 10 for no deletion. (1-10)
 * \return                     Pointer to a list of float points
 */
List *roadnetwork_from_rectlangle(int _minX, int _minY, int _maxX, int _maxY, int _dist, int _rndDeleteFactor);


/**
 * Joins all intersections that are closer than _minDist to one.
 * \param   _intersections  List of all intersections in the list.
 * \param   _dist           All intersections nearer than _minDist get joined.
 */
void roadnetwork_join_near_intersections(List *_intersections, float _minDist);

/**
 * Creates a new intersection.
 * \param   _pos            Position of the intersection
 * \return                  The newly created intersection.
 */
Intersection *intersection_create(VECTOR* _pos);

/**
 * Builds a mesh and ENTITY* from a given intersection object.
 * \param   _intersection          The _intersection to be built.
 * \return                         The ENTITY* build from the intersection.
 */
ENTITY *build_intersection(Intersection *_intersection);


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
 * \param   _placeOnGround  Align street to ground?
 * \param   _details        How detailed should the road be?
 * \return                  The ENTITY* build from the street.
 */
ENTITY *street_build(Street *street, BMAP* _streetTexture, BOOL _placeOnGround, float _details);

/**
 * Builds a mesh and ENTITY* from a given street object and connects the end vertices to certain points
 * \param   street             The street to be built.
 * \param   _streetTexture     The texture of the street. Should be tilable in y direction.
 * \param   _streetNormalMap   Normal map for the street texture
 * \param   _placeOnGround     Align street to ground?
 * \param   _details           How detailed should the road be?
 * \param   _v1_1              First vertex position of the first end
 * \param   _v1_2              Second vertex position of the first end
 * \param   _v2_1              First vertex position of the second end
 * \param   _v2_2              Second vertex position of the second end
 * \return                     The ENTITY* build from the street.
 */
ENTITY *street_build_ext(Street *street, BMAP* _streetTexture, BMAP* _streetNormalMap, BOOL _placeOnGround, float _details, VECTOR* _v1_1, VECTOR* _v1_2, VECTOR* _v2_1, VECTOR* _v2_2);

/**
 * Places a street or another flat model on the ground
 * \param   _street         The street to be placed on ground.
 * \param   _dist           Distance to the ground (Should be 3 to 5)
 */
void place_street_on_ground(ENTITY* _street, int _dist);

#include "proc_city.c"

#endif