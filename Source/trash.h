
/* The Ultimate Script Library 
-----------------------------------------------------------------------------------
   Module: Trash
   Linked trash or stack implementation.

   Read the documentation for further information: TUST\Documentation\index.html
----------------------------------------------------------------------------------- 
*/

#ifndef _TRASH_H_
#define _TRASH_H_

/**
 * \defgroup TrashModule Trash
 * \brief    Linked trash or stack implementation.
 * \ingroup  GenericScriptLibrary
 * \{
 *
 * \file  trash.h
 * \brief Linked trash or stack implementation header.
 *
 * \file  trash.c
 * \brief Linked trash or stack implementation code.
 *
 * \file  test_trash.c
 * \brief Linked trash or stack implementation test code.
 */

/**
 * \brief    Used instead of void to give a clear difference between a normal pointer and a pointer used in trash.
 */
typedef void TrashData;

/**
 * \brief    Used instead of void to give a clear difference between a normal function and a remover function used in trash.
 */
typedef void TrashRemover;

typedef struct TrashItem
{
	struct TrashItem *next;
	TrashData *data;
} TrashItem;

/**
 * \struct   TrashCollector
 * \brief    Generic trash struct
 */
typedef struct TrashCollector
{
	int count;
	int top;
	TrashItem *first;
	TrashRemover delete_function ( TrashData *data );
} TrashCollector;

/**
 * \brief    Open the trash system
 */
void trash_open ();

/**
 * \brief    Close the trash system
 */
void trash_close ();

/**
 * \brief    Creates a new trash.
 * \param    items_max         The maximum amount of items.
 * \param    remove_function   The trash to delete.
 * \return                     A new trash.
 */
TrashCollector *trash_create ( int items_max, TrashRemover *remove_function );

/**
 * \brief    Adds a new item to the trash.
 * \param    trash             The trash where the item should be added.
 * \param    item              The item which should be added to the trash.
 */
void trash_add ( TrashCollector *trash, TrashData *data );

/**
 * \brief    Get a trash item.
 * \param    trash             The trash to get the item from.
 * \return                     First item into the trash if exists, otherwise NULL
 */
TrashData *trash_recover ( TrashCollector *trash );

/**
 * \brief    Removes all items from a trash.
 * \param    trash             The trash which should be cleared.
 */
void trash_clear ( TrashCollector *trash );

/**
 * \brief    Deletes a trash.
 * \param    trash             The trash to delete.
 */
void trash_remove ( TrashCollector *trash );

/**
 * \}
 */
 
/**
 * \var      TrashCollector::count
 * \brief    Amount of items cointained by the trash.
 * \warning  Read only
 */
 
#include "trash.c"
#endif
