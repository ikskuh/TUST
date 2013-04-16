// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 050810
//
// title: Dialogue System (DS) code to handle
// class: Dialogue System
// type: DS
// help: Contains all code for Dialogue handling
// 
// prefix: DS_
// idcode: 7
// 
// ENDHEADER
// ---------------------------------------------------------------------

// create npc struct
typedef struct {
	STRING *DS_fileName;
	
	var DS_number;
	var DS_speed;
	var DS_delay;
	
	int DS_skipDS;
	int DS_answerDone;
	var DS_startNumber;
	int DS_endDS;
} DS;

// prototype functions
DS *init_myDialogue(STRING* file, var number, var speed, var delay);
void DS_typewriter(DS *tempDS, STRING *outStr, STRING *displayStr);
STRING *replace_char_DS(STRING *outStr, STRING *searchStr, STRING *replaceStr);
void DS_start(DS *tempDS);

// empty prototype function to run other functions from code
void DS_runStringFunction();

// includes
#include "dialogue.c"

// new definition of previously created struct
DS *init_myDialogue(STRING* file, var number, var speed, var delay) {
	// initialise memory for new item
	DS *myDS = sys_nxalloc( sizeof(DS) );
//	memset(myDS, 0, sizeof(DS) );
	
	// temp settings...
	myDS->DS_fileName = str_create("");
	str_cpy(myDS->DS_fileName, file);
	myDS->DS_number = number;
	myDS->DS_speed = speed;
	myDS->DS_delay = delay;
	
	myDS->DS_skipDS = 0;
	myDS->DS_answerDone = 0;
	myDS->DS_startNumber = myDS->DS_number;
	myDS->DS_endDS = 0;
	
	// return pointer
	return(myDS);
}

void DS_typewriter(DS *tempDS, STRING *outStr, STRING *displayStr) {
	STRING* typewriterStr = str_create(""); // Typewriter effect store string

	// get initial length of string
	var lengthStr = str_len(outStr);
	var skipped = 0;
	
	// empty typewriter string
	str_cpy(typewriterStr, outStr);
	
	// as long as the string hasnt reached it's end...
	while(lengthStr >= 0) {
		if(tempDS->DS_delay == 0) { break; }
		// when player skips through
		if(key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) { tempDS->DS_skipDS = 1; break; }
		
		str_cpy(typewriterStr, outStr); // paste input string
		str_trunc(typewriterStr, lengthStr); // cut off last characters
		str_cpy(displayStr, typewriterStr); // paste input into output string (only external used string)
		
		lengthStr -= tempDS->DS_speed; // decrease character cut-off amount
		
		wait(tempDS->DS_delay);
	}
	
	// copy full string over
	str_cpy(displayStr, outStr);
	
	ptr_remove(typewriterStr);
}

STRING *replace_char_DS(STRING *outStr, STRING *searchStr, STRING *replaceStr) {
	STRING* replaceStrStart = str_create(""); // first part of the dialogue text
	STRING* replaceStrEnd = str_create(""); // last part of the dialogue...
	
	var tempNum; // untill break
	var tempLen; // whole length
	var firstNum; // break number
	
	while(1) {
		// search the string for the next characters
		tempNum = str_stri(outStr, searchStr);
		
		// if not end of text reached...
		if(tempNum!=0) {
			str_cpy(replaceStrStart, outStr); // paste input string
			str_cpy(replaceStrEnd, outStr); // paste input string
			
			tempNum = str_stri(outStr, searchStr);
			tempLen = str_len(outStr);
			
			// cut off last strings part
			firstNum = tempLen - (tempNum-1);
			str_trunc(replaceStrStart, firstNum);
			str_cat(replaceStrStart, replaceStr); // paste correct player string
			
			// paste end back
			str_clip(replaceStrEnd, (tempNum+1));
			str_cat(replaceStrStart, replaceStrEnd);
			
			str_cpy(outStr, replaceStrStart);
		} else {
			break;
		}
	}
	
	ptr_remove(replaceStrStart);
	ptr_remove(replaceStrEnd);
	
	return(outStr);
}

void DS_processScript(DS *tempDS) {
	var DShandle_n; // variable to save dialogue file handle into
	
	STRING* numStr = str_create(""); // empty string to open up dialogue
	STRING* outputStr = str_create(""); // empty string to store 
	
	// first create a finder string that retrieves the correct string
	str_cpy(numStr, "$");
	str_cat(numStr, str_for_num(NULL, tempDS->DS_number));
	str_cat(numStr, ".script: ");
	
	//open the file to read out the string starting with the previous made string
	DShandle_n = file_open_read(tempDS->DS_fileName);
	result = file_find(DShandle_n, numStr);
	
	if(result > 0) {
		// copy found string into output string
		file_str_read(DShandle_n, outputStr);
		
		// execute function that's written here
		DS_runStringFunction = engine_getscript(_chr(outputStr));
		if(DS_runStringFunction) { DS_runStringFunction(); }
	}
	file_close(DShandle_n);
	
	ptr_remove(numStr);
	ptr_remove(outputStr);
}

