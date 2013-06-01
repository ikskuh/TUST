// include cct template:
#include "cct.h"

// camera positions:
VECTOR camPos;
// cct extents:
VECTOR cctExtents;
// input forces:
VECTOR force;
// relative distance:
VECTOR dist;
// absolute distance:
VECTOR absdist;


	// create cct and setup it's size/bounding ellipsoid:
	function cct_setup(ENTITY* actor, VECTOR* bbox_size, var groupId){
		// standing and crouching size's of cct:
		actor.standingSize = bbox_size.z;
		actor.crouchingSize = bbox_size.z * 0.6;
		// radius and half-size of the cct:
		actor.halfStandingSize = actor.standingSize * 0.5;
		actor.halfCrouchingSize = actor.crouchingSize * 0.5;
		actor.cctRadius = bbox_size.x;
		// setup scale vectors:
		vec_set(actor.standSize_x, vector(actor.cctRadius / actor.max_x, actor.cctRadius / actor.max_x, actor.halfStandingSize / actor.max_z));
		vec_set(actor.crawlSize_x, vector(actor.cctRadius / actor.max_x, actor.cctRadius / actor.max_x, actor.halfCrouchingSize / actor.max_z));
		// scale the bbox:
		vec_set(actor.scale_x, actor.standSize_x);
		// wait one frame:
		wait(1);
		// set both collusion flags:
		actor.eflags |= FAT | NARROW;
		// set bounding box to individual values:
		vec_set(actor.min_x, vector(-bbox_size.x, -bbox_size.y, -bbox_size.z * 0.5)); 
		vec_set(actor.max_x, vector(bbox_size.x, bbox_size.y, bbox_size.z * 0.5));
		// register the cct:
		pX3ent_settype(actor, PH_CHAR, PH_CAPSULE);
		// set group id:
		pX3ent_setgroup(actor, groupId);
		actor.group = groupId;
	}
	
	// check for the collusions of the cct (f.e. if we are trying to stand up from crawling):
	function cct_check_collusion(ENTITY* actor, var cct_max_x, var cct_min_x, var cct_max_z, var cct_min_z){
		// vectors:
		VECTOR vStart, vEnd;
		// free vectors:
		vec_fill(vStart.x, 0);
		vec_fill(vEnd.x, 0);
		// get the radius:
		var vRadius = (cct_max_x + cct_min_x) * 0.5;
		// defile start position to check:
		vec_set(vStart, vector(0, 0, cct_max_z));
		vec_add(vStart, actor.x);
		// defile end position to check:
		vec_set(vEnd, vector(0, 0, cct_min_z));		
		vec_add(vEnd, actor.x);
		// check for collusion:
		if(pX3_capsulecheckany(vStart, vEnd, vRadius, 0, 0, NULL, PX_IGNORE_ME)){
			// return 1:
			return(1);
		}
		// return 0:
		return(0);
	}

// movement speed:
VECTOR* strength = {
	x = 15;
	y = 15;
	z = 25;
}

// camera angles:
ANGLE camInput;
ANGLE camAngle;

// eye position offset:
var eyePos = 80;
// mouse speed:
var mouseSpeed = 0.3;
// angular friction of camera (should be below 1):
var angFric = 0.99;							
// movement collusion:
var moveFlag = 0;
// linear friction of movement (should be below 1):
var gndFric = 0.8;
// gravity force:
var gravity = -5.5;
// jumping switch:
var jump_time = 0;
// timer to go crawling:
var crawlTimer = 0;
// crawling speed:						
var crawlSpeed = 10;
// crawling switch (0 - if not crawling; 1 - if we are crawling;):
var crawlOn = 0;						

// fps-camera code:
void attachCamera(){
	// rotate camera:
	if(my.allowRotate == 1){
		// get input:
		camInput.pan -= mouseSpeed * mickey.x;
		camInput.tilt -= mouseSpeed * mickey.y;	
		camInput.roll = 0;
	}
	// limit tilt:
	camInput.tilt = clamp(camInput.tilt, -80, 80);
	// accelerate angles:
	vec_accelerate(camAngle.pan, camAngle.pan, camInput.pan, angFric);
	// limit tilt:
	camAngle.tilt = clamp(camAngle.tilt, -80, 80);
	// rotate camera itself:
	camera.pan = camAngle.pan;
	camera.tilt = camAngle.tilt;
	camera.roll = camAngle.roll;
	// rotate player as well:
	my.pan = camera.pan;
	// get the extents of the cct (radius, radius, height):
	pX3ent_getfootposition(my, cctExtents.x);
	// get the position of the camera:
	vec_set(camPos.x, vector(0, 0, eyePos + crawlTimer));
	// rotate with camera:
	vec_rotate(camPos.x, vector(camera.pan, 0, 0));
	// add player's position:
	vec_add(camPos.x, cctExtents.x);
	// attach camera to defined position:
	vec_set(camera.x, camPos.x);
}

