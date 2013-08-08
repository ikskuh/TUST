#ifndef __PROC_TERRAIN_C__
#define __PROC_TERRAIN_C__

#include "..\\Source\\noise.h"

ENTITY* generate_random_terrain(VECTOR* _pos, int _verticesX, int _verticesY, int _vertexSize, float _altitude) {
	
	// Loop variables
	int i,j;
	
	// Color and Height map values
	float fRed, fGreen, fBlue;
	COLOR cPixelColor;
	BMAP* bmapColorMap  = bmap_createblack(256, 256, 24);
	BMAP* bmapHeightMap = bmap_createblack(256, 256, 24);
	
	// Variables to draw on bitmaps
	var vFormat, vPixel;
	
	// Create a flat terrain
	ENTITY* entTerrain = ent_createterrain(bmapHeightMap, _pos, _verticesX, _verticesY, _vertexSize);
	
	// Enable terrain shader
	entTerrain.material = mat_terrain_multi_texture;
	
	for(i=0; i<bmap_width(bmapColorMap); i++) {
	
		for(j=0; j<bmap_height(bmapColorMap); j++) {
		
			// Get vertex position
			VECTOR* vecNextVertex = vector(
				(entTerrain.x - entTerrain.min_x) - i * ((_verticesX * _vertexSize) / bmap_width(bmapHeightMap)),
				(entTerrain.y - entTerrain.min_y) - j * ((_verticesY * _vertexSize) / bmap_height(bmapHeightMap)),
				entTerrain.z
			);
			
			var vertexNumber = ent_nextvertex(entTerrain, vecNextVertex);
			CONTACT* c = ent_getvertex(entTerrain,NULL,vertexNumber);
			
			// Calculate new vertex height
			float newHeight = noise2d((float)i / 32.0, (float)j / 32.0);
			
			// Set the new vertex
			if (c != NULL) {
				c.v.y = 20 * (newHeight * _altitude);
				ent_setvertex(entTerrain,c,vertexNumber);
			}
			
			
			
			// Calculate pixel for the height map
			fBlue  = (newHeight / 2.0 + 0.5) * 256.0;
			fGreen = (newHeight / 2.0 + 0.5) * 256.0;
			fRed   = (newHeight / 2.0 + 0.5) * 256.0;
			
			// Set pixel to the height map
			vFormat = bmap_lock(bmapHeightMap,0);
			vec_set(cPixelColor,vector(fBlue, fGreen, fRed));
			vPixel = pixel_for_vec(cPixelColor, 100, vFormat);
			pixel_to_bmap(bmapHeightMap, bmap_width(bmapHeightMap) - i - 1, j, vPixel);
			bmap_unlock (bmapHeightMap);
			
			
			
			// Calculate pixel for the color map
			fBlue  = 150 + (newHeight / 2.0 + 0.5) * 256.0;
			fGreen = 100 + (newHeight / 2.0 + 0.5) * 256.0;
			fRed   = 10  + (newHeight / 2.0 + 0.5) * 256.0;	
			
			// Set pixel to the color map
			vFormat = bmap_lock(bmapColorMap,0);
			vec_set(cPixelColor,vector(fBlue,fGreen,fRed));
			vPixel = pixel_for_vec(cPixelColor, 100, vFormat);
			pixel_to_bmap(bmapColorMap, bmap_width(bmapColorMap) - i - 1, j, vPixel);
			bmap_unlock (bmapColorMap);					
		}
	}
	
	// Update collosion hull
	//c_updatehull(entTerrain, 1);
	
	// Set skins (Order is important for the shader)
	ent_setskin(entTerrain, bmapHeightMap, 1);
	ent_setskin(entTerrain, bmapColorMap, 2);
	//BMAP* bmapDisplacementMap = bmap_to_displacementmap(bmapHeightMap);
	//ent_setskin(entTerrain, bmapDisplacementMap, 3);
	
	while(1) {
		//DEBUG_BMAP(bmapHeightMap, 1, 1);
		//DEBUG_BMAP(bmapColorMap, 257, 1);
		//DEBUG_BMAP(bmapDisplacementMap, 600, 1);
		wait(1);
	}
		
	return entTerrain;
}

