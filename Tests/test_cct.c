#include <acknex.h>
#include <default.c>

// include physX:
#include "..\\Source\\physX3\\ackphysX3.h"

// include cct template:
#include "..\\Source\\cct.h"


STRING* levelStr = "test_cct\\map01.wmb";

// main game function:
function main(){
	video_set(1024, 768, 16, 0); 
	video_aspect = 1.333;

	// init physX:
	physX3_open();

	level_load(levelStr);
}