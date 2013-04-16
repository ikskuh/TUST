// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 280610
//
// title: Code to handle all the camera related stuff
// class: Camera (3rd person static, and character following)
// type: Camera
// help: contains code handling all the camera related things
// 
// ENDHEADER
// ---------------------------------------------------------------------

// function to be called when event is triggered, making sure it's not triggered 2 times either
void cameraSwitch() {
	if (event_type == EVENT_PUSH) {
		if(!is(my, FLAG8) && !str_cmpi(CAM_LastUsedCam, str_for_entname(NULL, my))) {
			// do extra check to see if you == player?
			if(you == MOVE_globalPlayer->Character) {
				str_cpy(CAM_LastUsedCam, str_for_entname(NULL, my));
				set(my, FLAG8);
			}
		}
	}
}

// camera trigger setup
void setTriggerData(CAM_TRIGGER *myTempTrigger) {
	c_setminmax(myTempTrigger->CAM_trigger);
	// sensible for push collision
	set(myTempTrigger->CAM_trigger, INVISIBLE);
	// my.ENABLE_PUSH = ON
	myTempTrigger->CAM_trigger.emask |= ENABLE_PUSH;
	myTempTrigger->CAM_trigger.event = cameraSwitch;
	
	//setup push value to ignore lower objects
	myTempTrigger->CAM_trigger.push = -2;
	myTempTrigger->CAM_trigger.group = -2;
}

void FadeCamTrigger(CAM_TRIGGER *myTempTrigger) {
	// setup trigger stuff
	setTriggerData(myTempTrigger);
	
	// run own functions for camera cuts
	while(myTempTrigger->CAM_trigger) {
		if(is(myTempTrigger->CAM_trigger, FLAG8)) {
			// add function to blend the previous to the new view (comm. edition only)
			if(edition >= 3) {
				// this effect uses a render target, therefor commercial edition atleast is needed
				CAM_fadeCurrentCamView(2, 35);
			}
			
			// get the camera entity based on it's WED name
			myTempTrigger->CAM_newCamEnt = ent_for_name(myTempTrigger->CAM_newCamName);
			
			// set camera view to appropriate camera in WED
			vec_set(camera.x, myTempTrigger->CAM_newCamEnt.x);
			vec_set(camera.pan, myTempTrigger->CAM_newCamEnt.pan);
			
			// reset the camera trigger and make sure it's reset always
			reset(myTempTrigger->CAM_trigger, FLAG8);
		}
		
		wait(1);
	}
}

void InstantCamTrigger(CAM_TRIGGER *myTempTrigger) {
	// setup trigger stuff
	setTriggerData(myTempTrigger);
	
	while(myTempTrigger->CAM_trigger) {
		if(is(myTempTrigger->CAM_trigger, FLAG8)) {
			// get the camera entity based on it's WED name
			myTempTrigger->CAM_newCamEnt = ent_for_name(myTempTrigger->CAM_newCamName);
			
			// set camera view to appropriate camera in WED
			vec_set(camera.x, myTempTrigger->CAM_newCamEnt.x);
			vec_set(camera.pan, myTempTrigger->CAM_newCamEnt.pan);
			
			// reset the camera trigger and last used camera object
			reset(myTempTrigger->CAM_trigger, FLAG8);
		}
		
		wait(1);
	}
}

void SmoothCamTrigger(CAM_TRIGGER *myTempTrigger) {
	// setup trigger stuff
	setTriggerData(myTempTrigger);
	
	while(myTempTrigger->CAM_trigger) {
		if(is(myTempTrigger->CAM_trigger, FLAG8)) {
			// get the camera entity based on it's WED name
			myTempTrigger->CAM_newCamEnt = ent_for_name(myTempTrigger->CAM_newCamName);
			
			// reset the camera trigger and last used camera object
			reset(myTempTrigger->CAM_trigger, FLAG8);
			
			// smooth camera to new position very fast
			CAM_smoothCurrentCamView(myTempTrigger->CAM_newCamEnt, myTempTrigger->CAM_trigger.skill1);
		}
		
		wait(1);
	}
}

void PathCamTrigger(CAM_TRIGGER *myTempTrigger) {
	// setup trigger stuff
	setTriggerData(myTempTrigger);
	
	while(myTempTrigger->CAM_trigger) {
		// check to turn off all other path animations (you dont want some to move through another in most cases)
		if(CAM_pathTriggerSet == 1) {
			// if the camera will have to work no matter what, you can set flag7 to exclude it
			if(!is(myTempTrigger->CAM_trigger, FLAG7)) {
				set(myTempTrigger->CAM_trigger, PASSABLE); // turn off
			}
		} else {
			reset(myTempTrigger->CAM_trigger, PASSABLE); // turn on
		}
		
		if(is(myTempTrigger->CAM_trigger, FLAG8)) {
			// get the camera entity based on it's WED name
			myTempTrigger->CAM_newCamEnt = ent_for_name(myTempTrigger->CAM_newCamName);
			
			// reset the camera trigger and last used camera object
			reset(myTempTrigger->CAM_trigger, FLAG8);
			
			// smooth camera to new position very fast
			CAM_pathCurrentCamView(myTempTrigger->CAM_newCamEnt);
		}
		
		wait(1);
	}
}

void PathCamFollowTrigger(CAM_TRIGGER *myTempTrigger) {
	// setup trigger stuff
	setTriggerData(myTempTrigger);
	
	while(myTempTrigger->CAM_trigger) {
		if(is(myTempTrigger->CAM_trigger, FLAG8)) {
			// get the camera entity based on it's WED name
			myTempTrigger->CAM_newCamEnt = ent_for_name(myTempTrigger->CAM_newCamName);
			
			// reset the camera trigger and last used camera object
			reset(myTempTrigger->CAM_trigger, FLAG8);
			
			// smooth camera to new position following the player forever
			CAM_pathCurrentCamFollow(myTempTrigger->CAM_newCamEnt);
		}
		
		wait(1);
	}
}