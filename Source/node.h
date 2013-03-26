#ifndef _NODE_H_
#define _NODE_H_

/**
 * \file node.h
 * node.h provides a linked list based neighborhood management
 */

/**
 * Dependencies
 */
#include "list.h"

/**
 * NodeData is used instead of void to give a clear difference between a normal pointer and a pointer used in nodes.
 */
//typedef void NodeData;


/**
 * Node struct
*/
typedef struct Node
{
	/**
	 * World position
	 */
	var pos_x;
	var pos_y;
	var pos_z;
	
	/**
	 * 32 bits flags
	 */
//	var flags;
	
	/**
	 * General purpouse variables
	 */
//	var skill_x;
//	var skill_y;
//	var skill_z;
	
	/**
	 * Custom data pointer
	 */
//	NodeData *data;
	
	/**
	 * A linked list of node neighborhood.
	 */
	List *neighborhood;
	
} Node;

/**
 * Creates a new node.
 * \param	position		World position.
 * \param	group			Node group.
 * \return					A new node.
 */
Node *node_create ( VECTOR *position );

/**
 * Deletes a node.
 * \param	node			The node to delete.
 */
void *node_delete ( Node *node );

/**
 * Clones a node.
 * \param	node			The node to clone.
 * \return					A new node.
 */
Node *node_clone ( Node *node );

/**
 * Connects two nodes.
 * \param	nodeFrom		First node to connect.
 * \param	nodeTo		Second node to connect.
 */
void nodes_connect ( Node *nodeFrom, Node *nodeTo );

/**
 * Disconnects two nodes.
 * \param	nodeFrom		First node to disconnect.
 * \param	nodeTo		Second node to disconnect.
 */
void nodes_disconnect ( Node *nodeFrom, Node *nodeTo );

/**
 * Gives node neighbors amount.
 * \param	node			The node.
 * \return					Neighbor amount .
 */
int node_neighbor_count ( Node *node );

/**
 * Gives the distance between two nodes.
 * \param	nodeFrom		First node.
 * \param	nodeTo		Second node.
 * \return					Distance between nodes.
 */
var nodes_distance ( Node *nodeFrom, Node *nodeTo );

#include "node.c"
#endif