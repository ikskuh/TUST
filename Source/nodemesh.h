#ifndef _NODEMESH_H_
#define _NODEMESH_H_

/**
 * \defgroup NodeMeshModule NodeMesh
 * \brief    Node based pathfinder.
 * \ingroup  GenericScriptLibrary
 * \{
 *
 * \file  nodemesh.h
 * \brief Node based pathfinder header.
 *
 * \file  nodemesh.c
 * \brief Node based pathfinder code.
 */

#include "list.h"

typedef struct Node
{
	var x;
	var y;
	var z;
	
	int index;
	var weight;
	int includer;
	int *linked;
	int count;
	int capacity;
	struct Node *next;
} Node;

/**
 * \brief    Generic NodeMesh struct
 */
typedef struct NodeMesh
{
	Node *nodes;
	int count;
	int capacity;
} NodeMesh;

/**
 * \brief    Route is used instead of List to give a clear difference between a normal list and a route of nodes.
 */
typedef List Route;

/**
 * \brief    Creates a new nodemesh.
 * \return   A new nodemesh.
 */
NodeMesh *nodemesh_create ();

/**
 * \brief Removes a nodemesh.
 * \param    nodemesh      The nodemesh to delete.
 */
void nodemesh_remove ( NodeMesh *nodemesh );

/*
 * \brief Clones a nodemesh.
 * \param    nodemesh      The nodemesh to clone.
 * \return   A new nodemesh.
 */
//NodeMesh *nodemesh_clone ( NodeMesh *nodemesh );

/**
 * \brief    Adds a node to the nodemesh and builds it neighborhood with a c_trace.
 * \param    nodemesh      The nodemesh.
 * \param    pos           The position of the node to be added.
 * \return   The node index in the nodemesh list.
 */
int nodemesh_add ( NodeMesh *nodemesh, VECTOR *pos );

/**
 * \brief    Looks for the nearest node to a certain position.
 * \param    nodemesh      The nodemesh.
 * \param    pos           The position .
 * \return   The index of the node in the nodemesh list.
 */
int nodemesh_nearest ( NodeMesh *nodemesh, VECTOR *pos );

/**
 * \brief    Checks the visibilty between a node and all the other nodes inside a nodemesh.
 *           Connects them if visible
 * \param    nodemesh      The nodemesh.
 * \param    nodeindex     The index of the node.
 */
void nodemesh_ctrace_node ( NodeMesh *nodemesh, int nodeindex );

/**
 * \brief    Looks for a route between two nodes.
 * \param    nodemesh      The nodemesh.
 * \param    indexFrom     The index of the starting node
 * \param    indexTo       The index of the ending node
 * \return   A new route between nodes. NULL in the case there is no route.
 */
Route *nodemesh_find_route ( NodeMesh *nodemesh, int indexFrom, int indexTo );

/**
 * \brief    Deletes a route.
 * \param    route         The route to delete.
 */
void route_delete ( Route *route );

/*
 * \brief    Builds a nodemesh from an entity path.
 * \return   A pointer to the new nodemesh.
 */
//NodeMesh *nodemesh_from_path ( ENTITY *ent );

/**
 * \brief    A pointer to the nodemesh constructor collision entity
 */
ENTITY *nodemesh_collider = NULL;

/**
 * \}
 */

/**
 * \var      NodeMesh::count
 * \brief    Amount of nodes cointained by the mesh.
 * \warning  Read only
 */
 
#include "nodemesh.c"
#endif