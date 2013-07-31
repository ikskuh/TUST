#include <acknex.h>
#include <default.c>
#include "..\\Source\\math.h"

VECTOR pt[4];

void draw_mark(VECTOR *pos, COLOR *col)
{
	draw_line(vector(pos.x - 1, pos.y, 0), NULL, 100);
	draw_line(vector(pos.x - 1, pos.y, 0), col, 100);
	draw_line(vector(pos.x + 2, pos.y, 0), col, 100);
	
	draw_line(vector(pos.x, pos.y - 1, 0), NULL, 100);
	draw_line(vector(pos.x, pos.y - 1, 0), col, 100);
	draw_line(vector(pos.x, pos.y + 2, 0), col, 100);
}

function on_space_event()
{
	int i;
	for(i = 0; i < 4; i++)
	{
		vec_set(&pt[i], vector(random(screen_size.x), random(screen_size.y), 0));
	}
}

function main()
{
	random_seed(123);
	wait(1);
	
	VECTOR pos;

	
	int i;
	for(i = 0; i < 4; i++)
	{
		vec_set(&pt[i], vector(random(screen_size.x), random(screen_size.y), 0));
	}
	while(1)
	{
		draw_line(&pt[0], NULL, 100);
		for(i = 1; i < 4; i++)
		{
			draw_line(&pt[i], COLOR_BLUE, 100);
		}
		
		float f;
		draw_line(&pt[0], NULL, 100);
		var i;
		for(i=0; i<3; i++)
		{
			// To get a catmull rom spline, you have to cicle between previous, current, next and next next point in the 4-vector-set
			for(f = 0; f <= 1.0; f += 0.01)
			{
				draw_line(vec_to_catmull(pos, pt[cycle(i-1,0,4)], pt[cycle(i,0,4)], pt[cycle(i+1,0,4)], pt[cycle(i+2,0,4)], f), COLOR_RED, 100);
			}
		}
		wait(1);
	}
}