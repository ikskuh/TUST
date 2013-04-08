#ifndef INVENTORY_C
#define INVENTORY_C

Inventory* inv_create(BMAP* _bg, STRING* _title) {
	Inventory* inv = sys_malloc(sizeof(Inventory));
	
	int nInvItemsX = 0;
	int nInvItemsY = 0;
	
	// Create background panel
	inv.panel = pan_create("", INV_PANEL_LAYER);
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
				for(k=0; k<INV_ITEM_SIZE+2; k++) {
					// Top and left border
					pixel_to_bmap(inv.panel.bmap, INV_ITEMS_OFFSET_X+(i*(INV_ITEM_SIZE+INV_ITEM_GAP))+k, INV_ITEMS_OFFSET_Y+(j*(INV_ITEM_SIZE+INV_ITEM_GAP)), vPixel);
					pixel_to_bmap(inv.panel.bmap, INV_ITEMS_OFFSET_X+(i*(INV_ITEM_SIZE+INV_ITEM_GAP)), INV_ITEMS_OFFSET_Y+(j*(INV_ITEM_SIZE+INV_ITEM_GAP))+k, vPixel);
					// Right and bottom border
					pixel_to_bmap(inv.panel.bmap, INV_ITEMS_OFFSET_X+(i*(INV_ITEM_SIZE+INV_ITEM_GAP))+INV_ITEM_SIZE+1, INV_ITEMS_OFFSET_Y+(j*(INV_ITEM_SIZE+INV_ITEM_GAP))+k, vPixel);
					pixel_to_bmap(inv.panel.bmap, INV_ITEMS_OFFSET_X+(i*(INV_ITEM_SIZE+INV_ITEM_GAP))+k, INV_ITEMS_OFFSET_Y+(j*(INV_ITEM_SIZE+INV_ITEM_GAP))+INV_ITEM_SIZE+1, vPixel);
				}
			}
		}
		
	} else {
		inv.panel.bmap = _bg;
	}
	
	// Create title
	inv.title = txt_create(1, INV_PANEL_LAYER + 1);
	set(inv.title, OUTLINE | CENTER_X | WWRAP);
	inv.title.size_x = bmap_width(inv.panel.bmap);
	inv.title.font = fontInventoryTitle;
	if (_title != NULL) {
		str_cpy((inv.title.pstring)[0], _title);
	}
	
	// Create item description
	inv.itemDescription = txt_create(1, INV_PANEL_LAYER + 1);
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

int inv_is_visible(Inventory* _inv) {
	if (_inv == NULL) return 0;
	if (is(_inv.panel, SHOW)) {
		return 1;
	} else {
		return 0;
	}
}

void inv_show(Inventory* _inv) {
	if (_inv != NULL) {
		set(_inv.panel, SHOW);
		set(_inv.title, SHOW);
		
		// Show items
		_inv.itr = _inv.head;
		int i = 0;
		while(_inv.itr != NULL) {
			if (_inv.itr.panel != NULL)
			{	
				set(_inv.itr.panel, SHOW);
				_inv.itr.panel.pos_x = _inv.panel.pos_x + 1 + INV_ITEMS_OFFSET_X + (INV_ITEM_GAP * (i%INV_ITEMS_X)) + (INV_ITEM_SIZE*(i%INV_ITEMS_X));
				_inv.itr.panel.pos_y = _inv.panel.pos_y + 1 + INV_ITEMS_OFFSET_Y + (INV_ITEM_GAP * (integer(i/INV_ITEMS_Y))) + (INV_ITEM_SIZE*(integer(i/INV_ITEMS_Y)));		
			}
			i+=1;
			inv_increate_iterator(_inv);
		}
	}
}

void inv_hide(Inventory* _inv) {
	if (_inv != NULL) {
		reset(_inv.panel, SHOW);
		reset(_inv.title, SHOW);
		reset(_inv.itemDescription, SHOW);
		
		// Hide items
		_inv.itr = _inv.head;
		int i = 0;
		while(_inv.itr != NULL) {
			if (_inv.itr.panel != NULL)
			{	
				reset(_inv.itr.panel, SHOW);
			}
			i+=1;
			inv_increate_iterator(_inv);
		}
	}
}

