#ifndef _cAABBBroadPhase_HG_
#define _cAABBBroadPhase_HG_

#include "cAABBv2.h"
#include "cMesh.h"

class cAABBBroadPhase
{
public:
	cAABBBroadPhase();
	~cAABBBroadPhase();

	bool genAABBGridFromMesh(cMesh &theMesh);

	// Contained AABBs
	std::map< unsigned long long, cAABBv2* > mapIDToAABB;

};

#endif
