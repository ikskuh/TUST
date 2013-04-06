#ifndef _ARRAYLIST_H_
#define _ARRAYLIST_H_

/**
 * \file arraylist.h
 * Provides a generic array list. Array lists have fast direct access, but slower adding of elements
 */

typedef struct ArrayList
{
	int count;
	int capacity;
	int itemsize;
	void *array;
} ArrayList;

#define genarray(list, T, i) ((T*)(list->array))

/**
 * Creates a new array list.
 * \param	capacity	Initial capacity of the list.
 */
ArrayList *arraylist_create(int itemsize, int capacity);

/**
 * Frees an array list.
 * \param	list		The list to delete.
 */
void arraylist_delete(ArrayList *list);

#include "arraylist.c"
#endif