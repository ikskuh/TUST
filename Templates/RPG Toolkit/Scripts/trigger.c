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
// ENDHEADER
// ---------------------------------------------------------------------

// test
void testTrigFunc() {
	proc_mode = PROC_GLOBAL;
	
//	error("it's being called properly");
	wait(-5);
//	error("even when the entity is gone, this still works");
	
	// end test
}

// event when player hits trigger
void eventTrigger() {
	if (event_type == EVENT_PUSH) {
		// set trigger to be active (only if you was the player)
		if(you == MOVE_globalPlayer->Character) {
			set(my, FLAG8);
		}
	}
}

void runEventTrigger(TRIGGER* myTrigger) {
	c_setminmax(myTrigger.tempTrigger);
	
	// set push value lower then player to make sure of no collision
	myTrigger->tempTrigger.push = -5;
	
	// sensible for push collision
	set(myTrigger->tempTrigger, INVISIBLE);
	
	// set trigger to be able to get triggered by player (using push values)
	myTrigger->tempTrigger.emask |= ENABLE_PUSH;
	myTrigger->tempTrigger.event = eventTrigger;
	
	while(myTrigger->tempTrigger) {		
		// when trigger became active
		if(is(myTrigger->tempTrigger, FLAG8)) {
			// run function that is written under string 1 in the triggers action
			TRIG_runStringFunction = engine_getscript(myTrigger->tempTrigger.string1);
			if(TRIG_runStringFunction) { TRIG_runStringFunction(); }
			break;
		}
		
		wait(1);
	}
	
	set(myTrigger->tempTrigger, PASSABLE); // no colliding with ragdolls etc
	myTrigger->tempTrigger.emask |= NULL;
	myTrigger->tempTrigger.event = NULL;
	
	// remove the trigger to prevent calling the function several times
	ent_remove(myTrigger->tempTrigger);
	sys_free(myTrigger);
}