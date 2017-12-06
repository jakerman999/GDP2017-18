#include "ModelUtilities.h" 
#include "cVAOMeshManager.h"
#include "cMesh.h"
#include <sstream>

cModelAssetLoader* g_pModelAssetLoader = NULL;

bool Load3DModelsIntoMeshManager( int shaderID, 
								  cVAOMeshManager* pVAOManager, 
								  cModelAssetLoader* pModelAssetLoader,
								  std::string &error )
{
	std::stringstream ssError;
	bool bAllGood = true;

	{
		cMesh testMesh;
		testMesh.name = "MeshLabTerrain_xyz_n_uv";
		if ( ! pModelAssetLoader->LoadPlyFileIntoMeshWith_Normals_and_UV( "MeshLabTerrain_xyz_n_uv.ply", testMesh ) )
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
		if ( ! pModelAssetLoader->LoadPlyFileIntoMesh( "bun_zipper_res2_xyz.ply", testMesh ) )
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
		if ( ! pModelAssetLoader->LoadPlyFileIntoMesh( "Utah_Teapot_1Unit_xyz.ply", testMesh ) )
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
		if ( ! pModelAssetLoader->LoadPlyFileIntoMesh( "dolphin_xyz.ply", testMesh ) )
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
		if ( ! pModelAssetLoader->LoadPlyFileIntoMesh( "Flat_XZ_Plane_xyz.ply", testMesh ) )
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