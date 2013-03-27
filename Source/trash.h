#ifndef _TRASH_H_
#define _TRASH_H_

/**
 * \file trash.h
 * trash.h provides a generic linked trash
 */

/**
 * TrashData is used instead of void to give a clear difference between a normal pointer and a pointer used in trash.
 */
typedef void TrashData;
typedef void TrashRemover;

/**
 * A trash item.
*/
typedef struct TrashItem
{
//private:
	struct TrashItem *next;

//public:	
	/**
	 * The data stored in this trash item.
	 */
	TrashData *data;
} TrashItem;

/**
 * A generic trash which allows you handling a collection of structs.
 * The trash doesn't have a fixed size, so you can just add or remove items.
 */
typedef struct Trash
{
//private:
	int count;
	int top;
//public:
	/**
	 * The first item of the trash.
	 */
	TrashItem *first;
	
	TrashRemover *delete_function;
} Trash;

/**
 * Creates a new trash.
 * \param	items_max	The trash to delete.
 * \param	remove_function	The trash to delete.
 * \return					A new trash.
 */
Trash *trash_create ( int items_max, TrashRemover *remove_function );

/**
 * Deletes a trash.
 * \param	trash			The trash to delete.
 */
void trash_remove ( Trash *trash );

/**
 * Adds a new item to the trash.
 * \param	trash			The trash where the item should be added.
 * \param	item			The item which should be added to the trash.
 */
void trash_add ( Trash *trash, TrashData *data );

/**
 * Removes all items from a trash.
 * \param	trash			The trash which should be cleared.
 */
void trash_clear ( Trash *trash );

/**
 * Get a trash member.
 * \param	trash			The trash to get the member from.
 * \return					First member on the list if exists, otherwise NULL
 */
TrashData *trash_recover ( Trash *trash );


#include "trash.c"
#endif