#include "ModelUtilities.h" 
#include "cVAOMeshManager.h"
#include "cMesh.h"
#include <sstream>
#include <vector>
#include "cGameObject.h"

cModelAssetLoader* g_pModelAssetLoader = NULL;

// Returns 0 or NULL if not found
cGameObject* findObjectByFriendlyName(std::string friendlyName, std::vector<cGameObject*> &vec_pGameObjects)
{
	// Linear search, baby!
	unsigned int numObjects = (unsigned int)vec_pGameObjects.size();
	for (unsigned int index = 0; index != numObjects; index++)
	{
		if (vec_pGameObjects[index]->friendlyName == friendlyName)
		{
			return vec_pGameObjects[index];
		}
	}
	// Didn't find it
	return NULL;
}

cGameObject* findObjectByUniqueID(unsigned int ID, std::vector<cGameObject*> &vec_pGameObjects)
{
	// Linear search, baby!
	unsigned int numObjects = (unsigned int)vec_pGameObjects.size();
	for (unsigned int index = 0; index != numObjects; index++)
	{
		if ( ID = vec_pGameObjects[index]->getUniqueID() )
		{
			return vec_pGameObjects[index];
		}
	}
	// Didn't find it
	return NULL;
}

//class cLoaderThing
//{
//	class cMeshLoadData
//	{	cMeshLoadData : bIsLoadedFromFile(false), bIsLoadedIntoVAO(false) {};
//		cMesh theMesh;
//		bool bIsLoadedFromFile;		// false
//		bool bIsLoadedIntoVAO;		// false
//	};
//
//	cMeshLoadData theMeshesBeingLoaded[100]; 
//
//	bool Load3DModelsIntoMeshManager(  string fileName, cMeshLoadData* meshLoad );
//	void CheckStatusOfLoad(void):
//};

