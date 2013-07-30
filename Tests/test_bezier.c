#include <acknex.h>
#include <default.c>
#include "..\\Source\\tust.h"

#define POINT_COUNT 5
VECTOR pt[POINT_COUNT];

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
	for(i = 0; i < POINT_COUNT; i++)
	{
		vec_set(&pt[i], vector(random(screen_size.x), random(screen_size.y), 0));
	}
}

function main()
{
	wait(1);
	
	int i;
	for(i = 0; i < POINT_COUNT; i++)
	{
		vec_set(&pt[i], vector(random(screen_size.x), random(screen_size.y), 0));
	}
	while(1)
	{
		draw_line(&pt[0], NULL, 100);
		for(i = 1; i < POINT_COUNT; i++)
		{
			draw_line(&pt[i], COLOR_BLUE, 100);
		}
		
		float f;
		draw_line(&pt[0], NULL, 100);
		for(f = 0; f <= 1; f += 0.01)
		{
			draw_line(vec_to_bezier(NULL, pt, POINT_COUNT, f), COLOR_RED, 100);
		}
		wait(1);
	}
}