#include <acknex.h>
#include <default.c>
#include "..\\Source\\tust.h"

char* str = "memmove can be very useful......";

function main()
{
	ArrayList *list = arraylist_create(sizeof(int), 15);	// Initial capacity of 15 elements
	
	//genarray(list, int)[0] = 1;
	
	arraylist_add(list, 15);
	
	error(str_for_int(NULL, genarray(list, int)[0]));
	
	
	
	arraylist_delete(list);
}





















