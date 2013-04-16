// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 270810
//
// title: Code to handle gravity
// class: Gravity
// type: Gravity
// help: contains code handling all the gravity for entity's
// 
// ENDHEADER
// ---------------------------------------------------------------------

void GRAV_gravity(GRAVITY *Grav);

void GRAV_gravity(GRAVITY *Grav) {
	my = Grav->gravityEnt;
	
	if(DEF_DEBUG) {
		draw_line3d(my.x, vector(255,0,0), 0);
		draw_line3d(my.x, vector(255,0,0), 100);
	}
	
	// determine the ground distance by a downwards trace
	// and make sure there is always one group that is being ignored, for certain entitys (player for example)
	c_ignore(1,0);
	if(c_trace(Grav->gravityEnt.x, vector(my.x,my.y,my.z - 5000), IGNORE_ME | IGNORE_PASSABLE | IGNORE_PUSH) != 0) { // -USE_BOX: makes script speed go bunches faster!
		Grav->GRAV_distDown = (Grav->gravityEnt.z + Grav->GRAV_vFeet.z) - target.z; // get distance between player's feet and the ground
	} else {
		Grav->GRAV_distDown = 0;
	}
	
	if(DEF_DEBUG) {
		draw_line3d(target.x, vector(255,0,0), 100);
		draw_line3d(target.x, vector(255,0,0), 0);
		
		draw_point3d(target.x, vector(0,0,255), 100, 2);
	}
	
	// apply gravity when the player is in the air
	if (Grav->GRAV_distDown > 0) { // above floor, fall down with increasing speed
	   Grav->GRAV_distDown = clamp(Grav->GRAV_distDown, 0, accelerate(Grav->GRAV_speedDown, Grav->GRAV_fallSpeed, 0.1));
   } else { // on or below floor, set downward speed to zero
	   Grav->GRAV_speedDown = 0;
	}
	
	// and make sure there is always one group that is being ignored, for certain entitys (player for example)
	c_move(Grav->gravityEnt, nullvector, vector(0, 0, -Grav->GRAV_distDown), IGNORE_ME | IGNORE_PASSABLE | IGNORE_PUSH | GLIDE); // move the player
}