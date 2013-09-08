#include <acknex.h>
#include <default.c>
#include <MtlFX.c>
#include <d3d9.h>
#include <ackphysx.h>

#include "..\\Source\\proc_terrain.h"
#include "..\\Source\\math.h"
#include "..\\Source\\proc_street.h"
#include "..\\Source\\DynamicModels.h"
#include "..\\Source\\tust.h"

void main() {
	video_mode = 9;
	terrain_chunk = 0;
	level_load(NULL);
	random_seed(0);
	mouse_mode = 4;
	vec_set(camera.x, vector(-1101, -97, 800));
	vec_set(camera.pan, vector(4,-29,0));
	
	// Terrain
	BMAP* bmapHeightMap = generate_random_heightmap_noise(256, 256, 64);
	BMAP* bmapColorMap = heightmap_to_colormap(bmapHeightMap);
	ENTITY* entTerrain = terrain_from_heightmap(vector(0,0,-300), bmapHeightMap, 65, 65, 30, 0.4);
	ent_setskin(entTerrain, bmapColorMap, 2);
	entTerrain.material = mat_terrain_multi_texture;
	
	// Roads
	proc_city_create_skins();	
	List *points = roadnetwork_from_rectlangle(entTerrain.min_x + 100, entTerrain.min_y + 100, entTerrain.max_x - 50, entTerrain.max_y - 50, 200, 6);
	//List *points = roadnetwork_from_voronoi(30, entTerrain.min_x + 100, entTerrain.min_y + 100, entTerrain.max_x - 50, entTerrain.max_y - 50);
	List *intersections = roadnetwork_calculate(points);
	roadnetwork_join_near_intersections(intersections, 100); // Delete intersections which are too near to each other
	roadnetwork_build(intersections, 300, true);
}