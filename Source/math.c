#include <acknex.h>
#include "math.h"

var ang_lerp_single(var a1, var a2, var f)
{
	var angle = ang(a2 - a1);
	return ang(a2 - (f * angle));
}

ANGLE* ang_lerp(ANGLE* a, ANGLE* a1, ANGLE* a2, var f)
{
	ANGLE ai;
	ai.pan = ang_lerp_single(a1->pan, a2->pan, f);
	ai.tilt = ang_lerp_single(a1->tilt, a2->tilt, f);
	ai.roll = ang_lerp_single(a1->roll, a2->roll, f);
	
	if(a != NULL)
		vec_set(a, &ai);
	
	return vector(ai.pan, ai.tilt, ai.roll);
}