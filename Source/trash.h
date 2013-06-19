
/* 
   The Ultimate Script Library
   -----------------------------------------------------------------------------
   Module: Trash
 
   Read the documentation for further information: TUST\Documentation\index.html
   -----------------------------------------------------------------------------
 */

#ifndef _TRASH_H_
#define _TRASH_H_

typedef void TrashData;

typedef struct TrashItem
{
	struct TrashItem *next;
	TrashData *data;
} TrashItem;

typedef void TrashRemover;

typedef struct TrashCollector
{
	int count;
	int top;
	TrashItem *first;
	TrashRemover delete_function ( TrashData *data );
} TrashCollector;

void trash_open ();

void trash_close ();

TrashCollector *trash_create ( int items_max, TrashRemover *remove_function );

void trash_add ( TrashCollector *trash, TrashData *data );

TrashData *trash_recover ( TrashCollector *trash );

void trash_clear ( TrashCollector *trash );

void trash_remove ( TrashCollector *trash );


// DOXYGEN 
/**
 * \defgroup TrashGroup Trash
 * \ingroup GenericScriptLibrary
 * \brief Linked trash or stack implementation.
 *
 * \file trash.h
 * \ingroup TrashGroup
 * \brief Linked trash or stack implementation.
 *
 * \file trash.c
 * \ingroup TrashGroup
 * \brief Linked trash or stack implementation.
 *
 * \typedef void TrashData
 * \brief Used instead of void to give a clear difference between a normal pointer and a pointer used in trash.
 * \ingroup TrashGroup
 *
 * \typedef void TrashRemover
 * \brief Used instead of void to give a clear difference between a normal function and a remover function used in trash.
 * \ingroup TrashGroup
 *
 * \class TrashCollector
 * \ingroup TrashGroup
 * \brief Generic trash struct
 * \var TrashCollector::count
 * \brief Amount of item cointaned by the trash. (read only)
 *
 * \fn trash_open ()
 * \brief Open the trash system
 * \ingroup TrashGroup
 *
 * \fn trash_close ()
 * \brief Close the trash system
 * \ingroup TrashGroup
 *
 * \fn trash_create ( int items_max, TrashRemover *remove_function )
 * \brief Creates a new trash.
 * \ingroup TrashGroup
 * \param   items_max         The maximum amount of items.
 * \param   remove_function   The trash to delete.
 * \return                    A new trash.
 *
 * \fn trash_add ( TrashCollector *trash, TrashData *data )
 * \brief Adds a new item to the trash.
 * \ingroup TrashGroup
 * \param   trash             The trash where the item should be added.
 * \param   item              The item which should be added to the trash.
 *
 * \fn trash_recover ( TrashCollector *trash )
 * \brief Get a trash item.
 * \ingroup TrashGroup
 * \param   trash             The trash to get the item from.
 * \return                    First item into the trash if exists, otherwise NULL
 *
 * \fn trash_clear ( TrashCollector *trash )
 * \brief Removes all items from a trash.
 * \ingroup TrashGroup
 * \param   trash             The trash which should be cleared.
 *
 * \fn trash_remove ( TrashCollector *trash )
 * \brief   Deletes a trash.
 * \ingroup TrashGroup
 * \param   trash             The trash to delete.
 */
 
#include "trash.c"
#endif
