#include <acknex.h>
#include "..\\Source\\tust.h"
#include "..\\Source\\lvector.h"

void main()
{
	
	VECTOR *vec = lvector(10, 10, 10);
	
	VECTOR* v = lvector(0,0,0);

	fixed i;
	for (i = 0; i < 64; i++)
		lvector(i,1,1);
	
	printf("%f",(double)v->x);
	printf("%d",(int)lvector_usage());
}