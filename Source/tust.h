#ifndef _TUST_H_
#define _TUST_H_

/**
 * \file tust.h
 * \brief Script library core.
 *
 * The core of the scripting library. Include this file to get all functions
 * TUST provides.
 */

/**
 * Typedef of a boolean type.
 */
typedef unsigned char bool;

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

/**
 * \brief Move block of memory.
 * Copies the values of num bytes from the location pointed by source to the memory block pointed by destination. Copying takes place as if an intermediate buffer were used, allowing the destination and source to overlap.
 * The underlying type of the objects pointed by both the source and destination pointers are irrelevant for this function; The result is a binary copy of the data.
 * The function does not check for any terminating null character in source - it always copies exactly num bytes.
 * To avoid overflows, the size of the arrays pointed by both the destination and source parameters, shall be at least num bytes.
 * 
 * \param	destination		Pointer to the destination array where the content is to be copied, type-casted to a pointer of type void*.
 * \param	source			Pointer to the source of data to be copied, type-casted to a pointer of type void*.
 * \param	num				Number of bytes to copy.
 * \return					destination is returned.
 * \remarks Copied from http://www.cplusplus.com/reference/cstring/memmove/
 */
void *memmove(BYTE *destination, BYTE *source, unsigned int num);

#include "fs.h"
#include "ini.h"
#include "list.h"
#include "trash.h"
#include "math.h"
#include "dialogs.h"
#include "network.h"
#include "webkit.h"
#include "DynamicModels.h"
#include "noise.h"
#include "bmap.h"
#include "camera.h"

#ifdef _TEMPLATE_H_
	#error Template include guard still exists!
#endif

#include "tust.c"
#endif