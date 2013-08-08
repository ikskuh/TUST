#include <acknex.h>
#include <default.c>
//#include <d3d9.h>
//#include <MtlFX.c>

//#include "..\\Source\\math.h"
//#include "..\\Source\\proc_city.h"
//#include "..\\Source\\DynamicModels.h"
//#include "..\\Source\\tust.h"

#include "..\\Source\\proc_terrain.h"

void main() {
	//video_mode = 11;
	terrain_chunk = 32;
	level_load("");
	random_seed(0);
	vec_set(camera.x, vector(-1101, -97, 800));
	vec_set(camera.pan, vector(4,-29,0));
	ENTITY* entTerrain = generate_random_terrain(vector(0,0,0), 64, 64, 16, 4);
}