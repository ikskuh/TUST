#include <acknex.h>
#include "list.h"

List *list_create()
{
	List *list = sys_malloc(sizeof(List));
	list->first = NULL;
	list->last = NULL;
	list->count = 0;
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
	list->count += 1;
}

void list_add_range(List *list, ListData **array, int count)
{
	int i;
	for(i = 0; i < count; i++)
	{
		list_add(list, array[i]);
		list->count += 1;
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
		list->count -= 1;
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
		list->count -= 1;
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
		list->count -= 1;
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
	list->count = 0;
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
	//int count = 0;
	//ListItem *it = list->first;
	//for(it = list->first; it != NULL; it = it->next)
	//{
	//	count++;
	//}
	return list->count;
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


ListIterator *list_begin_iterate(List *list)
{
	ListIterator *iterator = sys_malloc(sizeof(ListIterator));
	
	iterator->list = list;
	iterator->current = NULL;
	iterator->valid = 0;
	iterator->hasNext = list->first != NULL;
	return iterator;
}

ListData *list_iterate(ListIterator *iterator)
{
	if(iterator->hasNext == 0)
		return NULL;
	if(iterator->valid)
	{
		iterator->current = iterator->current->next;
		iterator->hasNext = iterator->current != NULL;
		if(iterator->current == NULL)
			return NULL;
	}
	else
	{
		iterator->current = iterator->list->first;
		iterator->valid = 1;
	}
	return iterator->current->data;
}

void list_end_iterate(ListIterator *iterator)
{
	sys_free(iterator);
}

void list_clear_content ( List *list, void *remove_function )
{
	ListItem *it = list->first;
	void itemdata_remover ( ListData *data );
	itemdata_remover = remove_function;
	for ( it = list->first; it != NULL; )
	{
		ListItem *next = it->next;
		itemdata_remover ( it->data );
		sys_free(it);		
		it = next;
	}
	list->first = NULL;
	list->last = NULL;
	list->count = 0;
}

void list_add_first ( List *list, ListData *item )
{
	ListItem *listitem = sys_malloc(sizeof(ListItem));
	listitem->previous = NULL;
	listitem->next = list->first;
	listitem->data = item;
	if ( list->first == NULL )
		list->last = listitem;
	else
		list->first->previous = listitem;
	list->first = listitem;
	list->count += 1;
}

void list_add_sorted ( List *list, ListData *item, void *compare )
{
	ListItem *newitem = sys_malloc(sizeof(ListItem));
	newitem->previous = NULL;
	newitem->next = NULL;
	newitem->data = item;
	ListItem *it = list->first;
	if ( it == NULL )
	{
		list->last = newitem;
		list->first = newitem;
		list->count += 1;
		return;
	}
	int compare_func(ListData *left, ListData *right);
	compare_func = compare;
	if ( compare_func ( it->data, item ) > 0 )
	{
		newitem->next = list->first;
		list->first->previous = newitem;
		list->first = newitem;
		list->count += 1;
		return;
	}
	for ( it = list->first; it != list->last; )
	{
		ListItem *next = it->next;
		if ( compare_func ( next->data, item ) > 0 )
		{
			newitem->previous = it;
			newitem->next = next;
			next->previous = newitem;
			it->next = newitem;
			list->count += 1;
			return;
		}
		it = next;
	}
	newitem->previous = list->last;
	list->last->next = newitem;
	list->last = newitem;
	list->count += 1;
}