void DS_getNextDS(DS *tempDS, var answer) {
	STRING* numStr = str_create("");  // string to number conversion
	STRING* outputStr = str_create(""); // output (for each answer) string
	
	var DShandle_n;
	var tempNum;
	var nextDialogue;
	
	// first create a finder string that retrieves the correct string
	str_cpy(numStr, " $");
	str_cat(numStr, str_for_num(NULL, tempDS->DS_number));
	str_cat(numStr, ".");
	str_cat(numStr, str_for_num(NULL, answer));
	str_cat(numStr, ", ");
	
	// read dialogue text for next speech after the answer was given
	DShandle_n = file_open_read(tempDS->DS_fileName);
	result = file_find(DShandle_n, numStr);
	if(result < 0) { file_close(DShandle_n); return; }
	
	file_str_read(DShandle_n, outputStr);
	tempNum = str_stri(outputStr, "\\")+1;
	
	str_clip(outputStr, (tempNum));
	nextDialogue = str_to_num(outputStr);
	
	// close the file
	file_close(DShandle_n);
	
	tempDS->DS_number = nextDialogue;
	
	ptr_remove(numStr);
	ptr_remove(outputStr);
}

void DS_drawAnswer(DS *tempDS, STRING* answerOutputStr, var posX, var posY) {
	// create empty panel for the answers background...
//	PANEL* displayAnswerPan = pan_create("bmap = answerBG; flags = VISIBLE | OUTLINE;", 0);
//	displayAnswerPan.pos_x = posX;
//	displayAnswerPan.pos_y = posY;
	STRING* displayStr = str_create("");
	str_cpy(displayStr, answerOutputStr);

	// create display answer text
	TEXT *displayText = txt_create(1, 5);
	displayText.pos_x = 10;
	displayText.pos_y = 10;
	set(displayText, SHOW);
	
	// create empty answer string
	//STRING* displayAnswerStr = str_create(answerOutStr);
//	str_cpy(answerOutStr, answerInStr);
	
	displayText.pos_x = posX + out_text_offset;
	displayText.pos_y = posY;
	
//	set(answerOutTXT, VISIBLE);
	DS_typewriter(tempDS, displayStr, (displayText->pstring)[0]);
	
	// as long as there is no answer back
	while(tempDS->DS_answerDone == 0) {	wait(1); }
	
//	reset(answerOutTXT, VISIBLE);
	
	ptr_remove(displayStr);
	ptr_remove(displayText);
}

