#include <acknex.h>

#include "..\\Source\\inven.h"

void main() {
	level_load("");
	random_seed(0);
	mouse_mode = 4;
	Inventory* inv = inv_create(NULL, "Inventory");
	Item* i1 = inv_create_item(1, "Leather boots", "Some cheap leather boots", 3, ITEM_TYPE_CLOTH, bmap_create("..\\Ressources\\Graphics\\Items\\th_A_Shoes01.tga"));
	Item* i2 = inv_create_item(2, "Sword", "An ancient sword that's worth a lot!", 100, ITEM_TYPE_WEAPON, bmap_create("..\\Ressources\\Graphics\\Items\\th_W_Sword020.tga"));
	Item* i3 = inv_create_item(3, "Necklace", "A necklace to make your wife happy.", 30, ITEM_TYPE_CLOTH, bmap_create("..\\Ressources\\Graphics\\Items\\th_Ac_Necklace01.tga"));
	Item* i4 = inv_create_item(4, "A letter from mum", "A letter from mum. I'm supposed to take it to dad.", 0, ITEM_TYPE_QUEST, bmap_create("..\\Ressources\\Graphics\\Items\\th_I_Scroll02.tga"));
	Item* i5 = inv_create_item(5, "Dragon's bones", "Not really, it was a fish.", 3, ITEM_TYPE_NEUTRAL, bmap_create("..\\Ressources\\Graphics\\Items\\th_E_Bones02.tga"));
	Item* i6 = inv_create_item(6, "Banana", "Find a monkey, make him happy.", 3, ITEM_TYPE_FOOD, bmap_create("..\\Ressources\\Graphics\\Items\\th_I_C_Banana.tga"));
	Item* i7 = inv_create_item(7, "Bottle", "An empty bottle.", 3, ITEM_TYPE_NEUTRAL, bmap_create("..\\Ressources\\Graphics\\Items\\th_I_Bottle04.tga"));
	Item* i8 = inv_create_item(8, "Book", "A book about game programming.", 40, ITEM_TYPE_QUEST, bmap_create("..\\Ressources\\Graphics\\Items\\th_I_Book.tga"));
	Item* i9 = inv_create_item(9, "Armor", "A weak armor made of silk", 5, ITEM_TYPE_CLOTH, bmap_create("..\\Ressources\\Graphics\\Items\\th_A_Armor04.tga"));
	
	inv_add_item(inv, i1);
	inv_add_item(inv, i2);
	inv_add_item(inv, i3);
	inv_add_item(inv, i4);
	inv_add_item(inv, i5);
	inv_add_item(inv, i6);
	inv_add_item(inv, i7);
	inv_add_item(inv, i8);
	
	//inv_remove_item(inv, i8);
	
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