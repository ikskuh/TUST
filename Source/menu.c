#include <acknex.h>
#include "menu.h"

#include "system.h"

void menu_init() {

	var vFormat;
	var vPixel;
	int i;
	
	// TODO: Has to be done with generic TUST BITMAP FUNCTIONS
	// Create buttons - if not defined, generate bitmaps
	if (bmapMenuButtonOff == NULL) {
		bmapMenuButtonOff = bmap_createblack(MENU_BUTTON_SIZE_X,MENU_BUTTON_SIZE_Y,24);
		var vFormat = bmap_lock(bmapMenuButtonOff, 0);
		var vPixel = pixel_for_vec(vector(255,255,255), 100, vFormat);
		int i;
		// Draw Borders
		for (i=0; i<bmap_width(bmapMenuButtonOff); i++) {
			pixel_to_bmap(bmapMenuButtonOff, i, 0, vPixel);
			pixel_to_bmap(bmapMenuButtonOff, i, bmap_height(bmapMenuButtonOff)-1, vPixel);
		}
		for (i=0; i<bmap_height(bmapMenuButtonOff); i++) {
			pixel_to_bmap(bmapMenuButtonOff, 0, i, vPixel);
			pixel_to_bmap(bmapMenuButtonOff, bmap_width(bmapMenuButtonOff)-1, i, vPixel);
		}
		bmap_unlock(bmapMenuButtonOff);
	}
	if (bmapMenuButtonOn == NULL) {
		bmapMenuButtonOn = bmap_createblack(MENU_BUTTON_SIZE_X,MENU_BUTTON_SIZE_Y,24);
		bmap_fill(bmapMenuButtonOn, vector(0,0,128), 100);
		vFormat = bmap_lock(bmapMenuButtonOn, 0);
		vPixel = pixel_for_vec(vector(255,255,255), 100, vFormat);
		
		// Draw Borders
		for (i=0; i<bmap_width(bmapMenuButtonOn); i++) {
			pixel_to_bmap(bmapMenuButtonOn, i, 0, vPixel);
			pixel_to_bmap(bmapMenuButtonOn, i, bmap_height(bmapMenuButtonOn)-1, vPixel);
		}
		for (i=0; i<bmap_height(bmapMenuButtonOn); i++) {
			pixel_to_bmap(bmapMenuButtonOn, 0, i, vPixel);
			pixel_to_bmap(bmapMenuButtonOn, bmap_width(bmapMenuButtonOn)-1, i, vPixel);
		}
		bmap_unlock(bmapMenuButtonOn);
	}
	
	// TODO: Has to be done with generic TUST BITMAP FUNCTIONS
	// Create save/load slot bitmaps
	if (bmapSaveSlotOff == NULL) {
		bmapSaveSlotOff = bmap_createblack(MENU_SAVE_SLOT_SIZE_X, MENU_SAVE_SLOT_SIZE_Y, 24);
		vFormat = bmap_lock(bmapSaveSlotOff, 0);
		vPixel = pixel_for_vec(vector(255,255,255), 100, vFormat);
		// Draw Borders
		for (i=0; i<bmap_width(bmapSaveSlotOff); i++) {
			pixel_to_bmap(bmapSaveSlotOff, i, 0, vPixel);
			pixel_to_bmap(bmapSaveSlotOff, i, bmap_height(bmapSaveSlotOff)-1, vPixel);
		}
		for (i=0; i<bmap_height(bmapSaveSlotOff); i++) {
			pixel_to_bmap(bmapSaveSlotOff, 0, i, vPixel);
			pixel_to_bmap(bmapSaveSlotOff, bmap_width(bmapSaveSlotOff)-1, i, vPixel);
		}
		bmap_unlock(bmapSaveSlotOff);
	}
	if (bmapSaveSlotOn == NULL) {
		bmapSaveSlotOn = bmap_createblack(MENU_SAVE_SLOT_SIZE_X, MENU_SAVE_SLOT_SIZE_Y, 24);
		bmap_fill(bmapSaveSlotOn, vector(0,0,128), 100);
		vFormat = bmap_lock(bmapSaveSlotOn, 0);
		vPixel = pixel_for_vec(vector(255,255,255), 100, vFormat);
		// Draw Borders
		for (i=0; i<bmap_width(bmapSaveSlotOn); i++) {
			pixel_to_bmap(bmapSaveSlotOn, i, 0, vPixel);
			pixel_to_bmap(bmapSaveSlotOn, i, bmap_height(bmapSaveSlotOn)-1, vPixel);
		}
		for (i=0; i<bmap_height(bmapSaveSlotOn); i++) {
			pixel_to_bmap(bmapSaveSlotOn, 0, i, vPixel);
			pixel_to_bmap(bmapSaveSlotOn, bmap_width(bmapSaveSlotOn)-1, i, vPixel);
		}
		bmap_unlock(bmapSaveSlotOn);
	}
	
	// Create options elements
	if (bmapOptionsChoiceBg == NULL)	 {
		bmapOptionsChoiceBg = bmap_createblack(MENU_BUTTON_SIZE_X * 4 + MENU_BUTTON_GAP * 5, MENU_BUTTON_SIZE_Y + MENU_BUTTON_GAP * 2, 24);
		vFormat = bmap_lock(bmapOptionsChoiceBg, 0);
		vPixel = pixel_for_vec(vector(255,255,255), 100, vFormat);
		// Draw Borders
		for (i=0; i<bmap_width(bmapOptionsChoiceBg); i++) {
			pixel_to_bmap(bmapOptionsChoiceBg, i, 0, vPixel);
			pixel_to_bmap(bmapOptionsChoiceBg, i, bmap_height(bmapOptionsChoiceBg)-1, vPixel);
		}
		for (i=0; i<bmap_height(bmapOptionsChoiceBg); i++) {
			pixel_to_bmap(bmapOptionsChoiceBg, 0, i, vPixel);
			pixel_to_bmap(bmapOptionsChoiceBg, bmap_width(bmapOptionsChoiceBg)-1, i, vPixel);
		}
		bmap_unlock(bmapOptionsChoiceBg);
	}
	
	// Create the background for the options menues
	if (bmapOptionsBg == NULL) {
		// If the define is set to -1, the size of the options choice bar
		// is used to determine the width of the options window.
		if (MENU_OPTIONS_SIZE_X == -1) {
			bmapOptionsBg = bmap_createblack(bmap_width(bmapOptionsChoiceBg), MENU_OPTIONS_SIZE_Y, 24);
		} else {
			bmapOptionsBg = bmap_createblack(MENU_OPTIONS_SIZE_X, MENU_OPTIONS_SIZE_Y, 24);
		}
		vFormat = bmap_lock(bmapOptionsBg, 0);
		vPixel = pixel_for_vec(vector(255,255,255), 100, vFormat);
		// Draw Borders
		for (i=0; i<bmap_width(bmapOptionsBg); i++) {
			pixel_to_bmap(bmapOptionsBg, i, 0, vPixel);
			pixel_to_bmap(bmapOptionsBg, i, bmap_height(bmapOptionsBg)-1, vPixel);
		}
		for (i=0; i<bmap_height(bmapOptionsBg); i++) {
			pixel_to_bmap(bmapOptionsBg, 0, i, vPixel);
			pixel_to_bmap(bmapOptionsBg, bmap_width(bmapOptionsBg)-1, i, vPixel);
		}
		bmap_unlock(bmapOptionsBg);		
	}
	
	// Create Knob and Slider bitmaps
	if (bmapSliderKnob == NULL) {
		bmapSliderKnob = bmap_createblack(MENU_KNOB_SIZE_X, MENU_KNOB_SIZE_Y, 24);
		bmap_fill(bmapSliderKnob, vector(0,0,128), 100);
		vFormat = bmap_lock(bmapSliderKnob, 0);
		vPixel = pixel_for_vec(vector(255,255,255), 100, vFormat);
		// Draw Borders
		for (i=0; i<bmap_width(bmapSliderKnob); i++) {
			pixel_to_bmap(bmapSliderKnob, i, 0, vPixel);
			pixel_to_bmap(bmapSliderKnob, i, bmap_height(bmapSliderKnob)-1, vPixel);
		}
		for (i=0; i<bmap_height(bmapSliderKnob); i++) {
			pixel_to_bmap(bmapSliderKnob, 0, i, vPixel);
			pixel_to_bmap(bmapSliderKnob, bmap_width(bmapSliderKnob)-1, i, vPixel);
		}
		bmap_unlock(bmapSliderKnob);		
	}
	
	if (bmapSliderBg == NULL) {
		bmapSliderBg = bmap_createblack(MENU_SLIDER_SIZE_X, MENU_SLIDER_SIZE_Y, 24);
		vFormat = bmap_lock(bmapSliderBg, 0);
		vPixel = pixel_for_vec(vector(255,255,255), 100, vFormat);
		// Draw Borders
		for (i=0; i<bmap_width(bmapSliderBg); i++) {
			pixel_to_bmap(bmapSliderBg, i, 0, vPixel);
			pixel_to_bmap(bmapSliderBg, i, bmap_height(bmapSliderBg)-1, vPixel);
		}
		for (i=0; i<bmap_height(bmapSliderBg); i++) {
			pixel_to_bmap(bmapSliderBg, 0, i, vPixel);
			pixel_to_bmap(bmapSliderBg, bmap_width(bmapSliderBg)-1, i, vPixel);
		}
		bmap_unlock(bmapSliderBg);		
	}
	
	// Checkbox: TODO draw cross instead of other fill color
	if (bmapCheckBoxCheckedOn == NULL) {
		bmapCheckBoxCheckedOn = bmap_createblack(MENU_CHECKBOX_SIZE_X, MENU_CHECKBOX_SIZE_Y, 24);
		bmap_fill(bmapCheckBoxCheckedOn, vector(0,0,128), 100);
		vFormat = bmap_lock(bmapCheckBoxCheckedOn, 0);
		vPixel = pixel_for_vec(vector(255,255,255), 100, vFormat);
		// Draw Borders
		for (i=0; i<bmap_width(bmapCheckBoxCheckedOn); i++) {
			pixel_to_bmap(bmapCheckBoxCheckedOn, i, 0, vPixel);
			pixel_to_bmap(bmapCheckBoxCheckedOn, i, bmap_height(bmapCheckBoxCheckedOn)-1, vPixel);
		}
		for (i=0; i<bmap_height(bmapCheckBoxCheckedOn); i++) {
			pixel_to_bmap(bmapCheckBoxCheckedOn, 0, i, vPixel);
			pixel_to_bmap(bmapCheckBoxCheckedOn, bmap_width(bmapCheckBoxCheckedOn)-1, i, vPixel);
		}
		bmap_unlock(bmapCheckBoxCheckedOn);		
	}
	
	if (bmapCheckBoxCheckedOff == NULL) {
		bmapCheckBoxCheckedOff = bmap_createblack(MENU_CHECKBOX_SIZE_X, MENU_CHECKBOX_SIZE_Y, 24);
		bmap_fill(bmapCheckBoxCheckedOff, vector(0,0,128), 100);		
		vFormat = bmap_lock(bmapCheckBoxCheckedOff, 0);
		vPixel = pixel_for_vec(vector(255,255,255), 100, vFormat);
		// Draw Borders
		for (i=0; i<bmap_width(bmapCheckBoxCheckedOff); i++) {
			pixel_to_bmap(bmapCheckBoxCheckedOff, i, 0, vPixel);
			pixel_to_bmap(bmapCheckBoxCheckedOff, i, bmap_height(bmapCheckBoxCheckedOff)-1, vPixel);
		}
		for (i=0; i<bmap_height(bmapCheckBoxCheckedOff); i++) {
			pixel_to_bmap(bmapCheckBoxCheckedOff, 0, i, vPixel);
			pixel_to_bmap(bmapCheckBoxCheckedOff, bmap_width(bmapCheckBoxCheckedOff)-1, i, vPixel);
		}
		bmap_unlock(bmapCheckBoxCheckedOff);		
	}
	
	if (bmapCheckBoxUncheckedOn == NULL) {
		bmapCheckBoxUncheckedOn = bmap_createblack(MENU_CHECKBOX_SIZE_X, MENU_CHECKBOX_SIZE_Y, 24);
		bmap_fill(bmapCheckBoxUncheckedOn, vector(0,0,128), 100);
		vFormat = bmap_lock(bmapCheckBoxUncheckedOn, 0);
		vPixel = pixel_for_vec(vector(255,255,255), 100, vFormat);
		// Draw Borders
		for (i=0; i<bmap_width(bmapCheckBoxUncheckedOn); i++) {
			pixel_to_bmap(bmapCheckBoxUncheckedOn, i, 0, vPixel);
			pixel_to_bmap(bmapCheckBoxUncheckedOn, i, bmap_height(bmapCheckBoxUncheckedOn)-1, vPixel);
		}
		for (i=0; i<bmap_height(bmapCheckBoxUncheckedOn); i++) {
			pixel_to_bmap(bmapCheckBoxUncheckedOn, 0, i, vPixel);
			pixel_to_bmap(bmapCheckBoxUncheckedOn, bmap_width(bmapCheckBoxUncheckedOn)-1, i, vPixel);
		}
		bmap_unlock(bmapCheckBoxUncheckedOn);		
	}	
	
	if (bmapCheckBoxUncheckedOff == NULL) {
		bmapCheckBoxUncheckedOff = bmap_createblack(MENU_CHECKBOX_SIZE_X, MENU_CHECKBOX_SIZE_Y, 24);
		vFormat = bmap_lock(bmapCheckBoxUncheckedOff, 0);
		vPixel = pixel_for_vec(vector(255,255,255), 100, vFormat);
		// Draw Borders
		for (i=0; i<bmap_width(bmapCheckBoxUncheckedOff); i++) {
			pixel_to_bmap(bmapCheckBoxUncheckedOff, i, 0, vPixel);
			pixel_to_bmap(bmapCheckBoxUncheckedOff, i, bmap_height(bmapCheckBoxUncheckedOff)-1, vPixel);
		}
		for (i=0; i<bmap_height(bmapCheckBoxUncheckedOff); i++) {
			pixel_to_bmap(bmapCheckBoxUncheckedOff, 0, i, vPixel);
			pixel_to_bmap(bmapCheckBoxUncheckedOff, bmap_width(bmapCheckBoxUncheckedOff)-1, i, vPixel);
		}
		bmap_unlock(bmapCheckBoxUncheckedOff);		
	}
	
	// Combobox
	if (bmapComboboxOn == NULL) {
		bmapComboboxOn = bmap_createblack(MENU_COMBOBOX_SIZE_X, MENU_COMBOBOX_SIZE_Y, 24);
		vFormat = bmap_lock(bmapComboboxOn, 0);
		vPixel = pixel_for_vec(vector(255,255,255), 100, vFormat);
		// Draw Borders
		for (i=0; i<bmap_width(bmapComboboxOn); i++) {
			pixel_to_bmap(bmapComboboxOn, i, 0, vPixel);
			pixel_to_bmap(bmapComboboxOn, i, bmap_height(bmapComboboxOn)-1, vPixel);
		}
		for (i=0; i<bmap_height(bmapComboboxOn); i++) {
			pixel_to_bmap(bmapComboboxOn, 0, i, vPixel);
			pixel_to_bmap(bmapComboboxOn, bmap_width(bmapComboboxOn)-1, i, vPixel);
		}
		bmap_unlock(bmapComboboxOn);		
	}
	
	if (bmapComboboxOff == NULL) {
		bmapComboboxOff = bmap_createblack(MENU_COMBOBOX_SIZE_X, MENU_COMBOBOX_SIZE_Y, 24);
		bmap_fill(bmapComboboxOff, vector(0,0,128), 100);
		vFormat = bmap_lock(bmapComboboxOff, 0);
		vPixel = pixel_for_vec(vector(255,255,255), 100, vFormat);
		// Draw Borders
		for (i=0; i<bmap_width(bmapComboboxOff); i++) {
			pixel_to_bmap(bmapComboboxOff, i, 0, vPixel);
			pixel_to_bmap(bmapComboboxOff, i, bmap_height(bmapComboboxOff)-1, vPixel);
		}
		for (i=0; i<bmap_height(bmapComboboxOff); i++) {
			pixel_to_bmap(bmapComboboxOff, 0, i, vPixel);
			pixel_to_bmap(bmapComboboxOff, bmap_width(bmapComboboxOff)-1, i, vPixel);
		}
		bmap_unlock(bmapComboboxOff);		
	}	
		
		
	// Create panels
	// Start menu
	panStartMenu = pan_create("", 10);
	vec_set(panStartMenu.size_x, vector(MENU_BUTTON_SIZE_X + MENU_BUTTON_GAP * 2, MENU_BUTTON_SIZE_Y * 5 + MENU_BUTTON_GAP * 6, 0));
	pan_setbutton(panStartMenu, 0, 0, 0, 0, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_start_click, NULL, NULL); // New game
	pan_setbutton(panStartMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 1 + MENU_BUTTON_GAP * 1, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_start_click, NULL, NULL); // Load
	pan_setbutton(panStartMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 2 + MENU_BUTTON_GAP * 2, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_start_click, NULL, NULL); // Options
	pan_setbutton(panStartMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 3 + MENU_BUTTON_GAP * 3, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_start_click, NULL, NULL); // Credits
	pan_setbutton(panStartMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 4 + MENU_BUTTON_GAP * 4, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_start_click, NULL, NULL); // To windows
	
	panInGameMenu = pan_create("", 10);
	vec_set(panInGameMenu.size_x, vector(MENU_BUTTON_SIZE_X + MENU_BUTTON_GAP * 4, MENU_BUTTON_SIZE_Y * 6 + MENU_BUTTON_GAP * 7, 0));
	pan_setbutton(panInGameMenu, 0, 0, 0, 0, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_ingame_click, NULL, NULL); // New game
	pan_setbutton(panInGameMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 1 + MENU_BUTTON_GAP * 1, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_ingame_click, NULL, NULL); // New game
	pan_setbutton(panInGameMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 2 + MENU_BUTTON_GAP * 2, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_ingame_click, NULL, NULL); // Continue game
	pan_setbutton(panInGameMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 3 + MENU_BUTTON_GAP * 3, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_ingame_click, NULL, NULL); // Load game
	pan_setbutton(panInGameMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 4 + MENU_BUTTON_GAP * 4, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_ingame_click, NULL, NULL); // Options
	pan_setbutton(panInGameMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 5 + MENU_BUTTON_GAP * 5, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_ingame_click, NULL, NULL); // Exit to title

	
	panSaveMenu = pan_create("", 10);
	vec_set(panSaveMenu.size_x, vector(MENU_SAVE_SLOT_SIZE_X*2 + MENU_BUTTON_GAP * 3, MENU_BUTTON_SIZE_Y * 1 + MENU_SAVE_SLOT_SIZE_Y*3 + MENU_BUTTON_GAP * 3, 0));
	// 6 Save slots
	pan_setbutton(panSaveMenu, 0, 0, 0, 0, bmapSaveSlotOn, bmapSaveSlotOff, bmapSaveSlotOn, bmapSaveSlotOff, NULL, NULL, NULL);
	pan_setbutton(panSaveMenu, 0, 0, MENU_SAVE_SLOT_SIZE_X + MENU_BUTTON_GAP, 0, bmapSaveSlotOn, bmapSaveSlotOff, bmapSaveSlotOn, bmapSaveSlotOff, NULL, NULL, NULL);
	pan_setbutton(panSaveMenu, 0, 0, 0, MENU_SAVE_SLOT_SIZE_Y*1 + MENU_BUTTON_GAP*1, bmapSaveSlotOn, bmapSaveSlotOff, bmapSaveSlotOn, bmapSaveSlotOff, NULL, NULL, NULL);
	pan_setbutton(panSaveMenu, 0, 0, MENU_SAVE_SLOT_SIZE_X + MENU_BUTTON_GAP, MENU_SAVE_SLOT_SIZE_Y*1 + MENU_BUTTON_GAP*1, bmapSaveSlotOn, bmapSaveSlotOff, bmapSaveSlotOn, bmapSaveSlotOff, NULL, NULL, NULL);
	pan_setbutton(panSaveMenu, 0, 0, 0, MENU_SAVE_SLOT_SIZE_Y*2 + MENU_BUTTON_GAP*2, bmapSaveSlotOn, bmapSaveSlotOff, bmapSaveSlotOn, bmapSaveSlotOff, NULL, NULL, NULL);
	pan_setbutton(panSaveMenu, 0, 0, MENU_SAVE_SLOT_SIZE_X + MENU_BUTTON_GAP, MENU_SAVE_SLOT_SIZE_Y*2 + MENU_BUTTON_GAP*2, bmapSaveSlotOn, bmapSaveSlotOff, bmapSaveSlotOn, bmapSaveSlotOff, NULL, NULL, NULL);
	// Save and Back button
	pan_setbutton(panSaveMenu, 0, 0, 0, MENU_SAVE_SLOT_SIZE_Y*3 + MENU_BUTTON_GAP*3, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_back_click, NULL, NULL);
	pan_setbutton(panSaveMenu, 0, 0, MENU_BUTTON_SIZE_X + MENU_BUTTON_GAP, MENU_SAVE_SLOT_SIZE_Y*3 + MENU_BUTTON_GAP*3, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL);

	panLoadMenu = pan_create("", 10);
	vec_set(panLoadMenu.size_x, vector(MENU_SAVE_SLOT_SIZE_X*2 + MENU_BUTTON_GAP * 3, MENU_BUTTON_SIZE_Y * 1 + MENU_SAVE_SLOT_SIZE_Y*3 + MENU_BUTTON_GAP * 3, 0));
	// 6 Save slots
	pan_setbutton(panLoadMenu, 0, 0, 0, 0, bmapSaveSlotOn, bmapSaveSlotOff, bmapSaveSlotOn, bmapSaveSlotOff, NULL, NULL, NULL);
	pan_setbutton(panLoadMenu, 0, 0, MENU_SAVE_SLOT_SIZE_X + MENU_BUTTON_GAP, 0, bmapSaveSlotOn, bmapSaveSlotOff, bmapSaveSlotOn, bmapSaveSlotOff, NULL, NULL, NULL);
	pan_setbutton(panLoadMenu, 0, 0, 0, MENU_SAVE_SLOT_SIZE_Y*1 + MENU_BUTTON_GAP*1, bmapSaveSlotOn, bmapSaveSlotOff, bmapSaveSlotOn, bmapSaveSlotOff, NULL, NULL, NULL);
	pan_setbutton(panLoadMenu, 0, 0, MENU_SAVE_SLOT_SIZE_X + MENU_BUTTON_GAP, MENU_SAVE_SLOT_SIZE_Y*1 + MENU_BUTTON_GAP*1, bmapSaveSlotOn, bmapSaveSlotOff, bmapSaveSlotOn, bmapSaveSlotOff, NULL, NULL, NULL);
	pan_setbutton(panLoadMenu, 0, 0, 0, MENU_SAVE_SLOT_SIZE_Y*2 + MENU_BUTTON_GAP*2, bmapSaveSlotOn, bmapSaveSlotOff, bmapSaveSlotOn, bmapSaveSlotOff, NULL, NULL, NULL);
	pan_setbutton(panLoadMenu, 0, 0, MENU_SAVE_SLOT_SIZE_X + MENU_BUTTON_GAP, MENU_SAVE_SLOT_SIZE_Y*2 + MENU_BUTTON_GAP*2, bmapSaveSlotOn, bmapSaveSlotOff, bmapSaveSlotOn, bmapSaveSlotOff, NULL, NULL, NULL);
	// Load and Back button
	pan_setbutton(panLoadMenu, 0, 0, 0, MENU_SAVE_SLOT_SIZE_Y*3 + MENU_BUTTON_GAP*3, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_back_click, NULL, NULL); // Back
	pan_setbutton(panLoadMenu, 0, 0, MENU_BUTTON_SIZE_X + MENU_BUTTON_GAP, MENU_SAVE_SLOT_SIZE_Y*3 + MENU_BUTTON_GAP*3, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL);
	
	// Options Menu on top
	panOptionsMenu = pan_create("", 10);
	panOptionsMenu.bmap = bmapOptionsChoiceBg;
	// TODO: Should not be necessary!
	vec_set(panOptionsMenu.size_x, vector(bmap_width(bmapOptionsChoiceBg), bmap_height(bmapOptionsChoiceBg), 0));
	pan_setbutton(panOptionsMenu, 0, 4, MENU_BUTTON_GAP, MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_options_choose, NULL, NULL);
	pan_setbutton(panOptionsMenu, 0, 4, MENU_BUTTON_SIZE_X*1 + MENU_BUTTON_GAP * 2, MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_options_choose, NULL, NULL);
	pan_setbutton(panOptionsMenu, 0, 4, MENU_BUTTON_SIZE_X*2 + MENU_BUTTON_GAP * 3, MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_options_choose, NULL, NULL);
	pan_setbutton(panOptionsMenu, 0, 4, MENU_BUTTON_SIZE_X*3 + MENU_BUTTON_GAP * 4, MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_options_choose, NULL, NULL);
	
	// Game Options
	panOptionsGame = pan_create("", 10);
	panOptionsGame.bmap = bmapOptionsBg;
	// TODO: Should not be necessary!
	vec_set(panOptionsGame.size_x, vector(bmap_width(bmapOptionsBg), bmap_height(bmapOptionsBg), 0));
	pan_setbutton(panOptionsGame, 0, 0, bmap_width(bmapOptionsBg) - MENU_BUTTON_SIZE_X - MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_apply_click, NULL, NULL); // Apply
	pan_setbutton(panOptionsGame, 0, 0, MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_back_click, NULL, NULL); // Back
	pan_setbutton(panOptionsGame, 0, 2, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 2, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, NULL, NULL, NULL); // Violence
	pan_setbutton(panOptionsGame, 0, 2, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 3, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, NULL, NULL, NULL); // Dialogs
	pan_setbutton(panOptionsGame, 0, 2, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 4, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, NULL, NULL, NULL); // Hints
	
		// Difficulty
		panOptionsGameDifficulty = pan_create("", 11);
		pan_setbutton(panOptionsGameDifficulty, 0, 4, 0, 0, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, menu_difficulty_click, NULL, NULL);
		pan_setbutton(panOptionsGameDifficulty, 0, 4, 80, 0, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, menu_difficulty_click, NULL, NULL);
		pan_setbutton(panOptionsGameDifficulty, 0, 4, 160, 0, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, menu_difficulty_click, NULL, NULL);

	// Graphic Options
	panOptionsGraphics = pan_create("", 10);
	panOptionsGraphics.bmap = bmapOptionsBg;
	// TODO: Should not be necessary!
	vec_set(panOptionsGraphics.size_x, vector(bmap_width(bmapOptionsBg), bmap_height(bmapOptionsBg), 0));
	pan_setbutton(panOptionsGraphics, 0, 0, bmap_width(bmapOptionsBg) - MENU_BUTTON_SIZE_X - MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_apply_click, NULL, NULL); // Apply
	pan_setbutton(panOptionsGraphics, 0, 0, MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_back_click, NULL, NULL); // Back

		// Resolution combobox
		pan_setbutton(panOptionsGraphics, 0, 0, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 1, bmapComboboxOff, bmapComboboxOn, bmapComboboxOff, bmapComboboxOn, menu_resolution_show, NULL, NULL);
		
		{
			// Create resolution dropdown
			panOptionsGraphicsResolutionList = pan_create("", 11);
			panOptionsGraphicsResolutionList.size_x = bmap_width(bmapComboboxOn);
			panOptionsGraphicsResolutionList.size_y = bmap_height(bmapComboboxOn)*6;
			pan_setbutton(panOptionsGraphicsResolutionList, 0, 4, 0, 0, bmapComboboxOff, bmapComboboxOn, bmapComboboxOff, bmapComboboxOn, menu_resolution_click, NULL, NULL); // 800x600
			pan_setbutton(panOptionsGraphicsResolutionList, 0, 4, 0, bmap_height(bmapComboboxOn) * 1, bmapComboboxOff, bmapComboboxOn, bmapComboboxOff, bmapComboboxOn, menu_resolution_click, NULL, NULL); // 1024x768
			pan_setbutton(panOptionsGraphicsResolutionList, 0, 4, 0, bmap_height(bmapComboboxOn) * 2, bmapComboboxOff, bmapComboboxOn, bmapComboboxOff, bmapComboboxOn, menu_resolution_click, NULL, NULL); // 1280x1024
			pan_setbutton(panOptionsGraphicsResolutionList, 0, 4, 0, bmap_height(bmapComboboxOn) * 3, bmapComboboxOff, bmapComboboxOn, bmapComboboxOff, bmapComboboxOn, menu_resolution_click, NULL, NULL); // 1400x1050
			pan_setbutton(panOptionsGraphicsResolutionList, 0, 4, 0, bmap_height(bmapComboboxOn) * 4, bmapComboboxOff, bmapComboboxOn, bmapComboboxOff, bmapComboboxOn, menu_resolution_click, NULL, NULL); // 1600x1200
			pan_setbutton(panOptionsGraphicsResolutionList, 0, 4, 0, bmap_height(bmapComboboxOn) * 5, bmapComboboxOff, bmapComboboxOn, bmapComboboxOff, bmapComboboxOn, menu_resolution_click, NULL, NULL); // 1920x1200
		}
		
		pan_setslider(panOptionsGraphics, 0, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 3, bmapSliderBg, bmapSliderKnob, 0, 100, vNewGameBrightness); // Brightness
		pan_setdigits(panOptionsGraphics, 0, MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP, MENU_BUTTON_SIZE_Y * 3, "%.0f", fontMenu, 1, vNewGameBrightness); // Show brightness
		pan_setbutton(panOptionsGraphics, 0, 2, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 4, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, NULL, NULL, NULL); // Shader
		pan_setbutton(panOptionsGraphics, 0, 2, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 5, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, NULL, NULL, NULL); // Shadows
		
		// Details
		panOptionsGraphicsDetails = pan_create("", 11);
		panOptionsGraphicsDetails.size_x = 160;
		panOptionsGraphicsDetails.size_y = 15;
		pan_setbutton(panOptionsGraphicsDetails, 0, 4, 0, 0, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, NULL, NULL, NULL);
		pan_setbutton(panOptionsGraphicsDetails, 0, 4, 80, 0, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, NULL, NULL, NULL);
		pan_setbutton(panOptionsGraphicsDetails, 0, 4, 160, 0, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, NULL, NULL, NULL);
	
		
		// Antialiasing
		panOptionsGraphicsAntiAliasing = pan_create("", 11);
		panOptionsGraphicsAntiAliasing.size_x = 160;
		panOptionsGraphicsAntiAliasing.size_y = 15;
		pan_setbutton(panOptionsGraphicsAntiAliasing, 0, 4, 0, 0, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, menu_anti_aliasing_click, NULL, NULL);
		pan_setbutton(panOptionsGraphicsAntiAliasing, 0, 4, 50, 0, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, menu_anti_aliasing_click, NULL, NULL);
		pan_setbutton(panOptionsGraphicsAntiAliasing, 0, 4, 100, 0, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, menu_anti_aliasing_click, NULL, NULL);
		pan_setbutton(panOptionsGraphicsAntiAliasing, 0, 4, 150, 0, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, menu_anti_aliasing_click, NULL, NULL);
		
		pan_setbutton(panOptionsGraphics, 0, 2, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 7, bmapCheckBoxCheckedOff, bmapCheckBoxUncheckedOff, bmapCheckBoxCheckedOn, bmapCheckBoxUncheckedOn, NULL, NULL, NULL); // Fullscreen		
		
	// Audio Options
	panOptionsAudio = pan_create("", 10);
	panOptionsAudio.bmap = bmapOptionsBg;
	// TODO: Should not be necessary!
	vec_set(panOptionsAudio.size_x, vector(bmap_width(bmapOptionsBg), bmap_height(bmapOptionsBg), 0));
	pan_setbutton(panOptionsAudio, 0, 0, bmap_width(bmapOptionsBg) - MENU_BUTTON_SIZE_X - MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_apply_click, NULL, NULL); // Apply
	pan_setbutton(panOptionsAudio, 0, 0, MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_back_click, NULL, NULL); // Back
	pan_setslider(panOptionsAudio, 0, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 1, bmapSliderBg, bmapSliderKnob, 0, 100, vNewGameMusicVolume); // Music volume
	pan_setdigits(panOptionsAudio, 0, MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP, MENU_BUTTON_SIZE_Y * 1, "%.0f", fontMenu, 1, vNewGameMusicVolume); // Show music volume
	pan_setslider(panOptionsAudio, 0, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 2, bmapSliderBg, bmapSliderKnob, 0, 100, vNewGameSpeechVolume); // Speech volume
	pan_setdigits(panOptionsAudio, 0, MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP, MENU_BUTTON_SIZE_Y * 2, "%.0f", fontMenu, 1, vNewGameSpeechVolume); // Show speech volume
	pan_setslider(panOptionsAudio, 0, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 3, bmapSliderBg, bmapSliderKnob, 0, 100, vNewGameEffectsVolume); // Effects volume	
	pan_setdigits(panOptionsAudio, 0, MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP, MENU_BUTTON_SIZE_Y * 3, "%.0f", fontMenu, 1, vNewGameEffectsVolume); // Show effects volume
	pan_setbutton(panOptionsAudio, 0, 0, MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP * 6, MENU_BUTTON_SIZE_Y * 1, bmapCheckBoxUncheckedOn, bmapCheckBoxUncheckedOff, bmapCheckBoxUncheckedOn, bmapCheckBoxUncheckedOff, menu_audio_test_box_click, NULL, NULL);
	pan_setbutton(panOptionsAudio, 0, 0, MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP * 6, MENU_BUTTON_SIZE_Y * 2, bmapCheckBoxUncheckedOn, bmapCheckBoxUncheckedOff, bmapCheckBoxUncheckedOn, bmapCheckBoxUncheckedOff, menu_audio_test_box_click, NULL, NULL);
	pan_setbutton(panOptionsAudio, 0, 0, MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP * 6, MENU_BUTTON_SIZE_Y * 3, bmapCheckBoxUncheckedOn, bmapCheckBoxUncheckedOff, bmapCheckBoxUncheckedOn, bmapCheckBoxUncheckedOff, menu_audio_test_box_click, NULL, NULL);
	
	// Input Options
	panOptionsInput = pan_create("", 10);
	panOptionsInput.bmap = bmapOptionsBg;
	// TODO: Should not be necessary!
	vec_set(panOptionsInput.size_x, vector(bmap_width(bmapOptionsBg), bmap_height(bmapOptionsBg), 0));
	pan_setbutton(panOptionsInput, 0, 0, bmap_width(bmapOptionsBg) - MENU_BUTTON_SIZE_X - MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_apply_click, NULL, NULL); // Apply
	pan_setbutton(panOptionsInput, 0, 0, MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_back_click, NULL, NULL); // Back
	pan_setslider(panOptionsInput, 0, MENU_OPTIONS_CAPTION_POS_X, MENU_BUTTON_SIZE_Y * 1, bmapSliderBg, bmapSliderKnob, 0, 10, vNewGameMouseSensitivity); // Mouse sensitivity
	pan_setdigits(panOptionsInput, 0, MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP, MENU_BUTTON_SIZE_Y * 1, "%.0f", fontMenu, 1, vNewGameMouseSensitivity); // Show mouse sensitivity
		
	
	// Create texts
	txtMenuNewGame							= txt_create(1, 11);
	txtMenuContinueGame					= txt_create(1, 11);
	txtMenuLoadGame						= txt_create(1, 11);
	txtMenuSaveGame						= txt_create(1, 11);
	txtMenuOptions							= txt_create(1, 11);
	txtMenuOptionsGame					= txt_create(1, 11);
	txtMenuOptionsGraphics				= txt_create(1, 11);
	txtMenuOptionsAudio					= txt_create(1, 11);
	txtMenuOptionsInput					= txt_create(1, 11);
	txtMenuOptionsApply					= txt_create(1, 11);
	txtMenuDifficulty						= txt_create(1, 11);
	txtMenuViolence						= txt_create(1, 11);
	txtMenuShowDialogs					= txt_create(1, 11);
	txtMenuShowHints						= txt_create(1, 11);
	txtMenuFullscreen						= txt_create(1, 11);
	txtMenuResolutions					= txt_create(1, 11);
	txtMenuAntialias						= txt_create(1, 11);
	txtMenuBrightness						= txt_create(1, 11);
	txtMenuShader							= txt_create(1, 11);
	txtMenuShadows							= txt_create(1, 11);
	txtMenuDetails							= txt_create(1, 11);
	txtMenuCredits							= txt_create(1, 11);
	txtMenuExitGame						= txt_create(1, 11);
	txtMenuReturnToWin					= txt_create(1, 11);
	txtMenuBack								= txt_create(1, 11);
	
	txtOptionsGameEasy					= txt_create(1, 11);
	txtOptionsGameMedium					= txt_create(1, 11);
	txtOptionsGameHard					= txt_create(1, 11);
	
	txtOptionsGraphicsDetailsLow		= txt_create(1, 11);
	txtOptionsGraphicsDetailsMedium	= txt_create(1, 11);
	txtOptionsGraphicsDetailsHigh		= txt_create(1, 11);
	
	txtOptionsGraphicsAAOff				= txt_create(1, 11);
	txtOptionsGraphicsAA1x				= txt_create(1, 11);
	txtOptionsGraphicsAA4x				= txt_create(1, 11);
	txtOptionsGraphicsAA9x				= txt_create(1, 11);	
	
	txtResCurrent							= txt_create(1, 11);
	txtRes800x600							= txt_create(1, 12);	
	txtRes1024x768							= txt_create(1, 12);	
	txtRes1280x1024						= txt_create(1, 12);	
	txtRes1400x1050						= txt_create(1, 12);	
	txtRes1600x1200						= txt_create(1, 12);	
	txtRes1920x1200						= txt_create(1, 12);
	
	txtMenuMusicVolume					= txt_create(1, 11);
	txtMenuSpeechVolume					= txt_create(1, 11);
	txtMenuEffectsVolume					= txt_create(1, 11);
	
	txtMenuMusicVolumeTest				= txt_create(1, 11);
	txtMenuSpeechVolumeTest				= txt_create(1, 11);
	txtMenuEffectsVolumeTest			= txt_create(1, 11);
	
	txtMenuMouseSensivity				= txt_create(1, 11);
	
	txtMenuSaveGameTitles = sys_malloc(sizeof(TEXT*)*6);
	for(i=0; i<6; i++) {
		txtMenuSaveGameTitles[i] = txt_create(1, 11);
		str_cpy((txtMenuSaveGameTitles[i].pstring)[0], "Empty");
		set(txtMenuSaveGameTitles[i], CENTER_X | CENTER_Y | OUTLINE);
	}

	str_cpy((txtMenuNewGame.pstring)[0], "New game");
	str_cpy((txtMenuContinueGame.pstring)[0], "Continue");
	str_cpy((txtMenuLoadGame.pstring)[0], "Load game");
	str_cpy((txtMenuSaveGame.pstring)[0], "Save Game");
	str_cpy((txtMenuOptions.pstring)[0], "Options");
	str_cpy((txtMenuOptionsGame.pstring)[0], "Game"); // Game Options
	{
		str_cpy((txtMenuDifficulty.pstring)[0], "Difficulty:");
		{
			str_cpy((txtOptionsGameEasy.pstring)[0], "Easy");
			str_cpy((txtOptionsGameMedium.pstring)[0], "Medium");
			str_cpy((txtOptionsGameHard.pstring)[0], "Hard");
		}
		str_cpy((txtMenuViolence.pstring)[0], "Show blood:");
		str_cpy((txtMenuShowDialogs.pstring)[0], "Show dialogs:");
		str_cpy((txtMenuShowHints.pstring)[0], "Show Hints:");
	}
	str_cpy((txtMenuOptionsGraphics.pstring)[0], "Graphics");
	{
		str_cpy((txtMenuResolutions.pstring)[0], "Resolution:"); // Resolution
		{
			str_cpy((txtResCurrent.pstring)[0], "Current res");
			str_cpy((txtRes800x600.pstring)[0], "800x600");
			str_cpy((txtRes1024x768.pstring)[0], "1024x768");
			str_cpy((txtRes1280x1024.pstring)[0], "1280x1024");
			str_cpy((txtRes1400x1050.pstring)[0], "1400x1050");
			str_cpy((txtRes1600x1200.pstring)[0], "1600x1200");
			str_cpy((txtRes1920x1200.pstring)[0], "1920x1200");
		}
		str_cpy((txtMenuAntialias.pstring)[0], "Anti aliasing:"); // AntiAliasing
		{
			str_cpy((txtOptionsGraphicsAAOff.pstring)[0], "Off");
			str_cpy((txtOptionsGraphicsAA1x.pstring)[0], "1x");
			str_cpy((txtOptionsGraphicsAA4x.pstring)[0], "4x");
			str_cpy((txtOptionsGraphicsAA9x.pstring)[0], "9x");
		}
		str_cpy((txtMenuBrightness.pstring)[0], "Brightness:");
		str_cpy((txtMenuShader.pstring)[0], "Shader:");
		str_cpy((txtMenuShadows.pstring)[0], "Shadows:");
		str_cpy((txtMenuDetails.pstring)[0], "Details:"); // Details
		{
			str_cpy((txtOptionsGraphicsDetailsLow.pstring)[0], "Low");
			str_cpy((txtOptionsGraphicsDetailsMedium.pstring)[0], "Medium");
			str_cpy((txtOptionsGraphicsDetailsHigh.pstring)[0], "High");
		}
		str_cpy((txtMenuFullscreen.pstring)[0], "Full screen:");
	}
	str_cpy((txtMenuOptionsAudio.pstring)[0], "Audio");
	{
		str_cpy((txtMenuMusicVolume.pstring)[0], "Music volume:");
		str_cpy((txtMenuSpeechVolume.pstring)[0], "Speech volume:");
		str_cpy((txtMenuEffectsVolume.pstring)[0], "Effects volume:");
		
		str_cpy((txtMenuMusicVolumeTest.pstring)[0], "P");
		str_cpy((txtMenuSpeechVolumeTest.pstring)[0], "P");
		str_cpy((txtMenuEffectsVolumeTest.pstring)[0], "P");
	}
	str_cpy((txtMenuOptionsInput.pstring)[0], "Keyboard & Mouse");
	{
		str_cpy((txtMenuMouseSensivity.pstring)[0], "Mouse sensitivity");
	}
	str_cpy((txtMenuOptionsApply.pstring)[0], "Apply");
	str_cpy((txtMenuCredits.pstring)[0], "Credits");
	str_cpy((txtMenuExitGame.pstring)[0], "Exit game");
	str_cpy((txtMenuReturnToWin.pstring)[0], "Return to windows");
	str_cpy((txtMenuBack.pstring)[0], "Back");
	
	//wait(1); // Needed so that center_x and center_y work
	
	set(txtMenuNewGame, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuContinueGame, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuLoadGame, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuSaveGame, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuOptions, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuOptionsGame, CENTER_X | CENTER_Y | OUTLINE);
	{
		set(txtMenuDifficulty, OUTLINE);
		{
			set(txtOptionsGameEasy, OUTLINE);
			set(txtOptionsGameMedium, OUTLINE);
			set(txtOptionsGameHard, OUTLINE);
		}
		set(txtMenuViolence, OUTLINE);
		set(txtMenuShowDialogs, OUTLINE);
		set(txtMenuShowHints, OUTLINE);
	}
	set(txtMenuOptionsGraphics, CENTER_X | CENTER_Y | OUTLINE);
	{
		set(txtMenuResolutions, OUTLINE);
		{
			set(txtResCurrent, CENTER_X | CENTER_Y | OUTLINE);
			set(txtRes800x600, CENTER_X | CENTER_Y | OUTLINE);
			set(txtRes1024x768, CENTER_X | CENTER_Y | OUTLINE);
			set(txtRes1280x1024, CENTER_X | CENTER_Y | OUTLINE);
			set(txtRes1400x1050, CENTER_X | CENTER_Y | OUTLINE);
			set(txtRes1600x1200, CENTER_X | CENTER_Y | OUTLINE);
			set(txtRes1920x1200, CENTER_X | CENTER_Y | OUTLINE);
		}
		set(txtMenuAntialias, OUTLINE);
		{
			set(txtOptionsGraphicsAAOff, OUTLINE);
			set(txtOptionsGraphicsAA1x, OUTLINE);
			set(txtOptionsGraphicsAA4x, OUTLINE);
			set(txtOptionsGraphicsAA9x, OUTLINE);
		}
		set(txtMenuBrightness, OUTLINE);
		set(txtMenuShader, OUTLINE);
		set(txtMenuShadows, OUTLINE);
		set(txtMenuDetails, OUTLINE);
		{
			set(txtOptionsGraphicsDetailsLow, OUTLINE);
			set(txtOptionsGraphicsDetailsMedium, OUTLINE);
			set(txtOptionsGraphicsDetailsHigh, OUTLINE);
		}
		set(txtMenuFullscreen, OUTLINE);
	}
	set(txtMenuOptionsAudio, CENTER_X | CENTER_Y | OUTLINE);
	{
		set(txtMenuMusicVolume, OUTLINE);
		set(txtMenuSpeechVolume, OUTLINE);
		set(txtMenuEffectsVolume, OUTLINE);
		
		set(txtMenuMusicVolumeTest, OUTLINE | CENTER_X | CENTER_Y);
		set(txtMenuSpeechVolumeTest, OUTLINE | CENTER_X | CENTER_Y);
		set(txtMenuEffectsVolumeTest, OUTLINE | CENTER_X | CENTER_Y);
	}
	set(txtMenuOptionsInput, CENTER_X | CENTER_Y | OUTLINE);
	{
		set(txtMenuMouseSensivity, OUTLINE);
	}
	set(txtMenuOptionsApply, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuCredits, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuExitGame, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuReturnToWin, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuBack, CENTER_X | CENTER_Y | OUTLINE);
	
	menu_message_box_init();
	menu_center();
}

