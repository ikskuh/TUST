// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 210610
//
// title: WED actions list
// class: actions
// type: Editor
// help: has all actions written here to list in WED, for assigning to models
// 
// prefix: ACT_
// idcode: 15
// 
// ENDHEADER
// ---------------------------------------------------------------------

//SKILL2: GRAVITY 15
action ACT_player() {
	// create the player's struct with everything to it.
	init_myPlayer(me);
	
	// everything else...
}

// ---------------------------------------------------------------------
//SKILL2: GRAVITY 15
//SKILL3: WIDTH 40
action ACT_NPC() {
	// create the player's struct with everything to it.
	RunmyNPC(init_myNPC()); // create NPC handling function
	
	// everything else...
}

//SKILL1: MOVESPEED 10
//SKILL2: GRAVITY 15
//SKILL3: WIDTH 40
action ACT_NPC_Walk() {
	// create the player's struct with everything to it.
	RunmymovingNPC(init_myNPC()); // create NPC handling function
	
	// everything else...
}

// ---------------------------------------------------------------------
//STRING1: CAM_NAME
//FLAG8: DO_NOT_USE 0
action ACT_Trig_fade() {
	// create the trigger
	FadeCamTrigger(init_camTrigger());
	
	// anything else...
}

//STRING1: CAM_NAME
//FLAG8: DO_NOT_USE 0
action ACT_Trig_instant() {
	// create the trigger
	InstantCamTrigger(init_camTrigger());
	
	// anything else...
}

//STRING1: CAM_NAME
//SKILL1: CAM_SMOOTH_SPEED 1.1
//FLAG8: DO_NOT_USE 0
action ACT_Trig_smooth() {
	// create the trigger
	SmoothCamTrigger(init_camTrigger());
	
	// anything else...
}

//STRING1: CAM_NAME
//FLAG8: DO_NOT_USE 0
action ACT_Trig_path() {
	// create the trigger
	PathCamTrigger(init_camTrigger());
	
	// anything else...
}

//STRING1: PATH_NAME
//STRING2: FOLLOW_ENT
//FLAG8: DO_NOT_USE 0
action ACT_Trig_pathFollow() {
	// create the trigger
	PathCamFollowTrigger(init_camTrigger());
	
	// anything else...
}

// ---------------------------------------------------------------------

//STRING1: FUNCTION_NAME
action ACT_Event_Trig() {
	// create event trigger setup
	init_eventTrigger(); 
	
	// anything else
}

// ---------------------------------------------------------------------

action ACT_Lookat_object() {
	// create object to look at (without bone update)
	init_lookAtObject(me);
	
	// anything else
}

// ---------------------------------------------------------------------

//SKILL1: WIDTH 40
action ACT_Collision_object() {
	// create object to react to elipsoid collision 
	init_elipsoidCollision(me, my.skill1);
	
	// anything else
}

// ---------------------------------------------------------------------

//SKILL1: RANGE 50
action ACT_Dialogue_object() {
	DS *NPC_Dialogue = DS_getNPCData(my);
	
	ENTITY *getPlayer = MOVE_globalPlayer->Character;
	
	while(me) {
		// check if player is close enough and looking at NPC for dialogue
		if(vec_dist(my.x, getPlayer.x) < my.skill1) {
			if(key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) {
				while(key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) { wait(1); }
				MOVE_globalPlayer->MOVE_gameplayMode = DEF_GAMEPLAY_DIALOGUE;
				DS_start(NPC_Dialogue); wait(1);
				while(NPC_Dialogue->DS_endDS == 0) { wait(1); }
				MOVE_globalPlayer->MOVE_gameplayMode = DEF_GAMEPLAY_MOVEMENT;
			}
		}
		
		wait(1);
	}
}