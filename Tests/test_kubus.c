#include <acknex.h>
#include <default.c>
#include "..\\Source\\tust.h"
#include "..\\Source\\kubus.h"

BMAP *textures = "kubusTextures.png";

BlockType *blockGras = 
{
	textureTop = 0;
	textureBottom = 2;
	textureSide = 1;
}

BlockType *blockSandstone = 
{
	textureTop = 33;
	textureBottom = 34;
	textureSide = 35;
}

BlockType *blockStone = 
{
	textureTop = 3;
	textureBottom = 3;
	textureSide = 3;
}

BlockType *blockGlas = 
{
	textureTop = 96;
	textureBottom = 96;
	textureSide = 96;
	flags = TRANSLUCENT;
}

ENTITY *trunkTemplate;

void construct_wooden_slab(DynamicModel *model, BlockType *type, int x, int y, int z)
{
	trunkTemplate.x = 16 * x;
	trunkTemplate.y = 16 * y;
	trunkTemplate.z = 16 * z;
	dmdl_add_entity(model, trunkTemplate);
}

BlockType *blockTrunk = 
{
	flags = 0;
}

BlockType *blockLeaves = 
{
	textureTop = 97;
	textureBottom = 97;
	textureSide = 97;
	flags = TRANSLUCENT;
}

BlockType *blockWoodenPlanks = 
{
	textureTop = 82;
	textureBottom = 82;
	textureSide = 82;
}

void plant_tree(int x, int y, int z)
{
	int i, h, dx, dy;
	h = 3 + integer(random(3));
	for(i = 0; i < h; i++)
	{
		kubus_set_block(x, y, z, blockTrunk);
		z++;
	}
	for(i = 0; i < 5; i++)
	{
		int radius = 2;
		if(i == 0 || i == 4) radius = 1;
		for(dx = -2; dx <= 2; dx++)
		for(dy = -2; dy <= 2; dy++)
		{
			if(vec_length(vector(dx, dy, 0)) <= radius)
				kubus_set_block(x + dx, y + dy, z, blockLeaves);
		}
		z++;
	}
}

void on_mouse_left_event()
{
	VECTOR f, t;
	vec_set(f, screen_size.x);
	vec_scale(f, 0.5);
	f.z = 0;
	vec_set(t, f);
	t.z = 1000;
	vec_for_screen(f, camera);
	vec_for_screen(t, camera);
	
	if(!c_trace(f, t, IGNORE_PASSABLE | USE_POLYGON))
		return;
	VECTOR *pos = kubus_pos_from_hit();
	if(key_shift)
	{
		kubus_set_block(pos.x, pos.y, pos.z, NULL);
	}
	else
	{
		vec_add(pos, normal.x);
		kubus_set_block(pos.x, pos.y, pos.z, blockWoodenPlanks);
	}
}

function main()
{
//	fps_max = 60;
	on_esc = NULL;
	level_load(NULL);
	sun_angle.tilt = 60;
	camera.x = 256;
	camera.y = 256;
	camera.z = 256;
	
	kubus_init(textures);
	
	int x, y, z;
	for(x = 0; x < 512; x++)
	for(y = 0; y < 512; y++)
	{
		int h = 1 + integer(32 + 32 * noise2d(x / 64.0, y / 64.0));
		for(z = 0; z < h; z++)
		{
			kubus_set_block(x, y, z, blockStone);
		}
		kubus_set_block(x, y, h, blockGras);
		if(random(100) > 97)
			plant_tree(x, y, h + 1);
	}
	
	for(x = 16; x < 32; x++)
	for(y = 16; y < 32; y++)
	for(z = 16; z < 64; z++)
		kubus_set_block(x, y, z, blockGlas);
	
	blockTrunk.construct = construct_wooden_slab;
	trunkTemplate = ent_create("kubusTrunk.mdl", vector(0,0,0), NULL);
	
	while(1)
	{
		kubus_update();
		VECTOR f, t;
		vec_set(f, screen_size.x);
		vec_scale(f, 0.5);
		f.z = 0;
		vec_set(t, f);
		t.z = 1000;
		vec_for_screen(f, camera);
		vec_for_screen(t, camera);
		
		if(c_trace(f, t, IGNORE_PASSABLE | USE_POLYGON))
			draw_point3d(target, COLOR_WHITE, 100, 2);
		wait(1);
	}
}