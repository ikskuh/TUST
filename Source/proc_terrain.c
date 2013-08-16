#ifndef __PROC_TERRAIN_C__
#define __PROC_TERRAIN_C__

#include "noise.h"

BMAP* generate_random_heightmap(int _width, int _height, float _altitude) {
	
	// Loop variables
	int i,j;
	
	// Color and Height map values
	float fRed, fGreen, fBlue;
	COLOR cPixelColor;
	var vFormat, vPixel;
	
	// The heightmap
	BMAP* b = bmap_createblack(_width, _height, 24);
	
	
	for(i=0; i<_width; i++) {
	
		for(j=0; j<_height; j++) {
			// Calculate new vertex height
			float newHeight = noise2d((float)i / _altitude, (float)j / _altitude);	
			
			// Calculate pixel for the height map
			fBlue  = (newHeight / 2.0 + 0.5) * 256.0;
			fGreen = (newHeight / 2.0 + 0.5) * 256.0;
			fRed   = (newHeight / 2.0 + 0.5) * 256.0;	
			
			// Set pixel to the height map
			vFormat = bmap_lock(b,0);
			vec_set(cPixelColor,vector(fBlue, fGreen, fRed));
			vPixel = pixel_for_vec(cPixelColor, 100, vFormat);
			pixel_to_bmap(b, _width - i - 1, j, vPixel);
			bmap_unlock (b);					
		}	
		
	}
	return b;
}

ENTITY* terrain_from_heightmap(VECTOR* _pos, BMAP* _source, int _verticesX, int _verticesY, int _vertexSize, float _altitude) {
	
	// Loop variables
	int i,j;
	
	// Color and Height map values
	float fRed, fGreen, fBlue;
	COLOR cPixelColor;
	
	// Variables to draw on bitmaps
	var vFormat, vPixel;
	
	// Size
	int sizeX = bmap_width(_source);
	int sizeY = bmap_height(_source);
	
	// Create a flat terrain
	ENTITY* entTerrain = ent_createterrain(_source, _pos, _verticesX, _verticesY, _vertexSize);
	
	i = 25;
	j = 1;
	for(i=0; i<sizeX; i++) {
	
		for(j=0; j<sizeY; j++) {
		
			// Get vertex position
			VECTOR* vecNextVertex = vector(
				(entTerrain.x - entTerrain.min_x) - i * ((entTerrain.max_x - entTerrain.min_x) / sizeX),
				(entTerrain.y - entTerrain.min_y) - j * ((entTerrain.max_y - entTerrain.min_y) / sizeY),
				0
			);
			
			// Get vertex
			var vertexNumber = ent_nextvertex(entTerrain, vecNextVertex);
			CONTACT* c = ent_getvertex(entTerrain,NULL,vertexNumber);
			
			// Read pixel from heightmap
			vFormat = bmap_lock(_source, 0);
			vPixel = pixel_for_bmap(_source, sizeX - i, j);
			pixel_to_vec(cPixelColor, NULL, vFormat, vPixel);
			bmap_unlock(_source);
			
			// Set the new vertex position
			if (c != NULL) {
				c.v.y = 0.1+(cPixelColor.blue * _altitude);
				ent_setvertex(entTerrain,c,vertexNumber);
			}
			
		}
	}
	
	// Update collosion hull
	c_updatehull(entTerrain, 1);
	
	return entTerrain;	
}

BMAP* heightmap_to_colormap(BMAP* _heightmap) {

	BMAP* bmapColorMap = bmap_createblack(bmap_width(_heightmap), bmap_height(_heightmap), 24);
	var vFormat, vPixel;
	COLOR cPixelColor;
	int nWidth = bmap_width(_heightmap);
	int i,j;
	
	for(i=0; i<nWidth; i++) {
		for(j=0; j<nWidth; j++) {
			
			vFormat = bmap_lock(_heightmap,0);
			vPixel  = pixel_for_bmap(_heightmap,nWidth - i, j);
			pixel_to_vec(cPixelColor, NULL, vFormat, vPixel);
			
			
			cPixelColor.blue  = 150 + cPixelColor.blue;
			cPixelColor.green = 100 + cPixelColor.green;
			cPixelColor.red   = 10  + cPixelColor.red;
			
			vFormat = bmap_lock(bmapColorMap, 0);
			vPixel  = pixel_for_vec(cPixelColor, 100, vFormat);
			pixel_to_bmap(bmapColorMap,(nWidth-1)-i,j,vPixel);
			bmap_unlock(_heightmap);
			bmap_unlock(bmapColorMap);
		}
	}
	return bmapColorMap;
}

#endif