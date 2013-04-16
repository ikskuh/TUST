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
// ENDHEADER
// ---------------------------------------------------------------------

// this define stores the global keyinput, which reads the data
INPUT *INP_globalKeys;

void getGlobalKeymapping() {
	INP_globalKeys = init_globalKeys();
}