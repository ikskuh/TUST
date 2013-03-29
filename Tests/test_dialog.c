#include <acknex.h>
#include "..\\Source\\dialogs.h"

void main() {
	level_load("");
	mouse_mode = 4;
	dlg_init();
	dlg_start("test_dialog\\test_dialog.xml");
}