void menu_show(int _menu) {
	//if (panStartMenu == NULL) return;

	int i;
	// Hide all open menus
	menu_hide();
		
	switch (_menu) {
		case MENU_START:
			nCurrentMenu = MENU_START;
			set(panStartMenu, SHOW);
			set(txtMenuNewGame, SHOW);
			// ToDo: Only if there are save games!
			set(txtMenuLoadGame, SHOW);
			set(txtMenuOptions, SHOW);
			set(txtMenuCredits, SHOW);
			set(txtMenuReturnToWin, SHOW);
		break;
		case MENU_IN_GAME:
			nCurrentMenu = MENU_IN_GAME;
			set(panInGameMenu, SHOW);
			set(txtMenuNewGame, SHOW);
			set(txtMenuContinueGame, SHOW);
			set(txtMenuLoadGame, SHOW);
			set(txtMenuSaveGame, SHOW);
			set(txtMenuOptions, SHOW);
			set(txtMenuExitGame, SHOW);
		break;
		case MENU_SAVE:
			nCurrentMenu = MENU_SAVE;
			set(panSaveMenu, SHOW);
			set(txtMenuSaveGame, SHOW);
			for(i=0; i<6; i++) {
				set(txtMenuSaveGameTitles[i], SHOW);
			}
			set(txtMenuBack, SHOW);
		break;
		case MENU_LOAD:
			nCurrentMenu = MENU_LOAD;
			set(panLoadMenu, SHOW);
			set(txtMenuLoadGame, SHOW);
			for(i=0; i<6; i++) {
				set(txtMenuSaveGameTitles[i], SHOW);
			}
			set(txtMenuBack, SHOW);
		break;
		case MENU_OPTIONS_GAME:
			nCurrentMenu = MENU_OPTIONS_GAME;
			set(panOptionsMenu, SHOW);
			set(panOptionsGame, SHOW);
			set(txtMenuOptionsGame, SHOW);
			set(txtMenuOptionsGraphics, SHOW);
			set(txtMenuOptionsAudio, SHOW);
			set(txtMenuOptionsInput, SHOW);
			set(txtMenuOptionsApply, SHOW);
			set(txtMenuBack, SHOW);
			set(txtMenuDifficulty, SHOW);
			set(txtMenuViolence, SHOW);
			set(txtMenuShowDialogs, SHOW);
			set(txtMenuShowHints, SHOW);
			
			set(panOptionsGameDifficulty, SHOW);
			set(txtOptionsGameEasy, SHOW);
			set(txtOptionsGameMedium, SHOW);
			set(txtOptionsGameHard, SHOW);
			
			// Restore values
			button_state(panOptionsMenu, 1, 1);			
			vNewGameDifficulty	= vGameDifficulty;
			vNewGameBlood			= vGameBlood;
			vNewGameShowDialogs	= vGameShowDialogs;
			vNewGameShowHints		= vGameShowHints;
			if (vNewGameBlood) {
				button_state(panOptionsGame, 3, 1);
			} else {
				button_state(panOptionsGame, 3, 0);
			}
			
			if (vNewGameShowDialogs) {
				button_state(panOptionsGame, 4, 1);
			} else {
				button_state(panOptionsGame, 4, 0);
			}
			
			if (vNewGameShowHints) {
				button_state(panOptionsGame, 5, 1);
			} else {
				button_state(panOptionsGame, 5, 0);
			}
			
			switch(vNewGameDifficulty) {
				case 1: button_state(panOptionsGameDifficulty, 1, 1); break;
				case 2: button_state(panOptionsGameDifficulty, 2, 1); break;
				case 3: button_state(panOptionsGameDifficulty, 3, 1); break;
			}
			
		break;
		case MENU_OPTIONS_GRAPHICS:
			nCurrentMenu = MENU_OPTIONS_GRAPHICS;
			set(panOptionsMenu, SHOW);
			set(panOptionsGraphics, SHOW);
			set(txtMenuOptionsGame, SHOW);
			set(txtMenuOptionsGraphics, SHOW);
			set(txtMenuOptionsAudio, SHOW);
			set(txtMenuOptionsInput, SHOW);
			set(txtMenuOptionsApply, SHOW);
			set(txtMenuBack, SHOW);
			set(txtMenuResolutions, SHOW);
			set(txtMenuAntialias, SHOW);
			set(txtMenuBrightness, SHOW);
			set(txtMenuShader, SHOW);
			set(txtMenuShadows, SHOW);
			set(txtMenuDetails, SHOW);
			set(panOptionsGraphicsDetails, SHOW);
			set(panOptionsGraphicsAntiAliasing, SHOW);
			set(txtResCurrent, SHOW);
			set(txtMenuFullscreen, SHOW);
			
			// Restore values
			button_state(panOptionsMenu, 2, 1);	
			{
				set(txtOptionsGraphicsAAOff, SHOW);
				set(txtOptionsGraphicsAA1x, SHOW);
				set(txtOptionsGraphicsAA4x, SHOW);
				set(txtOptionsGraphicsAA9x, SHOW);
				vNewGameAntiAlias = d3d_antialias;
				switch(vNewGameAntiAlias) {
					case 0: button_state(panOptionsGraphicsAntiAliasing, 1, 1); break;
					case 1: button_state(panOptionsGraphicsAntiAliasing, 2, 1); break;
					case 4: button_state(panOptionsGraphicsAntiAliasing, 3, 1); break;
					case 9: button_state(panOptionsGraphicsAntiAliasing, 4, 1); break;
				}
			}

			{
				set(txtOptionsGraphicsDetailsLow, SHOW);
				set(txtOptionsGraphicsDetailsMedium, SHOW);
				set(txtOptionsGraphicsDetailsHigh, SHOW);
				
				switch(vNewGameDetails) {
					case 0: button_state(panOptionsGraphicsDetails, 1, 1); break;
					case 1: button_state(panOptionsGraphicsDetails, 1, 1); break;
					case 2: button_state(panOptionsGraphicsDetails, 1, 1); break;
				}
			}
			vNewGameResolution = video_mode;
			switch(vNewGameResolution) {
				case 7: str_cpy((txtResCurrent.pstring)[0], "800x600"); button_state(panOptionsGraphicsResolutionList, 1,1); break;
				case 8: str_cpy((txtResCurrent.pstring)[0], "1024x768"); button_state(panOptionsGraphicsResolutionList, 2,1); break;
				case 9: str_cpy((txtResCurrent.pstring)[0], "1280x1024"); button_state(panOptionsGraphicsResolutionList, 3,1); break;
				case 10: str_cpy((txtResCurrent.pstring)[0], "1400x1050"); button_state(panOptionsGraphicsResolutionList, 4,1); break;
				case 11: str_cpy((txtResCurrent.pstring)[0], "1600x1200"); button_state(panOptionsGraphicsResolutionList, 5,1); break;
				case 12: str_cpy((txtResCurrent.pstring)[0], "1920x1200"); button_state(panOptionsGraphicsResolutionList, 6,1); break;
			}
			
			vNewGameBrightness = video_gamma;
			
			vNewGameShaders = d3d_shaderversion;
			if (vNewGameShaders > 0) {
				button_state(panOptionsGraphics, 4, 1);
			} else {
				button_state(panOptionsGraphics, 4, 0);
			}
			
			vNewGameShadows = shadow_stencil;
			if (vNewGameShadows > -1) {
				button_state(panOptionsGraphics, 5, 1);
			} else {
				button_state(panOptionsGraphics, 5, 0);
			}	
			
			vNewGameFullscreen = video_screen;
			if (vNewGameFullscreen == 1) {
				button_state(panOptionsGraphics, 6, 1);
			} else {
				button_state(panOptionsGraphics, 6, 0);
			}
			
		break;
		case MENU_OPTIONS_AUDIO:
			nCurrentMenu = MENU_OPTIONS_AUDIO;
			set(panOptionsMenu, SHOW);
			set(panOptionsAudio, SHOW);
			set(txtMenuOptionsGame, SHOW);
			set(txtMenuOptionsGraphics, SHOW);
			set(txtMenuOptionsAudio, SHOW);
			set(txtMenuOptionsInput, SHOW);
			set(txtMenuOptionsApply, SHOW);
			set(txtMenuBack, SHOW);	
			set(txtMenuMusicVolume, SHOW);
			set(txtMenuSpeechVolume, SHOW);
			set(txtMenuEffectsVolume, SHOW);
			set(txtMenuMusicVolumeTest, SHOW);
			set(txtMenuSpeechVolumeTest, SHOW);
			set(txtMenuEffectsVolumeTest, SHOW);
			
			// Restore values
			button_state(panOptionsMenu, 3, 1);				
			vNewGameMusicVolume		= vGameMusicVolume;
			vNewGameSpeechVolume		= vGameSpeechVolume;
			vNewGameEffectsVolume	= vGameEffectsVolume;
		break;	
		case MENU_OPTIONS_INPUT:
			nCurrentMenu = MENU_OPTIONS_INPUT;
			set(panOptionsMenu, SHOW);
			set(panOptionsInput, SHOW);
			set(txtMenuOptionsGame, SHOW);
			set(txtMenuOptionsGraphics, SHOW);
			set(txtMenuOptionsAudio, SHOW);
			set(txtMenuOptionsInput, SHOW);
			set(txtMenuOptionsApply, SHOW);
			set(txtMenuBack, SHOW);	
			set(txtMenuMouseSensivity, SHOW);

			// Set input button to be checked
			button_state(panOptionsMenu, 4, 1);				
			vNewGameMouseSensitivity = vGameMouseSensitivity;
		break;	
	}
	
	nMenuVisible = 1;
	
	menu_align(_menu);
}

