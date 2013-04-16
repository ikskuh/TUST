// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 210610
//
// title: User key input code
// class: Input
// type: Player
// help: gets the key mapping, and stores it in a globalvariable
// 
// prefix: INP_
// idcode: 5
// 
// ENDHEADER
// ---------------------------------------------------------------------

// create input struct
typedef struct {
	STRING *INP_keyMapping[8];
} INPUT;

// all function prototypes
INPUT *init_globalKeys();
void getKeyMapping(INPUT *tempKeys);

//includes
#include "input.c"

INPUT *init_globalKeys() {
	INPUT *tempGlobalKeys = sys_malloc( sizeof(INPUT) );
	
	getKeyMapping(tempGlobalKeys);
	
	return(tempGlobalKeys);
}

// functions
void getKeyMapping(INPUT *tempKeys) {
	var fhandle_n = file_open_read("Data/keyMapping.ini"); // open file
	STRING* getCurrentKey = str_create("");
	
	var i;
	// empty mapArray
	for(i=0; i<8; i++) {
		tempKeys->INP_keyMapping[i] = str_create("");
	}
	
	// get forward movement key
	file_find(fhandle_n, "Move forward: ");
	file_str_read(fhandle_n, getCurrentKey); // get key
	str_cpy(tempKeys->INP_keyMapping[0], getCurrentKey); // store keystring
	
	// get forward backward key
	file_find(fhandle_n, "Move backward: ");
	file_str_read(fhandle_n, getCurrentKey); // get key
	str_cpy(tempKeys->INP_keyMapping[1], getCurrentKey); // store keystring
	
	// get right movement key
	file_find(fhandle_n, "Move left: ");
	file_str_read(fhandle_n, getCurrentKey); // get key
	str_cpy(tempKeys->INP_keyMapping[2], getCurrentKey); // store keystring
	
	// get right movement key
	file_find(fhandle_n, "Move right: ");
	file_str_read(fhandle_n, getCurrentKey); // get key
	str_cpy(tempKeys->INP_keyMapping[3], getCurrentKey); // store keystring
	
	// get right movement key
	file_find(fhandle_n, "Interaction: ");
	file_str_read(fhandle_n, getCurrentKey); // get key
	str_cpy(tempKeys->INP_keyMapping[4], getCurrentKey); // store keystring
	
	// get right movement key
	file_find(fhandle_n, "Inventory: ");
	file_str_read(fhandle_n, getCurrentKey); // get key
	str_cpy(tempKeys->INP_keyMapping[5], getCurrentKey); // store keystring
	
	// get right movement key
	file_find(fhandle_n, "Menu: ");
	file_str_read(fhandle_n, getCurrentKey); // get key
	str_cpy(tempKeys->INP_keyMapping[6], getCurrentKey); // store keystring
	
	// 8th key (tempKeys->INP_keyMapping[7]) is not used yet...
	
	file_close(fhandle_n); // close file
}