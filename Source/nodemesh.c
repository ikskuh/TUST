#include <acknex.h>
#include "nodemesh.h"

int nodes_trace ( Node *nodeFrom, Node *nodeTo, var mode, ENTITY *collider );
void nodes_connect ( Node *nodeFrom, Node *nodeTo );

void nodemesh_startup ()
{
	route_delete = list_delete;
}

void nodemesh_close ()
{
	
}

NodeMesh *nodemesh_create ()
{
	NodeMesh *nodemesh = sys_malloc ( sizeof(NodeMesh) );
	nodemesh->nodes = (Node*)sys_malloc ( sizeof(Node) * 4 );
	nodemesh->count = 0;
	nodemesh->capacity = 4;
	return nodemesh;
}

void nodemesh_remove ( NodeMesh *nodemesh )
{
	int i;
	Node *node = nodemesh->nodes;
	Node *nodeLast = node + nodemesh->count;
	for ( ; node<nodeLast; node++ )
	{
		if ( node->linked != NULL )
			sys_free ( node->linked );
	}
	sys_free ( nodemesh->nodes );
	sys_free ( nodemesh );
}

int nodes_trace ( Node *nodeFrom, Node *nodeTo, var mode, ENTITY *collider )
{
	ENTITY *me_old = me;
	me = collider;
	mode |= USE_BOX;
	int free_way = !(c_trace ( &nodeFrom->x, &nodeTo->x, mode ) > 0);
	me = me_old;
	return free_way;
}

void nodes_connect ( Node *nodeFrom, Node *nodeTo )
{
	if ( nodeFrom->count >= nodeFrom->capacity )
	{
		Node *newarray = sys_malloc ( sizeof(int) * ( nodeFrom->count + 4 ) );
		memcpy ( newarray, nodeFrom->linked, sizeof(int) * nodeFrom->count );
		sys_free ( nodeFrom->linked );
		nodeFrom->linked = newarray;
		nodeFrom->capacity += 4;
	}
	if ( nodeTo->count >= nodeTo->capacity )
	{
		Node *newarray = sys_malloc ( sizeof(int) * ( nodeTo->count + 4 ) );
		memcpy ( newarray, nodeTo->linked, sizeof(int) * nodeTo->count );
		sys_free ( nodeTo->linked );
		nodeTo->linked = newarray;
		nodeTo->capacity += 4;
	}
	*(nodeFrom->linked+nodeFrom->count) = nodeTo->index;
	nodeFrom->count += 1;
	*(nodeTo->linked+nodeTo->count) = nodeFrom->index;
	nodeTo->count += 1;
}

void nodemesh_ctrace_node ( NodeMesh *nodemesh, int nodeindex )
{
	int i;
	Node *node = nodemesh->nodes;
	Node *nodeLast = node + nodemesh->count;
	Node *nodeTemp = node + nodeindex;
	for ( ; node<nodeLast; node++ )
	{
		if ( node == nodeTemp ) 
			continue;
		if ( nodes_trace ( node, nodeTemp, IGNORE_PASSABLE, nodemesh_collider ) > 0 )
			nodes_connect ( node, nodeTemp );
	}
}


int nodemesh_add ( NodeMesh *nodemesh, VECTOR *pos )
{
	int i;
	
	if ( nodemesh_collider == NULL )
	{
		nodemesh_collider = ent_create ( CUBE_MDL, nullvector, NULL );
		vec_fill ( &nodemesh_collider->max_x, 20 );
		vec_fill ( &nodemesh_collider->min_x, -20 );
	}
	if ( nodemesh->count >= nodemesh->capacity )
	{
		Node *newarray = sys_malloc ( sizeof(Node) * ( nodemesh->count + 4 ) );
		memcpy ( newarray, nodemesh->nodes, sizeof(Node) * nodemesh->count );
		sys_free ( nodemesh->nodes );
		nodemesh->nodes = newarray;
		nodemesh->capacity += 4;
	}
	
	Node *newnode = nodemesh->nodes + nodemesh->count;
	vec_set ( &newnode->x, pos );
	newnode->weight = 0;
	newnode->index = nodemesh->count;
	newnode->includer = 0;
	newnode->linked = (int*)sys_malloc ( sizeof(int) * 4 );
	newnode->count = 0;
	newnode->capacity = 4;
	nodemesh->count ++;
	return newnode->index;
}