void menu_hide() {
	
	nMenuVisible = 0;
	
	if (panStartMenu != NULL) reset(panStartMenu, SHOW);
	if (panInGameMenu != NULL) reset(panInGameMenu, SHOW);
	if (panSaveMenu != NULL) reset(panSaveMenu, SHOW);
	if (panLoadMenu != NULL) reset(panLoadMenu, SHOW);
	if (panOptionsMenu != NULL) reset(panOptionsMenu, SHOW);
	if (panOptionsGame != NULL) reset(panOptionsGame, SHOW);
	if (panOptionsGraphics != NULL) reset(panOptionsGraphics, SHOW);
	if (panOptionsAudio != NULL) reset(panOptionsAudio, SHOW);
	if (panOptionsInput != NULL) reset(panOptionsInput, SHOW);
	if (txtMenuOptionsGame != NULL) reset(txtMenuOptionsGame, SHOW);
	if (txtMenuDifficulty != NULL) reset(txtMenuDifficulty, SHOW);
	if (txtMenuViolence != NULL) reset(txtMenuViolence, SHOW);
	if (txtMenuShowDialogs != NULL) reset(txtMenuShowDialogs, SHOW);
	if (txtMenuShowHints != NULL) reset(txtMenuShowHints, SHOW);
	if (txtMenuOptionsGraphics != NULL) reset(txtMenuOptionsGraphics, SHOW);
	if (txtMenuDetails != NULL) reset(txtMenuDetails, SHOW);
	if (txtMenuShadows != NULL) reset(txtMenuShadows, SHOW);
	if (txtMenuShader != NULL) reset(txtMenuShader, SHOW);
	if (txtMenuBrightness != NULL) reset(txtMenuBrightness, SHOW);
	if (txtMenuAntialias != NULL) reset(txtMenuAntialias, SHOW);
	if (txtMenuResolutions != NULL) reset(txtMenuResolutions, SHOW);
	if (txtMenuFullscreen != NULL) reset(txtMenuFullscreen, SHOW);
	if (txtMenuOptionsAudio != NULL) reset(txtMenuOptionsAudio, SHOW);
	if (txtMenuOptionsInput != NULL) reset(txtMenuOptionsInput, SHOW);
	if (txtMenuNewGame != NULL) reset(txtMenuNewGame, SHOW);
	if (txtMenuContinueGame != NULL) reset(txtMenuContinueGame, SHOW);
	if (txtMenuLoadGame != NULL) reset(txtMenuLoadGame, SHOW);
	if (txtMenuSaveGame != NULL) reset(txtMenuSaveGame, SHOW);
	if (txtMenuOptions != NULL) reset(txtMenuOptions, SHOW);
	if (txtMenuOptionsApply != NULL) reset(txtMenuOptionsApply, SHOW);
	if (txtMenuCredits != NULL) reset(txtMenuCredits, SHOW);
	if (txtMenuExitGame != NULL) reset(txtMenuExitGame, SHOW);
	if (txtMenuReturnToWin != NULL) reset(txtMenuReturnToWin, SHOW);	
	if (txtMenuBack != NULL) reset(txtMenuBack, SHOW);
	if (panOptionsGraphicsDetails != NULL) reset(panOptionsGraphicsDetails, SHOW);
	if (panOptionsGraphicsAntiAliasing != NULL) reset(panOptionsGraphicsAntiAliasing, SHOW);
	if (txtOptionsGraphicsAAOff != NULL) reset(txtOptionsGraphicsAAOff, SHOW);
	if (txtOptionsGraphicsAA1x != NULL) reset(txtOptionsGraphicsAA1x, SHOW);
	if (txtOptionsGraphicsAA4x != NULL) reset(txtOptionsGraphicsAA4x, SHOW);
	if (txtOptionsGraphicsAA9x != NULL) reset(txtOptionsGraphicsAA9x, SHOW);
	if (txtOptionsGraphicsDetailsLow != NULL) reset(txtOptionsGraphicsDetailsLow, SHOW);
	if (txtOptionsGraphicsDetailsMedium != NULL) reset(txtOptionsGraphicsDetailsMedium, SHOW);
	if (txtOptionsGraphicsDetailsHigh != NULL) reset(txtOptionsGraphicsDetailsHigh, SHOW);
	if (panOptionsGraphicsResolutionList != NULL) reset(panOptionsGraphicsResolutionList, SHOW);
	if (txtResCurrent != NULL) reset(txtResCurrent, SHOW);
	if (txtRes800x600 != NULL) reset(txtRes800x600, SHOW);
	if (txtRes1024x768 != NULL) reset(txtRes1024x768, SHOW);
	if (txtRes1280x1024 != NULL) reset(txtRes1280x1024, SHOW);
	if (txtRes1400x1050 != NULL) reset(txtRes1400x1050, SHOW);
	if (txtRes1600x1200 != NULL) reset(txtRes1600x1200, SHOW);
	if (txtRes1920x1200 != NULL) reset(txtRes1920x1200, SHOW);
	
	if (txtMenuMusicVolume != NULL) reset(txtMenuMusicVolume, SHOW);
	if (txtMenuSpeechVolume != NULL) reset(txtMenuSpeechVolume, SHOW);
	if (txtMenuEffectsVolume != NULL) reset(txtMenuEffectsVolume, SHOW);
	if (txtMenuMusicVolumeTest != NULL) reset(txtMenuMusicVolumeTest, SHOW);
	if (txtMenuSpeechVolumeTest != NULL) reset(txtMenuSpeechVolumeTest, SHOW);
	if (txtMenuEffectsVolumeTest != NULL) reset(txtMenuEffectsVolumeTest, SHOW);
	
	if (txtMenuMouseSensivity != NULL) reset(txtMenuMouseSensivity, SHOW);
	
	if (panOptionsGameDifficulty != NULL) reset(panOptionsGameDifficulty, SHOW);
	if (txtOptionsGameEasy != NULL) reset(txtOptionsGameEasy, SHOW);
	if (txtOptionsGameMedium != NULL) reset(txtOptionsGameMedium, SHOW);
	if (txtOptionsGameHard != NULL) reset(txtOptionsGameHard, SHOW);

	int i;
	for(i=0; i<6; i++) {
		if (txtMenuSaveGameTitles[i] != NULL) {
			reset(txtMenuSaveGameTitles[i], SHOW);
		}
	}
}