bool Load3DModelsIntoMeshManager( int shaderID, 
								  cVAOMeshManager* pVAOManager, 
								  cModelAssetLoader* pModelAssetLoader,
								  std::string &error )
{
	std::stringstream ssError;
	bool bAllGood = true;

	{
		cMesh testMesh;
		testMesh.name = "ReallyBigShip";
		if ( ! pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV( "basestar_REALLY_BIG.ply", testMesh ) )
		{ 
			//std::cout << "Didn't load model" << std::endl;
			ssError << "Didn't load model >" << testMesh.name << "<" << std::endl;
			bAllGood = false;
		}
		// ***********************************************************************
		// NOTE the TRUE so that it keeps the mesh!!!
		if ( ! pVAOManager->loadMeshIntoVAO( testMesh, shaderID, true ) )
		{
			//std::cout << "Could not load mesh into VAO" << std::endl;
			ssError << "Could not load mesh >" << testMesh.name << "< into VAO" << std::endl;
			bAllGood = false;
		}
		// ***********************************************************************
	}	
	{
		cMesh testMesh;
		testMesh.name = "SmoothSphereRadius1";
		if ( ! pModelAssetLoader->LoadPlyFileIntoMeshWithNormals( "SmoothSphere_xyz_n.ply", testMesh ) )
		{ 
			//std::cout << "Didn't load model" << std::endl;
			ssError << "Didn't load model >" << testMesh.name << "<" << std::endl;
			bAllGood = false;
		}
		// ***********************************************************************
		// NOTE the TRUE so that it keeps the mesh!!!
		if ( ! pVAOManager->loadMeshIntoVAO( testMesh, shaderID, true ) )
		{
			//std::cout << "Could not load mesh into VAO" << std::endl;
			ssError << "Could not load mesh >" << testMesh.name << "< into VAO" << std::endl;
			bAllGood = false;
		}
		// ***********************************************************************
	}	
	{
		cMesh testMesh;
		testMesh.name = "SmoothSphereRadius1InvertedNormals";
		if ( ! pModelAssetLoader->LoadPlyFileIntoMeshWithNormals( "SmoothSphere_Inverted_Normals_xyz_n.ply", testMesh ) )
		{ 
			//std::cout << "Didn't load model" << std::endl;
			ssError << "Didn't load model >" << testMesh.name << "<" << std::endl;
			bAllGood = false;
		}
		// ***********************************************************************
		// NOTE the TRUE so that it keeps the mesh!!!
		if ( ! pVAOManager->loadMeshIntoVAO( testMesh, shaderID, true ) )
		{
			//std::cout << "Could not load mesh into VAO" << std::endl;
			ssError << "Could not load mesh >" << testMesh.name << "< into VAO" << std::endl;
			bAllGood = false;
		}
		// ***********************************************************************
	}	
	{
		cMesh testMesh;
		testMesh.name = "MeshLabTerrain_xyz_n_uv";
		if ( ! pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV( "MeshLabTerrain_xyz_n_uv.ply", testMesh ) )
		//if ( ! pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV( "MeshLabTerrain_FLAT_xyz_n_uv.ply", testMesh ) )
		{ 
			//std::cout << "Didn't load model" << std::endl;
			ssError << "Didn't load model >" << testMesh.name << "<" << std::endl;
			bAllGood = false;
		}
		// ***********************************************************************
		// NOTE the TRUE so that it keeps the mesh!!!
		if ( ! pVAOManager->loadMeshIntoVAO( testMesh, shaderID, true ) )
		{
			//std::cout << "Could not load mesh into VAO" << std::endl;
			ssError << "Could not load mesh >" << testMesh.name << "< into VAO" << std::endl;
			bAllGood = false;
		}
		// ***********************************************************************
	}	
	{
		cMesh testMesh;
		testMesh.name = "teapotUV";
		if ( ! pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV( "Utah_Teapot_xyz_n_uv.ply", testMesh ) )
		{ 
			//std::cout << "Didn't load model" << std::endl;
			ssError << "Didn't load model >" << testMesh.name << "<" << std::endl;
			bAllGood = false;
		}
		if ( ! pVAOManager->loadMeshIntoVAO( testMesh, shaderID ) )
		{
			//std::cout << "Could not load mesh into VAO" << std::endl;
			ssError << "Could not load mesh >" << testMesh.name << "< into VAO" << std::endl;
			bAllGood = false;
		}
	}

	{
		cMesh testMesh;
		testMesh.name = "bunny";
		if ( ! pModelAssetLoader->LoadPlyFileIntoMeshXYZOnly( "bun_zipper_res2_xyz.ply", testMesh ) )
		{ 
			//std::cout << "Didn't load model" << std::endl;
			ssError << "Didn't load model >" << testMesh.name << "<" << std::endl;
			bAllGood = false;
		}
		if ( ! pVAOManager->loadMeshIntoVAO( testMesh, shaderID ) )
		{
			//std::cout << "Could not load mesh into VAO" << std::endl;
			ssError << "Could not load mesh >" << testMesh.name << "< into VAO" << std::endl;
			bAllGood = false;
		}
	}
	{
		cMesh testMesh;
		testMesh.name = "teapot";
		if ( ! pModelAssetLoader->LoadPlyFileIntoMeshXYZOnly( "Utah_Teapot_1Unit_xyz.ply", testMesh ) )
		{ 
			//std::cout << "Didn't load model" << std::endl;
			ssError << "Didn't load model >" << testMesh.name << "<" << std::endl;
			bAllGood = false;
		}
		if ( ! pVAOManager->loadMeshIntoVAO( testMesh, shaderID ) )
		{
			//std::cout << "Could not load mesh into VAO" << std::endl;
			ssError << "Could not load mesh >" << testMesh.name << "< into VAO" << std::endl;
			bAllGood = false;
		}
	}	
	{
		cMesh testMesh;
		testMesh.name = "dolphin";
		if ( ! pModelAssetLoader->LoadPlyFileIntoMeshXYZOnly( "dolphin_xyz.ply", testMesh ) )
		{ 
			//std::cout << "Didn't load model" << std::endl;
			ssError << "Didn't load model >" << testMesh.name << "<" << std::endl;
			bAllGood = false;
		}
		if ( ! pVAOManager->loadMeshIntoVAO( testMesh, shaderID ) )
		{
			//std::cout << "Could not load mesh into VAO" << std::endl;
			ssError << "Could not load mesh >" << testMesh.name << "< into VAO" << std::endl;
			bAllGood = false;
		}
	}	
	{
		cMesh testMesh;
		testMesh.name = "PlaneXZ";
		if ( ! pModelAssetLoader->LoadPlyFileIntoMeshXYZOnly( "Flat_XZ_Plane_xyz.ply", testMesh ) )
		{ 
			//std::cout << "Didn't load model" << std::endl;
			ssError << "Didn't load model >" << testMesh.name << "<" << std::endl;
			bAllGood = false;
		}
		if ( ! pVAOManager->loadMeshIntoVAO( testMesh, shaderID ) )
		{
			//std::cout << "Could not load mesh into VAO" << std::endl;
			ssError << "Could not load mesh >" << testMesh.name << "< into VAO" << std::endl;
			bAllGood = false;
		}
	}	
	{
		cMesh testMesh;
		testMesh.name = "SphereRadius1";
//		if ( ! LoadPlyFileIntoMesh( "Sphereply_xyz.ply", testMesh ) )
		if ( ! pModelAssetLoader->LoadPlyFileIntoMeshWithNormals( "Sphereply_xyz_n.ply", testMesh ) )
		{ 
			//std::cout << "Didn't load model" << std::endl;
			ssError << "Didn't load model >" << testMesh.name << "<" << std::endl;
			bAllGood = false;
		}
		if ( ! pVAOManager->loadMeshIntoVAO( testMesh, shaderID ) )
		{
			//std::cout << "Could not load mesh into VAO" << std::endl;
			ssError << "Could not load mesh >" << testMesh.name << "< into VAO" << std::endl;
			bAllGood = false;
		}
	}	// ENDOF: load models

	if ( ! bAllGood ) 
	{
		// Copy the error string stream into the error string that
		//	gets "returned" (through pass by reference)
		error = ssError.str();
	}

	return bAllGood;
}