#ifndef _NOISE_H_
#define _NOISE_H_

/**
 * \file noise.h
 * Provides an implementation of simplex noise.
 */

/**
 * Creates a noise value.
 * \param	x	Noise param.
 * \return		Noise value [-1;1] depening on the noise param.
 */
float noise1d(float x);

/**
 * Creates a noise value.
 * \param	x	First noise param.
 * \param	y	Second noise param.
 * \return		Noise value [-1;1] depening on the noise params.
 */
float noise2d(float x, float y);

/**
 * Creates a noise value.
 * \param	x	First noise param.
 * \param	y	Second noise param.
 * \param	z	Third noise param.
 * \return		Noise value [-1;1] depening on the noise params.
 */
float noise3d(float x, float y, float z);

#include "noise.c"
#endif