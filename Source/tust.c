#include <acknex.h>
#include "tust.h"

int tustCurrentColor = 0;
COLOR tustColorRepo[64];

COLOR *colorv(var r, var g, var b)
{
	tustCurrentColor++;
	tustColorRepo[tustCurrentColor % 64].red = clamp(r, 0, 255);
	tustColorRepo[tustCurrentColor % 64].green = clamp(g, 0, 255);
	tustColorRepo[tustCurrentColor % 64].blue = clamp(b, 0, 255);
	return &(tustColorRepo[tustCurrentColor % 64]);
}

COLOR *colorf(float r, float g, float b)
{
	return colorv(255.0f * r, 255.0f * g, 255.0f * b);
}