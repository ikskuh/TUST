#include <acknex.h>
#include "trash.h"

/**
 * trash to contain recyclable trashitem structs.
 */
Trash *itTrash;

void trashitem_remover ( void *item );

void trashitem_free ( TrashItem *item )
{
	sys_free(item);
}

void trash_startup ()
{
	itTrash = trash_create ( 10, trashitem_free );
}

void it_trash_close ()
{
	TrashItem *it = itTrash->first;
	for ( it = itTrash->first; it != NULL; )
	{
		TrashItem *next = it->next;
		sys_free ( it->data );
		sys_free ( it );
		it = next;
	}
}

Trash *trash_create ( int items_max, void *remove_function )
{
	Trash *trash = sys_malloc(sizeof(Trash));
	trash->first = NULL;
	trash->top = maxv(items_max,1);
	trash->count = 0;
	trash->delete_function = remove_function;
	return trash;
}

void trash_remove ( Trash *trash )
{
	trash_clear ( trash );
	sys_free ( trash );
}

void trash_add ( Trash *trash, TrashData *data )
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
		trashitem_remover = trash->delete_function;
		trashitem_remover ( data );
	}
}

void trash_clear ( Trash *trash )
{
	TrashItem *it = trash->first;
	for ( it = trash->first; it != NULL; )
	{
		TrashItem *next = it->next;
		trashitem_remover = trash->delete_function;
		trashitem_remover ( it->data );
		trash_add ( itTrash, it );
		it = next;
	}
	trash->first = NULL;
	trash->count = 0;
	trash->top = 0;
	trash->delete_function = NULL;
}

TrashData *trash_recover ( Trash *trash )
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

