#ifndef DIALOGS_C
#define DIALOGS_C

// ------------------------------------------------------------------------
// Free the dialog system
// ------------------------------------------------------------------------
void dlg_free()
{
	if (panDialogBg != NULL) {
		ptr_remove(panDialogBg);
		panDialogBg = NULL;
	}
	
	if (txtDialog != NULL) {
		ptr_remove(txtDialog);
		txtDialog = NULL;
	}
	
	if (txtDecisions != NULL) {
		ptr_remove(txtDecisions);
		txtDecisions = NULL;
	}
	
	if (txtSpeaker != NULL) {
		ptr_remove(txtSpeaker);
		txtSpeaker = NULL;
	}
	
	if (panDialogBar1 != NULL) {
		ptr_remove(panDialogBar1);
		panDialogBar1 = NULL;
	}
	
	if (panDialogBar2 != NULL) {
		ptr_remove(panDialogBar2);
		panDialogBar2 = NULL;
	}	
}

// ------------------------------------------------------------------------
// Hide all dialog items and stop all dialog functions
// ------------------------------------------------------------------------
void dlg_hide()
{
	proc_kill((void*)dlg_start);
	if (panDialogBg != NULL) { reset(panDialogBg, SHOW);}
	if (txtDialog != NULL) { reset(txtDialog, SHOW);}
	if (txtDecisions != NULL) { reset(txtDecisions, SHOW);}
	if (txtSpeaker != NULL) { reset(txtSpeaker, SHOW);}
	if (panDialogBar1 != NULL) { reset(panDialogBar1, SHOW);}
	if (panDialogBar2 != NULL) { reset(panDialogBar2, SHOW);}
}

// ------------------------------------------------------------------------
// Create all dialog items (No files required)
// ------------------------------------------------------------------------
void dlg_init()
{
	panDialogBg = pan_create("", 22);
	set(panDialogBg, OVERLAY | TRANSLUCENT | LIGHT);
	vec_set(panDialogBg.blue, vector(0,0,0));

	txtDialog = txt_create(1, 23);
	set(txtDialog, WWRAP);
	txtDialog.font = fontDialogs;
	vec_set(txtDialog.blue, vector(255,255,255));

	txtDecisions = txt_create(4, 23);
	set(txtDecisions, WWRAP);
	txtDecisions.font = fontDialogs;
	vec_set(txtDecisions.blue, vector(255,255,255));

	txtSpeaker = txt_create(1, 23);
	set(txtSpeaker, WWRAP);
	txtSpeaker.font = fontDialogBold;
	vec_set(txtSpeaker.blue, vector(0,0,255));

	panDialogBar1 = pan_create("", 21);
	set(panDialogBar1, OVERLAY | TRANSLUCENT | LIGHT);
	vec_set(panDialogBar1.blue, vector(0,0,0));

	panDialogBar2 = pan_create("", 21);
	set(panDialogBar2, OVERLAY | TRANSLUCENT | LIGHT);
	vec_set(panDialogBar2.blue, vector(0,0,0));

	// Align the dialog based on the screen resolution
	dlg_align();
}

// ------------------------------------------------------------------------
// Align the dialogs
// (Has to be called after every change of the screen resolution)
// ------------------------------------------------------------------------
void dlg_align()
{
	panDialogBg.size_x = screen_size.x;
	panDialogBg.size_y = 130;
	panDialogBg.pos_x = 0;
	panDialogBg.pos_y = screen_size.y - 130 - 110;

	txtDialog.size_x = screen_size.x;
	txtDialog.size_y = 120;
	txtDialog.pos_x = panDialogBg.pos_x + 40;
	txtDialog.pos_y = panDialogBg.pos_y + 40;

	txtDecisions.size_x = screen_size.x;
	txtDecisions.size_y = 120;
	txtDecisions.pos_x = panDialogBg.pos_x + 40;
	txtDecisions.pos_y = panDialogBg.pos_y + 40;	

	txtSpeaker.size_x = 100;
	txtSpeaker.size_y = 30;
	txtSpeaker.pos_x = panDialogBg.pos_x + 40;
	txtSpeaker.pos_y = panDialogBg.pos_y + 30;

	panDialogBar1.size_x = screen_size.x;
	panDialogBar1.size_y = 100;
	panDialogBar1.pos_x = 0;
	panDialogBar1.pos_y = 0;

	panDialogBar2.size_x = screen_size.x;
	panDialogBar2.size_y = 100;
	panDialogBar2.pos_x = 0;
	panDialogBar2.pos_y = screen_size.y - 100;
}

