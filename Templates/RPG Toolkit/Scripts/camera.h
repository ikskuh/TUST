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
// prefix: CAM_
// idcode: 9
// 
// ENDHEADER
// ---------------------------------------------------------------------

typedef struct {
	ENTITY *CAM_trigger;
	ENTITY *CAM_newCamEnt;
	
	STRING *CAM_newCamName;
} CAM_TRIGGER;

typedef struct {
	ENTITY *CAM_camera;
	
	VECTOR CAM_lookAt;
} CAM_CAMERA;

// variables
BMAP *CAM_screenShotBmap; // bmap used as render target for fading camera's
STRING *CAM_LastUsedCam; // global which stores last used camera per level

var CAM_pathTriggerSet = 0; // variable used to switch on/off all path triggers

// prototypes
void CAM_fadeCurrentCamView(int viewLayer, var fadeSpeed);
void CAM_smoothCurrentCamView(ENTITY *newCamPos, var fadeSpeed);
void CAM_pathCurrentCamView(ENTITY *newCamPos);
void CAM_pathCurrentCamFollow(ENTITY *newCamPos);

// include
#include "camera.c"

// functions
CAM_TRIGGER *init_camTrigger() {
	CAM_TRIGGER *myTempTrigger = sys_nxalloc( sizeof(CAM_TRIGGER) );
	
	// store the current camera trigger
	myTempTrigger->CAM_trigger = my;
	myTempTrigger->CAM_newCamEnt = NULL;
	
	// store the camera object it has to find
	myTempTrigger->CAM_newCamName = str_create("");
	str_cpy(myTempTrigger->CAM_newCamName, myTempTrigger->CAM_trigger.string1);
	
	// do a check if the camera entity actually exists... if not output error telling which trigger has wrong setup
	if(!ent_for_name(myTempTrigger->CAM_newCamName)) {
		STRING *errorStr = str_create("");
		str_cpy(errorStr, "Trigger (");
		str_cat(errorStr, str_for_entname(NULL, myTempTrigger->CAM_trigger));
		str_cat(errorStr,  ") has camera attached that does not exist (");
		str_cat(errorStr, myTempTrigger->CAM_trigger.string1);
		str_cat(errorStr, ").");
		
		printf(_chr(errorStr), NULL);
	}
	
	return(myTempTrigger);
}

void CAM_fadeCurrentCamView(int viewLayer, var fadeSpeed) {
	CAM_screenShotBmap = bmap_createblack(screen_size.x, screen_size.y, 32);
//	bmap_for_screen(CAM_screenShotBmap, 0, 1);

	// create 2nd view
	VIEW *fadeView = view_create(viewLayer);
	vec_set(fadeView.x, camera.x);
	vec_set(fadeView.pan, camera.pan);
	
	// set the camera to be the view (render target)
	fadeView.bmap = CAM_screenShotBmap;
	set(fadeView, SHOW);
	
	// create a panel that displays the 2nd view
	PANEL* fade_camera = pan_create("bmap = CAM_screenShotBmap;", viewLayer);
	set(fade_camera, SHOW | TRANSLUCENT);
	
	// fade the previous view out
	for(fade_camera.alpha=99; fade_camera.alpha>0; fade_camera.alpha-=fadeSpeed*time_step){wait(1);}
	ptr_remove(fade_camera);
	ptr_remove(fadeView);
}

void CAM_smoothCurrentCamView(ENTITY *newCamPos, var fadeSpeed) {
	STRING *lastUsedCam = str_create("");
	str_cpy(lastUsedCam, CAM_LastUsedCam);
	
	while(str_cmpi(lastUsedCam, CAM_LastUsedCam)) {
		// smoothly move the camera to the new position
		camera.x += ((newCamPos.x - camera.x) / fadeSpeed) * time_step;
		camera.y += ((newCamPos.y - camera.y) / fadeSpeed) * time_step;
		camera.z += ((newCamPos.z - camera.z) / fadeSpeed) * time_step;
		
		// smoothyl rotate the camera to the new position (take into account gimbal lock as well)
		camera.pan  += (ang((newCamPos.pan - camera.pan)) / fadeSpeed) * time_step;
		camera.tilt += (ang((newCamPos.tilt - camera.tilt)) / fadeSpeed) * time_step;
		camera.roll += (ang((newCamPos.roll - camera.roll)) / fadeSpeed) * time_step;
		
		str_cpy(lastUsedCam, CAM_LastUsedCam);
		wait(1);
	}
	
	ptr_remove(lastUsedCam);
}

