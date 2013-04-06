#include <acknex.h>
#include "nodemesh.h"

//---------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------
ListItem *node_contained_by ( List *list, ListData *item )
{
	ListItem *it = list->first;
	for(it = list->first; it != NULL; it = it->next)
	{
		if(it->data != item)
			continue;
		return it;
	}
	return NULL;
}

void nmesh_list_remove_container ( List *list, ListItem *item )
{
	if ( list->first == item )
	{
		list->first = item->next;
		if ( list->last == item )
			list->last = NULL;
		else
			list->first->previous = NULL;
	}
	else if ( list->last == item )
	{
		list->last = item->previous;
		list->last->next = NULL;
	}
	else
	{
		ListItem *prev = item->previous;
		ListItem *next = item->next;
		prev->next = next;
		next->previous = prev;
	}
	list->count -= 1;
	sys_free ( item );
}

ListData *list_extract_first ( List *list )
{
	ListItem *it = list->first;
	if ( it == NULL )
		return NULL;
	list->first = it->next;
	if ( list->first == NULL )
		list->last = NULL;
	else
		list->first->previous = NULL;
	ListData *data = it->data;
	sys_free ( it );
	list->count -= 1;
	return data;
}
//---------------------------------------------------------------------------------

void nmesh_startup ()
{
	nmesh_create = list_create;
	nmesh_clone = list_clone;
	route_delete = list_delete;
}

void nmesh_close ()
{
	
}

void nmesh_remove ( NodeMesh *nmesh )
{
	Node *node;
	ListIterator *it = list_begin_iterate(nmesh);
	for ( node = list_iterate(it); it->hasNext; node = list_iterate(it) )
		node_remove ( node );
	list_delete ( nmesh );
}

int nmesh_add ( NodeMesh *nmesh, VECTOR *pos )
{
	if ( nmesh_collider == NULL )
	{
		nmesh_collider = ent_create ( CUBE_MDL, nullvector, NULL );
		vec_fill ( &nmesh_collider.max_x, 20 );
		vec_fill ( &nmesh_collider.min_x, -20 );
	}
	
	Node *newnode = node_create ( pos );
	Node *node;
	ListIterator *it = list_begin_iterate(nmesh);
	for ( node = list_iterate(it); it->hasNext; node = list_iterate(it) )
	{
		if ( nodes_trace ( node, newnode, IGNORE_PASSABLE, nmesh_collider ) > 0 )
				nodes_connect ( node, newnode );
	}
	list_end_iterate(it);
	list_add ( nmesh, newnode );
	return list_get_count ( nmesh );
}

int nmesh_nearest ( NodeMesh *nmesh, VECTOR *pos )
{
	var distance = 100000;
	Node *node, *nearest=NULL;
	int index = 0;
	ListIterator *it = list_begin_iterate(nmesh);
	for ( node = list_iterate(it); it->hasNext; node = list_iterate(it) )
	{
		index ++;
		if ( vec_dist ( &node->pos_x, pos ) < distance )
			nearest = node;
	}
	list_end_iterate(it);
	if ( distance < 100000 )
		return index;
	else
		return 0;
}

int compare_nodes_weight ( Node *left, Node *right )
{
	return left->skill_x - right->skill_x;
}

/* Node struct
/* skill_x 		Weight. Computed from the sum of the previous node weight and the distance between nodes
 * skill_y 		Includer, the node that includes this node into the open list
 */
