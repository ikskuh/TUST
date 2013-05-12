#include <acknex.h>
#include <default.c>

#include "..\\Source\\bmap.h"

int main ()
{
	level_load("");

	BMAP* b = bmap_create("test_bmap\\brownie.bmp");
	BMAP* b1 = bmap_create("test_bmap\\house.bmp");
	
	while(1) {
		
		if (key_a) { while(key_a) wait(1); b = bmp_blur(b, 10); }	
		if (key_b) { while(key_b) wait(1); b = bmp_resize(b, 50, 50);}
		if (key_c) { while(key_c) wait(1); b = bmp_mirror(b, BMP_MIRROR_X); }	
		if (key_d) { while(key_d) wait(1); b = bmp_mirror(b, BMP_MIRROR_Y); }
		if (key_1) { while(key_1) wait(1); b = bmap_crop(b, 5, 5, 100, 100); }
		if (key_2) { while(key_2) wait(1); b = bmap_erode(b, 10); }
		if (key_3) { while(key_3) wait(1); b = bmap_dilate(b, 10); }
		if (key_4) { while(key_4) wait(1); b = bmap_sharpen(b, 100); }
		if (key_5) { while(key_5) wait(1); b = bmap_laplace(b); }
		if (key_6) { while(key_6) wait(1); b = bmap_grayscale(b); }
		
		if (key_q) { while(key_q) wait(1); b = bmap_draw_point(b, 10, 10, vector(255,0,0),  1); }
		if (key_w) { while(key_w) wait(1); b = bmap_draw_line(b, 5, 5, 100, 2, vector(0,0,255), 1); }
		if (key_e) { while(key_e) wait(1); b = bmap_draw_arrow(b, 5, 20, 100, 20, vector(0,255,0), 1, 30); }
		if (key_r) { while(key_r) wait(1); b = bmap_draw_spline(b, 5, 30, 0, 20, 100, 30, 5, 50, vector(0,0,255), 1); }
		if (key_t) { while(key_t) wait(1); b = bmap_draw_triangle(b, 5, 40, 100, 40, 50, 70, vector(0,0,255), 1); }
		if (key_z) { while(key_z) wait(1); b = bmap_draw_triangle_filled(b, 5, 40, 100, 40, 50, 70, vector(0,0,255), 1); }
		if (key_u) { while(key_u) wait(1); b = bmap_draw_bmap(b, b1, 5, 110, 0.5); }
		if (key_i) { while(key_i) wait(1); b = bmap_draw_text(b, 1, 130, "Welcome to Gamestudio", vector(255,0,0), vector(255,255,255), 1, 13); }
		if (key_o) { while(key_o) wait(1); b = bmap_fill_ext(b, 100, 100, vector(0,0,255), 1); }
		if (key_p) { while(key_p) wait(1); b = bmap_plasma(b, 1); }
		if (key_s) { while(key_s) wait(1); b = bmap_draw_border(b, 10, 100, vector(0,0,255), 1); }
		
		if (key_f) { while(key_d) wait(1); bmap_savetga(b, "b.tga"); }
		
		DEBUG_BMAP(b, 1, 1);
		
		wait(1);
	}
}