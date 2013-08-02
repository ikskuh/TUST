#include <acknex.h>
#include <default.c>

#include "..\\Source\\tust.h"
#include "..\\Source\\attachment.h"

action soldier()
{
	// Create the sword
	ENTITY *sword = ent_create("../Ressources/Models/Weapons/katana.mdl", vector(0, 0, 0), NULL); // Create a katana
	
	// Scale it down a little
	vec_fill(sword.scale_x, 0.05);
	
	while(me)
	{
		// Create a simple animation to see if the character shape fits the animated model.
		ent_animate(me, "walk", 5 * total_ticks, ANM_CYCLE);
		
		// Attach the sword to the soldiers back. 158 and 157 are the top and bottom
		// center vertex of the back pack.
		attach_entity_to_vertices(sword, me, 158, 157, vector(-2, -4, 4), vector(0, 90, 0));
		wait(1);
	}
}


// Load a level and create the model
function main()
{
	level_load(NULL);
	you = ent_create("../Ressources/Models/Characters/soldier.mdl", vector(128, 0, 0), soldier);
}