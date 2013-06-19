#ifndef _TRASH_H_
#define _TRASH_H_

// DOXYGEN MODULE CONFIG 
/**
 * \defgroup TrashGroup Trash
 * \ingroup GenericScriptLibrary
 * \brief Linked trash or stack implementation.
 * \file trash.h
 * \ingroup TrashGroup
 * \brief Linked trash or stack implementation.
 * \file trash.c
 * \ingroup TrashGroup
 * \brief Linked trash or stack implementation.
 */

/**
 * \brief Used instead of void to give a clear difference between a normal pointer and a pointer used in trash.
 * \ingroup TrashGroup
 */
typedef void TrashData;

// Private
typedef struct TrashItem
{
	struct TrashItem *next;
	TrashData *data;
} TrashItem;

/**
 * \brief Used instead of void to give a clear difference between a normal function and a remover function used in trash.
 * \ingroup TrashGroup
 */
typedef void TrashRemover;

/**
 * \class TrashCollector
 * \ingroup TrashGroup
 * \brief Generic trash struct
 * \var TrashCollector::count
 * \brief Amount of item cointaned by the trash. (read only)
 */
typedef struct TrashCollector
{
	int count;
	int top;
	TrashItem *first;
	TrashRemover delete_function ( TrashData *data );
} TrashCollector;

/**
 * \brief Open the trash system
 * \ingroup TrashGroup
 */
void trash_open ();

/**
 * \brief Close the trash system
 * \ingroup TrashGroup
 */
void trash_close ();


/**
 * \brief Creates a new trash.
 * \ingroup TrashGroup
 * \param   items_max         The maximum amount of items.
 * \param   remove_function   The trash to delete.
 * \return                    A new trash.
 */
TrashCollector *trash_create ( int items_max, TrashRemover *remove_function );

/**
 * \brief Adds a new item to the trash.
 * \ingroup TrashGroup
 * \param   trash       The trash where the item should be added.
 * \param   item        The item which should be added to the trash.
 */
void trash_add ( TrashCollector *trash, TrashData *data );

/**
 * \brief Get a trash item.
 * \ingroup TrashGroup
 * \param   trash       The trash to get the item from.
 * \return              First item into the trash if exists, otherwise NULL
 */
TrashData *trash_recover ( TrashCollector *trash );

/**
 * \brief Removes all items from a trash.
 * \ingroup TrashGroup
 * \param   trash       The trash which should be cleared.
 */
void trash_clear ( TrashCollector *trash );

/**
 * \brief   Deletes a trash.
 * \ingroup TrashGroup
 * \param   trash       The trash to delete.
 */
void trash_remove ( TrashCollector *trash );

#include "trash.c"
#endif