// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 290810
//
// title: Code that handles everything Inventory related
// class: Inventory
// type: Inventory
// help: contains code handling all the Inventory data
// 
// prefix: INV_
// idcode: 12
// 
// ENDHEADER
// ---------------------------------------------------------------------

typedef struct INV_ITEM {
	STRING *ITEM_name;
	var ITEM_amount;
	
	STRING *ITEM_eventFunction;
	
	struct INV_ITEM *ITEM_prev;
	struct INV_ITEM *ITEM_next;
} INV_ITEM;

typedef struct {
	INV_ITEM *INV_firstItem;
	INV_ITEM *INV_lastItem;
} INVENTORY;

// prototypes
INVENTORY *init_inventory();
INV_ITEM *init_item(STRING *addItem);
void INV_addItem(INVENTORY *INV_add, STRING* addItem);
void INV_removeItem(INVENTORY *INV_add, STRING* removeItem);
void INV_startItemEvent(INVENTORY *INV_add, STRING* eventItem);

// empty prototype function to run other functions from code
void INV_runEventFunction(INV_ITEM* myItem);

// includes
#include "inventory.c"

INVENTORY *init_inventory() {
	INVENTORY *myInventory = sys_malloc(sizeof(INVENTORY));
	
	myInventory->INV_firstItem = NULL;
	myInventory->INV_lastItem = NULL;
	
	return(myInventory);
}

INV_ITEM *init_item(STRING *addItem) {
	INV_ITEM *myItem = sys_malloc(sizeof(INV_ITEM));
	
	myItem->ITEM_name = str_create("");
	str_cpy(myItem->ITEM_name, addItem);
	myItem->ITEM_amount = 1;
	
	myItem->ITEM_eventFunction = str_create("");
	str_cpy(myItem->ITEM_eventFunction, "testingINVEventFunc"); // TEMP, should be replaced with custom event from file
	
	myItem->ITEM_prev = NULL;
	myItem->ITEM_next = NULL;
	
	return(myItem);
}

void INV_addItem(INVENTORY *INV_add, STRING* addItem) {
	INV_ITEM *checkItem = INV_add->INV_firstItem;
	INV_ITEM *newItem;
	
	// check if inventory has atleast somethig already
	if(checkItem == NULL) {
		newItem = init_item(addItem); // store new item
		INV_add->INV_firstItem = newItem; // make first item new item
		INV_add->INV_lastItem = newItem; // and also last item
		return;
	}
	// check inventory stuff if item already exists
	while(checkItem != NULL) {
		if(str_cmpi(checkItem->ITEM_name, addItem)) {
			checkItem->ITEM_amount += 1; // add item if it already exists
			return;
		}
		
		checkItem = checkItem->ITEM_next;
	}
	
	// add new item if it didnt already exist
	newItem = init_item(addItem); // store new item
	INV_add->INV_lastItem->ITEM_next = newItem; // make new item linked to last item
	newItem->ITEM_prev = INV_add->INV_lastItem; // make previous link to last item
	INV_add->INV_lastItem = newItem; // make last item new item
}

void INV_removeItem(INVENTORY *INV_add, STRING* removeItem) {
	INV_ITEM *checkItem = INV_add->INV_firstItem;
	
	// get the number the item is at in the inventory
	while(checkItem != NULL) {
		if(str_cmpi(checkItem->ITEM_name, removeItem)) {
			//item was found, now to remove, and relink previous and next
			
			// check to see if item is not the first item in the list
			if(checkItem != INV_add->INV_firstItem) {
				// if the item is the last item
				if(checkItem == INV_add->INV_lastItem) {
					checkItem->ITEM_prev->ITEM_next = NULL;
					INV_add->INV_lastItem = checkItem->ITEM_prev;
				} else {
					checkItem->ITEM_prev->ITEM_next = checkItem->ITEM_next;
					checkItem->ITEM_next->ITEM_prev = checkItem->ITEM_prev;
				}
			} else {
				// if it IS the first item
				INV_add->INV_firstItem = checkItem->ITEM_next;
			}
			
			break;
		}
		
		checkItem = checkItem->ITEM_next;
	}
	
	if(checkItem != NULL) { sys_free(checkItem); } // remove item
}

void INV_startItemEvent(INVENTORY *INV_add, STRING* eventItem) {
	INV_ITEM *checkItem = INV_add->INV_firstItem;
	
	// check inventory stuff if item already exists
	while(checkItem != NULL) {
		if(str_cmpi(checkItem->ITEM_name, eventItem)) {
			// execute function that's written here
			INV_runEventFunction = engine_getscript(_chr(checkItem->ITEM_eventFunction));
			if(INV_runEventFunction) { INV_runEventFunction(checkItem); }
			break;
		}
		
		checkItem = checkItem->ITEM_next;
	}
}