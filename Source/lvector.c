#include <acknex.h>
#include "lvector.h"

int lvectorLastFrame = 0;
int lvectorStoragePointer = 0;
int lvectorStorageSize = 0;
VECTOR *lvectorStorage = NULL;

VECTOR *lvector(var x, var y, var z)
{
	if(lvectorStorage == NULL)
	{
		lvectorStorageSize = 1024;
		lvectorStorage = sys_malloc(lvectorStorageSize * sizeof(VECTOR));
	}
	if(lvectorLastFrame < total_frames)
	{
		lvectorStoragePointer = 0;
		lvectorLastFrame = total_frames;
	}
	
	int currentPos = lvectorStoragePointer;
	lvectorStoragePointer += 1;
	
	if(lvectorStoragePointer >= lvectorStorageSize)
	{
		VECTOR *oldLvectorStorage = lvectorStorage;
		int oldLvectorStorageSize = lvectorStorageSize;
		
		lvectorStorageSize += 1024;
		lvectorStorage = sys_malloc(lvectorStorageSize * sizeof(VECTOR));
		memcpy(lvectorStorage, oldLvectorStorage, sizeof(VECTOR) * oldLvectorStorageSize);
		
		sys_free(oldLvectorStorage);
	}
	
	lvectorStorage[currentPos].x = x;
	lvectorStorage[currentPos].y = y;
	lvectorStorage[currentPos].z = z;
	
	return &(lvectorStorage[currentPos]);
}

int lvector_usage()
{
	return sizeof(VECTOR) * lvectorStorageSize;
}