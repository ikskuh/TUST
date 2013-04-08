#include <acknex.h>

#include "..\\Source\\inven.h"

void main() {
	level_load("");
	random_seed(0);
	mouse_mode = 4;
	Inventory* inv = inv_create(NULL, "Inventory");
	Item* i1 = inv_create_item(1, "Red square", "A beautiful red square", 1, ITEM_TYPE_NEUTRAL);
	Item* i2 = inv_create_item(2, "Sword", "An ancient sword that's worth a lot!", 100, ITEM_TYPE_WEAPON);
	Item* i3 = inv_create_item(3, "Scarf", "Beautiful green scarf.", 3, ITEM_TYPE_CLOTH);
	Item* i4 = inv_create_item(4, "A letter from mum", "A letter from mum. I'm supposed to take it to dad.", 0, ITEM_TYPE_QUEST);
	Item* i5 = inv_create_item(5, "A health potion", "Drink it and feel better.", 3, ITEM_TYPE_USABLE);
	Item* i6 = inv_create_item(6, "Cup", "A cup.", 3, ITEM_TYPE_NEUTRAL);
	Item* i7 = inv_create_item(7, "Bottle", "An empty bottle.", 3, ITEM_TYPE_NEUTRAL);
	Item* i8 = inv_create_item(8, "Book", "A book about game programming.", 40, ITEM_TYPE_QUEST);
	inv_add_item(inv, i1);
	inv_add_item(inv, i2);
	inv_add_item(inv, i3);
	inv_add_item(inv, i4);
	inv_add_item(inv, i5);
	inv_add_item(inv, i6);
	inv_add_item(inv, i7);
	inv_add_item(inv, i8);
	
	inv_remove_item(inv, i8);
	
	inv_center(inv);
	
	inv_show(inv);
	
	while(1) {
		if (key_i) {
			while(key_i) wait(1);
			if (inv_is_visible(inv)) {
				inv_hide(inv);
			} else {
				inv_show(inv);
			}
		}
		wait(1);
	}
	
}