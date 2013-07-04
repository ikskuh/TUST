
/* The Ultimate Script Library 
-----------------------------------------------------------------------------------
   Module: Compact Menu
   Extendable collapsable menu panels manager.

   Read the documentation for further information: TUST\Documentation\index.html
----------------------------------------------------------------------------------- 
*/

#ifndef _COMPACT_MENU_H_
#define _COMPACT_MENU_H_

/**
 * \defgroup  CompactMenuModule Compact Menu
 * \ingroup   GenericScriptLibrary
 * \brief     Extendable manager for collapsable menu panels.
 *
 * The compact menu module is a tool that lets you create and use collapsable menus very easily. It is also conceived to be easily extensible with your own compact_menu members.
 *
 * ## How it does work
 * Every compact_menu is a secuence of members that are automatically shown, hidden or modified with the mouse interaction.
 * Compact menu parses the content of strings in order to build the data structs needed to manage the automatic menu drawing and menu member selection.
 *
 * \{
 *
 * \file  compact_menu.h
 * \brief Header of the extendable manager for collapsable menu panels.
 *
 * \file  compact_menu.c
 * \brief Code of the extendable manager for collapsable menu panels.
 *
 * \file  test_compact_menu.c
 * \brief Test code of the extendable manager for collapsable menu panels.
 */

#define PRAGMA_PATH "..\\Source\\compact_menu"

/**
 * \brief    Checks for menu member errors
 */
#define CM_SAFE_MODE

/**
 * \brief    Enables the debug & statics compact menu panel template
 */
#define CM_DEBUG_PANEL

/**
 * \brief    Enables the test01 compact menu panel template
 */
#define CM_TEST01_PANEL


/**
 * \struct   CMStyle
 * \brief    Generic compact menu style struct
 */
typedef struct CMStyle
{
	COLOR colText;
	COLOR colBack;
	COLOR colOver;
	FONT *font;
} CMStyle;

/**
 * \brief    Creates a new compact menu style
 * \param    font       A pointer to a font.
 * \param    colText    The color for the text, 
 * \param    colBack    The color for the background
 * \param    colOver    Selection color
 * \returns  A pointer to the newly created style.
 */
CMStyle *cmstyle_create ( FONT *font, COLOR *colText, COLOR *colBack, COLOR *colOver );

/**
 * \brief    Removes a compact menu style
 * \param    style     The style to be removed.
 */
void cmstyle_remove ( CMStyle *style );

