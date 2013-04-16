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
// ENDHEADER
// ---------------------------------------------------------------------

// Prototypes
DS *DS_getNPCData(ENTITY *DSCharacter);

var answer_box_offset = 20; // offset in Y pixels (from the top down)
var answer_box_height = 16; // offset between eacht answerbox (from the top down)
var answer_box_side_distance = 28; // offset in X pixels (from left to right)
var answer_select_side_distance = 0; // offset in X pixels for the answer selection panel
var out_text_offset = 23; // offset in X pixels for the dialogue text (offset is added to answer_box_side_distance)

DS *DS_getNPCData(ENTITY *DSCharacter) {
	var filehandle_n = file_open_read(DSCharacter.string1);
	
	file_find(filehandle_n, DSCharacter.string2); // get current Character
	
	file_find(filehandle_n, "Dialogue file: "); // get dialogue file
	STRING* DSdatafile = str_create("");
	file_str_read(filehandle_n, DSdatafile);
	
	file_find(filehandle_n, "Dialogue start: "); // get dialogue number
	STRING* DSdatanum = str_create("");
	file_str_read(filehandle_n, DSdatanum);
	var DSnumber = str_to_num(DSdatanum);
	
	file_find(filehandle_n, "Dialogue speed: "); // get dialogue speed
	STRING* DSdataspeed = str_create("");
	file_str_read(filehandle_n, DSdataspeed);
	var DSspeed = str_to_num(DSdataspeed);
	
	file_find(filehandle_n, "Dialogue delay: "); // get dialogue delay
	STRING* DSdatadelay = str_create("");
	file_str_read(filehandle_n, DSdatadelay);
	var DSdelay = str_to_num(DSdatadelay);
	
	DS *DS_myTempDS = init_myDialogue(DSdatafile, DSnumber, DSspeed, DSdelay);
	return(DS_myTempDS);
}