int menu_is_visible() {
	return nMenuVisible;
}

void menu_align(int _menu) {
	if (panStartMenu == NULL) return;
	
	int i;
	
	switch(_menu) {
		case MENU_START:
			txtMenuNewGame.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuNewGame.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y / 2;
			
			// Only if there are save games!
			//txtMenuContinueGame.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			//txtMenuContinueGame.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y * 1 + MENU_BUTTON_GAP * 1 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuLoadGame.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuLoadGame.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y * 1 + MENU_BUTTON_GAP * 1 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuOptions.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptions.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y * 2 + MENU_BUTTON_GAP * 2 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuCredits.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuCredits.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y * 3 + MENU_BUTTON_GAP * 3 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuReturnToWin.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuReturnToWin.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y * 4 + MENU_BUTTON_GAP * 4 + MENU_BUTTON_SIZE_Y / 2;
		break;
		case MENU_IN_GAME:
		
			txtMenuNewGame.pos_x = panInGameMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuNewGame.pos_y = panInGameMenu.pos_y + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuContinueGame.pos_x = panInGameMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuContinueGame.pos_y = panInGameMenu.pos_y + MENU_BUTTON_SIZE_Y * 1 + MENU_BUTTON_GAP * 1 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuLoadGame.pos_x = panInGameMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuLoadGame.pos_y = panInGameMenu.pos_y + MENU_BUTTON_SIZE_Y * 2 + MENU_BUTTON_GAP * 2 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuSaveGame.pos_x = panInGameMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuSaveGame.pos_y = panInGameMenu.pos_y + MENU_BUTTON_SIZE_Y * 3 + MENU_BUTTON_GAP * 3 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuOptions.pos_x = panInGameMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptions.pos_y = panInGameMenu.pos_y + MENU_BUTTON_SIZE_Y * 4 + MENU_BUTTON_GAP * 4 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuExitGame.pos_x = panInGameMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuExitGame.pos_y = panInGameMenu.pos_y + MENU_BUTTON_SIZE_Y * 5 + MENU_BUTTON_GAP * 5 + MENU_BUTTON_SIZE_Y / 2;								
		break;
		case MENU_SAVE:
			i = 0;
			for(i=0; i<3; i++) {
				txtMenuSaveGameTitles[i].pos_x = panSaveMenu.pos_x + MENU_SAVE_SLOT_SIZE_X / 2;
				txtMenuSaveGameTitles[i].pos_y = panSaveMenu.pos_y + MENU_SAVE_SLOT_SIZE_Y * i + MENU_BUTTON_GAP * i + MENU_BUTTON_SIZE_Y / 2;
			}
			for(i=3; i<6; i++) {
				txtMenuSaveGameTitles[i].pos_x = panSaveMenu.pos_x + MENU_SAVE_SLOT_SIZE_X + MENU_BUTTON_GAP + MENU_SAVE_SLOT_SIZE_X / 2;
				txtMenuSaveGameTitles[i].pos_y = panSaveMenu.pos_y + MENU_SAVE_SLOT_SIZE_Y * (i-3) + MENU_BUTTON_GAP * (i-3) + MENU_BUTTON_SIZE_Y / 2;
			}
			
			txtMenuBack.pos_x = panSaveMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuBack.pos_y = panSaveMenu.pos_y + MENU_SAVE_SLOT_SIZE_Y*3 + MENU_BUTTON_GAP*3 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuSaveGame.pos_x = panSaveMenu.pos_x + MENU_BUTTON_SIZE_X + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuSaveGame.pos_y = panSaveMenu.pos_y + MENU_SAVE_SLOT_SIZE_Y*3 + MENU_BUTTON_GAP*3 + MENU_BUTTON_SIZE_Y / 2;
		break;
		case MENU_LOAD:
			i = 0;
			for(i=0; i<3; i++) {
				txtMenuSaveGameTitles[i].pos_x = panLoadMenu.pos_x + MENU_SAVE_SLOT_SIZE_X / 2;
				txtMenuSaveGameTitles[i].pos_y = panLoadMenu.pos_y + MENU_SAVE_SLOT_SIZE_Y * i + MENU_BUTTON_GAP * i + MENU_BUTTON_SIZE_Y / 2;
			}
			for(i=3; i<6; i++) {
				txtMenuSaveGameTitles[i].pos_x = panLoadMenu.pos_x + MENU_SAVE_SLOT_SIZE_X + MENU_BUTTON_GAP + MENU_SAVE_SLOT_SIZE_X / 2;
				txtMenuSaveGameTitles[i].pos_y = panLoadMenu.pos_y + MENU_SAVE_SLOT_SIZE_Y * (i-3) + MENU_BUTTON_GAP * (i-3) + MENU_BUTTON_SIZE_Y / 2;
			}
			
			txtMenuBack.pos_x = panLoadMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuBack.pos_y = panLoadMenu.pos_y + MENU_SAVE_SLOT_SIZE_Y*3 + MENU_BUTTON_GAP*3 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuLoadGame.pos_x = panLoadMenu.pos_x + MENU_BUTTON_SIZE_X + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuLoadGame.pos_y = panLoadMenu.pos_y + MENU_SAVE_SLOT_SIZE_Y*3 + MENU_BUTTON_GAP*3 + MENU_BUTTON_SIZE_Y / 2;
		break;
		case MENU_OPTIONS_GAME:
			panOptionsGame.pos_x = panOptionsMenu.pos_x;
			panOptionsGame.pos_y = panOptionsMenu.pos_y + panOptionsMenu.size_y + MENU_BUTTON_GAP;
			
			txtMenuOptionsGame.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsGame.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuOptionsGraphics.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_SIZE_X*1 + MENU_BUTTON_GAP * 2 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsGraphics.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
		
			txtMenuOptionsAudio.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_SIZE_X*2 + MENU_BUTTON_GAP * 3 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsAudio.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
		
			txtMenuOptionsInput.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_SIZE_X*3 + MENU_BUTTON_GAP * 4 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsInput.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuOptionsApply.pos_x = panOptionsGame.pos_x + bmap_width(bmapOptionsBg) - MENU_BUTTON_SIZE_X / 2 - MENU_BUTTON_GAP;
			txtMenuOptionsApply.pos_y = panOptionsGame.pos_y + bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;
			
			txtMenuBack.pos_x = panOptionsGame.pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuBack.pos_y = panOptionsGame.pos_y + bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP; 
			
			txtMenuDifficulty.pos_x = panOptionsGame.pos_x + 10;
			txtMenuDifficulty.pos_y = panOptionsGame.pos_y + MENU_BUTTON_SIZE_Y * 1.1;
			
			panOptionsGameDifficulty.pos_x = panOptionsGame.pos_x + MENU_OPTIONS_CAPTION_POS_X;
			panOptionsGameDifficulty.pos_y = panOptionsGame.pos_y + MENU_BUTTON_SIZE_Y * 1;
			
			txtOptionsGameEasy.pos_x = panOptionsGameDifficulty.pos_x + 20;
			txtOptionsGameEasy.pos_y = panOptionsGameDifficulty.pos_y * 1.01;
			
			txtOptionsGameMedium.pos_x = panOptionsGameDifficulty.pos_x + 100;
			txtOptionsGameMedium.pos_y = panOptionsGameDifficulty.pos_y * 1.01;
			
			txtOptionsGameHard.pos_x = panOptionsGameDifficulty.pos_x + 180;
			txtOptionsGameHard.pos_y = panOptionsGameDifficulty.pos_y * 1.01;
			
			txtMenuViolence.pos_x = panOptionsGame.pos_x + 10;
			txtMenuViolence.pos_y =panOptionsGame.pos_y + MENU_BUTTON_SIZE_Y * 2.1;
			
			txtMenuShowDialogs.pos_x = panOptionsGame.pos_x + 10;
			txtMenuShowDialogs.pos_y = panOptionsGame.pos_y + MENU_BUTTON_SIZE_Y * 3.1;
			
			txtMenuShowHints.pos_x = panOptionsGame.pos_x + 10;
			txtMenuShowHints.pos_y = panOptionsGame.pos_y + MENU_BUTTON_SIZE_Y * 4.1;
		break;
		case MENU_OPTIONS_GRAPHICS:
			panOptionsGraphics.pos_x = panOptionsMenu.pos_x;
			panOptionsGraphics.pos_y = panOptionsMenu.pos_y + panOptionsMenu.size_y + MENU_BUTTON_GAP;
			
			txtMenuOptionsGame.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsGame.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuOptionsGraphics.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_SIZE_X*1 + MENU_BUTTON_GAP * 2 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsGraphics.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
		
			txtMenuOptionsAudio.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_SIZE_X*2 + MENU_BUTTON_GAP * 3 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsAudio.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
		
			txtMenuOptionsInput.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_SIZE_X*3 + MENU_BUTTON_GAP * 4 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsInput.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuOptionsApply.pos_x = panOptionsGraphics.pos_x + bmap_width(bmapOptionsBg) - MENU_BUTTON_SIZE_X / 2 - MENU_BUTTON_GAP;
			txtMenuOptionsApply.pos_y = panOptionsGraphics.pos_y + bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;
			
			txtMenuBack.pos_x = panOptionsGraphics.pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuBack.pos_y = panOptionsGraphics.pos_y + bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;
			
			txtMenuResolutions.pos_x = panOptionsGraphics.pos_x + 10;
			txtMenuResolutions.pos_y = panOptionsGraphics.pos_y + MENU_BUTTON_SIZE_Y * 1.1;
			{
				panOptionsGraphicsResolutionList.pos_x = panOptionsGraphics.pos_x + MENU_OPTIONS_CAPTION_POS_X;
				panOptionsGraphicsResolutionList.pos_y = panOptionsGraphics.pos_y + MENU_BUTTON_SIZE_Y * 1 + bmap_height(bmapComboboxOn);
				
				txtResCurrent.pos_x = panOptionsGraphics.pos_x + MENU_OPTIONS_CAPTION_POS_X + bmap_width(bmapComboboxOn) / 2;
				txtResCurrent.pos_y = panOptionsGraphics.pos_y + MENU_BUTTON_SIZE_Y * 1 + bmap_height(bmapComboboxOn) / 2; // TODO
				
				txtRes800x600.pos_x = panOptionsGraphicsResolutionList.pos_x + bmap_width(bmapComboboxOn) / 2;
				txtRes800x600.pos_y = panOptionsGraphicsResolutionList.pos_y + bmap_height(bmapComboboxOn) * 0.5;
				
				txtRes1024x768.pos_x = panOptionsGraphicsResolutionList.pos_x + bmap_width(bmapComboboxOn) / 2;
				txtRes1024x768.pos_y = panOptionsGraphicsResolutionList.pos_y + bmap_height(bmapComboboxOn) * 1.5;
				
				txtRes1280x1024.pos_x = panOptionsGraphicsResolutionList.pos_x + bmap_width(bmapComboboxOn) / 2;
				txtRes1280x1024.pos_y = panOptionsGraphicsResolutionList.pos_y + bmap_height(bmapComboboxOn) * 2.5;
				
				txtRes1400x1050.pos_x = panOptionsGraphicsResolutionList.pos_x + bmap_width(bmapComboboxOn) / 2;
				txtRes1400x1050.pos_y = panOptionsGraphicsResolutionList.pos_y + bmap_height(bmapComboboxOn) * 3.5;
				
				txtRes1600x1200.pos_x = panOptionsGraphicsResolutionList.pos_x + bmap_width(bmapComboboxOn) / 2;
				txtRes1600x1200.pos_y = panOptionsGraphicsResolutionList.pos_y + bmap_height(bmapComboboxOn) * 4.5;
				
				txtRes1920x1200.pos_x = panOptionsGraphicsResolutionList.pos_x + bmap_width(bmapComboboxOn) / 2;
				txtRes1920x1200.pos_y = panOptionsGraphicsResolutionList.pos_y + bmap_height(bmapComboboxOn) * 5.5;
			}
			
			txtMenuAntialias.pos_x = panOptionsGraphics.pos_x + 10;
			txtMenuAntialias.pos_y = panOptionsGraphics.pos_y + MENU_BUTTON_SIZE_Y * 2.1;
			
			txtMenuBrightness.pos_x = panOptionsGraphics.pos_x + 10;
			txtMenuBrightness.pos_y = panOptionsGraphics.pos_y + MENU_BUTTON_SIZE_Y * 3.1;
			
			txtMenuShader.pos_x = panOptionsGraphics.pos_x + 10;
			txtMenuShader.pos_y = panOptionsGraphics.pos_y + MENU_BUTTON_SIZE_Y * 4.1;
			
			txtMenuShadows.pos_x = panOptionsGraphics.pos_x + 10;
			txtMenuShadows.pos_y = panOptionsGraphics.pos_y + MENU_BUTTON_SIZE_Y * 5.1;
			
			txtMenuDetails.pos_x = panOptionsGraphics.pos_x + 10;
			txtMenuDetails.pos_y = panOptionsGraphics.pos_y + MENU_BUTTON_SIZE_Y * 6.1;
			
			panOptionsGraphicsDetails.pos_x = panOptionsGraphics.pos_x + MENU_OPTIONS_CAPTION_POS_X;
			panOptionsGraphicsDetails.pos_y = panOptionsGraphics.pos_y + MENU_BUTTON_SIZE_Y * 6;
			{
				txtOptionsGraphicsDetailsLow.pos_x = panOptionsGraphicsDetails.pos_x + 20;	
				txtOptionsGraphicsDetailsLow.pos_y = panOptionsGraphicsDetails.pos_y;
				
				txtOptionsGraphicsDetailsMedium.pos_x = panOptionsGraphicsDetails.pos_x + 100;
				txtOptionsGraphicsDetailsMedium.pos_y = panOptionsGraphicsDetails.pos_y;
				
				txtOptionsGraphicsDetailsHigh.pos_x = panOptionsGraphicsDetails.pos_x + 180; 
				txtOptionsGraphicsDetailsHigh.pos_y = panOptionsGraphicsDetails.pos_y;
			}				
			
			panOptionsGraphicsAntiAliasing.pos_x = panOptionsGraphics.pos_x + MENU_OPTIONS_CAPTION_POS_X; 
			panOptionsGraphicsAntiAliasing.pos_y = panOptionsGraphics.pos_y + MENU_BUTTON_SIZE_Y * 2;
			{
				txtOptionsGraphicsAAOff.pos_x = panOptionsGraphicsAntiAliasing.pos_x + 20;
				txtOptionsGraphicsAAOff.pos_y = panOptionsGraphicsAntiAliasing.pos_y;
				
				txtOptionsGraphicsAA1x.pos_x = panOptionsGraphicsAntiAliasing.pos_x + 70;
				txtOptionsGraphicsAA1x.pos_y = panOptionsGraphicsAntiAliasing.pos_y;
				
				txtOptionsGraphicsAA4x.pos_x = panOptionsGraphicsAntiAliasing.pos_x + 120;
				txtOptionsGraphicsAA4x.pos_y = panOptionsGraphicsAntiAliasing.pos_y;
				
				txtOptionsGraphicsAA9x.pos_x = panOptionsGraphicsAntiAliasing.pos_x + 170;
				txtOptionsGraphicsAA9x.pos_y = panOptionsGraphicsAntiAliasing.pos_y;												
			}
			
			txtMenuFullscreen.pos_x = panOptionsGraphics.pos_x + 10;
			txtMenuFullscreen.pos_y = panOptionsGraphics.pos_y + MENU_BUTTON_SIZE_Y * 7.1;
		break;
		case MENU_OPTIONS_AUDIO:
			panOptionsAudio.pos_x = panOptionsMenu.pos_x;
			panOptionsAudio.pos_y = panOptionsMenu.pos_y + panOptionsMenu.size_y + MENU_BUTTON_GAP;
			
			txtMenuOptionsGame.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsGame.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuOptionsGraphics.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_SIZE_X*1 + MENU_BUTTON_GAP * 2 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsGraphics.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
		
			txtMenuOptionsAudio.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_SIZE_X*2 + MENU_BUTTON_GAP * 3 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsAudio.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
		
			txtMenuOptionsInput.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_SIZE_X*3 + MENU_BUTTON_GAP * 4 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsInput.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuOptionsApply.pos_x = panOptionsAudio.pos_x + bmap_width(bmapOptionsBg) - MENU_BUTTON_SIZE_X / 2 - MENU_BUTTON_GAP;
			txtMenuOptionsApply.pos_y = panOptionsAudio.pos_y + bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;
			
			txtMenuBack.pos_x = panOptionsAudio.pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuBack.pos_y = panOptionsAudio.pos_y + bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;
			
			txtMenuMusicVolume.pos_x = panOptionsAudio.pos_x + 10;
			txtMenuMusicVolume.pos_y = panOptionsAudio.pos_y + MENU_BUTTON_SIZE_Y * 1.1;
			
			txtMenuSpeechVolume.pos_x = panOptionsAudio.pos_x + 10;
			txtMenuSpeechVolume.pos_y = panOptionsAudio.pos_y + MENU_BUTTON_SIZE_Y * 2.1;
			
			txtMenuEffectsVolume.pos_x = panOptionsAudio.pos_x + 10;
			txtMenuEffectsVolume.pos_y = panOptionsAudio.pos_y + MENU_BUTTON_SIZE_Y * 3.1;
			
			txtMenuMusicVolumeTest.pos_x = panOptionsAudio.pos_x + MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP * 6 + MENU_CHECKBOX_SIZE_X / 2;				
			txtMenuMusicVolumeTest.pos_y = panOptionsAudio.pos_y + MENU_BUTTON_SIZE_Y * 1 + MENU_CHECKBOX_SIZE_Y / 2;
			
			txtMenuSpeechVolumeTest.pos_x = panOptionsAudio.pos_x + MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP * 6 + MENU_CHECKBOX_SIZE_X / 2;
			txtMenuSpeechVolumeTest.pos_y = panOptionsAudio.pos_y + MENU_BUTTON_SIZE_Y * 2 + MENU_CHECKBOX_SIZE_Y / 2;
			
			txtMenuEffectsVolumeTest.pos_x = panOptionsAudio.pos_x + MENU_OPTIONS_CAPTION_POS_X + MENU_SLIDER_SIZE_X + MENU_BUTTON_GAP * 6 + MENU_CHECKBOX_SIZE_X / 2;				
			txtMenuEffectsVolumeTest.pos_y = panOptionsAudio.pos_y + MENU_BUTTON_SIZE_Y * 3 + MENU_CHECKBOX_SIZE_Y / 2;
		break;
		case MENU_OPTIONS_INPUT:
			panOptionsInput.pos_x = panOptionsMenu.pos_x;
			panOptionsInput.pos_y = panOptionsMenu.pos_y + panOptionsMenu.size_y + MENU_BUTTON_GAP;
			
			txtMenuOptionsGame.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsGame.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuOptionsGraphics.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_SIZE_X*1 + MENU_BUTTON_GAP * 2 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsGraphics.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
		
			txtMenuOptionsAudio.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_SIZE_X*2 + MENU_BUTTON_GAP * 3 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsAudio.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
		
			txtMenuOptionsInput.pos_x = panOptionsMenu.pos_x + MENU_BUTTON_SIZE_X*3 + MENU_BUTTON_GAP * 4 + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptionsInput.pos_y = panOptionsMenu.pos_y + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuOptionsApply.pos_x = panOptionsInput.pos_x + bmap_width(bmapOptionsBg) - MENU_BUTTON_SIZE_X / 2 - MENU_BUTTON_GAP;
			txtMenuOptionsApply.pos_y = panOptionsInput.pos_y + bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;
			
			txtMenuBack.pos_x = panOptionsInput.pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
			txtMenuBack.pos_y = panOptionsInput.pos_y + bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;		
			
			txtMenuMouseSensivity.pos_x = panOptionsInput.pos_x + 10;
			txtMenuMouseSensivity.pos_y = panOptionsInput.pos_y + MENU_BUTTON_SIZE_Y * 1.1;
		break;
	}
}

