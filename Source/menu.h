#ifndef _MENU_H_
#define _MENU_H_

/**
 * \file menu.h
 * menu.h provides a complete main menu API for your game.
 */
 
#define MENU_BUTTON_SIZE_X 150			// Size for auto generated button images
#define MENU_BUTTON_SIZE_Y 30
#define MENU_SAVE_SLOT_SIZE_X 150		// For optimal alignment set to the same as MENU_BUTTON_SIZE_X
#define MENU_SAVE_SLOT_SIZE_Y 100
#define MENU_SLIDER_SIZE_X 140			// Size of the slider
#define MENU_SLIDER_SIZE_Y 10
#define MENU_KNOB_SIZE_X 15				// Size of the knob for the sliders
#define MENU_KNOB_SIZE_Y 15
#define MENU_CHECKBOX_SIZE_X 15			// Size of Checkboxes
#define MENU_CHECKBOX_SIZE_Y 15
#define MENU_COMBOBOX_SIZE_X 200			// Size of the combobox
#define MENU_COMBOBOX_SIZE_Y 15
#define MENU_KEY_BUTTON_SIZE_X 180		// Button to change the key code
#define MENU_KEY_BUTTON_SIZE_Y 15

#define MENU_BUTTON_GAP 5					// GAPS (X,Y) between buttons in the menues
#define MENU_OPTIONS_SIZE_X -1			// Size of the entire options menu. -1 = Same size as Options choice bar
#define MENU_OPTIONS_SIZE_Y 380

#define MENU_OPTIONS_CAPTION_POS_X 130	// x-distance between option caption and control

/**
 * Defines for the current menu that is visible
 */
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

FONT* fontMenu = "Arial#12b";

int nCurrentMenu			= 0;
int nMenuVisible			= 0;

int nMessageBoxResult	= 0;

BMAP* bmapMenuButtonOn			= NULL;
BMAP* bmapMenuButtonOff			= NULL;
BMAP* bmapOptionsChoiceBg		= NULL;
BMAP* bmapOptionsBg				= NULL;

BMAP* bmapComboboxOn				= NULL;
BMAP* bmapComboboxOff			= NULL;

BMAP* bmapSaveSlotOn				= NULL;
BMAP* bmapSaveSlotOff			= NULL;

BMAP* bmapSliderBg				= NULL;
BMAP* bmapSliderKnob				= NULL;

BMAP* bmapCheckBoxCheckedOn	= NULL;
BMAP* bmapCheckBoxCheckedOff	= NULL;
BMAP* bmapCheckBoxUncheckedOn	= NULL;
BMAP* bmapCheckBoxUncheckedOff= NULL;

BMAP* bmapKeyButtonOn			= NULL;
BMAP* bmapKeyButtonOff			= NULL;

PANEL* panStartMenu				= NULL;
PANEL* panInGameMenu				= NULL;
PANEL* panLoadMenu				= NULL;
PANEL* panSaveMenu				= NULL;
PANEL* panOptionsMenu			= NULL; // The top bar to switch between option panels
PANEL* panOptionsGame			= NULL;
	PANEL* panOptionsGameDifficulty	= NULL;
		TEXT* txtOptionsGameEasy					= NULL;
		TEXT* txtOptionsGameMedium					= NULL;
		TEXT* txtOptionsGameHard					= NULL;
PANEL* panOptionsGraphics		= NULL;
	PANEL* panOptionsGraphicsDetails				= NULL;
		TEXT* txtOptionsGraphicsDetailsLow		= NULL;
		TEXT* txtOptionsGraphicsDetailsMedium	= NULL;
		TEXT* txtOptionsGraphicsDetailsHigh		= NULL;
	PANEL* panOptionsGraphicsAntiAliasing		= NULL;
		TEXT* txtOptionsGraphicsAAOff				= NULL;
		TEXT* txtOptionsGraphicsAA1x				= NULL;
		TEXT* txtOptionsGraphicsAA4x				= NULL;
		TEXT* txtOptionsGraphicsAA9x				= NULL;
	PANEL* panOptionsGraphicsResolutionList	= NULL;
		TEXT* txtResCurrent			= NULL;
		TEXT* txtRes800x600			= NULL;
		TEXT* txtRes1024x768			= NULL;
		TEXT* txtRes1280x1024		= NULL;
		TEXT* txtRes1400x1050		= NULL;
		TEXT* txtRes1600x1200		= NULL;
		TEXT* txtRes1920x1200		= NULL;
PANEL* panOptionsAudio			= NULL;
PANEL* panOptionsInput			= NULL;
PANEL* panGraphicMenu			= NULL;
PANEL* panAudioMenu				= NULL;
PANEL* panInputMenu				= NULL;
	PANEL* panInputMenuKeys		= NULL;
	TEXT** txtInputMenuKeyNames= NULL;
	TEXT** txtInputMenuKeyValues= NULL;
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
		TEXT* txtMenuFullscreen				= NULL;
		TEXT* txtMenuResolutions			= NULL;
		TEXT* txtMenuAntialias				= NULL;
		TEXT* txtMenuBrightness				= NULL;
		TEXT* txtMenuCurrentBrightness	= NULL;
		TEXT* txtMenuShader					= NULL;
		TEXT* txtMenuShadows					= NULL;
		TEXT* txtMenuDetails					= NULL;
	TEXT* txtMenuOptionsAudio		= NULL;
		TEXT* txtMenuMusicVolume			= NULL;
		TEXT* txtMenuMusicVolumeTest		= NULL;
		TEXT* txtMenuSpeechVolume			= NULL;
		TEXT* txtMenuSpeechVolumeTest		= NULL;
		TEXT* txtMenuEffectsVolume			= NULL;
		TEXT* txtMenuEffectsVolumeTest	= NULL;
	TEXT* txtMenuOptionsInput		= NULL;
		TEXT* txtMenuMouseSensivity	= NULL;
