#include <acknex.h>
#include "trash.h"

TrashCollector *itTrash;

void trashitem_free ( TrashItem *item )
{
	sys_free(item);
}

void trash_open ()
{
	itTrash = trash_create ( 10, trashitem_free );
}

void trash_close ()
{
	TrashItem *it = itTrash->first;
	for ( ; it != NULL; )
	{
		TrashItem *next = it->next;
		sys_free ( it->data );
		sys_free ( it );
		it = next;
	}
}

TrashCollector *trash_create ( int items_max, TrashRemover *remove_function )
{
	TrashCollector *trash = sys_malloc(sizeof(TrashCollector));
	trash->first = NULL;
	trash->top = maxv(items_max,1);
	trash->count = 0;
	trash->delete_function = remove_function;
	return trash;
}

void trash_remove ( TrashCollector *trash )
{
	trash_clear ( trash );
	sys_free ( trash );
}

void trash_add ( TrashCollector *trash, TrashData *data )
{
	if ( trash->count < trash->top )
	{
		TrashItem *item = trash_recover ( itTrash );
		if ( item == NULL )
			item = sys_malloc(sizeof(TrashItem));
		
		item->data = data;
		item->next = trash->first;
		trash->first = item;
		trash->count += 1;
	}
	else
	{
		trash->delete_function ( data );
	}
}

void trash_clear ( TrashCollector *trash )
{
	TrashItem *it = trash->first;
	for ( ; it != NULL; )
	{
		TrashItem *next = it->next;
		trash->delete_function ( it->data );
		trash_add ( itTrash, it );
		it = next;
	}
	trash->first = NULL;
	trash->count = 0;
	trash->top = 0;
	trash->delete_function = NULL;
}

TrashData *trash_recover ( TrashCollector *trash )
{
	if ( trash->first == NULL )
		return NULL;
	
	TrashItem *it = trash->first;
	trash->first = it->next;
	TrashData *data = it->data;
	trash_add ( itTrash, it );
	trash->count -= 1;
	return data;
}
