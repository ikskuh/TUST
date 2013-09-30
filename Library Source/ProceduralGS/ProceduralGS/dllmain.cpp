// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "stdafx.h"

#include <iostream>
#include <math.h>
#include <algorithm>
#include <time.h>


/*
*
* Voronoi generator
*
*/
#include "VoronoiDiagramGenerator.h"


#define DLL_USE
#include "adll.h"

std::vector<float> *xPoints = NULL;
std::vector<float> *yPoints = NULL;
VoronoiDiagramGenerator *vdg = NULL;
std::vector<float> *xResultsFrom = NULL;
std::vector<float> *yResultsFrom = NULL;
std::vector<float> *xResultsTo = NULL;
std::vector<float> *yResultsTo = NULL;
int result_count = 0;

DLLFUNC void vo_init() {
	if (xPoints != NULL) { xPoints->clear(); free(xPoints); xPoints = NULL;	}
	if (yPoints != NULL) { yPoints->clear(); free(yPoints);	yPoints = NULL; }
	if (xResultsFrom != NULL) { xResultsFrom->clear(); free(xResultsFrom); xResultsFrom = NULL;	}
	if (yResultsFrom != NULL) { yResultsFrom->clear(); free(yResultsFrom); yResultsFrom = NULL; }
	if (xResultsTo != NULL) { xResultsTo->clear(); free(xResultsTo); xResultsTo = NULL;	}
	if (yResultsTo != NULL) { yResultsTo->clear(); free(yResultsTo); yResultsTo = NULL; }
	if (vdg != NULL) { free(vdg); vdg = NULL; }

	vdg = new VoronoiDiagramGenerator();
	xPoints = new std::vector<float>();
	yPoints = new std::vector<float>();
	xResultsFrom = new std::vector<float>();
	yResultsFrom = new std::vector<float>();
	xResultsTo = new std::vector<float>();
	yResultsTo = new std::vector<float>();
	result_count = 0;
}

DLLFUNC void vo_free() {
	if (xPoints != NULL) { xPoints->clear(); free(xPoints); xPoints = NULL;	}
	if (yPoints != NULL) { yPoints->clear(); free(yPoints);	yPoints = NULL; }
	if (xResultsFrom != NULL) { xResultsFrom->clear(); free(xResultsFrom); xResultsFrom = NULL;	}
	if (yResultsFrom != NULL) { yResultsFrom->clear(); free(yResultsFrom); yResultsFrom = NULL; }
	if (xResultsTo != NULL) { xResultsTo->clear(); free(xResultsTo); xResultsTo = NULL;	}
	if (yResultsTo != NULL) { yResultsTo->clear(); free(yResultsTo); yResultsTo = NULL; }
	if (vdg != NULL) { free(vdg); vdg = NULL; }
	result_count = 0;
}

DLLFUNC void vo_add_point(float _x, float _y) {
	if (xPoints != NULL) {
		xPoints->push_back(_x);
		yPoints->push_back(_y);
	}
}

DLLFUNC void vo_get_point(int _i, float &_x, float &_y) {
	if (xPoints != NULL) {
		_x = xPoints->at(_i);
		_y = yPoints->at(_i);
	}
}

DLLFUNC int vo_get_point_count() {
	if (xPoints != NULL) {
		return xPoints->size();
	}
	return -1;
}

DLLFUNC void vo_execute(float _minX, float _maxX, float _minY, float _maxY, float _minDist) {
	if (vdg != NULL) {
		xResultsFrom->clear();
		yResultsFrom->clear();
		xResultsTo->clear();
		yResultsTo->clear();
		vdg->generateVoronoi(xPoints, yPoints, _minX, _maxX, _minY, _maxY, _minDist);
		vdg->resetIterator();
		result_count = 0;
		float x1,y1,x2,y2;
		while(vdg->getNext(x1, y1, x2, y2)) {
			xResultsFrom->push_back(x1);
			yResultsFrom->push_back(y1);
			xResultsTo->push_back(x2);
			yResultsTo->push_back(y2);
			result_count++;
		}
	}
}