BMAP* bmap_to_displacementmap(BMAP* _bitmap) {
	//var i;
	int px, py;
	
	var vFormat1,vFormat2;
	var pixel,pixel2;
	
	VECTOR pixelcolor;
	var pixelalpha;
	VECTOR vektor1;
	VECTOR vektor2;
	VECTOR vektor3;
	VECTOR vektor4;
	VECTOR vektor5;
	VECTOR vektor6;
	VECTOR vektor7;
	VECTOR vektor8;
	VECTOR vektor9;
	int nSizeX = bmap_width(_bitmap);
	
	BMAP* bmapResult = bmap_createblack(nSizeX, nSizeX, 24);

	py=0;
	while(py<nSizeX)
	{
		px=0;
		while(px<nSizeX)
		{
			vFormat1 = bmap_lock(_bitmap,0);
			vFormat2 = bmap_lock(bmapResult,0);
			
			pixel=pixel_for_bmap(_bitmap,clamp((nSizeX-1)-px-1,0,nSizeX-1),clamp(py+1,0,nSizeX-1));
			pixel_to_vec(vektor1,pixelalpha,vFormat1,pixel);
			pixel=pixel_for_bmap(_bitmap,clamp((nSizeX-1)-px-1,0,nSizeX-1),clamp(py,0,nSizeX-1));
			pixel_to_vec(vektor2,pixelalpha,vFormat1,pixel);
			pixel=pixel_for_bmap(_bitmap,clamp((nSizeX-1)-px,0,nSizeX-1),clamp(py+1,0,nSizeX-1));
			pixel_to_vec(vektor3,pixelalpha,vFormat1,pixel);
			pixel=pixel_for_bmap(_bitmap,clamp((nSizeX-1)-px,0,nSizeX-1),clamp(py-1,0,nSizeX-1));
			pixel_to_vec(vektor4,pixelalpha,vFormat1,pixel);
			pixel=pixel_for_bmap(_bitmap,clamp((nSizeX-1)-px,0,nSizeX-1),clamp(py+1,0,nSizeX-1));
			pixel_to_vec(vektor5,pixelalpha,vFormat1,pixel);
			pixel=pixel_for_bmap(_bitmap,clamp((nSizeX-1)-px+1,0,nSizeX-1),clamp(py-1,0,nSizeX-1));
			pixel_to_vec(vektor6,pixelalpha,vFormat1,pixel);
			pixel=pixel_for_bmap(_bitmap,clamp((nSizeX-1)-px+1,0,nSizeX-1),clamp(py,0,nSizeX-1));
			pixel_to_vec(vektor7,pixelalpha,vFormat1,pixel);
			pixel=pixel_for_bmap(_bitmap,clamp((nSizeX-1)-px+1,0,nSizeX-1),clamp(py+1,0,nSizeX-1));
			pixel_to_vec(vektor8,pixelalpha,vFormat1,pixel);
			pixelcolor.x=(vektor6.x+1 * vektor7.x+vektor8.x - vektor1.x-1 * vektor2.x-vektor3.x);
			pixelcolor.y=(vektor3.x+1 * vektor5.x+vektor8.x - vektor1.x-1 * vektor4.x-vektor6.x);
			
			pixel2=pixel_for_vec(pixelcolor,100,vFormat2);
			pixel_to_bmap(bmapResult,(nSizeX-1)-px,py,pixel2);
			
			bmap_unlock(_bitmap);
			bmap_unlock(bmapResult);
			px+=1;
		}
		py+=1;
	}
	bmapResult = bmap_to_blur(bmapResult, 2);
	return bmapResult;
}

