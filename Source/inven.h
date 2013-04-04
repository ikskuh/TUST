#ifndef INVENTORY_H
#define INVENTORY_H

#define INV_SIZE_X 254
#define INV_SIZE_Y 290

// Settings for item placement
#define INV_ITEM_SIZE 32
#define INV_ITEMS_X 6
#define INV_ITEMS_Y 6
#define INV_ITEMS_OFFSET_X 20
#define INV_ITEMS_OFFSET_Y 30

FONT* fontInventoryTitle = "Arial#18b";
FONT* fontInventoryDescription = "Arial#12b";

typedef struct item {
	struct item* next;
	struct item* prev;
	
	int id;
	STRING* name;
	STRING* description;
	int value;
	int itemType;
	BMAP* image;
	PANEL* panel;
	
	//void use(struct item* _item);
	//void drop(struct item* _item);
} Item;

typedef struct {
	Item* head;
	Item* tail;
	int count;
	Item* itr;
	PANEL* panel;
	TEXT* title;
	TEXT* itemDescription;
} Inventory;

Inventory* inv_create(BMAP* _bg, STRING* _title, int _slots);
void inv_free(Inventory* _inv);
void inv_show(Inventory* _inv);
void inv_hide(Inventory* _inv);
void inv_align(Inventory* _inv);
void inv_set_pos(Inventory* _inv, int _x, int _y);
void inv_add_item(Inventory* _inv, Item* _item);

// Internal use
void inv_increate_iterator(Inventory* _inv);
void inv_hide_items(Inventory* _inv);
void inv_show_items(Inventory* _inv);
void inv_draw_slot(Inventory* _inv, int _x, int _y);

#include "inven.c"

#endif