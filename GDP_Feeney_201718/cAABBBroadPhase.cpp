#include "cAABBBroadPhase.h"

#include "cDebugRenderer.h"

#include <algorithm>			// for the std::copy

cAABBBroadPhase::cAABBBroadPhase()
{
	return;
}

cAABBBroadPhase::~cAABBBroadPhase()
{
	return;
}

bool cAABBBroadPhase::genAABBGridFromMesh(cMesh &theMesh)
{
	// Assume that the debug renderer is valid
//	this->pDebugRenderer->addTriangle( )


	return true;
}


bool cAABBBroadPhase::areThereTrianglesInAABB(glm::vec3 testPoint)
{
	// Generate an AABB ID for this test location
	unsigned long long testPointAABBID = cAABBv2::calculateID(testPoint);

	// Find an AABB IF it exists
	std::map< unsigned long long, cAABBv2* >::iterator itAABB = 
		this->mapIDToAABB.find( testPointAABBID );

	// Does it exist? 
	if (itAABB == this->mapIDToAABB.end())
	{
		// NOPE. There are NO AABBs at that location
		return false;
	}
	// Otherwise there IS an AABB
	return true;
}

bool cAABBBroadPhase::getVecTrianglesInAABB(glm::vec3 testPoint, std::vector<sTriAABB> &vecTris)
{
	// Exactly the same as the areThereTrianglesInAABB() but copies triangle info

	// Generate an AABB ID for this test location
	unsigned long long testPointAABBID = cAABBv2::calculateID(testPoint);

	// Find an AABB IF it exists
	std::map< unsigned long long, cAABBv2* >::iterator itAABB =
		this->mapIDToAABB.find(testPointAABBID);

	// Does it exist? 
	if (itAABB == this->mapIDToAABB.end())
	{
		// NOPE. There are NO AABBs at that location
		return false;
	}
	
	// There IS an AABB here, so copy the AABBs triangles for the narrow phase
	//std::copy(
	//	itAABB->second->vecTriangles.begin(),
	//	itAABB->second->vecTriangles.end(),
	//	vecTris.begin());

	return true;
}
