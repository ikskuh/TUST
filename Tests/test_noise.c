#include <acknex.h>
#include <default.c>
#include "..\\Source\\tust.h"

function main()
{
	wait(1);
	BMAP *bmp = bmap_createblack(256, 256, 888);
	bmap_lock(bmp, 0);
	int x, y;
	for (x = 0; x < 256; x++) for (y = 0; y < 256; y++)
	{
		int value = 127 + 128 * noise2d(x / 128.0, y / 128.0);
		pixel_to_bmap(bmp, x, y, pixel_for_vec(vector(value, value, value), 100, 888));
	}
	bmap_unlock(bmp);
	
	while(1)
	{
		draw_quad(bmp, NULL, NULL, NULL, NULL, NULL, 100, 0);
		wait(1);
	}
}





















