// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 260810
//
// title: Code to handle custom event triggers
// class: Trigger
// type: Trigger
// help: contains code handling all the trigger related things
// 
// prefix: TRIG_
// idcode: 10
// 
// ENDHEADER
// ---------------------------------------------------------------------

typedef struct {
	ENTITY* tempTrigger;
} TRIGGER;

// variables

//prototypes
// empty prototype function to run other functions from code
void TRIG_runStringFunction();

// includes
#include "trigger.c"

TRIGGER* init_eventTrigger() {
	// initialise memory for new item
	TRIGGER* myTrigger = sys_nxalloc( sizeof(TRIGGER) );
	
	myTrigger->tempTrigger = me;
	runEventTrigger(myTrigger);
	
	return(myTrigger);
}