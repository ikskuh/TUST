#include <acknex.h>
#include <default.c>
#include "..\\Source\\tust.h"

char* str = "memmove can be very useful......";

function main()
{
	ArrayList *list = arraylist_create(sizeof(int), 15);	// Initial capacity of 15 elements
	
	arraylist_add(list, 30);
	arraylist_add(list, 45);
	arraylist_insert(list, 0, 15);
	
	int i;
	for(i = 0; i < list->count; i++)
	{
		error(str_for_int(NULL, genarray(list, int)[i]));
	}
	
	arraylist_delete(list);
}





