void menu_center() {
	if (panStartMenu != NULL) {
		panStartMenu.pos_x = screen_size.x / 2 - panStartMenu.size_x / 2;
		panStartMenu.pos_y = screen_size.y / 2 - panStartMenu.size_y / 2;	
		panInGameMenu.pos_x = screen_size.x / 2 - panStartMenu.size_x / 2;
		panInGameMenu.pos_y = screen_size.y / 2 - panStartMenu.size_y / 2;
		panSaveMenu.pos_x = screen_size.x / 2 - panSaveMenu.size_x / 2;
		panSaveMenu.pos_y = screen_size.y / 2 - panSaveMenu.size_y / 2;
		panLoadMenu.pos_x = screen_size.x / 2 - panSaveMenu.size_x / 2;
		panLoadMenu.pos_y = screen_size.y / 2 - panSaveMenu.size_y / 2;
		panOptionsMenu.pos_x = screen_size.x / 2 - panOptionsMenu.size_x / 2;
		panOptionsMenu.pos_y = 30;	
		menu_align(nCurrentMenu);
	}
}

void menu_set_pos(int _x, int _y) {
	if (panStartMenu != NULL) {
		panStartMenu.pos_x = _x;
		panStartMenu.pos_y = _y;
		panInGameMenu.pos_x = _x;
		panInGameMenu.pos_y = _x;
		panSaveMenu.pos_x = _x;
		panSaveMenu.pos_y = _y;
		panLoadMenu.pos_x = _x;
		panLoadMenu.pos_y = _y;	
		panOptionsMenu.pos_x = _x;
		panOptionsMenu.pos_y = _y;	
		menu_align(nCurrentMenu);
	}	
}

