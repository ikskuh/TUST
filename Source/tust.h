#ifndef _TUST_H_
#define _TUST_H_

/**
 * \file tust.h
 * The core of the scripting library. Include this file to get all functions
 * TUST provides.
 */

typedef unsigned char bool;

#include "fs.h"
#include "ini.h"
#include "list.h"
#include "trash.h"
#include "node.h"
#include "nodemesh.h"
#include "math.h"
#include "dialogs.h"
#include "network.h"
#include "webkit.h"
#include "DynamicModels.h"
#include "noise.h"
#include "arraylist.h"

/**
 * This macro maps a varible from type a into type b without changing the bits.
 * It does the same as floatv, but not with the restriction of floats.
 * \param	variable	The variable to be converted.
 * \param	T			The target type of the conversion.
 * \return				The value of variable mapped into T.
 */
#define type_convert(variable, T) (*((T*)(&variable)))

/**
 * Returns a temporary color value. The internal values are clamped to a range between 0 and 255.
 * \param	r	The red component of the color.
 * \param	g	The green component of the color.
 * \param	b	The blue component of the color.
 * \return		A temporary pointer to the color.
 */
COLOR *colorv(var r, var g, var b);

/**
 * Returns a temporary color value. The internal values are clamped to a range between 0.0 and 1.0.
 * \param	r	The red component of the color.
 * \param	g	The green component of the color.
 * \param	b	The blue component of the color.
 * \return		A temporary pointer to the color.
 */
COLOR *colorf(float r, float g, float b);

#ifdef _TEMPLATE_H_
	#error Template include guard still exists!
#endif

#include "tust.c"
#endif