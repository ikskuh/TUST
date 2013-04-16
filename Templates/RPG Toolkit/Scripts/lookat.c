// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 210610
//
// title: Code to handle all characters/objects to look at their surrounding
// class: NPC Player Objects
// type: look at
// help: contains code handling bone rotations to look at entity's
// 
// ENDHEADER
// ---------------------------------------------------------------------

void RunBoneRotateLookAt(LOOKAT *myLookAt) {
	VECTOR LOOK_moveAway;
	
	var LOOK_headRotation = 1;
	var LOOK_objectDist;
	
	VECTOR LOOK_lookAtVec;
	myLookAt->LOOK_lookAtObject = LOOK_getLookAtObject(myLookAt->LOOK_object, myLookAt->LOOK_maxBoneRotAngle); // get look at object
	vec_set(LOOK_lookAtVec.x, myLookAt->LOOK_lookAtObject.x);
	
	var LOOK_rotValue;
	var i;
	
	LINKED_BONEROT *myboneRotObject;
	
	// nothing
	while(myLookAt->LOOK_object) {
		// do look at "lookAt object" check
		// get new look at object
		myLookAt->LOOK_lookAtObject = LOOK_getLookAtObject(myLookAt->LOOK_object, myLookAt->LOOK_maxBoneRotAngle); // get look at object
		// smooth the look at vector to the position of the look at object
		LOOK_lookAtVec.x += ((myLookAt->LOOK_lookAtObject.x - LOOK_lookAtVec.x) / myLookAt->LOOK_smoothSpeed) * time_step;
		LOOK_lookAtVec.y += ((myLookAt->LOOK_lookAtObject.y - LOOK_lookAtVec.y) / myLookAt->LOOK_smoothSpeed) * time_step;
		LOOK_lookAtVec.z += ((myLookAt->LOOK_lookAtObject.z - LOOK_lookAtVec.z) / myLookAt->LOOK_smoothSpeed) * time_step;
		
		// DEBUG: line that shows a line to the item to look at
		if(DEF_DEBUG) {
			draw_line3d(LOOK_lookAtVec.x, vector(0,0,255), 0);
			draw_line3d(LOOK_lookAtVec.x, vector(0,0,255), 100);
			draw_line3d(myLookAt->LOOK_object.x, vector(0,0,255), 100);
			draw_line3d(myLookAt->LOOK_object.x, vector(0,0,255), 0);
			
			draw_point3d(LOOK_lookAtVec.x, vector(0,0,255), 100, 1);
		}
		
		// do the actual rotation
		//get object difference and angle
		vec_diff(LOOK_moveAway.x, LOOK_lookAtVec.x, myLookAt->LOOK_object.x);
		vec_to_angle(LOOK_moveAway.x, LOOK_moveAway.x);
		LOOK_objectDist = ang(LOOK_moveAway.x);
		
		// rotation of the headbone update...
		if(abs(ang(myLookAt->LOOK_object.pan - LOOK_objectDist)) < myLookAt->LOOK_maxBoneRotAngle) {
			LOOK_headRotation = clamp(LOOK_headRotation + myLookAt->LOOK_headSpeed*time_step, 0, 1);
		} else {
			LOOK_headRotation = clamp(LOOK_headRotation - myLookAt->LOOK_headSpeed*time_step, 0, 1);
		}
		
		// if head is able to update...
		LOOK_objectDist = ang(LOOK_objectDist - myLookAt->LOOK_object.pan);
		LOOK_rotValue = (1 / myLookAt->LOOK_boneAmount);
		
		// get bone item
		myboneRotObject = myLookAt->LOOK_firstBone;
		
		for(i=0; i<myLookAt->LOOK_boneAmount; i++) {
			// reset bone, and then rotate it again (so it wont rotate too much)
			ent_bonereset(myLookAt->LOOK_object, myboneRotObject->LOOK_boneRot);
			ent_bonerotate(myLookAt->LOOK_object, myboneRotObject->LOOK_boneRot, vector(LOOK_headRotation*LOOK_objectDist*LOOK_rotValue, 0, 0));
			
			// DEBUG: shows a line indicating where each bone is rotated at
			if(DEF_DEBUG) {
				VECTOR tempBonePos;
				vec_for_bone(tempBonePos, myLookAt->LOOK_object, myboneRotObject->LOOK_boneRot); // get bone positions
				// draw line start at bone position
				draw_line3d(tempBonePos.x, vector(0,255,0), 0);
				draw_line3d(tempBonePos.x, vector(0,255,0), 100);
				
				VECTOR tempBoneRot;
				VECTOR tempRotPos;
				vec_set(tempRotPos.x, vector(25,0,(tempBonePos.z - myLookAt->LOOK_object.z)));// get offset (length of the point)
				ang_for_bone(tempBoneRot, myLookAt->LOOK_object, myboneRotObject->LOOK_boneRot);// get rotation from bone to find angle to point towards
				vec_rotate(tempRotPos, tempBoneRot); // add rotation to vector
				vec_add(tempRotPos.x, myLookAt->LOOK_object.x); // set vector to right position
				// draw point where bone faces towards
				draw_line3d(tempRotPos.x, vector(0,255,0), 100);
				draw_line3d(tempRotPos.x, vector(0,255,0), 0);
				
				draw_point3d(tempRotPos.x, vector(0,255,0), 100, 1);
			}
			
			// get next bone item to rotate
			myboneRotObject = myboneRotObject->LOOK_nextBone;
		}
		
		wait(1);
	}
}