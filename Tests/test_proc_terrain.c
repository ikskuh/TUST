#include <acknex.h>
#include <default.c>
#include <MtlFX.c>

#include "..\\Source\\proc_terrain.h"

void main() {
	//video_mode = 11;
	terrain_chunk = 0;
	level_load(NULL);
	random_seed(0);
	vec_set(camera.x, vector(-1101, -97, 800));
	vec_set(camera.pan, vector(4,-29,0));
	
	BMAP* bmapHeightMap = generate_random_heightmap_noise(256, 256, 128);
	flatten_heightmap(bmapHeightMap, 6);
	BMAP* bmapColorMap = heightmap_to_colormap(bmapHeightMap);
	ENTITY* entTerrain = terrain_from_heightmap(nullvector, bmapHeightMap, 65, 65, 30, 2);
	ent_setskin(entTerrain, bmapColorMap, 2);
	entTerrain.material = mat_terrain_multi_texture;
}
	