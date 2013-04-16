#ifndef INVENTORY_H
#define INVENTORY_H

#define INV_SIZE_X 254
#define INV_SIZE_Y 290
#define INV_PANEL_LAYER 10

// Settings for item placement
#define INV_ITEM_SIZE 32
#define INV_ITEMS_X 6
#define INV_ITEMS_Y 6
#define INV_ITEMS_OFFSET_X 20
#define INV_ITEMS_OFFSET_Y 30
#define INV_ITEM_GAP 4 // Gap between item slots

// Item types
#define ITEM_TYPE_FOOD 1
#define ITEM_TYPE_QUEST 2
#define ITEM_TYPE_WEAPON 3
#define ITEM_TYPE_CLOTH 4
#define ITEM_TYPE_USABLE 5
#define ITEM_TYPE_NEUTRAL 6

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
	void* inv; // Pointer to Inventory - Untyped because of later typedef
	
	// Function pointers - to be called when item
	// is either used or dropped
	void use(struct item* _item);
	void drop(struct item* _item);
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

Inventory* inv_create(BMAP* _bg, STRING* _title);
void inv_free(Inventory* _inv);
void inv_show(Inventory* _inv);
void inv_hide(Inventory* _inv);
int inv_is_visible(Inventory* _inv);
void inv_center(Inventory* _inv);
void inv_set_pos(Inventory* _inv, int _x, int _y);
void inv_add_item(Inventory* _inv, Item* _item);
void inv_remove_item(Inventory* _inv, Item* _item);
Item* inv_create_item(int _id, STRING* _name, STRING* _descr, int _value, int _type);

// Internal use
void inv_increate_iterator(Inventory* _inv);
void inv_hide_items(Inventory* _inv);
void inv_show_items(Inventory* _inv);
void inv_draw_slot(Inventory* _inv, int _x, int _y);
void inv_item_click(int _buttonNumber, PANEL* _panel);
void inv_item_enter(int _buttonNumber, PANEL* _panel);
void inv_item_leave(int _buttonNumber, PANEL* _panel);

#include "inven.c"

#endif