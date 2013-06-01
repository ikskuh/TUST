#ifndef _COMPACT_MENU_H_
#define _COMPACT_MENU_H_

#define CM_SAFE_MODE

PANEL *cmenu_create ( var pos_x, var pos_y, var size_x, var layer, var flags, TEXT *txtMembers );

void cmenu_resize ( PANEL *panel, var size_x );

void cmenu_remove ( PANEL *panel );

void cmenu_remove_all ();

#include "compact_menu.c"
#endif