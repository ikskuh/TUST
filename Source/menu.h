#ifndef _MENU_H_
#define _MENU_H_

/**
 * \file menu.h
 * menu.h provides a complete main menu API for your game.
 */
 
#define MENU_BUTTON_SIZE_X 150
#define MENU_BUTTON_SIZE_Y 30
#define MENU_SAVE_SLOT_SIZE_X 150 // For optimal alignment set to the same as MENU_BUTTON_SIZE_X
#define MENU_SAVE_SLOT_SIZE_Y 100
#define MENU_BUTTON_GAP 5
#define MENU_OPTIONS_SIZE_X -1 // -1 = Same size as Options choice bar
#define MENU_OPTIONS_SIZE_Y 380
  
#define MENU_NONE 0
#define MENU_START 1
#define MENU_IN_GAME 2
#define MENU_LOAD 3
#define MENU_SAVE 4
#define MENU_OPTIONS_GAME 5
#define MENU_OPTIONS_GRAPHICS 6
#define MENU_OPTIONS_AUDIO 7
#define MENU_OPTIONS_INPUT 8
#define MENU_CREDITS 9

int nCurrentMenu = 0;
int nMenuVisible = 0;

BMAP* bmapMenuButtonOn			= NULL;
BMAP* bmapMenuButtonOff			= NULL;
BMAP* bmapOptionsChoiceBg		= NULL;
BMAP* bmapOptionsBg				= NULL;

BMAP* bmapSaveSlotOn				= NULL;
BMAP* bmapSaveSlotOff			= NULL;

BMAP* bmapSliderBg				= NULL;
BMAP* bmapSliderKnob				= NULL;

BMAP* bmapCheckBoxCheckedOn	= NULL;
BMAP* bmapCheckBoxCheckedOff	= NULL;
BMAP* bmapCheckBoxUncheckedOn	= NULL;
BMAP* bmapCheckBoxUncheckedOff= NULL;

PANEL* panStartMenu				= NULL;
PANEL* panInGameMenu				= NULL;
PANEL* panLoadMenu				= NULL;
PANEL* panSaveMenu				= NULL;
PANEL* panOptionsMenu			= NULL; // The top bar to switch between option panels
PANEL* panOptionsGame			= NULL;
PANEL* panOptionsGraphics		= NULL;
PANEL* panOptionsAudio			= NULL;
PANEL* panOptionsInput			= NULL;
PANEL* panGraphicMenu			= NULL;
PANEL* panAudioMenu				= NULL;
PANEL* panInputMenu				= NULL;
PANEL* panCreditsMenu			= NULL;

TEXT* txtMenuNewGame				= NULL;
TEXT* txtMenuContinueGame		= NULL;
TEXT* txtMenuLoadGame			= NULL;
TEXT* txtMenuSaveGame			= NULL;
TEXT* txtMenuOptions				= NULL;
	TEXT* txtMenuOptionsGame		= NULL;
		// Game options
		TEXT* txtMenuDifficulty			= NULL;
		TEXT* txtMenuViolence			= NULL;
		TEXT* txtMenuShowDialogs		= NULL;
		TEXT* txtMenuShowHints			= NULL;
	TEXT* txtMenuOptionsGraphics	= NULL;
		TEXT* txtMenuResolutions		= NULL;
		TEXT* txtMenuAntialias			= NULL;
		TEXT* txtMenuBrightness			= NULL;
		TEXT* txtMenuShader				= NULL;
		TEXT* txtMenuShadows				= NULL;
		TEXT* txtMenuDetails				= NULL;
	TEXT* txtMenuOptionsAudio		= NULL;
	TEXT* txtMenuOptionsInput		= NULL;
TEXT* txtMenuOptionsApply		= NULL;
TEXT* txtMenuCredits				= NULL;
TEXT* txtMenuExitGame			= NULL;
TEXT* txtMenuReturnToWin		= NULL;
TEXT* txtMenuBack					= NULL;
TEXT** txtMenuSaveGameTitles	= NULL;




// Dummy settings - to be replaced
var vOptionsDifficulty;
var vOptionsAntiAlias;
var vOptionsResolution;
var vOptionsBrightness;
var vOptionsDetails;


void menu_init();
void menu_show(int _menu);
void menu_hide();
int menu_is_visible();
void menu_align(int _menu);
void menu_center();
void menu_set_pos(int _x, int _y);

void menu_show_message(STRING* _msg);
void menu_show_message_fullscreen(STRING* _msg);
int menu_show_choice_message(STRING* _msg, STRING* _button1, STRING* _button2);



#include "menu.c"
#endif