void menu_message_box_init() {
	// Message box background
	panMessageBoxBg = pan_create("", 99);
	set(panMessageBoxBg, LIGHT | TRANSLUCENT);
	panMessageBoxBg.alpha = 70;
	vec_set(panMessageBoxBg.blue, vector(30,30,30));
	
	// Message box text
	txtMessageBoxCaption = txt_create(1, 101);
	set(txtMessageBoxCaption, OUTLINE);
	
	// Buttons
	txtMessageBoxBtn1 = txt_create(1, 101);
	set(txtMessageBoxBtn1, OUTLINE);
	txtMessageBoxBtn2 = txt_create(1, 101);
	set(txtMessageBoxBtn2, OUTLINE);
}

void menu_show_message(STRING* _msg, STRING* _button) {
	
	// ToDo: Debug
	if (panMessageBoxBg == NULL) {
		printf("Please init menu first!");
		return;
	}
	
	// Already a message box showing?
	if (is(panMessageBoxBg, SHOW)) {
		printf("There is already a message box!");
		return;
	}
	
	panMessageBoxBg.size_x = screen_size.x;
	panMessageBoxBg.size_y = screen_size.y;
	
	// Needs to be recreated due to button count
	if (panMessageBox != NULL) ptr_remove(panMessageBox);
	panMessageBox = pan_create("", 100);
	set(panMessageBox, LIGHT);
	vec_set(panMessageBox.blue, vector(10,10,10));
	panMessageBox.size_x = 400;
	panMessageBox.size_y = 100;
	//pan_setbutton(panMessageBox, 0, 1, MENU_BUTTON_GAP, 100-MENU_BUTTON_SIZE_Y-MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_message_box_click, NULL, NULL);
	//pan_setbutton(panMessageBox, 0, 1, 400 - MENU_BUTTON_GAP - MENU_BUTTON_SIZE_X, 100-MENU_BUTTON_SIZE_Y-MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_message_box_click, NULL, NULL);
	
	pan_setbutton(panMessageBox, 0, 1, 200 - MENU_BUTTON_SIZE_X / 2, 100-MENU_BUTTON_SIZE_Y-MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_message_box_click, NULL, NULL);
	pan_setcolor(panMessageBox, 3, 1, vector(255,255,255));
	//pan_setcolor(panMessageBox, 3, 2, vector(255,255,255));
	panMessageBox.pos_x = screen_size.x / 2 - panMessageBox.size_x / 2;
	panMessageBox.pos_y = screen_size.y / 2 - panMessageBox.size_y / 2;
	
	str_cpy((txtMessageBoxBtn1.pstring)[0], _button);
	
	str_cpy((txtMessageBoxCaption.pstring)[0], _msg);
	txtMessageBoxCaption.pos_x = panMessageBox.pos_x + 200;
	txtMessageBoxCaption.pos_y = panMessageBox.pos_y + 30;
	//vec_set(txtMessageBoxCaption.pos_x, vector(screen_size.x / 2, panMessageBox.pos_y + MENU_BUTTON_GAP, 0));
	
	str_cpy((txtMessageBoxBtn1.pstring)[0], "Okay");
	txtMessageBoxBtn1.pos_x = panMessageBox.pos_x + 200;
	txtMessageBoxBtn1.pos_y = panMessageBox.pos_y + 100 - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;
	
	// Show them all
	set(panMessageBoxBg, SHOW);
	set(panMessageBox, SHOW);
	set(txtMessageBoxCaption, SHOW | CENTER_X | CENTER_Y);
	set(txtMessageBoxBtn1, SHOW | CENTER_X | CENTER_Y);
}

