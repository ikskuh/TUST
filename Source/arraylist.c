#include <acknex.h>
#include "arraylist.h"

ArrayList *arraylist_create(int itemsize, int capacity)
{
	if(itemsize == NULL) return NULL;
	ArrayList *list = sys_malloc(sizeof(ArrayList));
	
	list->count = 0;
	list->itemsize = itemsize;
	list->capacity = maxv(1, capacity);
	
	list->array = sys_malloc(itemsize * list->capacity);
	
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
}

int arraylist_add_internal(ArrayList *list, void *element)
{
	if(list == NULL) return -1;
	return arraylist_insert_internal(list, list->count, element);
}