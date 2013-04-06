#include <acknex.h>
#include <default.c>
#include "..\\Source\\tust.h"

function main()
{
	ArrayList *list = arraylist_create(sizeof(int), 15);	// Initial capacity of 15 elements
	
	genarray(list, int)[0] = 1;
	error(str_for_int(NULL, genarray(list, int)[0]));
	
	arraylist_delete(list);
}





















