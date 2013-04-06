#include <acknex.h>
//#include <default.c>
#include "..\\..\\Source\\tust.h"

#define PRAGMA_POINTER

NodeMesh *myNMesh = NULL;

function main()
{
	video_mode = 12;
	random_seed(0);
	wait(1);
	myNMesh = nmesh_create ();
	level_load ( "test_nodemesh.wmb" );
	camera->z = 2000;
	camera->tilt = -90;
	camera->roll = 90;
	wait(3);
	
	while (!key_esc)
	{
		int start = nmesh_nearest ( myNMesh, vector(fsin(total_ticks,1000), fcos(total_ticks,1000), 0) );
//		error(str_for_int(NULL,start));
		int end = integer(random(myNMesh->count));
		while ( end == start )
			end = integer(random(myNMesh->count));
		Route *route = nmesh_find_route ( myNMesh, start, end );
		var clock = total_ticks; 
//		error(str_for_int(NULL,end));
		while ( ( total_ticks - clock < 8 ) && !key_esc )
		{
			if ( mouse_middle )
			{
				vec_add ( &camera->x, vector(mickey.x,-mickey.y,0) );
			}
			route_draw ( route, COLOR_WHITE );
			nmesh_draw ( myNMesh, COLOR_RED );
			wait(1);
		}
		route_delete ( route );
	}
	
	nmesh_remove ( myNMesh );
	
	sys_exit ( NULL );
}


action nmesh_node ()
{
	VECTOR pos;
	vec_set ( &pos, &my->x );
	ent_remove ( me );
	wait(1);
	nmesh_add ( myNMesh, &pos );
}




















