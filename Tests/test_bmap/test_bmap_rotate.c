#include <acknex.h>
#define PRAGMA_PATH "..\\..\\Source\\Shaders"
#include "..\\..\\Source\\tust.h"

function main ()
{
	video_mode = 12;
	wait(1);
	
	vec_fill ( &screen_color, 128 );
	BMAP *destiny = bmap_createblack ( 512, 256, 32 );
	BMAP *source = bmap_create ( "b3.tga" );
	
	var Speed = 55;
	var angle = 0;
	while ( !key_esc )
	{
		if ( Speed > 0 )
		{
			Speed -= time_step * 0.7;
			angle += time_step * Speed;
			bmap_rotate ( destiny, source, 64, 64, fcos(angle*0.1,64), -fsin(angle*0.1,64), angle );
		}
		DEBUG_BMAP ( source, 0, 1 );
		DEBUG_BMAP ( destiny, 300, 1 );
		DEBUG_VAR ( angle, 300 );
		wait(1);
	}
	
	sys_exit(NULL);
}