TEXT* txtMenuOptionsApply		= NULL;
TEXT* txtMenuCredits				= NULL;
TEXT* txtMenuExitGame			= NULL;
TEXT* txtMenuReturnToWin		= NULL;
TEXT* txtMenuBack					= NULL;
TEXT** txtMenuSaveGameTitles	= NULL;

PANEL* panMessageBoxBg			= NULL;
PANEL* panMessageBox				= NULL;
TEXT* txtMessageBoxCaption		= NULL;
TEXT* txtMessageBoxBtn1			= NULL;
TEXT* txtMessageBoxBtn2			= NULL;

SOUND* sndMusicTest				= "..\\Ressources\\Audio\\Sounds\\beep1.wav";
SOUND* sndSpeechTest				= "..\\Ressources\\Audio\\Sounds\\beep1.wav";
SOUND* sndEffectsTest			= "..\\Ressources\\Audio\\Sounds\\beep1.wav";


// Variables to hold a game state until changes are applied
var vNewGameDifficulty			= 0;
var vNewGameBlood					= 0;
var vNewGameShowDialogs			= 0;
var vNewGameShowHints			= 0;

var vNewGameResolution			= 7;
var vNewGameAntiAlias			= 0;
var vNewGameBrightness			= 80;
var vNewGameShaders				= 0;
var vNewGameShadows				= 0;
var vNewGameDetails				= 1;
var vNewGameFullscreen			= 0;

var vNewGameMusicVolume			= 0;
var vNewGameSpeechVolume		= 0;
var vNewGameEffectsVolume		= 0;

var vNewGameMouseSensitivity	= 0;


/**
 * Initializes all menues (GUI creation, BMAP creation, STRING loading, ...)
 */
void menu_init();

/**
 * Shows one of the defined menues
 * \param	int	Type of menu -> see defines above
 */
void menu_show(int _menu);

/**
 * Hides all menues.
 */
void menu_hide();

/**
 * Is any menue visible?
 * \return			1 if any menu is visible, 0 if not
 */
int menu_is_visible();

/**
 * Fits a menu to the screen resolution
 * \param	int	Menu that should be aligned.
 */
void menu_align(int _menu);

/**
 * Centers all menues corresponding to the screen.
 */
void menu_center();

/**
 * Moves the menu to a certain position
 * \param	int	x-position of all menues
 * \param	int	y-position of all menues
 */
void menu_set_pos(int _x, int _y);

/**
 * Shows a message window that covers all other elements on screen.
 * \param	STRING*	Message to be shown
 * \param	STRING* 	Message on the button (If NULL, no button is shown)
 */
void menu_show_message(STRING* _msg, STRING* _button);

/**
 * Shows a message box with 2 choices
 * \param	STRING*	Message to be shown
 * \param	STRING*	Caption of the first button.
 * \param	STRING*	Caption of the second button.
 * \return			0 if first button was clicked, 1 if second
 */
int menu_show_choice_message(STRING* _msg, STRING* _button1, STRING* _button2);

/**
 * Hide the message box programatically
 */
void menu_hide_message();

/**
 * Is a messaage box visible?
 * \return	0 if messagebox is not visible, 1 if it is
 */
int menu_message_visible();

/**
 * Get the last message box result
 * \return	1 if first key pressed, 2 if second, 0 if message box was closed
 */
int menu_message_result();



// Internal functions

/**
 * Shows the resolution drop down
 */
void menu_resolution_show();

/**
 * Click event of a resolution button.
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
void menu_resolution_click(var _button_number, PANEL* _panel);


/**
 * Click on the game, graphics, audio, input menu
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
void menu_options_choose(var _button_number, PANEL* _panel);

/**
 * Apply button clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
void menu_apply_click(var _button_number, PANEL* _panel);

/**
 * Back button clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
void menu_back_click(var _button_number, PANEL* _panel);

/**
 * A button on the start menu was clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
void menu_start_click(var _button_number, PANEL* _panel);

/**
 * A button on the ingame menu was clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
void menu_ingame_click(var _button_number, PANEL* _panel);

/**
 * A button on the difficulty panel was clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
void menu_difficulty_click(var _button_number, PANEL* _panel);

/**
 * A button on the anti aliasing panel was clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
void menu_anti_aliasing_click(var _button_number, PANEL* _panel);

/**
 * A button on the details panel was clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
void menu_details_click(var _button_number, PANEL* _panel);

/**
 * Initializes message boxes (is called in menu_init())
 */
void menu_message_box_init();

/**
 * A button on of of the message boxes was clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
void menu_message_box_click(var _button_number, PANEL* _panel);

/**
 * A button on of of the audio test boxes was clicked
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
void menu_audio_test_box_click(var _button_number, PANEL* _panel);

/**
 * Change an input key
 * \param	var	Button number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
void menu_input_key_click(var _button_number, PANEL* _panel);

#include "menu.c"
#endif