#include <acknex.h>
#include <default.c>
#include "..\\Source\\tust.h"

function main()
{
	fps_max = 60;
	on_esc = NULL;
	level_load(NULL);
	
	DMDLSettings.flags |= DMDL_FIXNORMALS;
	
	DynamicModel* myModel = dmdl_create();
	
	you = ent_create(CUBE_MDL, vector(0, 0, 0), NULL);
	
	var i;
	VECTOR pos;
	for(i = 0; i < 100; i += 10)
	{
		ent_animate(you, "walk", i, ANM_CYCLE);
		
		you->pan = 3.6 * i;
		vec_set(&pos, vector(0, -100, 0));
		vec_rotate(&pos, vector(3.6 * i, 0, 0));
		vec_set(&(you->x), &pos);
		
		dmdl_add_entity(myModel, you);
	}
	
	ent_remove(you);
	
	myModel->skin[0] = bmap_create("sid.bmp");
	ENTITY* ent = dmdl_create_instance(myModel,vector(0,0,0),NULL);
	
	camera.x = -512;
	
	while(!key_esc)
	{
		ent->pan += 2 * time_step;
		ent->tilt += 3.5 * time_step;
		ent->roll -= 1.5 * time_step;
		wait(1);
	}
	
	dmdl_delete(myModel);
}