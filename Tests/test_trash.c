#include <acknex.h>
#include <default.c>
#include "..\\Source\\trash.h"

#define LOOPS		20

function main()
{
	int i, ii;
	random_seed (0);
	wait(1);
	def_debug();
	level_load ( "" );
	trash_open ();
	
	wait(-1);
	error ( "Create a 10 entities trash with 'ent_remove' as item remover" );
	TrashCollector *EntityTrash = trash_create ( 10, ent_remove );
	
	error ( "Create many entities" );
	error ( str_for_int(NULL,LOOPS) );
	ENTITY *actors[LOOPS];
	for ( i=0; i<LOOPS; i++ )
	{
		actors[i] = ent_create ( SPHERE_MDL, nullvector, NULL );
	}
	wait(3);
	error ( "drop all them to trash" );
	for ( i=0; i<LOOPS; i++ )
	{
		trash_add ( EntityTrash, actors[i] );
	}
	wait(3);
	error ( "EntityTrash->count" );
	error ( str_for_int(NULL,EntityTrash->count) );
	error ( "TrashOfTrashItems->count" );
	error ( str_for_int(NULL,itTrash->count) );
	
	wait(3);
	error ( "recover an entity" );
	you = trash_recover ( EntityTrash );
	wait(3);
	
	error ( "EntityTrash->count" );
	error ( str_for_int(NULL,EntityTrash->count) );
	error ( "TrashOfTrashItems->count" );
	error ( str_for_int(NULL,itTrash->count) );
	
	error ( "clear trash" );
	trash_clear ( EntityTrash );
	wait(3);
	
	error ( "EntityTrash->count" );
	error ( str_for_int(NULL,EntityTrash->count) );
	error ( "TrashOfTrashItems->count" );
	error ( str_for_int(NULL,itTrash->count) );
	
	trash_remove ( EntityTrash );
	
	trash_close (); // <-needs a global solution
	
	sys_exit ( NULL );
}