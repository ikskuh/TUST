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
	return colorv(255.0 * r, 255.0 * g, 255.0 * b);
}

void *memmove(BYTE *destination, BYTE *source, unsigned int num)
{
	// Nothing to do here!
	if(num <= 0) return destination;
	if(destination == source) return destination;

	if(abs((int)destination - (int)source) >= num)
	{
		// No overlapping
		memcpy(destination, source, num);
		return destination;
	}
	
	if(destination < source)
	{
		while(num > 0)
		{
			*destination = *source;
			destination++;
			source++;
			num--;
		}
	}
	else
	{
		destination += num - 1;
		source += num - 1;
		while(num > 0)
		{
			*destination = *source;
			destination--;
			source--;
			num--;
		}
	}
	
	return destination;
} 

VECTOR *vec_create(var x, var y, var z)
{
	VECTOR *vec = sys_malloc(sizeof(VECTOR));
	vec->x = x;
	vec->y = y;
	vec->z = z;
	return vec;
}

void draw_font(STRING* text, var x, var y, COLOR* color, FONT *font, int flags, var alpha)
{
	static TEXT *txt = NULL;
	if(txt == NULL)
		txt = txt_create(1, -1);
	
	flags = flags & (CENTER_X | CENTER_Y | FILTER | ARIGHT | CONDENSED | SHADOW | OUTLINE);
	
	txt->pos_x = x;
	txt->pos_y = y;
	txt->flags = flags | LIGHT | TRANSLUCENT;
	txt->alpha = alpha;
	txt->font = font;
	(txt->pstring)[0] = text;
	if(color != NULL)
		vec_set(&txt->blue, color);
	
	draw_obj(txt);
}


