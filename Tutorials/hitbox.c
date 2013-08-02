#include <acknex.h>
#include <default.c>

#include "..\\Source\\tust.h"
#include "..\\Source\\hitbox.h"

// Small function to show a hitbox half transparent
void debug_box(ENTITY *box)
{
	reset(box, INVISIBLE);
	set(box, TRANSLUCENT);
}

action soldier()
{
	ENTITY *box; // Temporary pointer to store every created hitbox and modify it a little
	
	// Create a new character shape (hitbox container)
	CharacterShape *shape = charshape_attach(me);
	
	// Setup our character shape:
	//                         Shape Model          Vertices             Scaling                           Box visible        Bodypart
	box = charshape_add(shape, "cylinderShape.mdl", 53, 57, 36, 39);     box.scale_y = box.scale_z = 0.5;  debug_box(box); // Right arm
	box = charshape_add(shape, "cylinderShape.mdl", 273, 291, 312, 313); box.scale_y = box.scale_z = 0.5;  debug_box(box); // Left arm
	box = charshape_add(shape, "cylinderShape.mdl", 273, 291, 53, 57);   box.scale_y = box.scale_z = 0.6;  debug_box(box); // Shoulders
	box = charshape_add(shape, "cylinderShape.mdl", 156, 73, 72, 131);   box.scale_y = box.scale_z = 0.75; debug_box(box); // Right upper leg
	box = charshape_add(shape, "cylinderShape.mdl", 156, 290, 303, 197); box.scale_y = box.scale_z = 0.75; debug_box(box); // Left upper leg
	box = charshape_add(shape, "cylinderShape.mdl", 303, 197, 217, 265); box.scale_y = box.scale_z = 0.75; debug_box(box); // Left lower leg
	box = charshape_add(shape, "cylinderShape.mdl", 72, 131, 116, 71);   box.scale_y = box.scale_z = 0.75; debug_box(box); // Right lower leg
	box = charshape_add(shape, "cylinderShape.mdl", 85, 126, 133, 64);   box.scale_y = box.scale_z = 0.4;  debug_box(box); // Right foot
	box = charshape_add(shape, "cylinderShape.mdl", 278, 209, 272, 232); box.scale_y = box.scale_z = 0.4;  debug_box(box); // Left foot
	box = charshape_add(shape, "cylinderShape.mdl", 159, 184, 177, 188); box.scale_y = box.scale_z = 1.2;  debug_box(box); // Body
	box = charshape_add(shape, "cylinderShape.mdl", 177, 188, 183, 183); box.scale_y = box.scale_z = 0.6;  debug_box(box); // Head
	
	// We can make our character passable, we have bounding boxes for it.
	set(me, PASSABLE);
	
	while(me)
	{
		// Create a simple animation to see if the character shape fits the animated model.
		ent_animate(me, "walk", 5 * total_ticks, ANM_CYCLE);
		
		// Update the character shape
		charshape_update(shape);
		wait(1);
	}
}


// Load a level and create the model
function main()
{
	level_load(NULL);
	you = ent_create("../Ressources/Models/Characters/soldier.mdl", vector(128, 0, 0), soldier);
	you.pan = 180; // Rotate towards the camera.
}