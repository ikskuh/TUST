#ifndef _LIST_H_
#define _LIST_H_

/**
 * \file list.h
 * \brief Linked list implementation.
 *
 * list.h provides a generic linked list
 */

/**
 * ListData is used instead of void to give a clear difference between a normal pointer and a pointer used in lists.
 */
typedef void ListData;

/**
 * A list item. You can use a list item to iterate through a list.
*/
typedef struct ListItem
{
//private:
	struct ListItem *previous;
	struct ListItem *next;

//public:	
	/**
	 * The data stored in this list item.
	 */
	ListData *data;
} ListItem;

/**
 * A generic list which allows you handling a collection of structs.
 * The list doesn't have a fixed size, so you can just add or remove items.
 */
typedef struct List
{
//private:
	int count;
//public:
	/**
	 * The first item of the list.
	 */
	ListItem *first;
	
	/**
	 * The last item of the list.
	 */
	ListItem *last;
} List;

/**
 * An iterator for a list. You need the iterator to go through all items in a list.
 */
typedef struct ListIterator
{
//private:
	List *list;
	ListItem *current;
	int valid;
//public:
	int hasNext;
} ListIterator;

/**
 * Creates a new list.
 * \return				A new list.
 */
List *list_create();

/**
 * Deletes a list.
 * \param	list		The list to delete.
 */
void list_delete(List *list);

/**
 * Clones a list with all list items.
 * \param	list		The list to clone.
 * \return				A new list with all items which were in the list to clone.
 */
List *list_clone(List *list);

/**
 * Adds a new item to the last position of a list
 * \param	list		The list where the item should be added.
 * \param	item		The item which should be added to the list.
 */
void list_add(List *list, ListData *item);

/**
 * Adds a range of items to the list.
 * \param	list		The list where the items should be added.
 * \param	array		The array where the items should be copied from.
 * \param	count		The number of items which should be copied.
 */
void list_add_range(List *list, ListData **array, int count);

/**
 * Adds a new item in the first position of a list.
 * \param	list		The list where the item should be added.
 * \param	item		The item which should be added to the list.
 */
void list_add_first ( List *list, ListData *item );

/**
 * Adds a new item sorted by a comparision function
 * \param	list			The list where the item should be added.
 * \param	item			The item which should be added to the list.
 * \param	compare		A pointer to a comparision function. The function needs this signature: int compare(ListData *left, ListData *right) and returns 1 if left>right, 0 if left=right and -1 if left<right.
 */
void list_add_sorted ( List *list, ListData *item, void *compare );

/**
 * Removes the item from the list.
 * \param	list		The list where the item should be removed.
 * \param	item		The item to remove from the list.
 */
void list_remove(List *list, ListData *item);

/**
 * Removes all matching item sfrom the list.
 * \param	list		The list where the items should be removed.
 * \param	item		The item to remove from the list.
 */
void list_remove_all(List *list, ListData *item);

/**
 * Removes the item at a given index from the list.
 * \param	list		The list where the item should be removed.
 * \param	index		The index of the item to remove.
 */
void list_remove_at(List *list, int index);

/**
 * Checks if a list contains an item.
 * \param	list		The list which should be checked.
 * \param	item		The item which should be searched.
 * \return				0 if the item is not found, else != 0.
 */
int list_contains(List *list, ListData *item);


/**
 * Returns the item stored at a given index.
 * \param	list		The list where the item is.
 * \param	index		The index of the item.
 * \return				The item or NULL if index out of range.
 */
ListData *list_item_at(List *list, int index);

/**
 * Removes all items from a list.
 * \param	list		The list which should be cleared.
 */
void list_clear(List *list);

/**
 * Clears the listitems and the listitems data of a list
 * \param	list		The list where the item should be added.
 * \param	remove_function		A pointer to a remove function. The function needs this signature: void remove_function ( ListData *item )
 */
void list_clear_content ( List *list, void *remove_function );


/**
 * Sorts a list.
 * \param	list		The list which should be sorted.
 * \param	compare		A pointer to a comparision function. The function needs this signature: int compare(ListData *left, ListData *right) and returns 1 if left>right, 0 if left=right and -1 if left<right.
 */
void list_sort(List *list, void *compare);

/**
 * Reverses a list (first item will be last, last will be first, ...).
 * \param	list		The list to be reversed.
 */
void list_reverse(List *list);

/**
 * Gets the count of items in a list.
 * \param	list		The list to get the count from.
 * \return				The amount of items in the list.
 */
int list_get_count(List *list);

/**
 * Copies a list to an array of items.
 * \param	list		The list to be reversed.
 * \param	array		The array where the items should be copied to.
 * \param	arrayLength	The max. number of items in the array.
 * \return				The amount of items copied.
 */
int list_copy_to(List *list, ListData **array, int arrayLength);

/**
 * Begins a list iteration.
 * \param	list		The list to iterate through
 * \return				A new iterator for the given list.
 */
ListIterator *list_begin_iterate(List *list);

/**
 * Gets the next value in a list iteration.
 * \param	iterator	The iterator to be used.
 * \return				The next value in the iteration.
 */
ListData *list_iterate(ListIterator *iterator);

/**
 * Ends an iteration and frees the iterator.
 * \param	iterator	The iteration to be freed.
 */
void list_end_iterate(ListIterator *iterator);




#include "list.c"
#endif