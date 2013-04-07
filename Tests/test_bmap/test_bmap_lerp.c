#include <acknex.h>
#define PRAGMA_PATH "..\\..\\Source\\Shaders"
#include "..\\..\\Source\\tust.h"

function main ()
{
	video_mode = 12;
	wait(1);
	
	vec_fill ( &screen_color, 128 );
	BMAP *destiny = bmap_createblack ( 256, 512, 24 );
	BMAP *source1 = bmap_create ( "b1.tga" );
	BMAP *source2 = bmap_create ( "b2.tga" );
	
	wait(5);
	while (!key_esc)
	{
		var factor = cycle(total_ticks/16, -1, 2);
		bmap_lerp ( destiny, source1, source2, factor );
		DEBUG_BMAP ( source1, 0, 1 );
		DEBUG_BMAP ( source2, 300, 1 );
		DEBUG_BMAP ( destiny, 900, 1 );
		DEBUG_VAR ( factor, 300 );
		wait(1);
	}
	sys_exit(NULL);
}