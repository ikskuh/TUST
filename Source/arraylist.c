#include <acknex.h>
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

int arraylist_insert_internal(ArrayList *list, int index, void *element)
{
	if(list == NULL) return -1;
	if(element == NULL) return -1;
	index = clamp(index, 0, list->count);
	
	if(list->count >= list->capacity)
	{
		//Enlarge list size with 4 elements
		void *newMem = sys_malloc(list->itemsize * (list->capacity + 4));
		memcpy(newMem, list->array, list->itemsize * list->count);
		sys_free(list->array);
		list->array = newMem;
	}
	if(index < list->count)
	{
		// Move elements to free index position
		void *memOld = list->array + index * list->itemsize;
		void *memNew = memOld + list->itemsize;
		int len = list->itemsize * (list->count - index);
		
		error(str_for_int(NULL, list->count - index));
		
		memmove(memNew, memOld, len);
	}
	void *mem = list->array + index * list->itemsize;
	memcpy(mem, element, list->itemsize);
	list->count += 1;
	return index;
}

int arraylist_add_internal(ArrayList *list, void *element)
{
	if(list == NULL) return -1;
	return arraylist_insert_internal(list, list->count, element);
}