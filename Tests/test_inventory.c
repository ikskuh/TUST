#include <acknex.h>

#include "..\\Source\\inven.h"

void main() {
	level_load("");
	mouse_mode = 4;
	Inventory* inv = inv_create(NULL, "Inventar", 12);
	inv_align(inv);
	inv_show(inv);
}