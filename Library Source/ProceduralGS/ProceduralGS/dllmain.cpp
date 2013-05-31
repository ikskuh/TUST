// dllmain.cpp : Definiert den Einstiegspunkt für die DLL-Anwendung.
#include "stdafx.h"

#include <iostream>
#include <math.h>
#include <algorithm>
#include <time.h>

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


BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
  engine_bind();
  return TRUE;
}

