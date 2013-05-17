#ifndef _NODEMESH_H_
#define _NODEMESH_H_

/**
 * \file nodemesh.h
 * nodemesh.h provides a nodes collection management and pathfinder
 */

/**
 * Dependencies
 */
#include "list.h"


typedef struct Node
{
// public
	var x;
	var y;
	var z;
	
// private
	int index;
	var weight;
	int includer;
	int *linked;
	int count;
	int capacity;
	struct Node *next;
} Node;

typedef struct NodeMesh
{
// private
	Node *nodes;
	int count;
	int capacity;
} NodeMesh;

/**
 * Route is used instead of List to give a clear difference between a normal list and a route of nodes.
 */
typedef List Route;

/**
 * Creates a new nodemesh.
 * \return					A new nodemesh.
 */
NodeMesh *nodemesh_create ();

/**
 * Deletes a nodemesh.
 * \param	nodemesh			The nodemesh to delete.
 */
void nodemesh_remove ( NodeMesh *nodemesh );

/**
 * Clones a nodemesh.
 * \param	nodemesh			The nodemesh to clone.
 * \return					A new nodemesh.
 */
//NodeMesh *nodemesh_clone ( NodeMesh *nodemesh );

/**
 * Adds a node to the nodemesh and builds it neighborhood with a c_trace.
 * \param	nodemesh		The nodemesh.
 * \param	pos			The position of the node to be added.
 * \return					The node index in the nodemesh list.
 */
int nodemesh_add ( NodeMesh *nodemesh, VECTOR *pos );

/**
 * Looks for the nearest node to a certain position.
 * \param	nodemesh		The nodemesh.
 * \param	pos			The position .
 * \return					The index of the node in the nodemesh list.
 */
int nodemesh_nearest ( NodeMesh *nodemesh, VECTOR *pos );

/**
 * Checks the visibilty between a node and all the other nodes inside a nodemesh.
 * Connects them if visible
 * \param	nodemesh		The nodemesh.
 * \param	nodeindex	The index of the node.
 */
void nodemesh_ctrace_node ( NodeMesh *nodemesh, int nodeindex );

/**
 * Looks for a route between two nodes.
 * \param	nodemesh			The nodemesh.
 * \param	indexFrom	The index of the starting node
 * \param	indexTo		The index of the ending node
 * \return					A new route between nodes. NULL in the case there is no route.
 */
Route *nodemesh_find_route ( NodeMesh *nodemesh, int indexFrom, int indexTo );

/**
 * Deletes a route.
 * \param	route			The route to delete.
 */
void route_delete ( Route *route );

/**
 * Builds a nodemesh from an entity path.
 * \return					a pointer to the new nodemesh.
 */
//NodeMesh *nodemesh_from_path ( ENTITY *ent );


/**
 * A pointer to the nodemesh constructor collision entity
 */
ENTITY *nodemesh_collider = NULL;



#include "nodemesh.c"
#endif