DLLFUNC int vo_get_result_count() {
	return result_count;
}

DLLFUNC void vo_get_result_at(int _i, float &_x1, float &_y1, float &_x2, float &_y2) {
	if (xResultsFrom->size() > _i) {
		_x1 = xResultsFrom->at(_i);
		_y1 = yResultsFrom->at(_i);
		_x2 = xResultsTo->at(_i);
		_y2 = yResultsTo->at(_i);
	}
}


/*
*
* Triangulation
*
*/

#include "advancing_front.h"
#include "cdt.h"

// All polylines
std::vector<std::vector<p2t::Point*>*> tr_polylines;

// The current polyline
std::vector<p2t::Point*> *tr_current_polyline;

// Results
std::vector<p2t::Triangle*> tr_triangles;
std::list<p2t::Triangle*> tr_map;

DLLFUNC void tr_free() {
	for(int i=0; i<tr_polylines.size(); i++) {
		tr_polylines.at(i)->clear();
	}
	tr_polylines.clear();
	tr_triangles.clear();
	tr_map.clear();
}

DLLFUNC void tr_init() {
	tr_free();
	tr_current_polyline = new std::vector<p2t::Point*>();
}

DLLFUNC void tr_new_polyline() {

	// Only push if there are elements in the last list
	if (tr_current_polyline->size() > 0) {
		tr_polylines.push_back(tr_current_polyline);
		tr_current_polyline = new std::vector<p2t::Point*>();
	}
}

DLLFUNC void tr_add_point(float _x, float _y) {
	tr_current_polyline->push_back(new p2t::Point(_x, _y));
}

DLLFUNC void tr_get_point(int _polylineID, int _pointID, float &_x, float &_y) {
	if (tr_polylines.size() > _polylineID) {
		if (tr_polylines.at(_polylineID)->size() > _pointID) {
			_x = tr_polylines.at(_polylineID)->at(_pointID)->x;
			_y = tr_polylines.at(_polylineID)->at(_pointID)->y;
		}
	}
}

DLLFUNC int tr_get_polyline_count() {
	return tr_polylines.size();
}

DLLFUNC int tr_get_point_count(int _polylineID) {
	if (tr_polylines.size() > _polylineID) {
		return tr_polylines.at(_polylineID)->size();
	}
	return -1;
}

DLLFUNC void tr_execute() {

	// If there is still a polygon in the pipe, add it
	if (tr_current_polyline->size() > 0) {
		tr_polylines.push_back(tr_current_polyline);
		tr_current_polyline = new std::vector<p2t::Point*>();
	}

	if (tr_polylines.size() > 0) {
		p2t::CDT* cdt = new p2t::CDT(*tr_polylines.at(0));
		cdt->Triangulate();
		tr_triangles = cdt->GetTriangles();
		tr_map = cdt->GetMap();
	}
}

DLLFUNC int tr_get_triangle_count() {
	return tr_triangles.size();
}

DLLFUNC void tr_get_triangle_at(int _i, float &_x1, float &_y1, float &_x2, float &_y2, float &_x3, float &_y3) {
	if (tr_triangles.size() > _i) {
		_x1 = tr_triangles.at(_i)->GetPoint(0)->x;
		_y1 = tr_triangles.at(_i)->GetPoint(0)->y;
		_x2 = tr_triangles.at(_i)->GetPoint(1)->x;
		_y2 = tr_triangles.at(_i)->GetPoint(1)->y;
		_x3 = tr_triangles.at(_i)->GetPoint(2)->x;
		_y3 = tr_triangles.at(_i)->GetPoint(2)->y;
	}
}


/*
*
* DLL Main
*
*/


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
  engine_bind();
  return TRUE;
}

