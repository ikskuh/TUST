#include <acknex.h>
#include <default.c>

#include "..\\Source\\tust.h"
#include "..\\Source\\attachment.h"

action soldier()
{
	// Setup the animations system with 2 animations
	Animator *anim = animator_attach(me, 2);
	
	// Setup the looped stand animation with speed 5.0
	animator_set(anim, 0, "stand", 5.0, true);
	
	// Setup the non-looped jump animation with speed 10.0
	animator_set(anim, 1, "jump", 10.0, false);
	
	while(me)
	{
		if(key_space) // If we press space, play the "jump" animation.
			animator_play(anim, 1, false);
		else // Else we play our stand animation
			animator_play(anim, 0, false);
		animator_update(anim); // Animate our model
		wait(1);
	}
}


// Load a level and create the model
function main()
{
	level_load(NULL);
	you = ent_create("../Ressources/Models/Characters/soldier.mdl", vector(128, 0, 0), soldier);
	you->pan = 180;
}