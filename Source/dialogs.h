#ifndef DIALOGS_H
#define DIALOGS_H

#include "xmlreader.h"

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

void dlg_init();
void dlg_free();
void dlg_hide();
void dlg_align();
void dlg_click_dialog(var _buttonNumber, PANEL* _panel);
int dlg_is_dialog_active();
XmlTag* get_dialog_item_by_id(XmlTag *_myXML, int _id);
int get_dialog_item_id(XmlTag* _XMLItem);
int dlg_start(STRING* _dialogFile);
void dlg_start(STRING* _speaker, STRING* _text, SOUND* _audio);

#include "dialogs.c"

#endif