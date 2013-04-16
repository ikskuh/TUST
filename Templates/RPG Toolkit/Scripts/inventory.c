// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 300810
//
// title: Code that handles everything Inventory related
// class: Inventory
// type: Inventory
// help: contains code handling all the Inventory data
// 
// ENDHEADER
// ---------------------------------------------------------------------

// variables
INVENTORY *INV_inventory;

// TEMP
void testingINVEventFunc(INV_ITEM* myItem) {
	myItem->ITEM_amount -= 1;
	
	if(myItem->ITEM_amount == 0) {
		INV_removeItem(INV_inventory, myItem->ITEM_name);
	}
}

// inventory test code
void createInventoryTest() {
	// create the inventory
	INV_inventory = init_inventory();
	
	// add items
	INV_addItem(INV_inventory, "Sword");
	INV_addItem(INV_inventory, "Potion");
	INV_addItem(INV_inventory, "Potion");
	INV_addItem(INV_inventory, "Potion");
	INV_addItem(INV_inventory, "Key");
	INV_addItem(INV_inventory, "Armour");
	INV_addItem(INV_inventory, "Ring");
	INV_addItem(INV_inventory, "Herb");
	INV_addItem(INV_inventory, "Herb");
	INV_addItem(INV_inventory, "Potion");
	
	// draw inventory content
	INV_ITEM *checkItem;
	var index;
	var randInvItem;
	while(1) {
		checkItem = INV_inventory->INV_firstItem;
		index = 0;
		// get the number the item is at in the inventory
		while(checkItem != NULL) {
			//item was found, now to draw it's content and amount
			draw_text(checkItem->ITEM_name, 10, 20*index, vector(255,255,255));
			draw_text(str_for_num(NULL, checkItem->ITEM_amount), 110, 20*index, vector(255,255,255));
			
			index += 1;
			checkItem = checkItem->ITEM_next;
		}
		
		// remove first item
		if(key_r) {
			while(key_r) { wait(1); }
			randInvItem = integer(random(6));
			
			switch(randInvItem) {
				case 0:
					INV_startItemEvent(INV_inventory, "Key");
					break;
				
				case 1:
					INV_startItemEvent(INV_inventory, "Ring");
					break;
				
				case 2:
					INV_startItemEvent(INV_inventory, "Armour");
					break;
				
				case 3:
					INV_startItemEvent(INV_inventory, "Herb");
					break;
				
				case 4:
					INV_startItemEvent(INV_inventory, "Potion");
					break;
				
				case 5:
					INV_startItemEvent(INV_inventory, "Crap");
					break;
			}
			
		}
		
		// add random item
		if(key_t) {
			while(key_t) { wait(1); }
			randInvItem = integer(random(6));
			
			switch(randInvItem) {
				case 0:
					INV_addItem(INV_inventory, "Key");
					break;
				
				case 1:
					INV_addItem(INV_inventory, "Ring");
					break;
				
				case 2:
					INV_addItem(INV_inventory, "Armour");
					break;
				
				case 3:
					INV_addItem(INV_inventory, "Herb");
					break;
				
				case 4:
					INV_addItem(INV_inventory, "Potion");
					break;
				
				case 5:
					INV_addItem(INV_inventory, "Crap");
					break;
			}
		}
		
		wait(1);
	}
}