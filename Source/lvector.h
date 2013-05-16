#ifndef _LVECTOR_H_
#define _LVECTOR_H_

/**
 * \file lvector.h
 * \brief Provides a improved version of the vector function.
 */

/**
 * Returns a temporary vector that is valid at least one frame.
 * \param	x	X component
 * \param	y	Y component
 * \param	z	Z component
 */
VECTOR *lvector(var x, var y, var z);

/**
 * Returns the current memory used by lvector.
 */
int lvector_usage();

#include "lvector.c"
#endif