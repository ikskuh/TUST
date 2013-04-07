#include <acknex.h>
#include "bmap.h"

typedef MATERIAL BmapMaterial;

BmapMaterial *bmapmatLerp;

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
