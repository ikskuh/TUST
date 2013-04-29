#ifndef _DIALOGS_H_
#define _DIALOGS_H_

#include "xmlreader.h"

/**
 * \file dialogs.h
 * \Functions for in game dialogs
 *
 * Provides in general two functions to start dialogs and all the initialization stuff.
 */

BMAP* bmapDialogBtnDown = NULL;
BMAP* bmapDialogBtnUp = NULL;

int nDecisionsMade = 0;

FONT* fontDialogs = "Arial#14";
FONT* fontDialogBold = "Arial#14b";

PANEL* panDialogBg;
PANEL* panDialogBar1;
PANEL* panDialogBar2;
TEXT* txtDialog;
PANEL* panDecisionBg;
TEXT* txtDecisions;
TEXT* txtSpeaker;

var vDialogSpeechHandle = 0;

int nIsDialogActive = 0;

/**
 * Initializes the dialog system
 */
void dlg_init();

/**
 * Removes dialog GUI, ... from memory
 */
void dlg_free();

/**
 * Hides all dialog GUI elements
 */
void dlg_hide();

/**
 * Aligns all GUI elements to the screen
 */
void dlg_align();

/**
 * A button the the dialog panel was clicked
 * \param	var	Number of clicked button
 * \param	PANEL*	Panel that contains the button
 */
void dlg_click_dialog(var _buttonNumber, PANEL* _panel);

/**
 * Evaluates if dialog is running
 * \return			1 if a dialog is visible, 0 if none
 */
int dlg_is_dialog_active();

/**
 * Shows a message box with 2 choices
 * \param	XmlTag*	The XML object
 * \param	int	The ID of the desired XML tag
 * \return			The XML tag with the specific ID
 */
XmlTag* get_dialog_item_by_id(XmlTag *_myXML, int _id);

/**
 * Gets the ID from a dialog item
 * \param	XmlTag*	The XML tag to extract the ID from
 * \return			The desired ID
 */
int get_dialog_item_id(XmlTag* _XMLItem);

/**
 * Starts a dialog from an XML file
 * \param	STRING*	File to the XML
 * \return			Returns an integer that reports the result of the dialog (choosen by programmer)
 */
int dlg_start(STRING* _dialogFile);

/**
 * Starts a one line dialog
 * \param	STRING*	Who is speaking?
 * \param	STRING*	What is he saying?
 * \param	SOUND*	Should a sound be played? (May be NULL)
 */
void dlg_start(STRING* _speaker, STRING* _text, SOUND* _audio);

#include "dialogs.c"

#endif