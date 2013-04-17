#ifndef _BMAP_H_
#define _BMAP_H_


/**
 * \file bmap.h
 * \brief Additional bitmap functions.
 * 
 * bmap.h provides a set of new bmap_* functions.
 */

/**
 * Interpolates two bitmaps by a factor. Bitmap size independant.
 * \param	destiny		A bitmap to contain the interpolation.
 * \param	source1		First bitmap to interpolate.
 * \param	source2		Second bitmap to interpolate.
 * \param	factor		The factor of the interpolation.
 */
void bmap_lerp ( BMAP *destiny, BMAP* source1, BMAP *source2, var factor );

/**
 * Draws a rotated bitmap over another bitmap
 * \param	destiny				A bitmap to contain the rotated bitmap.
 * \param	source				The bitmap to be rotated.
 * \param	source_offset_x	Offset from the source center
 *				source_offset_y	
 * \param	destiny_offset_x	Offset from the destiny center
 *				destiny_offset_y	
 * \param	angle					The amount of rotation.
 */
void bmap_rotate ( BMAP *destiny, BMAP* source, var source_offset_x, var source_offset_y, var destiny_offset_x, var destiny_offset_y, var angle );

BOOL bmap_savetga (BMAP* b, char* filename);

#include "bmap.c"
#endif



