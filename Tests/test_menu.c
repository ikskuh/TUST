#include <acknex.h>
#include "..\\Source\\menu.h"

function main()
{
	level_load("");
	mouse_mode = 4;
	menu_init();
	//menu_show(MENU_START);
	//menu_show(MENU_IN_GAME);
	//menu_show(MENU_SAVE);
	//menu_show(MENU_LOAD);
	menu_show(MENU_OPTIONS_GAME);
}