int menu_show_choice_message(STRING* _msg, STRING* _button1, STRING* _button2) {
	// ToDo: Debug
	if (panMessageBoxBg == NULL) {
		printf("Please init menu first!");
		return;
	}
	
	// Already a message box showing?
	if (is(panMessageBoxBg, SHOW)) {
		printf("There is already a message box!");
		return;
	}	
	
	panMessageBoxBg.size_x = screen_size.x;
	panMessageBoxBg.size_y = screen_size.y;
	
	// Needs to be recreated due to button count
	if (panMessageBox != NULL) ptr_remove(panMessageBox);
	panMessageBox = pan_create("", 100);
	set(panMessageBox, LIGHT);
	vec_set(panMessageBox.blue, vector(10,10,10));
	panMessageBox.size_x = 400;
	panMessageBox.size_y = 100;
	pan_setbutton(panMessageBox, 0, 1, MENU_BUTTON_GAP, 100-MENU_BUTTON_SIZE_Y-MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_message_box_click, NULL, NULL);
	pan_setbutton(panMessageBox, 0, 1, 400 - MENU_BUTTON_GAP - MENU_BUTTON_SIZE_X, 100-MENU_BUTTON_SIZE_Y-MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, menu_message_box_click, NULL, NULL);
	pan_setcolor(panMessageBox, 3, 1, vector(255,255,255));
	pan_setcolor(panMessageBox, 3, 2, vector(255,255,255));
	panMessageBox.pos_x = screen_size.x / 2 - panMessageBox.size_x / 2;
	panMessageBox.pos_y = screen_size.y / 2 - panMessageBox.size_y / 2;
	
	str_cpy((txtMessageBoxBtn1.pstring)[0], _button1);
	str_cpy((txtMessageBoxBtn2.pstring)[0], _button2);
	
	str_cpy((txtMessageBoxCaption.pstring)[0], _msg);
	txtMessageBoxCaption.pos_x = panMessageBox.pos_x + 200;
	txtMessageBoxCaption.pos_y = panMessageBox.pos_y + 30;
	
	str_cpy((txtMessageBoxBtn1.pstring)[0], _button1);
	txtMessageBoxBtn1.pos_x = panMessageBox.pos_x + MENU_BUTTON_GAP + MENU_BUTTON_SIZE_X / 2;
	txtMessageBoxBtn1.pos_y = panMessageBox.pos_y + 100 - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;
	
	str_cpy((txtMessageBoxBtn2.pstring)[0], _button2);
	txtMessageBoxBtn2.pos_x = panMessageBox.pos_x + 400 - MENU_BUTTON_GAP - MENU_BUTTON_SIZE_X / 2;
	txtMessageBoxBtn2.pos_y = panMessageBox.pos_y + 100 - MENU_BUTTON_SIZE_Y / 2 - MENU_BUTTON_GAP;
	
	// Show them all
	set(panMessageBoxBg, SHOW);
	set(panMessageBox, SHOW);
	set(txtMessageBoxCaption, SHOW | CENTER_X | CENTER_Y);
	set(txtMessageBoxBtn1, SHOW | CENTER_X | CENTER_Y);
	set(txtMessageBoxBtn2, SHOW | CENTER_X | CENTER_Y);
}

