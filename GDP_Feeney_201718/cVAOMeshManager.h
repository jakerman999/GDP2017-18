#ifndef _cVAOMeshManager_HG_
#define _cVAOMeshManager_HG_

#include <string>

//#include "cMesh.h"
// Instead of including, which means possibly including 
//	stuff we don't need or want to know about, we 
//  'forward declare'
class cMesh;		// Forward declare

#include <map>		// "dictonary" or PHP array

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN  
#endif 

#include <Windows.h>		// For the critical sections

#include "sVAOInfo.h"
#include <vector>

//struct sVAOInfo
//{
//	sVAOInfo() : VAO_ID(0), shaderID(-1), 
//		         vertex_buffer_ID(0), index_buffer_ID(0),
//		         numberOfVertices(0), numberOfTriangles(0),
//				 numberOfIndices(0), 
//				 scaleForUnitBBox(1.0f)
//	{ }
//	std::string friendlyName;
//	// OpenGL rendering specific stuff we need...
//	unsigned int VAO_ID;	// **** The thing we REALLY want
//
//	// We might need this information to draw (or "render")
//	unsigned int numberOfVertices;
//	unsigned int numberOfTriangles;
//	unsigned int numberOfIndices;		/****/
//
//
//	int shaderID;			// Perhaps?
//	// Spoiler alert: we don't need these, actually...
//	unsigned int vertex_buffer_ID;		// GLuint
//	unsigned int index_buffer_ID;
//
//	float scaleForUnitBBox;
//};



// This will handle: 
// - taking a cMesh and loading into the various buffers
// - take a mesh "name" and returning the desired VAO info 
//   (for rendering)
class cVAOMeshManager
{
public:
	cVAOMeshManager();
	~cVAOMeshManager();
	// Take name from mesh for lookup (for rendering)
	bool loadMeshIntoVAO( cMesh &theMesh, int shaderID, bool bKeepMesh = false  );
	// Same as above, but also stores distance information for rendering 
	//	different resolutions of LOD... 
	bool loadMeshIntoVAO( cMesh &theMesh, int shaderID, 
						  float LOD_MinDistFromCamera, bool bKeepMesh = false  );

	// During rendering (aka 'drawing'), get the info we need.
	bool lookupVAOFromName( std::string name, sVAOInfo &theVAOInfo );
	// If we pass zero (0.0f), then we return the CLOSEST distance
	// (in other words, the HIGHEST resolution model - this is in cases
	//  where there ISN'T several LOD models loaded)
	bool lookupVAOFromName( std::string name, sVAOInfo &theVAOInfo, 
						    // Pass in a float indicating how far the object is... 
						    float LOD_MinDistFromCamera );

	// Return mesh by name
	bool lookupMeshFromName( std::string name, cMesh &theMesh);
	void Lock_mapNumberToName(void);
	void Lock_mapNameToVAO(void);
	void Lock_mapNameToMesh(void);

	void UnLock_mapNumberToName(void);
	void UnLock_mapNameToVAO(void);
	void UnLock_mapNameToMesh(void);

	bool IsLocked_mapNumberToName(void);
	bool IsLocked_mapNameToVAO(void);
	bool IsLocked_mapNameToMesh(void);

private:

	std::map<int, std::string> m_mapNumberToName;
	bool m_mapNumberToName_IsLocked;
	CRITICAL_SECTION m_CS_mapNumberToName;

	// Loop up from name to sVAOInfo
	// 1st is what I'm indexing by (i.e. type)
	// 2nd is what I'm actually storing (the type)
	// The vector stores MORE than one mesh IF there are LOD meshes here
	std::map< std::string, std::vector<sVAOInfo> > m_mapNameTo_vecVAO;
	bool m_mapNameToVAO_Is_Locked;
	CRITICAL_SECTION m_CS_mapNameToVAO;


	// The dynamic VAOs
	// Meshes that I want to keep around (like the terrain?)
	// TODO: Deal with the LOD information for the mesh
	// ****************************************************************
	// GREAT BIG NOTE: 
	//  These are stored by the "friendly names", but for LOD meshes,
	//  the friendly name can link to MORE THAN ONE mesh. 
	//  However, if you are dynamically altering the mesh, you are 
	//  very likely NOT using LOD at the same time. 
	// ****************************************************************
	std::map< std::string, cMesh > m_mapNameToMesh;
	bool m_mapNameToMesh_Is_Locked;
	CRITICAL_SECTION m_CS_mapNameToMesh;

//	sVAOInfo theVAOS[10];
};


#endif
