#ifndef _UNIT_C_
#define _UNIT_C_

ENTITY *unitPreview = NULL;

void unit_preview_set(STRING *file)
{
	if(unitPreview != NULL)
		ent_morph(unitPreview, file);
	else
	{
		unitPreview = ent_create(file, vector(0, 0, 0), NULL);
		unitPreview.flags2 = UNTOUCHABLE; // We are untouchable for the mouse
		vec_fill(unitPreview.scale_x, MODEL_SCALE);
		set(unitPreview, PASSABLE);
		set(unitPreview, TRANSLUCENT);
		set(unitPreview, INVISIBLE);
		unitPreview.alpha = 50;
	}
}

void unit_preview_setpos()
{
	VECTOR from, to;
	vec_set(from, mouse_pos.x);
	vec_set(to, mouse_pos.x);
	from.z = 0;
	to.z = 10000;
	vec_for_screen(from, camera);
	vec_for_screen(to, camera);
	if(c_trace(from, to, IGNORE_SPRITES | USE_POLYGON | IGNORE_PASSABLE))
	{
		vec_set(unitPreview.x, target);
		reset(unitPreview, INVISIBLE);
		if(you != NULL)
		{
			// Hide the preview if he hit a normal model
			if(ent_type(you) != 4)
				set(unitPreview, INVISIBLE);
		}
		if(mouse_panel)
			set(unitPreview, INVISIBLE);
	}
	else
	{
		set(unitPreview, INVISIBLE);
	}
}

void building_place(STRING *model, EVENT act)
{
	unit_preview_set(model);
	while(mouse_left)
	{
		unit_preview_setpos();
		wait(1);
	}
	if(is(unitPreview, INVISIBLE) == 0)
	{
		// If the preview is not invisible, place the model
		ENTITY *building = ent_create(model, unitPreview.x, act);
		vec_fill(building.scale_x, MODEL_SCALE);
	}
	set(unitPreview, INVISIBLE);
}

void barracks_place()
{
	building_place("barracks.mdl", NULL);
}

#endif // #ifndef _UNIT_C_