BMAP* bmap_to_blur(BMAP* _bitmap, int _passes) {
	
	int i;
	var px;
	var py;
	var format;
	var pixel;
	VECTOR pixelcolor;
	var pixelalpha;
	VECTOR vektor1;
	VECTOR vektor2;
	VECTOR vektor3;
	VECTOR vektor4;
	VECTOR vektor5;
	VECTOR vektor6;
	VECTOR vektor7;
	VECTOR vektor8;
	VECTOR vektor9;
	var sizex=bmap_width(_bitmap);
	
	BMAP* bmapResult = bmap_createblack(256, 256, 24);

	i=0;

	while(i<_passes)
	{
		py=0;
		while(py<sizex)
		{
			px=0;
			while(px<sizex)
			{
				format = bmap_lock(_bitmap, 0);
				bmap_lock(bmapResult, 0);
				
				pixel=pixel_for_bmap(_bitmap,clamp((sizex-1)-px,0,sizex-1),clamp(py,0,sizex-1));
				pixel_to_vec(vektor1,pixelalpha,format,pixel);
				
				pixel=pixel_for_bmap(_bitmap,clamp((sizex-1)-px-1,0,sizex-1),clamp(py,0,sizex-1));
				pixel_to_vec(vektor2,pixelalpha,format,pixel);
				pixel=pixel_for_bmap(_bitmap,clamp((sizex-1)-px-1,0,sizex-1),clamp(py+1,0,sizex-1));
				pixel_to_vec(vektor3,pixelalpha,format,pixel);
				pixel=pixel_for_bmap(_bitmap,clamp((sizex-1)-px,0,sizex-1),clamp(py+1,0,sizex-1));
				pixel_to_vec(vektor4,pixelalpha,format,pixel);
				pixel=pixel_for_bmap(_bitmap,clamp((sizex-1)-px+1,0,sizex-1),clamp(py+1,0,sizex-1));
				pixel_to_vec(vektor5,pixelalpha,format,pixel);
				pixel=pixel_for_bmap(_bitmap,clamp((sizex-1)-px+1,0,sizex-1),clamp(py,0,sizex-1));
				pixel_to_vec(vektor6,pixelalpha,format,pixel);
				pixel=pixel_for_bmap(_bitmap,clamp((sizex-1)-px+1,0,sizex-1),clamp(py-1,0,sizex-1));
				pixel_to_vec(vektor7,pixelalpha,format,pixel);
				pixel=pixel_for_bmap(_bitmap,clamp((sizex-1)-px,0,sizex-1),clamp(py-1,0,sizex-1));
				pixel_to_vec(vektor8,pixelalpha,format,pixel);
				pixel=pixel_for_bmap(_bitmap,clamp((sizex-1)-px-1,0,sizex-1),clamp(py-1,0,sizex-1));
				pixel_to_vec(vektor9,pixelalpha,format,pixel);
				pixelcolor.x=integer((vektor1.x*7+vektor2.x*2+vektor3.x+vektor4.x*2+vektor5.x+vektor6.x*2+vektor7.x+vektor8.x*2+vektor9.x)/19);
				pixelcolor.y=integer((vektor1.y*7+vektor2.y*2+vektor3.y+vektor4.y*2+vektor5.y+vektor6.y*2+vektor7.y+vektor8.y*2+vektor9.y)/19);
				pixelcolor.z=integer((vektor1.z*7+vektor2.z*2+vektor3.z+vektor4.z*2+vektor5.z+vektor6.z*2+vektor7.z+vektor8.z*2+vektor9.z)/19);
				pixel=pixel_for_vec(pixelcolor,100,format);
				
				
				pixel_to_bmap(bmapResult,(sizex-1)-px,py,pixel);
				
				bmap_unlock(bmapResult);
				bmap_unlock(_bitmap);
				
				px+=1;
			}
			py+=1;
			//wait(1);
		}
		i+=1;
	}
	return bmapResult;
}


#endif