#ifndef INVENTORY_C
#define INVENTORY_C

Inventory* inv_create(BMAP* _bg, STRING* _title, int _slots) {
	Inventory* inv = sys_malloc(sizeof(Inventory));
	
	// Create background panel
	inv.panel = pan_create("", 10);
	if (_bg == NULL) {
		inv.panel.bmap = bmap_createblack(INV_SIZE_X, INV_SIZE_Y, 24);
		// Draw item boxes
		var vFormat = bmap_lock(inv.panel.bmap, 0);
		var vPixel = pixel_for_vec(vector(255,255,255), 100, vFormat);
		int i,j, k;
		
		// For each box...
		for(i=0; i<INV_ITEMS_X; i++) {
			for(j=0; j<INV_ITEMS_Y; j++) {
				
				// Draw the borders
				for(k=0; k<INV_ITEM_SIZE; k++) {
					// Left an right border
					pixel_to_bmap(inv.panel.bmap, INV_ITEMS_OFFSET_X+(i*(INV_ITEM_SIZE+4))+k, INV_ITEMS_OFFSET_Y+(j*(INV_ITEM_SIZE+4)), vPixel);
					pixel_to_bmap(inv.panel.bmap, INV_ITEMS_OFFSET_X+(i*(INV_ITEM_SIZE+4)), INV_ITEMS_OFFSET_Y+(j*(INV_ITEM_SIZE+4))+k, vPixel);
					// Top and bottom border
					pixel_to_bmap(inv.panel.bmap, INV_ITEMS_OFFSET_X+(i*(INV_ITEM_SIZE+4))+INV_ITEM_SIZE, INV_ITEMS_OFFSET_Y+(j*(INV_ITEM_SIZE+4))+k, vPixel);
					pixel_to_bmap(inv.panel.bmap, INV_ITEMS_OFFSET_X+(i*(INV_ITEM_SIZE+4))+k, INV_ITEMS_OFFSET_Y+(j*(INV_ITEM_SIZE+4))+INV_ITEM_SIZE, vPixel);
				}
			}
		}
		
	} else {
		inv.panel.bmap = _bg;
	}
	
	// Create title
	inv.title = txt_create(1, 11);
	set(inv.title, OUTLINE | CENTER_X | WWRAP);
	inv.title.size_x = bmap_width(inv.panel.bmap);
	inv.title.font = fontInventoryTitle;
	if (_title != NULL) {
		str_cpy((inv.title.pstring)[0], _title);
	}
	
	// Create item description
	inv.itemDescription = txt_create(1, 11);
	set(inv.itemDescription, OUTLINE | WWRAP);
	inv.itemDescription.size_x = bmap_width(inv.panel.bmap);
	inv.itemDescription.font = fontInventoryDescription;
	str_cpy((inv.itemDescription.pstring)[0], "Dies ist ein\nTest!");
	
	return inv;
}

void inv_free(Inventory* _inv) {
	if (_inv != NULL) {
		ptr_remove(_inv.panel);
		ptr_remove(_inv.title);
		ptr_remove(_inv.itemDescription);
		sys_free(_inv);
		_inv = NULL;
	}
}

void inv_show(Inventory* _inv) {
	if (_inv != NULL) {
		set(_inv.panel, SHOW);
		set(_inv.title, SHOW);
		set(_inv.itemDescription, SHOW);
	}
}

void inv_hide(Inventory* _inv) {
	if (_inv != NULL) {
		reset(_inv.panel, SHOW);
		reset(_inv.title, SHOW);
		reset(_inv.itemDescription, SHOW);
	}
}

void inv_align(Inventory* _inv) {
	if (_inv != NULL) {
		inv_set_pos(_inv, screen_size.x / 2 - bmap_width(_inv.panel.bmap) / 2, screen_size.y / 2 - bmap_height(_inv.panel.bmap) / 2);
	}
}

void inv_set_pos(Inventory* _inv, int _x, int _y) {
	if (_inv != NULL) {
		_inv.panel.pos_x = _x;
		_inv.panel.pos_y = _y;
		_inv.title.pos_x = _inv.panel.pos_x + bmap_width(_inv.panel.bmap) / 2 + 5;
		_inv.title.pos_y = _inv.panel.pos_y + 5;
		_inv.itemDescription.pos_x = _inv.panel.pos_x + 5;
		_inv.itemDescription.pos_y = _inv.panel.pos_y + bmap_height(_inv.panel.bmap) - 35;
	}
}

// Internal use
void inv_increate_iterator(Inventory* _inv) {
	if(_inv.itr != NULL) {
		_inv.itr = _inv.itr.next;
	}
}

void inv_hide_items(Inventory* _inv) {
	if (_inv != NULL) {
		_inv.itr = _inv.head;
		while(_inv.itr != NULL)
		{
			if (_inv.itr.panel != NULL)
			{
				reset(_inv.itr.panel, SHOW);
			}
			inv_increate_iterator(_inv);
		}
	}
}

void inv_show_items(Inventory* _inv) {
	if (_inv != NULL) {
		_inv.itr = _inv.head;
		while(_inv.itr != NULL)
		{
			if (_inv.itr.panel != NULL)
			{
				set(_inv.itr.panel, SHOW);
			}
			inv_increate_iterator(_inv);
		}
	}
}

#endif