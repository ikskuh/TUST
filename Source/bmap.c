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
