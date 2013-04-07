#include <acknex.h>
#include "tust.h"

int tustCurrentColor = 0;
COLOR tustColorRepo[64];

COLOR *colorv(var r, var g, var b)
{
	tustCurrentColor++;
	tustColorRepo[tustCurrentColor % 64].red = r;
	tustColorRepo[tustCurrentColor % 64].green = g;
	tustColorRepo[tustCurrentColor % 64].blue = b;
	return &(tustColorRepo[tustCurrentColor % 64]);
}

COLOR *colorf(float r, float g, float b)
{
	return colorv(255.0f * r, 255.0f * g, 255.0f * b);
}