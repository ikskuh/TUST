// TextureGen.cpp : Definiert die exportierten Funktionen für die DLL-Anwendung.
//

#include "stdafx.h"
#include <math.h>

#define DLL_USE

#include "adll.h"

BOOL APIENTRY DllMain(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{

}

DLLFUNC var ldexpc(var x,var n)
{
	return (_VAR(_FLOAT(x)*pow(2,_FLOAT(n))));
}