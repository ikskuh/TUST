#include <acknex.h>

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

// ---------------------------------------------------------------------------------
// Splines
// ---------------------------------------------------------------------------------

static double U[50];

double _spline_calc(int _k, int _t, double _u) {
	double firstterm, secondterm;
	
	if (_t == 1) {
		if (U[_k] <= _u && _u < U[_k+1]) {
			return 1.0;
		} else {
			return 0.0;
		}
	}
	
	if (U[_k+_t-1]-U[_k] < 1.0e-10) {
		firstterm = 0.0;
	} else {
		firstterm = ((_u-U[_k])/(U[_k+_t-1]-U[_k]))*_spline_calc(_k,_t-1,_u);
	}
	
	if (U[_k+_t]-U[_k+1] < 1.0e-10) {
		secondterm = 0.0;
	} else {
		secondterm = ((U[_k+_t]-_u)/(U[_k+_t]-U[_k+1]))*_spline_calc(_k+1,_t-1,_u);
	}
	return firstterm + secondterm;
}

List* calculate_spline(List* _points,  int _detail) {
	if (_points == NULL) return NULL;
	if (list_get_count(_points) == 1) return _points;
	if (list_get_count(_points) == 2) return _points;
	
	List* results = list_create();
	
	int t=4;
	int n=list_get_count(_points)-1;
	int m=_detail;
	
	int j,k;
	
	double temp, u;
	

	for (j = 0; j <= n + t; j++) {
		if (j < t) {
			U[j] = 0.0;
		}else {
			if (t <= j && j <= n) {
				U[j] = j-t+1;
			} else {
				U[j] = n-t+2;
			}
		}
	}
	
	for (j = 0; j <= m; j++) {
		u = ((double)j / m)*(n-t+2-.00000001);
		VECTOR* vecCurveTemp = sys_malloc(sizeof(VECTOR));
		vecCurveTemp->x = 0; vecCurveTemp->y = 0; vecCurveTemp.z = 0;
		
		for (k = 0; k <= n; k++) {
			temp = _spline_calc(k, t, u);
			
			VECTOR* vecPointTemp = list_item_at(_points, k);
			
			vecCurveTemp->x += vecPointTemp->x * temp;
			vecCurveTemp->y += vecPointTemp->y * temp;
			vecCurveTemp->z += vecPointTemp->z * temp;
		}
		list_add(results, vecCurveTemp);
	}
	
	list_clear(U);
	list_delete(U);
	return results;
}