// ------------------------------------------------------------------------
// Read an XML tag with a certain ID
// ------------------------------------------------------------------------
XmlTag* get_dialog_item_by_id(XmlTag* _myXML, int _id)
{
	XmlTag* tempItem;
	XmlAttribute* tempAttr;
	STRING* tempStr = str_create("");
	STRING* tempStr2 = str_create("");
	int i = 0;
	
	tempItem = xml_tag_get_element_by_index(_myXML,i);
	while (tempItem != NULL)
	{
		tempAttr = xml_attribute_get_elements_by_attribute(tempItem, "id");
		if (tempAttr != NULL)
		{
			xml_attribute_get_content(tempAttr,tempStr);
			str_for_num(tempStr2,_id);
			if (str_cmp(tempStr,tempStr2))
			{
				return tempItem;
			}
			i +=1;
			tempItem = xml_tag_get_element_by_index(_myXML,i);
		}
	}
	return NULL;
}

// ------------------------------------------------------------------------
// Get the ID of a tag
// ------------------------------------------------------------------------
int get_dialog_item_id(XmlTag* _XMLItem)
{
	STRING* tempStr = str_create("");
	XmlAttribute* tempAttr;
	tempAttr = xml_attribute_get_elements_by_attribute(_XMLItem, "id");
	if (tempAttr != NULL)
	{
		xml_attribute_get_content(tempAttr,tempStr);
		return str_to_int(tempStr);
	}
}

// ------------------------------------------------------------------------
// Creating a simple dialog
// ------------------------------------------------------------------------
void dlg_start(STRING* _speaker, STRING* _text, SOUND* _audio)
{
	// Cannot start a dialog if another is active
	if (dlg_is_dialog_active()) return;
	nIsDialogActive = 1;
	while(mouse_left) wait(1);  // Or other interaction key
	
	str_cpy((txtSpeaker.pstring)[0], _speaker);
	str_cpy((txtDialog.pstring)[0], _text);
	set(txtSpeaker,SHOW);
	set(panDialogBg,SHOW);
	set(txtDialog,SHOW);

	// If a sound has been defined, play it and wait for the next dialog
	// to start until it is over. Otherwise, wait a certain time based on
	// the length of the text.
	if (_audio != NULL)
	{
		#ifdef OPTIONS_H
			var vTemp = snd_play(_audio, vSpeechVolume, 0);
		#else
			var vTemp = snd_play(_audio, 100, 0);
		#endif
		while(snd_playing(vTemp) != 0)
		{
			wait(1);
			if (mouse_left) // Or other interaction key
			{
				while(mouse_left) wait(1); // Or other interaction key
				break;
			}
		}
	}
	else
	{
		var vWaitTime = str_len(_text) * 30 * time_step;
		while (vWaitTime > 0)
		{
			wait(1);
			if (mouse_left) // Or other interaction key
			{
				while(mouse_left) wait(1); // Or other interaction key
				break;
			}
			vWaitTime -=1;
		}
	}

	reset(txtSpeaker,SHOW);
	reset(panDialogBg,SHOW);
	reset(txtDialog,SHOW);
	nIsDialogActive = 0;
}