// Internal functions
void menu_resolution_show() {
	if(is(panOptionsGraphicsResolutionList, SHOW)) {
		// Hide resolution list
		reset(panOptionsGraphicsResolutionList, SHOW);
		reset(txtRes800x600, SHOW);
		reset(txtRes1024x768, SHOW);
		reset(txtRes1280x1024, SHOW);
		reset(txtRes1400x1050, SHOW);
		reset(txtRes1600x1200, SHOW);
		reset(txtRes1920x1200, SHOW);			
	} else {
		// Show resolution list
		set(panOptionsGraphicsResolutionList, SHOW);
		set(txtRes800x600, SHOW);
		set(txtRes1024x768, SHOW);
		set(txtRes1280x1024, SHOW);
		set(txtRes1400x1050, SHOW);
		set(txtRes1600x1200, SHOW);
		set(txtRes1920x1200, SHOW);	
	}
}

void menu_resolution_click(var _button_number, PANEL* _panel) {
	
	// Hide resolution list
	reset(panOptionsGraphicsResolutionList, SHOW);
	reset(txtRes800x600, SHOW);
	reset(txtRes1024x768, SHOW);
	reset(txtRes1280x1024, SHOW);
	reset(txtRes1400x1050, SHOW);
	reset(txtRes1600x1200, SHOW);
	reset(txtRes1920x1200, SHOW);	
	
	vNewGameResolution = _button_number + 6;
	switch(_button_number) {
		case 1: str_cpy((txtResCurrent.pstring)[0], "800x600"); break;
		case 2: str_cpy((txtResCurrent.pstring)[0], "1024x768"); break;
		case 3: str_cpy((txtResCurrent.pstring)[0], "1280x1024"); break;
		case 4: str_cpy((txtResCurrent.pstring)[0], "1400x1050"); break;
		case 5: str_cpy((txtResCurrent.pstring)[0], "1600x1200"); break;
		case 6: str_cpy((txtResCurrent.pstring)[0], "1920x1200"); break;
	}
}

void menu_options_choose(var _button_number, PANEL* _panel) {
	switch(_button_number) {
		case 1: menu_show(MENU_OPTIONS_GAME); break;
		case 2: menu_show(MENU_OPTIONS_GRAPHICS); break;
		case 3: menu_show(MENU_OPTIONS_AUDIO); break;
		case 4: menu_show(MENU_OPTIONS_INPUT); break;
	}
}

void menu_back_click(var _button_number, PANEL* _panel) {
	if (nInGame == 0) {
		menu_show(MENU_START);
	} else {
		menu_show(MENU_IN_GAME);
	}
}

void menu_start_click(var _button_number, PANEL* _panel) {
	
	switch(_button_number) {
		case 1:
			// New
			printf("new game");
		break;
		case 2:
			menu_show(MENU_LOAD);
		break;
		case 3:
			menu_show(MENU_OPTIONS_GAME);
		break;
		case 4:
			// Credits
			printf("credits");
		break;
		case 5:
			// Exit to Windows
			printf("exit to windows");
		break;
	}
}

void menu_ingame_click(var _button_number, PANEL* _panel) {
	switch(_button_number) {
		case 1:
			// New Game
			printf("new game");
		break;
		case 2:
			// Continue
			printf("continue");
		break;
		case 3:
			menu_show(MENU_LOAD);
		break;
		case 4:
			menu_show(MENU_SAVE);
		break;
		case 5:
			menu_show(MENU_OPTIONS_GAME);
		break;
		case 6:
			// Exit game
			nInGame = 0;
			menu_show(MENU_START);
		break;
	}
}

void menu_apply_click(var _button_number, PANEL* _panel) {
	
	if (_panel == panOptionsGame) {
		
		// These if comparision serve as method to check, if the
		// original value has changed. Can be deleted if wanted.
		if (vNewGameDifficulty != vGameDifficulty) {
			vGameDifficulty = vNewGameDifficulty;
		}
		
		vNewGameBlood = button_state(panOptionsGame, 1, -1);
		if (vNewGameBlood != vGameBlood) {
			vGameBlood = vNewGameBlood;
		}
		
		vNewGameShowDialogs = button_state(panOptionsGame, 2, -1);
		if (vNewGameShowDialogs != vGameShowDialogs) {
			vGameShowDialogs = vNewGameShowDialogs;
		}
		
		vNewGameShowHints = button_state(panOptionsGame, 3, -1);
		if (vNewGameShowHints != vGameShowHints) {
			vGameShowHints = vNewGameShowHints;
		}				
	}
	
	if (_panel == panOptionsGraphics) {
		
		// Resolution and full screen
		if (button_state(panOptionsGraphics, 6, -1) == 1) {
			vNewGameFullscreen = 1;
		} else {
			vNewGameFullscreen = 2;
		}
		if ((vNewGameResolution != video_mode) || (vNewGameFullscreen != video_screen)) {
			video_switch(vNewGameResolution, 0, vNewGameFullscreen);
		}
		
		// Anti aliasing
		if (vNewGameAntiAlias != d3d_antialias) {
			d3d_antialias = vNewGameAntiAlias;
			// ToDo: Requires game restart
		}
		
		// Brightness
		if (vNewGameBrightness != video_gamma) {
			video_gamma = vNewGameBrightness;
		}
		
		vNewGameShaders = button_state(panOptionsGraphics, 4, -1);
		if (vNewGameShaders != d3d_shaderversion) {
			if (vNewGameShaders == 0) {
				d3d_shaderversion = 0;
			} else {
				d3d_shaderversion = vInitialShaderVersion;
			}
		}
		
		vNewGameShadows = button_state(panOptionsGraphics, 5, -1);
		if (vNewGameShadows == 1) {
			vNewGameShadows = 2;
		} else {
			vNewGameShadows = -1;
		}
		if (vNewGameShadows != shadow_stencil) {
			shadow_stencil = vNewGameShadows;
		}		
		
		if (vNewGameDetails != vGameDetails) {
			vGameDetails = vNewGameDetails;
		}
	}
	
	if (_panel == panOptionsAudio) {
		
		if (vNewGameMusicVolume != vGameMusicVolume) {
			vGameMusicVolume = vNewGameMusicVolume;
		}
		
		if (vNewGameSpeechVolume != vGameSpeechVolume) {
			vGameSpeechVolume = vNewGameSpeechVolume;
		}
		
		if (vNewGameEffectsVolume != vGameEffectsVolume) {
			vGameEffectsVolume = vNewGameEffectsVolume;
		}
	}
	
	if (_panel == panOptionsInput) {
		
		if (vNewGameMouseSensitivity != vGameMouseSensitivity) {
			vGameMouseSensitivity = vNewGameMouseSensitivity;
		}
	}
}

void menu_difficulty_click(var _button_number, PANEL* _panel) {
	vNewGameDifficulty = _button_number;
}

void menu_anti_aliasing_click(var _button_number, PANEL* _panel) {
	switch(_button_number) {
		case 1: vNewGameAntiAlias = 0; break;
		case 2: vNewGameAntiAlias = 1; break;
		case 3: vNewGameAntiAlias = 4; break;
		case 4: vNewGameAntiAlias = 9; break;
	}
}

void menu_details_click(var _button_number, PANEL* _panel) {
	vNewGameDetails = _button_number;
}

void menu_message_box_click(var _button_number, PANEL* _panel) {
	
	// Hide message box
	reset(panMessageBoxBg, SHOW);
	reset(panMessageBox, SHOW);
	reset(txtMessageBoxCaption, SHOW | CENTER_X | CENTER_Y);
	reset(txtMessageBoxBtn1, SHOW | CENTER_X | CENTER_Y);
	reset(txtMessageBoxBtn2, SHOW | CENTER_X | CENTER_Y);
}

void menu_audio_test_box_click(var _button_number, PANEL* _panel) {
	switch(_button_number) {
		case 3:
			snd_play(sndMusicTest, vNewGameMusicVolume, 0);
		break;
		case 4:
			snd_play(sndSpeechTest, vNewGameSpeechVolume, 0);
		break;
		case 5:
			snd_play(sndEffectsTest, vNewGameEffectsVolume, 0);
		break;
	}
}