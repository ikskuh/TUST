#ifndef _NODEMESH_H_
#define _NODEMESH_H_

/**
 * \file nodemesh.h
 * nodemesh.h provides a nodes collection management and pathfinder
 */

/**
 * Dependencies
 */
#include "node.h"
#include "list.h"


/**
 * NodeMesh is used instead of List to give a clear difference between a normal list and a list of nodes.
 */
typedef List NodeMesh;

/**
 * Route is used instead of List to give a clear difference between a normal list and a route of nodes.
 */
typedef List Route;

/**
 * Creates a new nodemesh.
 * \return					A new nodemesh.
 */
NodeMesh *nmesh_create ();

/**
 * Deletes a nmesh.
 * \param	nmesh			The nmesh to delete.
 */
void nmesh_remove ( NodeMesh *nmesh );

/**
 * Clones a nmesh.
 * \param	nmesh			The nmesh to clone.
 * \return					A new nmesh.
 */
NodeMesh *nmesh_clone ( NodeMesh *nmesh );

/**
 * Adds a node to the nodemesh and builds it neighborhood with a c_trace.
 * \param	nmesh			The nmesh.
 * \param	pos			The position of the node.
 * \return					The node index in the nodemesh list.
 */
int nmesh_add ( NodeMesh *nmesh, VECTOR *pos );

/**
 * Looks for the nearest node to a certain position.
 * \param	nmesh			The nmesh.
 * \param	pos			The position .
 * \return					The node index in the nodemesh list.
 */
int nmesh_nearest ( NodeMesh *nmesh, VECTOR *pos );

/**
 * Looks for a route between two nodes.
 * \param	nmesh			The nmesh.
 * \param	indexFrom	The index of the starting node
 * \param	indexTo		The index of the ending node
 * \return					A new route between nodes. NULL in the case there is no route.
 */
Route *nmesh_find_route ( NodeMesh *nmesh, int indexFrom, int indexTo );

/**
 * Deletes a route.
 * \param	route			The route to delete.
 */
void route_delete ( Route *route );

/**
 * Builds a nodemesh from an entity path.
 * \return					a pointer to the new nodemesh.
 */
//NodeMesh *nmesh_from_path ( ENTITY *ent );


/**
 * A pointer to the nmesh constructor collision entity
 */
ENTITY *nmesh_collider;



#include "nodemesh.c"
#endif