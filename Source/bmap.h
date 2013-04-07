#ifndef _BMAP_H_
#define _BMAP_H_

/**
 * Interpolates two bitmaps by a factor. Bitmap size independant.
 * \param	destiny		A bitmap to contain the interpolation.
 * \param	source1		First bitmap to interpolate.
 * \param	source2		Second bitmap to interpolate.
 * \param	factor		The factor of the interpolation.
 */
void bmap_lerp ( BMAP *destiny, BMAP* source1, BMAP *source2, var factor );


#include "bmap.c"
#endif



