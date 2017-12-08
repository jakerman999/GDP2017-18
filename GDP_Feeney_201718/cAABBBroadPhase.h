#ifndef _cAABBBroadPhase_HG_
#define _cAABBBroadPhase_HG_

#include "cAABBv2.h"
#include "cMesh.h"

// Forward declare to avoid include
class cDebugRenderer;

class cAABBBroadPhase
{
public:
	cAABBBroadPhase();
	~cAABBBroadPhase();

	bool genAABBGridFromMesh(cMesh &theMesh);

	// The triangle collision information
	// Because there isn't a useful triangle class already 
	//	made, here's a specific one for the point-triangle detection
	//	for the narrow phase
	struct sTriAABB
	{
		sTriAABB()
		{	// Clear the vertex values to zeros
			verts[0] = verts[1] = verts[2] = glm::vec3(0.0f);
		}
		glm::vec3 verts[3];		// 3 corners of the triangle
		float largestLength;
		// TODO: Add sexy math code to do this...
		void CalcLargestSide(void);
	};

	// Call this method to see what triangles MIGHT hit this test point
	bool areThereTrianglesInAABB(glm::vec3 testPoint);
	bool getVecTrianglesInAABB(glm::vec3 testPoint, std::vector<sTriAABB> &vecTris);

	// Contained AABBs
	std::map< unsigned long long, cAABBv2* > mapIDToAABB;

	cDebugRenderer* pDebugRenderer;
};

#endif
