#include <acknex.h>
#include "list.h"

List *list_create()
{
	List *list = sys_malloc(sizeof(List));
	list->first = NULL;
	list->last = NULL;
	return list;
}

void list_delete(List *list)
{
	// Free all items.
	list_clear(list);
	sys_free(list);
}

List *list_clone(List *list)
{
	List *clone = list_create();
	
	// Just iterate through all items and add them to the new list.
	ListItem *it = list->first;
	for(it = list->first; it != NULL; it = it->next)
	{
		list_add(clone, it->data);
	}
	
	return clone;
}

void list_add(List *list, ListData *data)
{
	ListItem *item = sys_malloc(sizeof(ListItem));
	
	// Add at the end of the list.
	item->previous = list->last;
	item->next = NULL;
	item->data = data;
	
	if(list->first == NULL)
	{
		// Set as first if list is empty.
		list->first = item;
	}
	else
	{
		// Else append to the last item.
		list->last->next = item;
	}
	list->last = item;
}

void list_add_range(List *list, ListData **array, int count)
{
	int i;
	for(i = 0; i < count; i++)
	{
		list_add(list, array[i]);
	}
}

void list_remove(List *list, ListData *data)
{
	ListItem *it = list->first;
	for(it = list->first; it != NULL; it = it->next)
	{
		if(it->data != data)
			continue;
		ListItem *previous = it->previous;
		ListItem *next = it->next;
		
		if(previous != NULL)
		{
			previous->next = next;
		}
		else
		{
			list->first = next;
		}
		
		if(next != NULL)
		{
			next->previous = previous;
		}
		else
		{
			list->last = previous;
		}
		sys_free(it);
		return;
	}
}

void list_remove_all(List *list, ListData *data)
{
	ListItem *it = list->first;
	for(it = list->first; it != NULL; )
	{
		if(it->data != data)
		{
			it = it->next;
			continue;
		}
		ListItem *previous = it->previous;
		ListItem *next = it->next;
		
		if(previous != NULL)
		{
			previous->next = next;
		}
		else
		{
			list->first = next;
		}
		
		if(next != NULL)
		{
			next->previous = previous;
		}
		else
		{
			list->last = previous;
		}
		sys_free(it);
		it = next;
	}
}

void list_remove_at(List *list, int index)
{
	int i = 0;
	ListItem *it = list->first;
	for(it = list->first; it != NULL; it = it->next)
	{
		if(i != index)
		{
			i++;
			continue;
		}
		ListItem *previous = it->previous;
		ListItem *next = it->next;
		
		if(previous != NULL)
		{
			previous->next = next;
		}
		else
		{
			list->first = next;
		}
		
		if(next != NULL)
		{
			next->previous = previous;
		}
		else
		{
			list->last = previous;
		}
		sys_free(it);
		return;
	}
}

int list_contains(List *list, ListData *item)
{
	ListItem *it = list->first;
	for(it = list->first; it != NULL; it = it->next)
	{
		if(it->data != item)
			continue;
		return 1;
	}
	return 0;
}

ListData *list_item_at(List *list, int index)
{
	int i = 0;
	ListItem *it = list->first;
	for(it = list->first; it != NULL; it = it->next)
	{
		if(i != index)
		{
			i++;
			continue;
		}
		return it->data;
	}
	return NULL;
}

void list_clear(List *list)
{
	ListItem *it = list->first;
	for(it = list->first; it != NULL; )
	{
		ListItem *next = it->next;
		sys_free(it);		
		it = next;
	}
	list->first = NULL;
	list->last = NULL;
}

void list_sort(List *list, void *compare)
{
	int swapped = 0;
	do
	{
		swapped = 0;
		ListItem *it;
		for(it = list->first; it != list->last; it = it->next)
		{
			int compare_func(ListData *left, ListData *right);
			compare_func = compare;
			if(compare_func(it->data, it->next->data) > 0)
			{
				/* swap them and remember something changed */
				ListData *data2 = it->next->data;
				it->next->data = it->data;
				it->data = data2;
				swapped = 1;
			}
		}
	}
	while(swapped != 0);
}

void list_reverse(List *list)
{
	ListItem *it = list->first;
	for(it = list->first; it != NULL; )
	{
		ListItem *next = it->next;
		ListItem *prev = it->previous;
		it->previous = it->next;
		it->next = prev;
		it = next;
	}
	ListItem *last = list->last;
	list->last = list->first;
	list->first = last;
}

int list_get_count(List *list)
{
	int count = 0;
	ListItem *it = list->first;
	for(it = list->first; it != NULL; it = it->next)
	{
		count++;
	}
	return count;
}

int list_copy_to(List *list, ListData **array, int arrayLength)
{
	int count = 0;
	ListItem *it = list->first;
	for(it = list->first; it != NULL; it = it->next)
	{
		if(count >= arrayLength)
			return count;
		array[count] = it->data;
		count++;
	}
	return count;
}



