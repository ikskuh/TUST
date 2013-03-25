#include <acknex.h>
#include "..\\..\\Source\\tust.h"

// Global ressources
#define PRAGMA_PATH "..\\..\\Ressources\\Models\\Characters"
#define PRAGMA_PATH "..\\..\\Ressources\\Models\\Buildings"
#define PRAGMA_PATH "..\\..\\Ressources\\Terrains\\"

// RTS ressources
#define PRAGMA_PATH "Ressources"

// Global scale for all created models...
#define MODEL_SCALE	0.25

#include "controls.c"
#include "unit.c"

function main()
{
	mouse_mode = 4;
	video_set(1024, 768, 32, 2);
	level_load("helymap1.hmp");
	controls_start();
}