void nodemesh_draw ( NodeMesh *nodemesh, COLOR *color )
{
	int i, ii;
	VECTOR vtemp;
	Node *node = nodemesh->nodes;
	Node *nodeLast = node + nodemesh->count;
	for ( ; node<nodeLast; node++ )
	{
		vec_set ( &vtemp, &node->x );
		vec_to_screen ( &vtemp, camera );
		draw_text(str_for_int(NULL,node->index),vtemp.x,vtemp.y-20,COLOR_WHITE);
//		draw_text(str_for_int(NULL,node->count),vtemp.x,vtemp.y,COLOR_GREY);
		draw_text(str_for_int(NULL,node->includer),vtemp.x,vtemp.y,COLOR_GREEN);
		
		int *link = node->linked;
		int *linkLast = link + node->count;
		for ( ; link<linkLast; link++ )
		{
			Node *neighbor = nodemesh->nodes + *link;
			draw_line3d ( &neighbor->x, color, 0 );
			draw_line3d ( &neighbor->x, color, 100 );
			draw_line3d ( &node->x, color, 100 );
			draw_line3d ( &node->x, color, 0 );
		}
	}
}

int nodemesh_nearest ( NodeMesh *nodemesh, VECTOR *pos )
{
	var distance = 1000000;
	int i, nearest_index = -1;
	for ( i=0; i<nodemesh->count; i++ )
	{
		Node *node = nodemesh->nodes + i;
		var new_distance = vec_dist ( &node->x, pos );
		if ( new_distance > distance )
			continue;
		distance = new_distance;
		nearest_index = i;
	}
	
	return nearest_index;
}

NodeMesh *nodemesh_clone ( NodeMesh *nodemesh )
{
	return NULL;
}

void *nodemesh_connect_nodes ( NodeMesh *nodemesh, int indexFrom, int indexTo )
{
	nodes_connect ( nodemesh->nodes + indexFrom, nodemesh->nodes + indexTo ); 
}

Route *nodemesh_find_route ( NodeMesh *nodemesh, int indexFrom, int indexTo )
{
	Node *nodesall = nodemesh->nodes;
	int i, ii;
	Node *start = nodesall + indexFrom;
	if ( start == NULL )
		return NULL;
	start->weight = -1000000;
	start->includer = -1;
	Node *end = nodesall + indexTo;
	if ( end == NULL )
		return NULL;
	end->includer = -1;
	
	Node *open = start;
	open->next = NULL;
	Node *close = NULL;
	
	while ( open != NULL )
	{
		Node *node = open;
		open = open->next;
		for ( i=0; i<node->count; i++ )
		{
			int index = *(node->linked + i);
			if ( index == node->includer )
				continue;
			Node *neighbor = nodesall + index;
			var distance = vec_dist ( &node->x, &neighbor->x );
			Node *prev = NULL, *tclose = close, *topen = open;
			for ( tclose = close; tclose != NULL; )
			{
				if ( tclose != neighbor )
				{
					prev = tclose;
					tclose = tclose->next;
					continue;
				}
				if ( neighbor->weight > node->weight + distance )
				{
					if ( close == neighbor )
						close = close->next;
					else
						prev->next = neighbor->next;
					
					topen = NULL;
				}
				else
				{
					topen = !NULL;
				}
				break;
			}
			if ( tclose == NULL )
			{
				for ( topen = open; topen != NULL; )
				{
					if ( topen != neighbor )
					{
						topen = topen->next;
						continue;
					}
					if ( neighbor->weight > node->weight + distance )				
					{
						neighbor->weight = node->weight + distance;
						neighbor->includer = node->index;
					}
					break;
				}
			}
			if ( topen == NULL )
			{
				neighbor->weight = node->weight + distance;
				neighbor->includer = node->index;
				if ( open == NULL )
				{
					neighbor->next = NULL;
					open = neighbor;
				}
				else
				{
					prev = NULL;
					for ( topen = open; topen != NULL; )
					{
						if ( topen->weight < neighbor->weight )
						{
							if ( topen->next == NULL )
							{
								topen->next = neighbor;
								neighbor->next = NULL;
								break;
							}
							prev = topen;
							topen = topen->next;
							continue;
						}
						if ( prev == NULL )
						{
							neighbor->next = open;
							open = neighbor;
						}
						else
						{
							neighbor->next = prev->next;
							prev->next = neighbor;
						}
						break;
					}			
				}
			}
		}
		node->next = close;
		close = node;
		
		if ( ( end->includer >= 0 ) && ( node->weight > end->weight ) )
			break;
	}
	
	if ( end->includer < 0 )
		return NULL;
	
	Route *route = list_create ();
	Node *node = end;
	i = 0;
	while ( 1 )
	{
		list_add_first ( route, node );
		i++;
		if ( node->includer >= 0 )
			node = nodesall + node->includer;
		else
			break;
	}
	
	return route;
}


void route_draw ( Route *route, COLOR *color )
{
	Node *node, *prev=NULL;
	ListIterator *it = list_begin_iterate(route);
	for ( node = list_iterate(it); it->hasNext; node = list_iterate(it) )
	{
		if ( prev != NULL )
		{
			draw_line3d ( &node->x, color, 0 );
			draw_line3d ( &node->x, color, 100 );
			draw_line3d ( &prev->x, color, 100 );
			draw_line3d ( &prev->x, color, 0 );
		}
		prev = node;
	}
	list_end_iterate(it);
}