Route *nmesh_find_route ( NodeMesh *nmesh, int indexFrom, int indexTo )
{
	Node *start = list_item_at ( nmesh, indexFrom ); // get starting node
	if ( start == NULL )
		return NULL;
	start->skill_x = -1000000; // very low weight, but it really does not matter 
	start->skill_y = NULL;
	Node *end = list_item_at ( nmesh, indexTo ); // get ending node
	if ( end == NULL )
		return NULL;
	end->skill_x = 1000000; // very high weight
	end->skill_y = NULL;
	
	List *open = list_create (); 	// List for conputable nodes
	list_add ( open, start ); 		// insert starting node
	List *close = list_create (); // List for computed nodes
	
	while ( open->count > 0 ) // while open list is not empty
	{
		Node *node = list_extract_first ( open ); 	// take the first member of the open list
		list_add_first ( close, node ); 					// and add it to the close list
		
		Node *neighbor;
		ListIterator *it = list_begin_iterate(node->neighborhood);
		for ( neighbor = list_iterate(it); it->hasNext; neighbor = list_iterate(it) )		// compute the taken nodes neighborhood
		{
			if ( neighbor == (Node*)node->skill_y ) 									// Obviate the nodes includer. We can be sure that it has been previously computed and its weight is lower
				continue;
			var distance = nodes_distance ( node, neighbor );
			ListItem *container = node_contained_by ( close, neighbor );		// Check if the actual neighbor is into the close list
			if ( container != NULL ) 														// if there is a ListItem struct that contains the actual neighbor, it is has been previously computed
			{																					 
				if ( neighbor->skill_x > node->skill_x + distance ) 				// if its previously computed weight is bigger than the incoming weight
				{
					neighbor->skill_x = node->skill_x + distance;					// set the new weight
					neighbor->skill_y = (Node*)node;										// set the new includer node
					nmesh_list_remove_container ( close, container );				// move from close to open list
					list_add_sorted ( open, neighbor, compare_nodes_weight );	// sorted from low to high weight
					// a better way wold be to move the container from open to close list
				}
			}
			else if ( list_contains ( open, neighbor ) > 0 )						// Since its not into the close list, check if the actual neighbor is into the open list
			{
				if ( neighbor->skill_x > node->skill_x + distance )				// if its previously computed weight is bigger than the incoming weight
				{
					neighbor->skill_x = node->skill_x + distance;					// set the new weight
					neighbor->skill_y = (Node*)node;										// set the new includer node
					// since open list is sorted
					// a function that moves desired member of the list to its sorted position
					// would be better
					list_sort ( open, compare_nodes_weight );							// sort the list from nearest to furthest node
				}
			}
			else																					// Since its not into the close list neither into the open list
			{
				neighbor->skill_x = node->skill_x + distance;						// set the new weight
				neighbor->skill_y = (Node*)node;											// set the new includer node
				list_add_sorted ( open, neighbor, compare_nodes_weight );		// add to the open list sorted from low to high weight
			}
		}
		list_end_iterate(it);
		
		if ( ( end->skill_y != NULL ) && ( node->skill_x > end->skill_x ) ) 	// if the end node has been computed and is thinner than the actual node
			break; 																				// finish the loop
	}
	
	list_delete ( open );
	list_delete ( close );
	
	if ( end->skill_y == NULL ) 	// If the end node has not been computed.
		return NULL;					// the nodes are not connected
	
	Route *route = list_create (); 		// Build a list with the path nodes
	Node *node = end;					 		// from the ending node
	while ( node != NULL )			 		// to the starting node
	{
		list_add_first ( route, node );	// add to the list in a inverse way
		node = (Node*)node->skill_y;		// continued by its includer into the open list
	}
	
	return route;
}

void nmesh_draw ( NodeMesh *nmesh, COLOR *color )
{
	VECTOR vtemp;
	Node *node, *neighbor;
	ListIterator *it = list_begin_iterate(nmesh);
	int i=0;
	for ( node = list_iterate(it); it->hasNext; node = list_iterate(it) )
	{
		vec_set ( &vtemp, &node->pos_x );
		vec_to_screen ( &vtemp, camera );
		draw_text(str_for_int(NULL,++i),vtemp.x,vtemp.y-20,COLOR_WHITE);
		draw_text(str_for_int(NULL,node->neighborhood->count),vtemp.x,vtemp.y,nullvector);
		ListIterator *it2 = list_begin_iterate(node->neighborhood);
		for ( neighbor = list_iterate(it2); it2->hasNext; neighbor = list_iterate(it2) )
		{
			draw_line3d ( &node->pos_x, color, 0 );
			draw_line3d ( &node->pos_x, color, 100 );
			draw_line3d ( &neighbor->pos_x, color, 100 );
			draw_line3d ( &neighbor->pos_x, color, 0 );
		}
		list_end_iterate(it2);
	}
	list_end_iterate(it);
}

void route_draw ( Route *route, COLOR *color )
{
	Node *node, *prev;
	ListIterator *it = list_begin_iterate(route);
	var i=0;
	prev = list_iterate(it);
//	if ( it->hasNext )
		for ( node = list_iterate(it); it->hasNext; node = list_iterate(it) )
		{
			draw_line3d ( &node->pos_x, color, 0 );
			draw_line3d ( &node->pos_x, color, 100 );
			draw_line3d ( &prev->pos_x, color, 100 );
			draw_line3d ( &prev->pos_x, color, 0 );
			prev = node;
			i++;
		}
	list_end_iterate(it);
}