void CAM_pathCurrentCamView(ENTITY *newCamPos) {
	STRING *lastUsedCam = str_create("");
	str_cpy(lastUsedCam, CAM_LastUsedCam);
	
	CAM_pathTriggerSet = 1; // set all path triggers (except those excluded with flag7) to be turned off
	
	ENTITY* nextCamPos = ent_for_name(newCamPos.string1); // find the end camera
	
	VECTOR splinePathVec;
	vec_set(splinePathVec.x, newCamPos.x);
	var dist = 0;
	var pathLength = path_length(newCamPos);
	
	while(str_cmpi(lastUsedCam, CAM_LastUsedCam)) {
		// move vector that camera will follow
		path_spline(newCamPos, splinePathVec.x, dist);
		dist += newCamPos.skill1 * time_step;
		dist = clamp(dist, 0, pathLength);
		
		// make camera follow path vector
		camera.x += ((splinePathVec.x - camera.x) / newCamPos.skill2) * time_step;
		camera.y += ((splinePathVec.y - camera.y) / newCamPos.skill2) * time_step;
		camera.z += ((splinePathVec.z - camera.z) / newCamPos.skill2) * time_step;
		
		// smoothly rotate the camera to the new position (take into account gimbal lock as well)
		camera.pan  += (ang((nextCamPos.pan - camera.pan)) / newCamPos.skill3) * time_step;
		camera.tilt += (ang((nextCamPos.tilt - camera.tilt)) / newCamPos.skill3) * time_step;
		camera.roll += (ang((nextCamPos.roll - camera.roll)) / newCamPos.skill3) * time_step;
		
		str_cpy(lastUsedCam, CAM_LastUsedCam);
		wait(1);
	}
	
	nextCamPos = NULL;
	ptr_remove(nextCamPos);
	ptr_remove(lastUsedCam);
	
	CAM_pathTriggerSet = 0; // set all path triggers to be turned on again
}

var CAM_getPathLengthToNode(ENTITY *pathCamPos, var nodeNumber) {
	int index = 0;
	var pathLength = 0;
	var pathSkills[3];
	
	while(index <= nodeNumber) {
		path_getedge(pathCamPos, index, 1, pathSkills);
		pathLength += pathSkills[0];
		
		index += 1;
	}
	
	return(pathLength);
}

var CAM_getClosestPathNode(ENTITY *pathCamPos, ENTITY *LookAt) {
	var foundNode = 1;
	var closestNode = 1;
	var index = 1;
	
	VECTOR distVec;
	var closestDistance;
	var currentDistance;
	
	path_getnode(pathCamPos, index, distVec, NULL);
	closestDistance = vec_dist(distVec.x, LookAt.x);
	
	while(foundNode != 0) {
		foundNode = path_getnode(pathCamPos, index, distVec, NULL);
		
		currentDistance = vec_dist(distVec.x, LookAt.x);
		if(currentDistance < closestDistance) {
			closestDistance = currentDistance;
			closestNode = index;
		}
		
		index += 1;
	}
	
	return(closestNode);
}

void CAM_pathCurrentCamFollow(ENTITY *newCamPos) {
	STRING *lastUsedCam = str_create("");
	str_cpy(lastUsedCam, CAM_LastUsedCam);
	
	VECTOR splinePathVec;
	vec_set(splinePathVec.x, newCamPos.x);
	
	if(newCamPos.skill2 == 0) {
		STRING *errorStr = str_create("");
		str_cat(errorStr, "Entity (");
		str_cat(errorStr, str_for_entfile(NULL, newCamPos));
		str_cat(errorStr, ") has set Skill2 to Zero, which will cause a crash. Value is set to 1, please update your levelfile!");
		error(errorStr);
		
		newCamPos.skill2 = 1;
	}
	
	ENTITY* LookAtEnt; // get entity to look at
	if(str_cmpi(newCamPos.string2, "player")) {
		LookAtEnt = MOVE_globalPlayer->Character;
	} else {
		LookAtEnt = ent_for_name(newCamPos.string2);
	}
	
	var closestNodeNumber = CAM_getClosestPathNode(newCamPos, LookAtEnt);
	var pathMoveLength = CAM_getPathLengthToNode(newCamPos, closestNodeNumber);
	
	VECTOR temp;
	var dist = pathMoveLength;
	
	while(str_cmpi(lastUsedCam, CAM_LastUsedCam)) {
		// get closest nodenumber and new length
		closestNodeNumber = CAM_getClosestPathNode(newCamPos, LookAtEnt);
		pathMoveLength = CAM_getPathLengthToNode(newCamPos, closestNodeNumber);
		
		// move vector that camera will follow
		path_spline(newCamPos, splinePathVec.x, dist);
		dist += ((pathMoveLength - dist) / newCamPos.skill1) * time_step;
		
		// make camera follow path vector
		camera.x += ((splinePathVec.x - camera.x) / newCamPos.skill2) * time_step;
		camera.y += ((splinePathVec.y - camera.y) / newCamPos.skill2) * time_step;
		camera.z += ((splinePathVec.z - camera.z) / newCamPos.skill2) * time_step;
		
		// make camera look at something (should be done in every trigger preferably)		
		vec_set(temp, LookAtEnt.x);
		vec_sub(temp, camera.x);
		vec_to_angle(camera.pan, temp);
		
//		camera.pan  += (ang((newCamPos.pan - camera.pan)) / newCamPos.skill1) * time_step;
//		camera.tilt += (ang((newCamPos.tilt - camera.tilt)) / newCamPos.skill1) * time_step;
//		camera.roll += (ang((newCamPos.roll - camera.roll)) / newCamPos.skill1) * time_step;
		
		str_cpy(lastUsedCam, CAM_LastUsedCam);
		wait(1);
	}
	
	LookAtEnt = NULL;
}