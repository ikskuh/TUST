#include <acknex.h>
#include "menu.h"

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
	
	// Create panels
	// Start menu
	panStartMenu = pan_create("", 10);
	vec_set(panStartMenu.size_x, vector(MENU_BUTTON_SIZE_X + MENU_BUTTON_GAP * 2, MENU_BUTTON_SIZE_Y * 7 + MENU_BUTTON_GAP * 6, 0));
	pan_setbutton(panStartMenu, 0, 0, 0, 0, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL); // New game
	pan_setbutton(panStartMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 1 + MENU_BUTTON_GAP * 1, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL); // Continue game
	pan_setbutton(panStartMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 2 + MENU_BUTTON_GAP * 2, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL); // Load game
	pan_setbutton(panStartMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 3 + MENU_BUTTON_GAP * 3, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL); // Options
	pan_setbutton(panStartMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 4 + MENU_BUTTON_GAP * 4, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL); // Credits
	pan_setbutton(panStartMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 5 + MENU_BUTTON_GAP * 5, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL); // To windows
	
	panInGameMenu = pan_create("", 10);
	vec_set(panInGameMenu.size_x, vector(MENU_BUTTON_SIZE_X + MENU_BUTTON_GAP * 4, MENU_BUTTON_SIZE_Y * 5 + MENU_BUTTON_GAP * 4, 0));
	pan_setbutton(panInGameMenu, 0, 0, 0, 0, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL); // Continue
	pan_setbutton(panInGameMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 1 + MENU_BUTTON_GAP * 1, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL); // Load
	pan_setbutton(panInGameMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 2 + MENU_BUTTON_GAP * 2, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL); // Save
	pan_setbutton(panInGameMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 3 + MENU_BUTTON_GAP * 3, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL); // Options
	pan_setbutton(panInGameMenu, 0, 0, 0, MENU_BUTTON_SIZE_Y * 4 + MENU_BUTTON_GAP * 4, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL); // Exit
	
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
	pan_setbutton(panSaveMenu, 0, 0, 0, MENU_SAVE_SLOT_SIZE_Y*3 + MENU_BUTTON_GAP*3, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL);
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
	// Save and Back button
	pan_setbutton(panLoadMenu, 0, 0, 0, MENU_SAVE_SLOT_SIZE_Y*3 + MENU_BUTTON_GAP*3, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL);
	pan_setbutton(panLoadMenu, 0, 0, MENU_BUTTON_SIZE_X + MENU_BUTTON_GAP, MENU_SAVE_SLOT_SIZE_Y*3 + MENU_BUTTON_GAP*3, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL);
	
	
	panOptionsMenu = pan_create("", 10);
	panOptionsMenu.bmap = bmapOptionsChoiceBg;
	// TODO: Should not be necessary!
	vec_set(panOptionsMenu.size_x, vector(bmap_width(bmapOptionsChoiceBg), bmap_height(bmapOptionsChoiceBg), 0));
	pan_setbutton(panOptionsMenu, 0, 4, MENU_BUTTON_GAP, MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL);
	pan_setbutton(panOptionsMenu, 0, 4, MENU_BUTTON_SIZE_X*1 + MENU_BUTTON_GAP * 2, MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL);
	pan_setbutton(panOptionsMenu, 0, 4, MENU_BUTTON_SIZE_X*2 + MENU_BUTTON_GAP * 3, MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL);
	pan_setbutton(panOptionsMenu, 0, 4, MENU_BUTTON_SIZE_X*3 + MENU_BUTTON_GAP * 4, MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL);
	
	panOptionsGame = pan_create("", 10);
	panOptionsGame.bmap = bmapOptionsBg;
	// TODO: Should not be necessary!
	vec_set(panOptionsGame.size_x, vector(bmap_width(bmapOptionsBg), bmap_height(bmapOptionsBg), 0));
	pan_setbutton(panOptionsGame, 0, 0, bmap_width(bmapOptionsBg) - MENU_BUTTON_SIZE_X - MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL); // Apply
	pan_setbutton(panOptionsGame, 0, 0, MENU_BUTTON_GAP, bmap_height(bmapOptionsBg) - MENU_BUTTON_SIZE_Y - MENU_BUTTON_GAP, bmapMenuButtonOn, bmapMenuButtonOff, bmapMenuButtonOn, bmapMenuButtonOff, NULL, NULL, NULL); // Back
	
	
