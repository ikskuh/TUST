#include <acknex.h>
#include <default.c>

/*
		Voronoi
*/
void vo_init();
void vo_free();
void vo_add_point(float _x, float _y);
void vo_get_point(int _i, float *_x, float *_y);
int vo_get_point_count();
void vo_execute(float _minX, float _maxX, float _minY, float _maxY, float _minDist);
int vo_get_result_count();
void vo_get_result_at(int _i, float *_x1, float *_y1, float *_x2, float *_y2);


void draw_point(int _x, int _y) {
	draw_line(vector(_x-1, _y-1, 0), NULL, 100);
	draw_line(vector(_x+1, _y-1, 0), COLOR_BLUE, 100);
	draw_line(vector(_x-1, _y, 0), NULL, 100);
	draw_line(vector(_x+1, _y, 0), COLOR_BLUE, 100);
	draw_line(vector(_x-1, _y+1, 0), NULL, 100);
	draw_line(vector(_x+1, _y+1, 0), COLOR_BLUE, 100);
}

void voronoi_test() {
	
	int i;
	
	vo_init();
	
	for(i=0; i<40; i++) {
		vo_add_point(integer(random(screen_size.x)), integer(random(screen_size.y)));
	}
	/*vo_add_point(13, 81);
	vo_add_point(54, 34);
	vo_add_point(12, 87);
	vo_add_point(25, 23);
	vo_add_point(44, 65);
	vo_add_point(77, 22);
	vo_add_point(42, 17);
	vo_add_point(142, 117);
	vo_add_point(242, 217);
	vo_add_point(342, 217);
	vo_add_point(442, 317);*/
	
	vo_execute(0, screen_size.x, 0, screen_size.y, 3);
	int nResults = vo_get_result_count();
	int nPoints = vo_get_point_count();
	
	//printf("p %i r %i", nPoints, nResults);
	

	float x1, y1, x2, y2;
	while(1) {
		
		// Draw diagram
		for(i=0; i<nResults; i++) {
			vo_get_result_at(i, &x1, &y1, &x2, &y2);
			
			draw_line(vector(x1, y1, 0), NULL, 100);
			draw_line(vector(x2, y2, 0), COLOR_RED, 100);
			
		}
		
		for(i=0; i<nPoints; i++) {
			vo_get_point(i, &x1, &y1);
			draw_point(x1, y1);
		}
		wait(1);
	}
	vo_free();
}



/*
		Triangulation
*/
void tr_free();
void tr_init();
void tr_new_polyline();
void tr_add_point(float _x, float _y);
void tr_get_point(int _polylineID, int _pointID, float *_x, float *_y);
int tr_get_polyline_count();
int tr_get_point_count(int _polylineID);
void tr_execute();
int tr_get_triangle_count();
void tr_get_triangle_at(int _i, float *_x1, float *_y1, float *_x2, float *_y2, float *_x3, float *_y3);

void triangulation_test() {
	tr_init();
	
	tr_add_point(-30, -10);
	tr_add_point(-10, -10);
	tr_add_point(-10, -30);
	tr_add_point( 10, -30);
	tr_add_point( 10, -30);
	tr_add_point( 10, -10);
	tr_add_point( 30, -10);
	tr_add_point( 30,  10);
	tr_add_point( 10,  10);
	tr_add_point( 10,  30);
	tr_add_point(-10,  30);
	tr_add_point(-10,  10);
	tr_add_point(-30,  10);
	
	tr_execute();
	
	int count      = tr_get_triangle_count();
	int pointCount = tr_get_point_count(0);
	
	int i;
	float x1, y1, x2, y2, x3, y3;
	
	while(1) {
		
		draw_text("Triangles: ", 10, 10, COLOR_GREEN);
		draw_text(str_for_int(NULL, count), 100, 10, COLOR_GREEN);
		for(i=0; i<count; i++) {
			tr_get_triangle_at(i, &x1, &y1, &x2, &y2, &x3, &y3);
			draw_line3d(vector(x1,y1,0), NULL, 100);
			draw_line3d(vector(x2,y2,0), COLOR_RED, 100);
			draw_line3d(vector(x3,y3,0), COLOR_RED, 100);
			draw_line3d(vector(x1,y1,0), COLOR_RED, 100);
		}
		
		tr_get_point(0, 0, &x1, &y1);
		draw_line3d(vector(x1,y1,-30), NULL, 100);
		
		for(i=1; i<pointCount; i++) {
			tr_get_point(0, i, &x1, &y1);
			draw_line3d(vector(x1,y1,-30), COLOR_GREEN, 100);
		}
		tr_get_point(0, 0, &x1, &y1);
		draw_line3d(vector(x1,y1,-30), COLOR_GREEN, 100);		
		wait(1);
	}
}

void main() {
	
	level_load("");
	vec_set(sky_color.blue, vector(0,0,0));
	random_seed(0);
	
	//voronoi_test();
	triangulation_test();
	
	
}