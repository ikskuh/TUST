#include <acknex.h>
#include "node.h"

/*
void node_startup ()
{
	
}
*/

Node *node_create ( VECTOR* position )
{
	Node *node = sys_malloc(sizeof(Node));
	vec_set ( node->pos_x, position );
	node->neighborhood = list_create ();
	return node;
}

void *node_delete ( Node *node )
{
	Node *neighbor;
	ListIterator *it = list_begin_iterate(node->neighborhood);
	for(neighbor = list_iterate(it); it->hasNext; neighbor = list_iterate(it))
		nodes_disconnect ( node, neighbor );
	list_end_iterate(it);
	list_delete ( node->neighborhood );
	sys_free ( node );
}

Node *node_clone ( Node *node )
{
	Node *clone = node_create ( &node->pos_x );
	Node *neighbor;
	ListIterator *it = list_begin_iterate(node->neighborhood);
	for(neighbor = list_iterate(it); it->hasNext; neighbor = list_iterate(it))
		nodes_connect ( clone, neighbor );
	return clone;
}

void nodes_connect ( Node *NodeFrom, Node *NodeTo )
{
	if ( !list_contains ( NodeFrom->neighborhood, NodeTo ) )
		list_add ( NodeFrom->neighborhood, NodeTo );
	if ( !list_contains ( NodeTo->neighborhood, NodeFrom ) )
		list_add ( NodeTo->neighborhood, NodeFrom );
}

void nodes_disconnect ( Node *nodeFrom, Node *nodeTo )
{
	list_remove ( nodeFrom->neighborhood, nodeTo );
	list_remove ( nodeTo->neighborhood, nodeFrom );
}

int node_neighbor_count ( Node *node )
{
	return list_get_count ( node->neighborhood );
}

var nodes_distance ( Node *nodeFrom, Node *nodeTo )
{
	return vec_dist ( nodeFrom->pos_x, nodeTo->pos_x );
}


