// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 210610
//
// title: NPC code for controlling characters to interact with (Non Player Controlled)
// class: NPC's
// type: NPC
// help: Contains all code for NPC handling
// 
// ENDHEADER
// ---------------------------------------------------------------------

// Prototypes
void RunmyNPC(NPC *tempChar);
void RunmymovingNPC(NPC *tempChar);

var NPC_DialogueDist = 50;
var NPC_MovementDist = 100;

void RunmyNPC(NPC *tempChar) {
	//setup push value to ignore lower objects
	tempChar->Character.push = -1;
	tempChar->Character.group = -1;
	
	ENTITY *getPlayer = MOVE_globalPlayer->Character;

	// run this function as long as the entity still exists and is not dead...
	while (tempChar->Character) {
		// set specific new animations here based on movement input.
		// because idle is setup as default animation, a static NPC doesnt need any animation code setup anymore
		
		// check if player is close enough and looking at NPC for dialogue
		if(vec_dist(tempChar->Character.x, getPlayer.x) < NPC_DialogueDist) {
			if(key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) {
				while(key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) { wait(1); }
				MOVE_globalPlayer->MOVE_gameplayMode = DEF_GAMEPLAY_DIALOGUE;
				DS_start(tempChar->NPC_Dialogue); wait(1);
				while(tempChar->NPC_Dialogue->DS_endDS == 0) { wait(1); }
				MOVE_globalPlayer->MOVE_gameplayMode = DEF_GAMEPLAY_MOVEMENT;
			}
		}
		
		// do gravity check
		GRAV_gravity(tempChar->NPC_Gravity);
		
		wait(1);
	}
}

void RunmymovingNPC(NPC *tempChar) {
	//setup push value to ignore lower objects
	tempChar->Character.push = -1;
	tempChar->Character.group = -1;
	
	STRING *currentAnim = str_create("");
	ENTITY *getPlayer = MOVE_globalPlayer->Character;
	var dist = 0;
	VECTOR lastPos;
	VECTOR direction;

	// run this function as long as the entity still exists and is not dead...
	while (tempChar->Character) {
		// set specific new animations here based on movement input.
		// because idle is setup as default animation, a static NPC doesnt need any animation code setup anymore
		
		// do movement checks
		if(vec_dist(tempChar->Character.x, getPlayer.x) < NPC_MovementDist) { // is player close to me?
			if(!str_cmpi(currentAnim, "idle")) {
				str_cpy(currentAnim, "idle");
				// new animation set to be blending 
				setNewAnimationBlend(tempChar->anim, tempChar->anim->ANIM_default->ANIM_idle);
			}
			
			// check if player is close enough and looking at NPC for dialogue
			if(vec_dist(tempChar->Character.x, getPlayer.x) < NPC_DialogueDist) {
				if(key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) {
					while(key_pressed(key_for_str(INP_globalKeys->INP_keyMapping[4]))) { wait(1); }
					MOVE_globalPlayer->MOVE_gameplayMode = DEF_GAMEPLAY_DIALOGUE;
					DS_start(tempChar->NPC_Dialogue); wait(1);
					while(tempChar->NPC_Dialogue->DS_endDS == 0) { wait(1); }
					MOVE_globalPlayer->MOVE_gameplayMode = DEF_GAMEPLAY_MOVEMENT;
				}
			}
		} else { // if hes not close, move on along path
			if(!str_cmpi(currentAnim, "move")) {
				str_cpy(currentAnim, "move");
				// new animation set to be blending 
				setNewAnimationBlend(tempChar->anim, tempChar->anim->ANIM_default->ANIM_move);
			}
			
			// move vector that camera will follow
			path_spline(tempChar->Character, tempChar->Character.x, dist);
			dist += tempChar->Character.skill1 * time_step;
			// make NPC look ahead on path
			vec_diff(direction, tempChar->Character.x, lastPos);
			vec_to_angle(tempChar->Character.pan, direction);
			vec_set(lastPos, tempChar->Character.x);
		}
		
		// do gravity check
		GRAV_gravity(tempChar->NPC_Gravity);
		
		wait(1);
	}
}