/*PANEL* panOptionsGraphics		= NULL;
PANEL* panOptionsAudio			= NULL;
PANEL* panOptionsInput			= NULL;*/
		
	// Create texts
	txtMenuNewGame				= txt_create(1, 11);
	txtMenuContinueGame		= txt_create(1, 11);
	txtMenuLoadGame			= txt_create(1, 11);
	txtMenuSaveGame			= txt_create(1, 11);
	txtMenuOptions				= txt_create(1, 11);
	txtMenuOptionsGame		= txt_create(1, 11);
	txtMenuOptionsGraphics	= txt_create(1, 11);
	txtMenuOptionsAudio		= txt_create(1, 11);
	txtMenuOptionsInput		= txt_create(1, 11);
	txtMenuOptionsApply		= txt_create(1, 11);
	txtMenuCredits				= txt_create(1, 11);
	txtMenuExitGame			= txt_create(1, 11);
	txtMenuReturnToWin		= txt_create(1, 11);
	txtMenuBack					= txt_create(1, 11);
	
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
	str_cpy((txtMenuOptionsGame.pstring)[0], "Game");
	str_cpy((txtMenuOptionsGraphics.pstring)[0], "Graphics");
	str_cpy((txtMenuOptionsAudio.pstring)[0], "Audio");
	str_cpy((txtMenuOptionsInput.pstring)[0], "Keyboard & Mouse");
	str_cpy((txtMenuOptionsApply.pstring)[0], "Apply");
	str_cpy((txtMenuCredits.pstring)[0], "Credits");
	str_cpy((txtMenuExitGame.pstring)[0], "Exit game");
	str_cpy((txtMenuReturnToWin.pstring)[0], "Return to windows");
	str_cpy((txtMenuBack.pstring)[0], "Back");
	
	wait(1); // Needed so that center_x and center_y work
	
	set(txtMenuNewGame, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuContinueGame, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuLoadGame, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuSaveGame, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuOptions, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuOptionsGame, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuOptionsGraphics, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuOptionsAudio, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuOptionsInput, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuOptionsApply, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuCredits, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuExitGame, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuReturnToWin, CENTER_X | CENTER_Y | OUTLINE);
	set(txtMenuBack, CENTER_X | CENTER_Y | OUTLINE);
	
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
			// Only if there are save games!
			set(txtMenuContinueGame, SHOW);
			set(txtMenuLoadGame, SHOW);
			set(txtMenuOptions, SHOW);
			set(txtMenuCredits, SHOW);
			set(txtMenuReturnToWin, SHOW);
		break;
		case MENU_IN_GAME:
			nCurrentMenu = MENU_IN_GAME;
			set(panInGameMenu, SHOW);
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
			// Set game button to be checked
			button_state(panOptionsMenu, 1, 1);
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
	if (panOptionsMenu != NULL) reset(panOptionsMenu, SHOW);
	if (panOptionsGame != NULL) reset(panOptionsGame, SHOW);
	if (panOptionsGraphics != NULL) reset(panOptionsGraphics, SHOW);
	if (panOptionsAudio != NULL) reset(panOptionsAudio, SHOW);
	if (panOptionsInput != NULL) reset(panOptionsInput, SHOW);
	if (txtMenuOptionsGame != NULL) reset(txtMenuOptionsGame, SHOW);
	if (txtMenuOptionsGraphics != NULL) reset(txtMenuOptionsGraphics, SHOW);
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
			txtMenuContinueGame.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuContinueGame.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y * 1 + MENU_BUTTON_GAP * 1 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuLoadGame.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuLoadGame.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y * 2 + MENU_BUTTON_GAP * 2 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuOptions.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptions.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y * 3 + MENU_BUTTON_GAP * 3 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuCredits.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuCredits.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y * 4 + MENU_BUTTON_GAP * 4 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuReturnToWin.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuReturnToWin.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y * 5 + MENU_BUTTON_GAP * 5 + MENU_BUTTON_SIZE_Y / 2;
		break;
		case MENU_IN_GAME:
			txtMenuContinueGame.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuContinueGame.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuLoadGame.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuLoadGame.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y * 1 + MENU_BUTTON_GAP * 1 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuSaveGame.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuSaveGame.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y * 2 + MENU_BUTTON_GAP * 2 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuOptions.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuOptions.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y * 3 + MENU_BUTTON_GAP * 3 + MENU_BUTTON_SIZE_Y / 2;
			
			txtMenuExitGame.pos_x = panStartMenu.pos_x + MENU_BUTTON_SIZE_X / 2;
			txtMenuExitGame.pos_y = panStartMenu.pos_y + MENU_BUTTON_SIZE_Y * 4 + MENU_BUTTON_GAP * 4 + MENU_BUTTON_SIZE_Y / 2;								
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

void menu_show_message(STRING* _msg) {
	
}

void menu_show_message_fullscreen(STRING* _msg) {
	
}

int menu_show_choice_message(STRING* _msg, STRING* _button1, STRING* _button2) {
	
}