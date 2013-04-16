#include <acknex.h>
#include "bmap.h"

typedef MATERIAL BmapMaterial;

BmapMaterial *bmapmatLerp = NULL;
BmapMaterial *bmapmatRotate = NULL;

void bmap_lerp ( BMAP *destiny, BMAP* source1, BMAP *source2, var factor )
{
	if ( bmapmatLerp == NULL )
	{
		bmapmatLerp = mtl_create ();
		effect_load ( bmapmatLerp, "bmap_lerp.fx" );
	}
	bmapmatLerp->skill1 = floatv(factor);
	bmapmatLerp->skin1 = source2;
	bmap_process ( destiny, source1, bmapmatLerp );
}

// By HeelX aka Christian Behrenberg
void bmap_rotate ( BMAP *destiny, BMAP* source, var source_offset_x, var source_offset_y, var destiny_offset_x, var destiny_offset_y, var angle )
{
	if ( bmapmatRotate == NULL )
	{
		bmapmatRotate = mtl_create ();
		effect_load ( bmapmatRotate, "bmap_rotate.fx" );
	}
	bmapmatRotate->skill1 = floatv(cosv(angle));
	bmapmatRotate->skill2 = floatv(-sinv(angle));
	bmapmatRotate->skill3 = floatv(sinv(angle));
	bmapmatRotate->skill4 = floatv(cosv(angle));
	bmapmatRotate->skill5 = floatd(source_offset_x, bmap_width(source));
	bmapmatRotate->skill6 = floatd(source_offset_y, bmap_height(source));
	bmapmatRotate->skill7 = floatd(destiny_offset_x, bmap_width(destiny));
	bmapmatRotate->skill8 = floatd(destiny_offset_y, bmap_height(destiny));
	bmapmatRotate->skill9 = floatd(bmap_width(destiny), bmap_width(source));
	bmapmatRotate->skill10 = floatd(bmap_height(destiny), bmap_height(source));
	bmapmatRotate->skill11 = floatd(bmap_width(source), bmap_height(source));
	bmap_process ( destiny, source, bmapmatRotate );
}

// Saves a bitmap as tga file
BOOL bmap_savetga (BMAP* b, char* filename)
{
    BOOL bSuccess = false;

    if (b && filename && strlen(filename) > 0)
    {
        var fh = file_open_write(filename);
        if (fh)
        {
            int height = b->height;
            int width = b->width;

            var format = bmap_lock(b, 0);

            // if the bitmap has an alpha channel
            BOOL bAlpha = (((format == 8888) || (format == 1555) || (format == 4444)) && (b->bytespp == 4));

            // header

            int i;
            for (i = 0; i < 2; i++)
                file_asc_write(fh, 0);

            file_asc_write(fh, 2); // uncompressed

            for (i = 0; i < 7; i++)
                file_asc_write(fh, 0);

            // y origin (lower left corner)
            file_asc_write(fh, height & 255);
            file_asc_write(fh, (height >> 8) & 255);

            file_asc_write(fh, width & 255);
            file_asc_write(fh, (width >> 8) & 255);

            file_asc_write(fh, height & 255);
            file_asc_write(fh, (height >> 8) & 255);
            
            int bypp = 24;
            if (bAlpha)
                bypp = 32;

            file_asc_write(fh, bypp); // bypp
            file_asc_write(fh, 0);

            // pixels

            COLOR bgr;
            var alpha;

            int ix, iy;
            for (iy = 0; iy < height; iy++)
            {
                for (ix = 0; ix < width; ix++)
                {
                    // retrieve BGR color and alpha
                    pixel_to_vec(&bgr, &alpha, format, pixel_for_bmap(b, ix, height - 1 - iy));

                    file_asc_write(fh, bgr.blue); // b
                    file_asc_write(fh, bgr.green); // g
                    file_asc_write(fh, bgr.red); // r

                    if (bAlpha)
                        file_asc_write(fh, alpha * 2.55); // a
                }
            }

            // done!
            bmap_unlock(b);
            file_close(fh);

            bSuccess = true;
        }
    }

    return(bSuccess);
}