void inv_center(Inventory* _inv) {
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

void inv_add_item(Inventory* _inv, Item* _item) {
	if (_inv == NULL || _item == NULL) return;
	
	// Inventory full?
	if (_inv.count >= INV_ITEMS_X * INV_ITEMS_Y) {
		return;
	}
	
	if(_inv.tail == NULL) {
      _inv.head = _inv.tail = _item;
      _inv.head.next = NULL;
      _inv.head.prev = NULL;
      layer_sort(_item.panel, _inv.panel.layer+1);
   } else {
      _item.prev = _inv.tail;
      _item.next = NULL;
      _inv.tail.next = _item;
      _inv.tail = _item;
      layer_sort(_item.panel, _inv.panel.layer+1);
   }
   // Assert an inventory to an item to be able to show
   // an item description
   _item.inv = _inv;
	_inv.count += 1;
}

void inv_remove_item(Inventory* _inv, Item* _item) {
	if (_inv == NULL || _item == NULL) return;
	Item* tempItem = NULL;
	_inv.itr = _inv.head;
	// Browse all items
	while(_inv.itr != NULL) {
		// Found the right one?
		if (_inv.itr == _item) {
			tempItem = _inv.itr;
			// Is it the first in the list?
			if (tempItem == _inv.head) {
				_inv.head = _inv.head.next;
			} else {
				// ... or the last?
				if (tempItem == _inv.tail) {
					_inv.tail = _inv.tail.prev;
				}
			}
			// Linkt items in front and behind
			if (tempItem.prev != NULL) {
				tempItem.prev.next = tempItem.next;
			}
			if (tempItem.next != NULL) {
				tempItem.next.prev = tempItem.prev;
			}
			if (_inv.head == NULL) {
				_inv.tail = NULL;
			}
			_inv.count -=1;
			break;
		}
		inv_increate_iterator(_inv);
	}
}

Item* inv_create_item(int _id, STRING* _name, STRING* _descr, int _value, int _type) {
	Item* item = sys_malloc(sizeof(Item));
	item.id = _id;
	item.name = str_create("");
	if (_name != NULL) str_cpy(item.name, _name);
	item.description = str_create("");
	if (_descr != NULL) str_cpy(item.description, _descr);
	item.value = _value;
	item.itemType = _type;
	item.image = bmap_createblack(INV_ITEM_SIZE, INV_ITEM_SIZE, 24);
	bmap_fill(item.image, vector(integer(random(255)),integer(random(255)),integer(random(255))), 100);
	item.panel = pan_create("", INV_PANEL_LAYER + 1);
	item.panel.size_x = INV_ITEM_SIZE;
	item.panel.size_y = INV_ITEM_SIZE;
	pan_setbutton(item.panel, 0, 0, 0, 0, item.image, item.image, item.image, item.image, inv_item_click, inv_item_leave, inv_item_enter);
	// Assert item to panel so that the item can be referenced from the panel (click events)
	item.panel.skill_x = item;
	return item;
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

void inv_item_click(int _buttonNumber, PANEL* _panel) {
	if (_panel != NULL) {
		if (_panel.skill_x != NULL) {
			Item* tempItem = (Item*)_panel.skill_x;
			if (tempItem != NULL) {
				printf("%s clicked!", _chr(tempItem.name));
			}
		}
	}
}

void inv_item_enter(int _buttonNumber, PANEL* _panel) {
	if (_panel != NULL) {
		if (_panel.skill_x != NULL) {
			Item* tempItem = (Item*)_panel.skill_x;
			if (tempItem != NULL) {
				Inventory* tempInv = (Inventory*)(tempItem.inv);
				if (tempInv != NULL) {
					if (tempInv.itemDescription != NULL) {
						// Show item description
						set(tempInv.itemDescription, SHOW);
						str_cpy((tempInv.itemDescription.pstring)[0], tempItem.description);
					}
				}
			}
		}
	}
}

void inv_item_leave(int _buttonNumber, PANEL* _panel) {
	if (_panel != NULL) {
		if (_panel.skill_x != NULL) {
			Item* tempItem = (Item*)_panel.skill_x;
			if (tempItem != NULL) {
				Inventory* tempInv = (Inventory*)(tempItem.inv);
				if (tempInv != NULL) {
					if (tempInv.itemDescription != NULL) {
						// Hide item description
						reset(tempInv.itemDescription, SHOW);
					}
				}
			}
		}
	}
}

#endif