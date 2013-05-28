#ifndef _INVENTORY_H_
#define _INVENTORY_H_

/**
 * \file inven.h
 * \Functions for a simple inventory
 *
 * Lets player collect items in an inventory.
 */

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


/**
 * Fixed item types
 */
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

/**
 * Creates a simple inventory
 * \param	BMAP*	Inventory background (May be NULL -> Background is drawn within the function)
 * \param	STRING*	Title of the inventory (May be NULL)
 * \return			The inventory pointer
 */
Inventory* inv_create(BMAP* _bg, STRING* _title);

/**
 * Frees an inventory
 * \param	Inventory*	The inventory to be freed
 */
void inv_free(Inventory* _inv);

/**
 * Shows an inventory
 * \param	Inventory* Inventory to be shown
 */
void inv_show(Inventory* _inv);

/**
 * Hides an inventory
 * \param	Inventory*	Inventory to be hidden
 */
void inv_hide(Inventory* _inv);

/**
 * Is a certain inventory visible?
 * \param	Inventory*	Inventory to be checked
 * \return			1 if visible, 0 if not
 */
int inv_is_visible(Inventory* _inv);

/**
 * Centers an inventory on screen
 * \param	Inventory*	Inventory that should be centered
 */
void inv_center(Inventory* _inv);

/**
 * An inventory is set to a certain position on screen
 * \param	Inventory*	Inventory that has to be moved
 * \param	int	x-position of the inventory
 * \param	int	y-position of the inventory
 */
void inv_set_pos(Inventory* _inv, int _x, int _y);

/**
 * Adds an item to the inventory
 * \param	Inventory*	The target inventory
 * \param	Item*	Item to be put in the inventory
 */
void inv_add_item(Inventory* _inv, Item* _item);

/**
 * Removes an item from an inventory
 * \param	Inventory*	Target inventory
 * \param	Item*	Item to be removed (if it exists)
 */
void inv_remove_item(Inventory* _inv, Item* _item);

/**
 * Creates an item
 * \param	int	A fixed ID for the item
 * \param	STRING*	The name of the item
 * \param	STRING*	A description
 * \param	int	A value (i.e. in gold)
 * \param	int	An item type (see defines above)
 * \return			The item
 */
Item* inv_create_item(int _id, STRING* _name, STRING* _descr, int _value, int _type);

/**
 * Creates an item with a bitmap
 * \param	int	A fixed ID for the item
 * \param	STRING*	The name of the item
 * \param	STRING*	A description
 * \param	int	A value (i.e. in gold)
 * \param	int	An item type (see defines above)
 * \param	BMAP*	Bitmap to illustrate the item
 * \return			The item
 */
Item* inv_create_item(int _id, STRING* _name, STRING* _descr, int _value, int _type, BMAP* _bitmap);




// Internal use

/**
 * Increases an internal inventory iterator
 * \param	Inventory*	The inventory containing the iterator
 */
void inv_increate_iterator(Inventory* _inv);

/**
 * Hides all items from an inventory
 * \param	Inventory*	The inventory
 */
void inv_hide_items(Inventory* _inv);

/**
 * Shows all items from an inventory
 * \param	Inventory*	The inventory
 */
void inv_show_items(Inventory* _inv);

/**
 * Item was cliked
 * \param	var	Button of the item
 * \param	var	Panel of the inventory
 */
void inv_item_click(var _buttonNumber, PANEL* _panel);

/**
 * Mouse entered an item button
 * \param	var	Button of the item
 * \param	var	Panel of the inventory
 */
void inv_item_enter(var _buttonNumber, PANEL* _panel);

/**
 * Mouse left an item button
 * \param	var	Button of the item
 * \param	var	Panel of the inventory
 */
void inv_item_leave(var _buttonNumber, PANEL* _panel);

#include "inven.c"

#endif