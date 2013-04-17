// CLTEXT.H ////////////////////////////////////////////////////////////////////

#ifndef __CLTEXT_H
#define __CLTEXT_H

/**
 * \file cltext.h
 * \brief Colourful TEXT objects.
 *
 * cltext.h provides COLOURFUL TEXT
 * 
 *  author: lemming
 *  version: 1.1
 *  version history:
 *      1.0: initial release
 *      1.1: * modified for TUST (mainly doxygen commenting and splitting file)
 *           + cltext_align added
 *
 *
 * -----------------------------------------------------------------------------
 *
 *  This library lets you create a CLTEXT-object, that holds several TEXT-
 *  objects with different colours and arranges them so they look as a single
 *  TEXT. This is done by parsing a tagged string.
 *  example: "[clff0000]red [clffff00]yellow [cl177935] dark green"
 *  The RGB-values are as hexcodes inside the text. The text following the tags
 *  will be coloured.
 *
 *  Features:
 *    - word warapping
 *    - indentation after word wrapping
 *    - aligning freely between left and right
 */

#include <strio.c>


//////////////////////////
// Structs and Prototypes

/**
 * Struct holding one of the TEXT-segments that form the text.
 */
typedef struct CLLIST {
	TEXT* 	element;
	var 		width;
	var 		linex;
	var 		liney;
	struct 	CLLIST* 	next;
} CLLIST;

/**
 * Struct holding the CLTEXT data.
 */
typedef struct CLTEXT {
	STRING* 	label;
	CLLIST* 	texts;
	FONT*		font;
	var 		indent;
	var 		sizex;
	var 		flags;
	
	var 		layer;
	var 		posx;
	var 		posy;
} CLTEXT;


/**
 * factor to align text left.
 */
#define CLTEXT_ALIGN_LEFT		0

/**
 * factor to align text centered.
 */
#define CLTEXT_ALIGN_CENTER	0.5

/**
 * factor to align text right
 */
#define CLTEXT_ALIGN_RIGHT		1


// for internal use:

/**
 * [Internal] Validates a CL-Tag
 * \param text		The string that holds the tag to check.
 * \param pos 		The position of the tag in text.
 * \return 			0 for valid, otherwise non zero.
 */
var 	cltext_valid_cltag(STRING* text, var pos);

/**
 * [Internal] Searches for a colour-tag,
 * \param text		String to search in.
 * \param from 		Position to start from.
 * \return			Position or <0.
 */
var 	cltext_search_tag(STRING* text, var from);

/**
 * [Internal] Searches for spaces backwards.
 * \param str		String to search in.
 * \param from		Position to start, counted from start.
 * \return 			Position from start or 0
 */
var 	cltext_search_space_backw(STRING* str, var from);



// for external use:

/**
 * Converts 2 digit Hex-Strings into var
 * \param hexs		Hex-String to convert
 * \param i			pointer to variable to hold the result (if any)
 * \return 			0 success, -1 no string, -2 too long string, -3 invalid char
 */
var 	cltext_hextovar(STRING* hexs, var* i);

/**
 * Returns the width of a string including trailing spaces, unlike str_width.
 * \param str 		The string to measure.
 * \param font 		The font to use when measuring.
 * \return 			The width of the string with trailing spaces.
 */
var 	str_width_hack(STRING* str, FONT* font);

/**
 * Removes tags from a string. For example when using it with user input.
 * \param str 		String to clean.
 * \return 			count of removed tags
 */
var 	cltext_removetags(STRING* str);

/**
 * Creates a CLTEXT object and parses the string.
 * \param text 		colourtagged text to display as a string
 * \param font 		display text with this font
 * \param indent 	after a linefeed use this indent
 * \param sizey 	max width for text. Will word wrap when reaching it.
 * \param flags 	not used, but will be written to the struct and can be read
 * \param dfltclr 	colour to use when text has no colour-tags
 * \return 			CLTEXT Pointer or NULL if failed (like when sizey < indent)
 */
CLTEXT* cltext_create(STRING* text, FONT* font, var indent, var sizey, var flags, COLOR* defaultcolor);

/**
 * Returns the height of a CLTEXT. This needs a font to be set.
 * \param cltext 	CLTEXT data
 * \return 			> 0 for height or 0 if no font is set.
 */
var 	cltext_getheight(CLTEXT* cltext);

/**
 * Removes a CLTEXT object from memory.
 * \param cltext 	CLTEXT to remove
 * \return 			NULL
 */
void* 	cltext_remove(CLTEXT* cltext);

/**
 * Shows, hides or moves the text.
 * \param cltext 	the CLTEXT object to handle
 * \param posx 		screen x coordinate to move to
 * \param posy 		screen y coordinate to move to
 * \param alpha 	alpha blending - 0 disables SHOW, 100 disables TRANSLUCENT
 * \param layer 	layer to move the text to
 */
void 	cltext_show(CLTEXT* cltext, var posx, var posy, var alpha, var layer);

/**
 * Rearranges the text from left to right by factor. The closer to the center,
 * the less impact has indent.
 * \param cltext 	CLTEXT to realign.
 * \param factor	factor between 0 and 1
 */
void 	cltext_align(CLTEXT* cltext, var factor);


#include <cltext.c>

#endif

// CLTEXT.H /////////////////////////////////////////////////////////// E O F //