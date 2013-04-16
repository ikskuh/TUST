#include <acknex.h>
#include "tust.h"
#include "arraylist.h"

ArrayList *arraylist_create(int itemsize, int capacity)
{
	if(itemsize == NULL) return NULL;
	ArrayList *list = sys_malloc(sizeof(ArrayList));
	
	list->count = 0;
	list->itemsize = itemsize;
	list->capacity = maxv(4, capacity);
	
	list->array = sys_malloc(list->itemsize * list->capacity);
	
	return list;
}

void arraylist_delete(ArrayList *list)
{
	if(list == NULL) return;
	sys_free(list->array);
	sys_free(list);
}

void *arraylist_get_mem_offset(ArrayList *list, int index)
{
	return (void*)((int)list->array + index * list->itemsize);
}

int arraylist_insert_internal(ArrayList *list, int index, void *element)
{
	if(list == NULL) return -1;
	if(element == NULL) return -1;
	index = clamp(index, 0, list->count);
	
	if(list->count >= list->capacity)
	{
		error("enlarge!");
		//Enlarge list size with 4 elements
		void *newMem = sys_malloc(list->itemsize * (list->capacity + 4));
		memcpy(newMem, list->array, list->itemsize * list->count);
		sys_free(list->array);
		list->array = newMem;
	}
	if(index < list->count)
	{
		// Move elements to free index position
		void *memOld = arraylist_get_mem_offset(list, index);
		void *memNew = arraylist_get_mem_offset(list, index + 1);
		int len = list->itemsize * (list->count - index);
		
		memmove(memNew, memOld, len);
	}
	
	
	// Copy new element
	void *mem = arraylist_get_mem_offset(list, index);
	memcpy(mem, element, list->itemsize);
	list->count += 1;
	return index;
}

int arraylist_add_internal(ArrayList *list, void *element)
{
	if(list == NULL) return -1;
	return arraylist_insert_internal(list, list->count, element);
}

void arraylist_remove_at_internal(ArrayList *list, int index)
{
	if(list == NULL) return;
	if(index < 0) return;
	if(index >= list->count) return;
	
	if(index < list->count)
	{		
		// Move elements one element to the begin of the list
		int length = list->itemsize * (list->count - index - 1);
		memmove(
			arraylist_get_mem_offset(list, index),
			arraylist_get_mem_offset(list, index + 1),
			length);
	}
	
	// Reset the last position
	list->count -= 1;
	void *mem = (int)list->array + (list->count * list->itemsize);
	memset(mem, 0, list->itemsize);
}

void arraylist_clear(ArrayList *list)
{
	if(list == NULL) return;
	memset(list->array, 0, list->itemsize * list->count);
	list->count = 0;
}







//add_range
//sort
//add sorted
//clear
//get_count
//get_capacity
//copy_to
//reverse















