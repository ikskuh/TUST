#ifndef _TEMPLATE_H_
#define _TEMPLATE_H_

/**
 * \file math.h
 * Provides math helpers and useful functions.
 */

/**
 * Interpolates a single angle.
 * \param	a1	The first angle.
 * \param	a2	The second angle.
 * \param	f	The interpolation factor between a1 and a2.
 * \return		Interpolated angle.
 */
var ang_lerp_single(var a1, var a2, var f);

/**
 * Interpolates an euler angle.
 * \param	a1	The first angle.
 * \param	a2	The second angle.
 * \param	f	The interpolation factor between a1 and a2.
 * \return		Interpolated angle.
 */
ANGLE* ang_lerp(ANGLE* a, ANGLE* a1, ANGLE* a2, var f);

#include "math.c"
#endif