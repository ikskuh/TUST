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
	//menu_show(MENU_OPTIONS_GRAPHICS);
	//menu_show(MENU_OPTIONS_AUDIO);
	//menu_show(MENU_OPTIONS_INPUT);
	
	menu_show_message("This is a longer test message for the new message buttons!", "Okay");
	//menu_show_choice_message("Second test message! Do you like them?", "Yes", "No man!");
}