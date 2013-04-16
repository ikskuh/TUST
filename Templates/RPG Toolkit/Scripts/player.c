// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 060610
//
// title: Player Movement code
// class: Movement
// type: Player
// help: Handles the movement of the player
//
// ENDHEADER
// ---------------------------------------------------------------------

// Prototypes
void RunmyChar(PLAYER *tempChar);
void moveMyChar_cameraBased(PLAYER *tempChar, var cameraAngle);

void RunmyChar(PLAYER *tempChar) {
	//setup push value to ignore lower objects
	tempChar->Character.push = 1;
	tempChar->Character.group = 1;
	
	STRING *currentAnim = str_create("");
	var lastFrameHeading = 0;
	var cameraRotateAngle;
	
	set(tempChar->Character, SHADOW);

	// run this function as long as the entity still exists and is not dead...
	while (tempChar->Character) {
		// get input from player
		if(tempChar->MOVE_gameplayMode == DEF_GAMEPLAY_MOVEMENT) {
			getInput(tempChar);
			
			// check the players previous heading, and if he pressed any movement keys
			if((lastFrameHeading != tempChar->MOVE_curHeading) || (tempChar->MOVE_pressedKey == 0)) {
				cameraRotateAngle = camera.pan; // update camera angle
			}
			
			// character only moves to a new heading when the player pressed a different key then previously
			moveMyChar_cameraBased(tempChar, cameraRotateAngle); // move player based on camera angle
			// store last heading for the player
			lastFrameHeading = tempChar->MOVE_curHeading;
		}
		
		// set specific new animations here based on movement input.
		if(tempChar->MOVE_pressedKey == 1 && tempChar->MOVE_gameplayMode == DEF_GAMEPLAY_MOVEMENT) {
			if(!str_cmpi(currentAnim, "move")) {
				str_cpy(currentAnim, "move");
				// new animation set to be blending 
				setNewAnimationBlend(tempChar->anim, tempChar->anim->ANIM_default->ANIM_move);
			}
		} else {
			if(!str_cmpi(currentAnim, "idle")) {
				str_cpy(currentAnim, "idle");
				// new animation set to be blending 
				setNewAnimationBlend(tempChar->anim, tempChar->anim->ANIM_default->ANIM_idle);
			}
		}
		
		// do gravity check
		GRAV_gravity(tempChar->MOVE_Gravity);
		
		wait(1);
	}
}

// functions used for different movement styles
// based on the rotation of the camera, make player move around
void moveMyChar_cameraBased(PLAYER *tempChar, var cameraAngle) {
	VECTOR tempCamRot;
	var rotationSmooth = 0.95; // change for different rotation speed update
	
	vec_set(tempChar->MOVE_playerRot, cameraAngle); // get camera rotation
	
	if(tempChar->MOVE_pressedKey) {
		// if player pressed a key to move around...
		vec_set(tempCamRot, tempChar->Character.pan); // store current rotation
		vec_add(tempChar->MOVE_playerRot, vector(tempChar->MOVE_curHeading, 0, 0)); // add keyheading press to the player rotation
		
		// update rotations..
		ang_lerp(tempChar->Character.pan, tempChar->Character.pan, tempChar->MOVE_playerRot, rotationSmooth * time_step); // smoothly update rotation
		tempChar->Character.tilt = tempCamRot.y; tempChar->Character.roll = tempCamRot.z; // kill tilt and roll rotation
		
		//make the player move around
		c_move(tempChar->Character, vector(10*time_step, 0, 0), nullvector, GLIDE | IGNORE_PASSABLE | IGNORE_PUSH | ACTIVATE_PUSH); // Move when key is pressed
	}
}

// move around based on player's rotation
void moveMyChar_playerBased(PLAYER *tempChar) {
	// nothing yet...
}