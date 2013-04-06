#include <acknex.h>
#include "arraylist.h"

ArrayList *arraylist_create(int itemsize, int capacity)
{
	ArrayList *list = sys_malloc(sizeof(ArrayList));
	
	list->count = 0;
	list->itemsize = itemsize;
	list->capacity = capacity;
	
	list->array = sys_malloc(itemsize * capacity);
	
	return list;
}

void arraylist_delete(ArrayList *list)
{
	sys_free(list->array);
	sys_free(list);
}