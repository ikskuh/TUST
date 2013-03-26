#include <acknex.h>
#include "..\\Source\\list.h"
#include "..\\Source\\node.h"

#define LOOPS		10

function main()
{
	int i, ii;
	random_seed (0);
	wait(1);
	
	// Create a nodes array
	Node **node = sys_malloc(sizeof(Node*)*LOOPS);
	for ( i=0; i<LOOPS; i++ )
	{
		node[i] = node_create ( vector(random(10000),random(10000),random(10000)) );
	}
	
	// Connect all
	for ( i=0; i<LOOPS; i++ )
	{
		for ( ii=0; ii<LOOPS; ii++ )
		{
			if ( i != ii )
				nodes_connect ( node[i], node[ii] );
		}
	}
	
	// Print first node neighbors amount
	error ( "First node neighbors amount" );
	error ( str_for_int(NULL,node_neighbor_count(node[0])) );
	
	// Disconnect to nodes randomly
	i = integer(random(LOOPS));
	ii = integer(random(LOOPS));
	while ( i == ii )
		ii = integer(random(LOOPS));
	nodes_disconnect ( node[i], node[ii] );
	
	// Print disconnected node neighbors amount
	error ( "two randomly disconnected neighbor amount" );
	error ( str_for_int(NULL,node_neighbor_count(node[i])) );
	error ( str_for_int(NULL,node_neighbor_count(node[ii])) );
	
	// Clone a node
	Node *nodeNew = node_clone ( node[i] );
	int amount = node_neighbor_count(nodeNew);
	error ( "cloned neighbor count" );
	error ( str_for_int(NULL,amount) );
	
	// Check cloned nodes neighborhood
	Node *neighbor1 = (Node*)list_item_at ( node[i]->neighborhood, 0 );
	Node *neighbor2 = (Node*)list_item_at ( nodeNew->neighborhood, 0 );
	int iii = 0;
	while ( neighbor1 != NULL )
	{
		if ( neighbor1 != neighbor2 )
		{
			error ( "Error while clonning!" );
			break;
		}
		iii ++;
		neighbor1 = (Node*)list_item_at ( node[i]->neighborhood, iii );
		neighbor2 = (Node*)list_item_at ( nodeNew->neighborhood, iii );
	}
	
	// Print distance between two nodes
	i = integer(random(LOOPS));
	ii = integer(random(LOOPS));
	while ( i == ii )
		ii = integer(random(LOOPS));
	var distance = nodes_distance ( node[i], node[ii] );
	error ( "distance between two random nodes" );
	error ( str_for_num(NULL,distance) );
	
	for ( i=0; i<LOOPS; i++ )
	{
		node_delete ( node[i] );
	}
	sys_free ( node );
	sys_free ( nodeNew );
	
	sys_exit ( NULL );
}