/**
 * \brief    Creates a new compact menu panel
 * \param    member    Character set to build the menu.
 * \param    pos_x     Panel x position.
 * \param    pos_y     Panel y position.
 * \param    size_x    Panel x size.
 * \param    layer     Panel layer.
 * \param    flags     Panel flags.
 * \param    style     Style for the panel.
 * \return   A new compact menu panel.
 * 
 * ## General member syntax rule
 *
 * <div class="memitem">
 * <div class="memproto"><div class="memname">name.member_type = parameters</div></div>
 * <div class="memdoc">
 * Parameter            | Description
 * :------------------- | :----------
 * <b>name</b>          | Text to be displayed in the left side of the member field.
 * <b>member_type</b>   | The member type name.
 * <b>parameters</b>    | One or more parameters in reference to member_type separated by commas.
 * </div></div>
 *
 * ## Default Members
 * 
 * <div class="memitem">
 * <div class="memproto"><div class="memname">submenu</div></div>
 * <div class="memdoc">
 * <div class="memname">name.submenu = TEXT*</div>
 * <div class="textblock">
 * The most important member in compact menu. It lets us create collapsable menus.
 * </div><div class="textblock">
 * Each submenu is created in reference to a TEXT struct that contains all the member construction strings of the submenu.
 * </div>
 * Parameter        | Description
 * :--------------- | :----------
 * <b>TEXT*</b>     | TEXT struct that contains the strings to be parsed as members of the submenu.
 * <div class="textblock">Example:</div>
 * ~~~
 * TEXT *txtMenu =
 * {
 *    string = (
 *       ".line=0",
 *       "camera.submenu=txtCamera", 
 *       "sun.submenu=txtSun", 
 *       "ambient.submenu=txtAmbient", 
 *       ".line=2" );
 * }
 * ~~~
 * </div></div>
 *
 * <div class="memitem">
 * <div class="memproto"><div class="memname">title</div></div>
 * <div class="memdoc">
 * <div class="memname">name.title = size_y</div>
 * Draws an underlined title
 * Parameter        | Description
 * :--------------- | :----------
 * <b>size_y</b>    | Height of the title field.
 * </div></div>
 * 
 * <div class="memitem">
 * <div class="memproto"><div class="memname">line</div></div>
 * <div class="memdoc">
 * <div class="memname">.line = size_y</div>
 * <div class="textblock">
 * Draws an horizontal gradient line from the style *text* color to background color. Conceived to use as visual separator.
 * </div>
 * Parameter        | Description
 * :--------------- | :----------
 * <b>size_y</b>    | Height of the line field.
 * </div></div>
 *
 * <div class="memitem">
 * <div class="memproto"><div class="memname">space</div></div>
 * <div class="memdoc">
 * <div class="memname">.space = size_y</div>
 * <div class="textblock">
 * Draws an horizontal gradient line from the style *over* color to background color. Conceived to use as visual separator.
 * </div>
 * Parameter        | Description
 * :--------------- | :----------
 * <b>size_y</b>    | Height of the space field.
 * </div></div>
 *
 * <div class="memitem">
 * <div class="memproto"><div class="memname">string</div></div>
 * <div class="memdoc">
 * <div class="memname">name.string = STRING*</div>
 * <div class="textblock">
 * Draws a string cutted to fit the panel size.
 * </div>
 * Parameter        | Description
 * :--------------- | :----------
 * <b>STRING*</b>   | STRING to be shown.
 * </div></div>
 *
 * <div class="memitem"><div class="memproto"><div class="memname">digit</div></div>
 * <div class="memdoc">
 * <div class="memname">name.digit = decimal, var</div>
 * <div class="memname">name.digit = decimal, var, event</div>
 * <div class="textblock">
 * Draws the content of a var. If the var is a pointer to a var, the digit will show the content of the var that was pointed by the pointer in the moment of the menu creation.
 * </div>
 * Parameter        | Description
 * :--------------- | :----------
 * <b>decimal</b>   | Decimal number. 0 <-> 3
 * <b>var</b>       | Variable to be shown.
 * <b>event</b>     | Function to fill the var.
 * <div class="textblock">
 * The event must be named as follows:
 * </div>
 * ~~~
 * var myFunctionName ()
 * {
 *    ...
 *    return myNewValue;
 * }
 * ~~~
 * </div></div>
 *
 * <div class="memitem">
 * <div class="memproto"><div class="memname">digedit</div></div>
 * <div class="memdoc">
 * <div class="memname">name.digedit = decimal, var</div>
 * <div class="memname">name.digedit = decimal, var, event</div>
 * <div class="textblock">
 * Draws and edits the content of a var. If the var is a pointer to a var, the digedit will show the content of the var that was pointed by the pointer in the moment of the menu creation.
 * </div><div class="textblock">
 * The digedits are edited by clicking over them and moving the mouse up and down. The change difference depends on the horizontal position of the mouse.
 * </div>
 * Parameter        | Description
 * :--------------- | :----------
 * <b>decimal</b>   | Decimal number. 0 <-> 3
 * <b>var</b>       | Variable to be shown and edited.
 * <b>event</b>     | Function to be called after editing the digedit. It can be obviated.
 * </div></div>
 * 
 * <div class="memitem"><div class="memproto"><div class="memname">slider</div></div>
 * <div class="memdoc">
 * <div class="memname">name.slider = min, max, step, decimal, var</div>
 * <div class="memname">name.slider = min, max, step, decimal, var, event</div>
 * <div class="textblock">
 * Draws an horizontal slider. If the var is a pointer to a var, the digedit will show the content of the var that was pointed by the pointer in the moment of the menu creation.
 * </div>
 * Parameter        | Description
 * :--------------- | :----------
 * <b>min</b>       | Minimum value in the slider. It can be a fixed value, a var or a pointer to a var.
 * <b>max</b>       | Maximum value in the slider. It can be a fixed value, a var or a pointer to a var.
 * <b>step</b>      | Step of change.
 * <b>decimal</b>   | Decimal number. 0 <-> 3
 * <b>var</b>       | Variable to be shown and edited.
 * <b>event</b>     | Function to be called after editing the slider. It can be obviated.
 * </div></div>
 */
PANEL *cmenu_create ( char *member, var pos_x, var pos_y, var size_x, var layer, var flags, CMStyle *style );

/**
 * \brief    Modifies a compact menu panel
 * \param    panel     The panel to be modified.
 * \param    size_x    New size for the panel.
 * \param    style     New style for the panel.
 */
void cmenu_modify ( PANEL *panel, var size_x, CMStyle *style );

/**
 * \brief    Removes a compact menu panel
 * \param    panel     The panel to be removed.
 */
void cmenu_remove ( PANEL *panel );

/**
 * \brief    Removes all the existing compact menu panels
 */
void cmenu_remove_all ();

/**
 * \}
 */

#include "compact_menu.c"
#include "cm_submenu.c"
#include "cm_title.c"
#include "cm_line.c"
#include "cm_space.c"
#include "cm_bitmap.c"
#include "cm_button.c"
#include "cm_slider.c"
#include "cm_digit.c"
#include "cm_string.c"

#include "cm_template_debug.c"
#include "cm_template_test01.c"
#endif