void goCrawl(){
	// if we press ctrl:
	if(key_ctrl){
		// if we need to lower the camera:
		if(crawlTimer > -45){
			// decrease camera offset:
			crawlTimer -= crawlSpeed * time_step;
		}
		else{
			// if we need to change the size:
			if(my.scale_z > my.crawlSize_z){
				// change the visual capsule size:
				vec_set(my.scale_x, my.crawlSize_x);
				// change the size of the cct to crawl:
				pX3ent_resizechar(my, 0);
				// update bbox:
				c_setminmax(my);
			}
		}
		// we are crawling:
		crawlOn = 1;
	}
	else{
		// if we need to stand up:
		if(crawlTimer < 0){
			// and there is no celling above the head (20 - are min/max_x and 40 - are min/max_z values):
			if(cct_check_collusion(my, 20, 20, 40, 40) == 0){
				// increase camera offset:
				crawlTimer += crawlSpeed * time_step;
			}
			else{
				// if we need to lower the camera:
				if(crawlTimer > -45){
					// decrease camera offset:
					crawlTimer -= crawlSpeed * time_step;
				}
			}
		}
		else{
			// if we need to change the size:
			if(my.scale_z < my.standSize_z){
				// change the visual capsule size:
				vec_set(my.scale_x, my.standSize_x);
				// change the size of the cct to crawl:
				pX3ent_resizechar(my, 40);
				// update bbox:
				c_setminmax(my);
				// we are not crawling:
				crawlOn = 0;
			}			
		}
	}
}

// main player's action:
action actHero() {
	// flags:
	set(my, INVISIBLE);
	// skills:
	my.health = 100;
	// movement:
	my.allowMove = 1;
	my.allowRotate = 1;
	// setup cct parameters:
	cct_setup(my, vector(20, 20, 80), PLAYER_GROUP);
	// reset movement vectors:
	vec_fill(force.x, 0);
	vec_fill(dist.x, 0);
	vec_fill(absdist.x, 0);
	// camera startup:
	camera.arc = 90;
	// loop:
	while(my.health > 1){
		// handle crawling:
		goCrawl();	
		// if we are allowed to move:
		if(my.allowMove == 1){
			// get input:
			force.x = strength.x * (key_w - key_s);
			force.y = strength.y * (key_a - key_d);
			// if pressed space key (and switch is set to - zero):
			if(key_space && jump_time == 0){
				// and if we are on the ground:
				if(pX3ent_ischargrounded(me)){
					// then jump:
					force.z = strength.z;
					// jumping switch on:
					jump_time = 1;
				}
			}
			// if we unpress the space key:
			if(!key_space){
				// if we need to reset the jump switch:
				if(jump_time != 0){
					// reset it:
					jump_time = 0; 
				} 
			}
			// if movement speed is more that allowed movement speed:
			if(vec_length(vector(force.x, force.y, 0)) > strength.x){
				// lower it till defined movement speed (depending on state machine):
				var len = sqrt(force.x * force.x + force.y * force.y);				
				force.x *= ((strength.x) / len);
				force.y *= ((strength.x) / len);
			}
			// if we are crawling:
			if(crawlOn == 1){
				// then crawl:
				force.x *= CRAWL_SPEED;
				force.y *= CRAWL_SPEED;				
			}
			else{
				// if we press the shift key:
				if(key_shift){
					// then run:
					force.x *= WALK_SPEED;
					force.y *= WALK_SPEED;				
				}
				else{
					// walk, if not pressing shift key:
					force.x *= RUN_SPEED;
					force.y *= RUN_SPEED;				
				}
			}
		}
		else{
			// reset movement:
			vec_set(force.x, nullvector);
			// reset the speed, to default:
			force.x *= RUN_SPEED;
			force.y *= RUN_SPEED;				
		}
		// accelerate forces:
		accelerate(dist.x, force.x * time_step, gndFric);
		accelerate(dist.y, force.y * time_step, gndFric);
		// reset relative Z movement:
		dist.z = 0;
		// move cct:
		moveFlag = pX3ent_move(my, dist.x, vector(0, 0, force.z * time_step));
		// gravity (if we are in air):
		if(!pX3ent_ischargrounded(me)){
			// accelerate downwards:
			force.z += gravity * time_step;
			// limit acceleration:
			force.z = maxv(force.z, -100);
			// if cct collided with celling:
			if(moveFlag & PX_CCT_COLLISION_UP){
				// reset jumping:
				force.z = gravity;
			}
		}
		else{
			// we are on ground:
			force.z = gravity;
		}
		// attach camera:
		attachCamera();
		
		// get the extents of the cct (radius, radius, height):
		pX3ent_getfootposition(my, cctExtents.x);
	
		// wait one frame:
		wait(1);
	}
}