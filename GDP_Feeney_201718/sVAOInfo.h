#ifndef _sVAOInfo_HG_
#define _sVAOInfo_HG_

#include <float.h>

struct sVAOInfo
{
	sVAOInfo() : VAO_ID(0), shaderID(-1), 
		         vertex_buffer_ID(0), index_buffer_ID(0),
		         numberOfVertices(0), numberOfTriangles(0),
				 numberOfIndices(0), 
				 scaleForUnitBBox(1.0f),
				 // Object won't draw until distance is this value or ABOVE
				 // For objects with no LOD, setting to 0.0 means it's always drawn
				 //  because we are always greater than 0.0 units from the camera
				 //  (we are at least 'near clip plane' away from camera, actually)
		         LOD_MinDistanceToDraw(0.0f)	
	{ }
	std::string friendlyName;
	// OpenGL rendering specific stuff we need...
	unsigned int VAO_ID;	// **** The thing we REALLY want

	// We might need this information to draw (or "render")
	unsigned int numberOfVertices;
	// For LOD, the both the draw distance and the numberOfTriangles is used
	//	to determine the most appropriate model
	unsigned int numberOfTriangles;
	unsigned int numberOfIndices;		/****/


	int shaderID;			// Perhaps?
	// Spoiler alert: we don't need these, actually...
	unsigned int vertex_buffer_ID;		// GLuint
	unsigned int index_buffer_ID;

	float scaleForUnitBBox;

	// for LOD lookup
	// Minimum distance to draw this object 
	// Greater than this and I don't draw
	float LOD_MinDistanceToDraw; 
};


#endif
