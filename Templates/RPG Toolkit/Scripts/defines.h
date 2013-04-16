// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 180510
//
// title: Definitions Code
// class: Defines
// type: Defines
// help: Handles the engine variables, and some Global variables
//
// prefix: DEF_
// idcode: 2
//
// ENDHEADER
// ---------------------------------------------------------------------

// Global variables/defines
// Debug mode, 0=no debugging, 1=debug values ingame
#define DEF_DEBUG 0

// gameplay modes definitions
#define DEF_GAMEPLAY_MOVEMENT 1
#define DEF_GAMEPLAY_DIALOGUE 2
#define DEF_GAMEPLAY_CUTSCENE 3

// functions

// returns smallest angle (used for rotating player on closest rotation angle)
// function borrowed from RUDI open-source project
VECTOR *ang_lerp(VECTOR* resultVec, VECTOR* srcVec, VECTOR* destVec, var blendFactor)
{
	VECTOR ANG_LERP_VEC; // calculation vector
	vec_set(ANG_LERP_VEC, nullvector);
	
	vec_set(resultVec, srcVec);						// Copy source into result
	vec_diff(ANG_LERP_VEC, destVec, resultVec);  // Difference vector

	ANG_LERP_VEC.x = ang(ANG_LERP_VEC.x);			// Adjust angle validity
	ANG_LERP_VEC.y = ang(ANG_LERP_VEC.y);
	ANG_LERP_VEC.z = ang(ANG_LERP_VEC.z);

	vec_scale(ANG_LERP_VEC.x, blendFactor);		// Interpolate the rotation
	vec_add(resultVec, ANG_LERP_VEC);				// Apply interpolated angle

	return(resultVec);									// Return result angle
}