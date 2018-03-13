#ifndef _DALEK_THREADED_COMMON_HG_
#define _DALEK_THREADED_COMMON_HG_

#include "Utilities.h"
#include <glm/glm.hpp>

struct sDalekThreadInfo
{
	sDalekThreadInfo() :
		handle(0), address(0) {};
	HANDLE handle;
	DWORD address;
};


class cGameObject;		// forward declare

// Makes a game object, sets the location, and returns pointer
cGameObject* MakeDalekGameObject(glm::vec3 position);

class cDalek;		// Forward declare

extern cRandThreaded* g_pThreadedRandom;


#endif