void DS_handleAnswer(DS *tempDS) {
	STRING* numStr = str_create("");  // string to number conversion
	STRING* outputStr = str_create(""); // output (for each answer) string
	
	var DShandle_n;
	
	var tempNum;
	var tempLen;
	
	var answer = 1;
	var tempBttn = 1; // stores what answer you are giving
	
	var nextDialogue;
	
	DShandle_n = file_open_read(tempDS->DS_fileName);
	// get and print answers
	while(1) {
		// first create a finder string that retrieves the correct string
		str_cpy(numStr, " $");
		str_cat(numStr, str_for_num(NULL, tempDS->DS_number));
		str_cat(numStr, ".");
		str_cat(numStr, str_for_num(NULL, answer));
		str_cat(numStr, ", ");
		
		result = file_find(DShandle_n, numStr);
		if(result < 0) { break; }
		
		answer += 1;
		
		file_str_read(DShandle_n, outputStr);
		tempNum = str_stri(outputStr, "\\")-2;
		tempLen = str_len(outputStr);
		
		// make sure the delimiter string is set to something else then ","
		// that screwed me over initially!
		str_trunc(outputStr, (tempLen - tempNum));
		
		// if dialogue is set to continue (without answer)
		if(str_cmpi(outputStr, "-->") == 1) {
			if(tempDS->DS_skipDS == 0){
				while(!key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) { wait(1); } // while key enter is not pressed, wait
				while(key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) { wait(1); }
			} else {
				while(key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) { wait(1); }
				while(!key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) { wait(1); } // while key enter is not pressed, wait
				while(key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) { wait(1); }
			}
			
			tempDS->DS_answerDone = 1;
			nextDialogue = 1;
			
			wait(1); // wait a single frame for all other functions to stop executing based on above variables
			break;
		}
		
		// call a routine function to replace some characters
		outputStr = replace_char_DS(outputStr, "/p", "Dennis");
		outputStr = replace_char_DS(outputStr, "/n", "
"); // hack?
		
		// check current answer and paste the right string in that
		DS_drawAnswer(tempDS, outputStr, answer_box_side_distance, (answer*answer_box_height)+answer_box_offset);
		// wait_for(DS_typewriter);
	}
	
	file_close(DShandle_n);
	
	if(tempDS->DS_skipDS == 1){
		while(key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) { wait(1); }
	}
	
	if(answer <= 1){  // marks the FINAL end of the dialogue
		while(!key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) { wait(1); } // while key enter is not pressed, wait
		while(key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) { wait(1); }
		
//		reset(questionTXT, VISIBLE); // turn off the dialogue display (should later be a function for panels as well)
//		reset(Dialogue_bg, VISIBLE); // same for the bg panel...
		
		//make sure the player image is reset before engaging another dialogue:
//		currentNPCimage = none;
//		dialogue_image_update(); // set panels off via this function
		
		tempDS->DS_number = tempDS->DS_startNumber;
		tempDS->DS_endDS = 1;
	}
	
	// add end of questions panel dynamically
//	PANEL* displayAnswerBotPan = pan_create("bmap = answerBG_bottom; flags = VISIBLE;", 1);
//	displayAnswerBotPan.pos_x = answer_box_side_distance;
//	displayAnswerBotPan.pos_y = (answer*answer_box_height)+answer_box_offset;
	
//	set(DS_answer, VISIBLE);
	// wait as long as space is still pressed, so you cant accidentally skip this with the wrong answer
	
	while(tempDS->DS_answerDone == 0 && tempDS->DS_endDS == 0) {
		// do routine to check what button is pressed
		if(key_w){ while(key_w){wait(1);} tempBttn-=1; }
		if(key_s){ while(key_s){wait(1);} tempBttn+=1; }
		tempBttn = cycle(tempBttn, 1, answer);
		
//		DS_answer.pos_x = answer_select_side_distance;
//		DS_answer.pos_y = answer_box_offset+(tempBttn*answer_box_height);
		draw_text("-->", answer_select_side_distance + answer_box_offset, answer_box_offset + (tempBttn * answer_box_height) + 7, vector(255,255,255));
		
		// continue dialoge when enter is pressed
		if(key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))){ 
			while(key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) { wait(1); } 
			tempDS->DS_answerDone = 1; 
		}
		
		// set next dialogue to the answer given
		nextDialogue = tempBttn;
		
		wait(1);
	}
	
	// remove EOD panel again!
//	ptr_remove(displayAnswerBotPan);
	
	// set dialogue text off...
//	reset(DS_answer, VISIBLE);
	tempDS->DS_skipDS = 0;
	
	if(tempDS->DS_endDS == 0) {
		DS_getNextDS(tempDS, nextDialogue);
		
		// start new dialogue
		DS_start(tempDS);
	}
	
	ptr_remove(numStr);
	ptr_remove(outputStr);
}

// function that initialises and start's off the dialogue
void DS_start(DS *tempDS) {
	var DShandle_n; // variable to save dialogue file handle into
	
	var tempNum;
	var tempLen;
	
	STRING *numStr = str_create(""); // empty string to open up dialogue
	STRING *tempStr = str_create(""); // temporary storage string
	STRING *outputStr = str_create(""); // empty string to store
	
	tempDS->DS_answerDone = 0;
	tempDS->DS_endDS = 0;
	
	// turn on the dialogue text and panel
	// (create the panel dynamically)
	TEXT *displayText = txt_create(1, 5);
	displayText.pos_x = 10;
	displayText.pos_y = 10;
	set(displayText, SHOW);
	
	// first create a finder string that retrieves the correct string
	str_cpy(numStr, "$");
	str_for_num(tempStr, tempDS->DS_number);
	str_cat(numStr, tempStr);
	str_cat(numStr, ", ");
	
	//open the file to read out the string starting with the previous made string
	DShandle_n = file_open_read(tempDS->DS_fileName);
	file_find(DShandle_n, numStr);
	
	// copy found string into output string
	file_str_read(DShandle_n, outputStr);
	file_close(DShandle_n);
	
	// call a routine function to replace some characters
	outputStr = replace_char_DS(outputStr, "/p", "Dennis");
	outputStr = replace_char_DS(outputStr, "/n", "
"); // hack?
	
	// process image and script first:
//	process_image(number);
	DS_processScript(tempDS);

	// make the output string come up with a typewriter effect
	DS_typewriter(tempDS, outputStr, (displayText->pstring)[0]);
	wait_for(DS_typewriter);
	
	// check for answers	
	DS_handleAnswer(tempDS);
	
	// as long as there is no answer back
	while(tempDS->DS_answerDone == 0 && tempDS->DS_endDS == 0) { wait(1); }
	
	// remove pointers (etc) here
	ptr_remove(numStr);
	ptr_remove(tempStr);
	ptr_remove(outputStr);
	ptr_remove(displayText);
}