// ---------------------------------------------------------------------
// STARTHEADER
//
// ver: 1.0
// engineReq: A7.xx (any should work, even Free)
// date: 070910
//
// title: Code that handles the elipsoid collision with entity's
// class: elipsoid collision
// type: elipsoid collision
// help: contains code handling all the elipsoid collision between entity's
// 
// ENDHEADER
// ---------------------------------------------------------------------

void RunElipsoidCollision(COLLISION *myCollision) {
	VECTOR tempVector;
	ANGLE tempAngle;
	
	while(myCollision->COL_collision->COL_collider) {
		// get closest collision entity
		myCollision->COL_collisionEnt = COL_getClosestCollider(myCollision->COL_collision->COL_collider);
		
		if(DEF_DEBUG) {
			// Do elipsoid drawing for debug here
			VECTOR tempPos;
			VECTOR tempNextPos;
			var drawCol=0;
			
			for(drawCol=0; drawCol<360; drawCol+=30) {
				// get current rotational piece
				vec_set(tempPos.x, vector(myCollision->COL_CollisionDist,0,0));
				vec_rotate(tempPos.x, vector(drawCol,0,0));
				vec_add(tempPos.x, myCollision->COL_collision->COL_collider.x);
				
				// get next rotational position
				vec_set(tempNextPos.x, vector(myCollision->COL_CollisionDist,0,0));
				vec_rotate(tempNextPos.x, vector(drawCol+30,0,0));
				vec_add(tempNextPos.x, myCollision->COL_collision->COL_collider.x);
				
				// draw a "quad" to show the collision shape
				draw_line3d(vector(tempPos.x,tempPos.y,tempPos.z-45), vector(0,0,255), 0);
				draw_line3d(vector(tempPos.x,tempPos.y,tempPos.z-45), vector(0,0,255), 100);
				draw_line3d(vector(tempPos.x,tempPos.y,tempPos.z+45), vector(0,0,255), 100);
				draw_line3d(vector(tempNextPos.x,tempNextPos.y,tempNextPos.z+45), vector(0,0,255), 100);
				draw_line3d(vector(tempNextPos.x,tempNextPos.y,tempNextPos.z+45), vector(0,0,255), 0);
				draw_line3d(vector(tempPos.x,tempPos.y,tempPos.z-45), vector(0,0,255), 0);
				draw_line3d(vector(tempPos.x,tempPos.y,tempPos.z-45), vector(0,0,255), 100);
				draw_line3d(vector(tempNextPos.x,tempNextPos.y,tempNextPos.z-45), vector(0,0,255), 100);
				draw_line3d(vector(tempNextPos.x,tempNextPos.y,tempNextPos.z-45), vector(0,0,255), 0);
			}
		}
		
		// distance check on collider and push away of too close (faked elipsoid collision)
		if(vec_dist(myCollision->COL_collision->COL_collider.x, myCollision->COL_collisionEnt.x) < myCollision->COL_CollisionDist)
		{
			vec_diff(tempVector.x, myCollision->COL_collisionEnt.x, myCollision->COL_collision->COL_collider.x);
			vec_to_angle(tempAngle.pan, tempVector.x); // angle to the player
			vec_set(tempVector.x, vector(myCollision->COL_collision->COL_collider.x + myCollision->COL_CollisionDist * cos(tempAngle.pan), myCollision->COL_collision->COL_collider.y + myCollision->COL_CollisionDist * sin(tempAngle.pan), 0));
			vec_diff(tempVector.x, tempVector.x, myCollision->COL_collisionEnt.x);
			
			c_move(myCollision->COL_collisionEnt, nullvector, vector(tempVector.x*time_step, tempVector.y*time_step, 0), GLIDE | IGNORE_PASSABLE | IGNORE_PUSH);
		}
		
		wait(1);
	}
}