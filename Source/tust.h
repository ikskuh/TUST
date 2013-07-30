
/* The Ultimate Script Library 
-----------------------------------------------------------------------------------
   Script Library Core

   Read the documentation for further information: TUST\Documentation\index.html
----------------------------------------------------------------------------------- 
*/

#ifndef _TUST_H_
#define _TUST_H_

// 3DGS STRUCTS FOR DOXYGEN
/**
 * \typedef fixed var
 * \brief var is defined in GStudio8\include\acknex.h
 * \typedef fixed function
 * \brief function is defined in GStudio8\include\acknex.h
 * \typedef void action
 * \brief action is defined in GStudio8\include\acknex.h
 * \typedef long BOOL
 * \brief BOOL is defined in GStudio8\include\litec.h
 * \typedef char BYTE
 * \brief BYTE is defined in GStudio8\include\litec.h
 * \class VECTOR
 * \brief The VECTOR struct is defined in GStudio8\include\atypes.h
 * \class ANGLE
 * \brief The ANGLE struct is defined in GStudio8\include\atypes.h
 * \class COLOR
 * \brief The COLOR struct is defined in GStudio8\include\atypes.h
 * \class COLOR4
 * \brief The COLOR4 struct is defined in GStudio8\include\atypes.h
 * \class EVENT
 * \brief The EVENT struct is defined in GStudio8\include\atypes.h
 * \class STRING
 * \brief The STRING struct is defined in GStudio8\include\atypes.h
 * \class SOUND
 * \brief The SOUND struct is defined in GStudio8\include\atypes.h
 * \class BMAP
 * \brief The BMAP struct is defined in GStudio8\include\atypes.h
 * \class D3DVERTEX
 * \brief The D3DVERTEX struct is defined in GStudio8\include\atypes.h
 * \class MATERIAL
 * \brief The MATERIAL struct is defined in GStudio8\include\atypes.h
 * \class ENTITY
 * \brief The ENTITY struct is defined in GStudio8\include\atypes.h
 * \class PARTICLE
 * \brief The PARTICLE struct is defined in GStudio8\include\atypes.h
 * \class CONTACT
 * \brief The CONTACT struct is defined in GStudio8\include\atypes.h
 * \class FONT
 * \brief The FONT struct is defined in GStudio8\include\atypes.h
 * \class TEXT
 * \brief The TEXT struct is defined in GStudio8\include\atypes.h
 * \class PANEL
 * \brief The PANEL struct is defined in GStudio8\include\atypes.h
 * \class VIEW
 * \brief The VIEW struct is defined in GStudio8\include\atypes.h
 */

/**
 * \defgroup GenericScriptLibrary Generic Script Library
 * \brief Generic script library
 *
 * \defgroup GameTemplates Game Templates
 * \brief Game template sets
 *
 * \defgroup ScriptLibraryCore Script Library Core
 * \brief The core of the scripting library. 
 * \{
 *
 * \file tust.h
 * \brief Script library core header.
 * 
 * Include this file to get all functions TUST provides.
 *
 * \file tust.c
 * \brief Script library core code.
 */

/**
 * Typedef of a boolean type.
 */
typedef unsigned char bool;

/**
 * \brief This macro maps a varible from type a into type b without changing the bits.
 * 
 * It does the same as floatv, but not with the restriction of floats.
 *
 * \param   variable  The variable to be converted.
 * \param   T         The target type of the conversion.
 * \return            The value of variable mapped into T.
 */
#define type_convert(variable, T) (*((T*)(&variable)))

/**
 * \brief Returns a temporary color value. 
 *
 * The internal values are clamped to a range between 0 and 255.
 *
 * \param	r	The red component of the color.
 * \param	g	The green component of the color.
 * \param	b	The blue component of the color.
 * \return		A temporary pointer to the color.
 */
COLOR *colorv(var r, var g, var b);

/**
 * \brief Returns a temporary color value. 
 *
 * The internal values are clamped to a range between 0.0 and 1.0.
 *
 * \param	r	The red component of the color.
 * \param	g	The green component of the color.
 * \param	b	The blue component of the color.
 * \return		A temporary pointer to the color.
 */
COLOR *colorf(float r, float g, float b);

/**
 * \brief Move block of memory.
 *
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

/**
 * \brief Creates a new vector and allocates memory for it.
 *
 * \warning Needs to be freed.
 * \param	x				X Value.
 * \param	y				Y Value.
 * \param	z				Z Value.
 * \return					Newly allocated VECTOR.
 */
VECTOR *vec_create(var x, var y, var z);

/**
 * \brief Sets all values of an angle between 0 and 360
 * \param   _ang        Then ANGLE to be "normalized"
 */
void ang_normalize(ANGLE* _ang);

/**
 * \brief Writes a string at the given XY screen position in the current frame, using a Gamestudio FONT.
 * \param text  The string to be written
 * \param x     The screen position in pixels from the left border
 * \param y     The screen position in pixels from the top border
 * \param color A BGR color vector that determines the text color
 * \param font  A FONT object that determines the font.
 * \param flags Flags to change the behaviour of draw_font: CENTER_X, CENTER_Y, FILTER, ARIGHT, CONDENSED, SHADOW, OUTLINE
 * \param alpha The alpha value of the text.
 */
void draw_font(STRING* text, var x, var y, COLOR* color, FONT *font, int flags, var alpha);


/**
 * \}
 */
 
#include "fs.h"
#include "ini.h"
#include "list.h"
#include "math.h"
#include "dialogs.h"
#include "network.h"
#include "webkit.h"
#include "DynamicModels.h"
#include "noise.h"
#include "bmap.h"
#include "camera.h"
#include "lvector.h"
#include "state_machine.h"
#include "nodemesh.h"
//#include "compact_menu.h"
#include "trash.h"
#include "function_timer.h"
#include "animator.h"

#ifdef _TEMPLATE_H_
	#error Template include guard still exists!
#endif

#include "tust.c"
#endif