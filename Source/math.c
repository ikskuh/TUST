#include <acknex.h>
#include "math.h"

var ang_lerp_single(var a1, var a2, var f)
{
	var angle = ang(a1 - a2);
	return ang(a1 - (f * angle));
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

void vec_rotateXY ( VECTOR *vec, var ang )
{
	var x = fcos(ang,vec->x) - fsin(ang,vec->y);
	vec.y = fsin(ang,vec->x) + fcos(ang,vec->y);
	vec.x = x;
}

void vec_rotateXZ ( VECTOR *vec, var ang )
{
	var x = fcos(ang,vec->x) - fsin(ang,vec->z);
	vec.z = fsin(ang,vec->x) + fcos(ang,vec->z);
	vec.x = x;
}

void vec_rotateYZ ( VECTOR *vec, var ang )
{
	var y = fcos(ang,vec->y) - fsin(ang,vec->z);
	vec.z = fsin(ang,vec->y) + fcos(ang,vec->z);
	vec.y = y;
}

var point_to_line2D_dist ( var px, var py, var l1x, var l1y, var l2x, var l2y )
{
	var varA = l2y - l1y;
	var varB = -( l2x - l1x );
	var varC = ( ( l2x - l1x ) * l1y ) - ( ( l2y - l1y ) * l1x );
	
	return ( ( varA * px ) + ( varB * py ) + varC ) / sqrt ( ( varA * varA ) + ( varB * varB ) );
}

var point_to_line2D_escalar ( var px, var py, var l1x, var l1y, var l2x, var l2y )
{
	return ( ( ( px - l1x ) * ( l2x - l1x ) ) + ( ( py - l1y ) * ( l2y - l1y ) ) ) / ( pow ( l2x - l1x, 2 ) + pow ( l2y - l1y, 2 ) );
}

