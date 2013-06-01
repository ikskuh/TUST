#include <acknex.h>
#include <default.c>

#include "..\\Source\\math.h"

void main() {
	level_load("");
	random_seed(0);
	vec_set(sky_color.blue, vector(8,8,8));
	
	List* points = list_create();
	int i;
	for(i=0; i<20; i++)
	{
		VECTOR* vecTemp = sys_malloc(sizeof(VECTOR));
		vec_set(vecTemp, vector(random(screen_size.x), random(screen_size.y), 0));
		list_add(points, vecTemp);
	}
	
	
	VECTOR splineData[20];
	VECTOR* value;
	ListIterator *it = list_begin_iterate(points);
	for(value = list_iterate(it), i = 0; it->hasNext; value = list_iterate(it))
	{
		vec_set(splineData[i], value);
		i++;
	} 
	list_end_iterate(it);
	
	List* splines = calculate_spline(points,  200);
	
	VECTOR* v1 = NULL;
	
	while(1) {
		
		draw_text("a - Draw points", 10, 10, COLOR_RED);
		draw_text("d - Draw spline", 10, 30, COLOR_BLUE);
		draw_text("s - Draw spline interpolation", 10, 50, COLOR_GREEN);
		
		if (key_a) {
			// Draw points
			v1 = list_item_at(points, 0);
			draw_line(v1, NULL, 100);
			for(i=1; i<list_get_count(points)-1; i++) {
				
				v1 = list_item_at(points, i);
				draw_line(v1, COLOR_RED, 100);
			}
		}

		if (key_d) {
			// Draw spline
			v1 = list_item_at(splines, 0);
			draw_line(v1, NULL, 100);
			
			for(i=1; i<list_get_count(splines)-1; i++) {
				v1 = list_item_at(splines, i);
				draw_line(v1, COLOR_BLUE, 100);
			}
		}

		if (key_s) {
			// Draw spline
			draw_line(splineData[0], NULL, 100);
			
			float f;
			for(f = 0; f < 1; f += 0.001)
			{
				VECTOR *pos = math_get_spline(splineData, 20, f);
				if(pos == NULL)
					error("??");
				draw_line(pos, COLOR_GREEN, 100);
			}
		}
		wait(1);
	}
}