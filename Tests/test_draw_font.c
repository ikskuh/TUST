#include <acknex.h>
#include <default.c>
#include "..\\Source\\tust.h"

FONT *font = "Arial#30b";

void main()
{
	vec_set(screen_color, vector(255, 0, 0));
	wait(1);
	while(1)
	{
		draw_font("Hello World", 16, 16, vector(0, 255, 0), font, SHADOW, 100);
		wait(1);
	}
}