// ------------------------------------------------------------------------
// Start a complex XML dialog
// ------------------------------------------------------------------------
int dlg_start(STRING* _dialogFile)
{	
	if (dlg_is_dialog_active()) return;

	if (_dialogFile == NULL) return;
	
	nIsDialogActive = 1;
	
	// 2 black bars on top and bottom of the screen
	panDialogBar1.alpha = 0;
	panDialogBar2.alpha = 0;
	set(panDialogBar1,SHOW);
	set(panDialogBar2,SHOW);
	while(panDialogBar1.alpha < 100)
	{
		panDialogBar1.alpha +=20*time_step;
		panDialogBar2.alpha +=20*time_step;
		wait(1);
	}
	
	// Creating bitmaps for the choice buttons
	bmapDialogBtnDown = bmap_createblack(screen_size.x, 15, 32);
	bmap_fill(bmapDialogBtnDown, vector(0,0,255), 50);
	bmapDialogBtnUp = bmap_createblack(screen_size.x, 15, 32);
	bmap_fill(bmapDialogBtnUp, vector(10,10,10), 50);

	XmlTag *pParHndl, *pPar, *pParMain, *pParGoto, *pParChoice;
	XmlAttribute * pAttrib;
	STRING* strXMLTag = str_create("");
	STRING* strXMLTemp = str_create("");
	STRING* strXMLSpeaker = str_create("");
	STRING* strXMLGotoTarget = str_create("");
	STRING* strChoiceTarget1 = str_create("");
	STRING* strChoiceTarget2 = str_create("");
	STRING* strChoiceTarget3 = str_create("");
	STRING* strChoiceTarget4 = str_create("");
	STRING* returnValue		 = str_create("");
	SOUND* tempSnd = NULL;
	XmlFile* pXml = xml_file_create(_dialogFile);
	pParHndl = xml_file_parse(pXml);

	int i = 0;
	int j = 0;

	// If the XML file could be read...
	if (pParHndl != NULL)
	{

		// Get the root element
		pParMain = xml_tag_get_element_by_tag(pParHndl, "Dialog");

		// get the first element beneath the root element
		pPar = xml_tag_get_element_by_index(pParMain,i);

		// Read the first tag
		xml_tag_get_tag(pPar, strXMLTag);
		
		// As long as the end-element has not been reached...
		while (str_cmp(strXMLTag,"End") != 1)
		{
			
			// Is a sound being played?
			if (snd_playing(vDialogSpeechHandle) > 0)
			{
				// Stop it!
				snd_stop(vDialogSpeechHandle);
			}
			
			// Player says something
			if (str_cmp(strXMLTag,"Player") == 1)
			{

				// Sound file is played
				pAttrib = xml_attribute_get_elements_by_attribute(pPar, "file");
				if (pAttrib != NULL)
				{
					xml_attribute_get_content(pAttrib, strXMLTemp);
					tempSnd = snd_create(strXMLTemp);
					if (tempSnd != NULL)
					{
						#ifdef OPTIONS_H
							vDialogSpeechHandle = snd_play(tempSnd, vSpeechVolume, 0);
						#else
							vDialogSpeechHandle = snd_play(tempSnd, 100, 0);
						#endif
					}
				}

				// Is there a "returnvalue" attribute, set it
				pAttrib = xml_attribute_get_elements_by_attribute(pPar, "returnvalue");
				if (pAttrib != NULL)
				{
					xml_attribute_get_content(pAttrib, returnValue);
				}				

				// Show the text of the dialog element
				xml_tag_get_content(pPar, strXMLTemp);
				str_cpy((txtDialog.pstring)[0],strXMLTemp);

				// ... as well as the player name
				pAttrib = xml_attribute_get_elements_by_attribute(pPar, "name");
				if (pAttrib != NULL)
				{
					xml_attribute_get_content(pAttrib, strXMLTemp);
					str_cpy((txtSpeaker.pstring)[0],strXMLTemp);
				}

				// Show the dialog
				set(txtSpeaker,SHOW);
				set(panDialogBg,SHOW);
				set(txtDialog,SHOW);
				i +=1;

				// Get the next element
				pPar = get_dialog_item_by_id(pParMain,i);
				i = get_dialog_item_id(pPar);
				
				// Continue dialog
				if (snd_playing(vDialogSpeechHandle) > 0)
				{
					while(snd_playing(vDialogSpeechHandle) > 0) wait(1);
				}
				else
				{
					// If the mouse was clicked anywhere on the screen, continue!
					while(mouse_left) wait(1);
					while(!mouse_left) wait(1);
				}
			}
			
			// NPC says something
			if (str_cmp(strXMLTag,"NPC") == 1)
			{

				// Playing a sound
				pAttrib = xml_attribute_get_elements_by_attribute(pPar, "file");
				if (pAttrib != NULL)
				{
					xml_attribute_get_content(pAttrib, strXMLTemp);
					
					tempSnd = snd_create(strXMLTemp);

					if (tempSnd != NULL)
					{
						#ifdef OPTIONS_H
							vDialogSpeechHandle = snd_play(tempSnd, vSpeechVolume, 0);
						#else
							vDialogSpeechHandle = snd_play(tempSnd, 100, 0);
						#endif
					}
				}

				// Change return value
				pAttrib = xml_attribute_get_elements_by_attribute(pPar, "returnvalue");
				if (pAttrib != NULL)
				{
					xml_attribute_get_content(pAttrib, returnValue);
				}


				// Displaying the text
				xml_tag_get_content(pPar, strXMLTemp);
				str_cpy((txtDialog.pstring)[0],strXMLTemp);
				
				// ... and the name of the NPC
				pAttrib = xml_attribute_get_elements_by_attribute(pPar, "name");
				if (pAttrib != NULL)
				{
					xml_attribute_get_content(pAttrib, strXMLTemp);
					str_cpy((txtSpeaker.pstring)[0],strXMLTemp);
				}
				set(txtSpeaker,SHOW);
				set(panDialogBg,SHOW);
				set(txtDialog,SHOW);				
				i +=1;
				pPar = get_dialog_item_by_id(pParMain,i);
				i = get_dialog_item_id(pPar);
				
				// Continue dialog
				if (snd_playing(vDialogSpeechHandle) > 0)
				{
					while(snd_playing(vDialogSpeechHandle) > 0) wait(1);
				}
				else
				{
					// Continue if mouse is clicked
					while(mouse_left) wait(1);
					while(!mouse_left) wait(1);
				}
			}
			
			// Choice box
			if (str_cmp(strXMLTag,"Choice") == 1)
			{
				
				// No decision has been made
				nDecisionsMade = 0;

				// Show the decision panel
				panDecisionBg = pan_create("", 23);
				set(panDecisionBg, OVERLAY | TRANSLUCENT);
				panDecisionBg.size_x = screen_size.x;
				panDecisionBg.size_y = 130;
				panDecisionBg.pos_x = panDialogBg.pos_x;
				panDecisionBg.pos_y = panDialogBg.pos_y;
				
				// Get the first element
				pParChoice = xml_tag_get_element_by_index(pPar,0);

				// If it exists, add a decitions
				if (pParChoice != NULL)
				{
					xml_tag_get_content(pParChoice, strXMLTemp);
					
					// Where do we have to jump if the first element was clicked?
					pAttrib = xml_attribute_get_elements_by_attribute(pParChoice, "target");
					if (pAttrib != NULL)
					{
						xml_attribute_get_content(pAttrib, strChoiceTarget1);
					}

					// Show the text
					str_cpy((txtDecisions.pstring)[0],strXMLTemp);

					// ... and create a button
					pan_setbutton(panDecisionBg,0,1,0,40,bmapDialogBtnDown,bmapDialogBtnUp,bmapDialogBtnDown,NULL,dlg_click_dialog,NULL,NULL);
				}
				else
				{
					str_cpy((txtDecisions.pstring)[0],"");
				}
				
				// A second choice...
				pParChoice = xml_tag_get_element_by_index(pPar,1);
				if (pParChoice != NULL)
				{
					xml_tag_get_content(pParChoice, strXMLTemp);
					pAttrib = xml_attribute_get_elements_by_attribute(pParChoice, "target");
					if (pAttrib != NULL)
					{
						xml_attribute_get_content(pAttrib, strChoiceTarget2);
					}
					str_cpy((txtDecisions.pstring)[1],strXMLTemp);
					pan_setbutton(panDecisionBg,0,1,0,55,bmapDialogBtnDown,bmapDialogBtnUp,bmapDialogBtnDown,NULL,dlg_click_dialog,NULL,NULL);
				}	
				else
				{
					str_cpy((txtDecisions.pstring)[1],"");
				}
					
				// A third choice...
				pParChoice = xml_tag_get_element_by_index(pPar,2);
				if (pParChoice != NULL)
				{
					xml_tag_get_content(pParChoice, strXMLTemp);
					pAttrib = xml_attribute_get_elements_by_attribute(pParChoice, "target");
					if (pAttrib != NULL)
					{
						xml_attribute_get_content(pAttrib, strChoiceTarget3);
					}
					str_cpy((txtDecisions.pstring)[2],strXMLTemp);
					pan_setbutton(panDecisionBg,0,1,0,70,bmapDialogBtnDown,bmapDialogBtnUp,bmapDialogBtnDown,NULL,dlg_click_dialog,NULL,NULL);
				}
				else
				{
					str_cpy((txtDecisions.pstring)[2],"");
				}
				
				// A fourth choice...
				pParChoice = xml_tag_get_element_by_index(pPar,3);
				if (pParChoice != NULL)
				{
					xml_tag_get_content(pParChoice, strXMLTemp);
					pAttrib = xml_attribute_get_elements_by_attribute(pParChoice, "target");
					if (pAttrib != NULL)
					{
						xml_attribute_get_content(pAttrib, strChoiceTarget4);
					}
					str_cpy((txtDecisions.pstring)[3],strXMLTemp);
					pan_setbutton(panDecisionBg,0,1,0,85,bmapDialogBtnDown,bmapDialogBtnUp,bmapDialogBtnDown,NULL,dlg_click_dialog,NULL,NULL);
				}
				else
				{
					str_cpy((txtDecisions.pstring)[3],"");
				}
				
				reset(txtDialog, SHOW);
				reset(txtSpeaker,SHOW);
				set(panDialogBg, SHOW);
				set(panDecisionBg, SHOW);
				set(txtDecisions, SHOW);

				// As long as no decision has been made, wait!
				while(nDecisionsMade == 0)
				{
					wait(1);
				}
				reset(panDecisionBg, SHOW);
				reset(txtDecisions, SHOW);
				
				switch (nDecisionsMade)
				{
					case 1: i = str_to_int(strChoiceTarget1); break;
					case 2: i = str_to_int(strChoiceTarget2); break;
					case 3: i = str_to_int(strChoiceTarget3); break;
					case 4: i = str_to_int(strChoiceTarget4); break;
					default:
						error("Decision not defined!");
				}

				pPar = xml_tag_get_element_by_index(pParMain,i);
			}
			
			
			
			// A "Goto" tells the system to jump to a certain item
			if (str_cmp(strXMLTag,"Goto") == 1)
			{
				pAttrib = xml_attribute_get_elements_by_attribute(pPar, "target");
				if (pAttrib != NULL)
				{
					xml_attribute_get_content(pAttrib, strXMLGotoTarget);
					
					// Find the element with the ID referenced in "target"
					pParGoto = xml_tag_get_element_by_index(pParMain,j);
					pAttrib = xml_attribute_get_elements_by_attribute(pParGoto, "id");
					if (pAttrib != NULL)
					{
						xml_attribute_get_content(pAttrib, strXMLTemp);
						while(str_cmp(strXMLTemp,strXMLGotoTarget) != 1)
						{
							j +=1;
							pParGoto = xml_tag_get_element_by_index(pParMain,j);
							pAttrib = xml_attribute_get_elements_by_attribute(pParGoto, "id");
							if (pAttrib != NULL)
							{
								xml_attribute_get_content(pAttrib, strXMLTemp);
							}
							else break;
						}
					}
				}
				j = 0;
				pPar = pParGoto;

				// Load the referenced item
				i = get_dialog_item_id(pPar);
			}
			
			xml_tag_get_tag(pPar, strXMLTag);	
			
			wait(1);
		}

		// The dialog is finished, clean up!
		reset(panDialogBg,SHOW);
		reset(txtDialog,SHOW);
		reset(panDialogBg, SHOW);
		reset(txtDialog, SHOW);
		reset(txtSpeaker,SHOW);

	}
	xml_file_remove(pXml);

	// Hide the black bars
	while(panDialogBar1.alpha > 0)
	{
		panDialogBar1.alpha -=20*time_step;
		panDialogBar2.alpha -=20*time_step;
		wait(1);
	}
	reset(panDialogBar1,SHOW);
	reset(panDialogBar2,SHOW);
	
	nIsDialogActive = 0;
	
	// "returnValue" is returned if it exists, otherwise return -1
	if (str_cmp(returnValue,"") == 1)
	{
		return -1;
	}
	else
	{
		return str_to_num(returnValue);
	}
}

// ------------------------------------------------------------------------
// Evaluates which button has been clicked
// ------------------------------------------------------------------------
void dlg_click_dialog(var _buttonNumber, PANEL* _panel)
{
	switch (_buttonNumber)
	{
		case 1: nDecisionsMade = 1; break;
		case 2: nDecisionsMade = 2; break;
		case 3: nDecisionsMade = 3; break;
		case 4: nDecisionsMade = 4; break;
	}
}

// ------------------------------------------------------------------------
// Returns if a dialog is active at the moment
// ------------------------------------------------------------------------
int dlg_is_dialog_active()
{
	return nIsDialogActive;
}

#endif