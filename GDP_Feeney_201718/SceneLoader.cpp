// This file is used to laod the models
#include "cGameObject.h"
#include <vector>
#include "Utilities.h"		// getRandInRange()
#include <glm/glm.hpp>
#include "globalGameStuff.h"

extern std::vector< cGameObject* >  g_vecGameObjects;
extern cGameObject* g_pTheDebugSphere;


//const float SURFACEOFGROUND = -10.0f;
//const float RIGHTSIDEWALL = 15.0f;
//const float LEFTSIDEWALL = -15.0f;

void LoadModelsIntoScene(void)
{
//	std::vector< cGameObject* >  g_vecGameObjects;

	//{	// ReallyBigShip
	//	cGameObject* pTempGO = new cGameObject();
	//	pTempGO->friendlyName = "ReallyBigShip";
	//	cPhysicalProperties physState;
	//	physState.integrationUpdateType = cPhysicalProperties::EXCLUDED_FROM_INTEGRATION;
	//	physState.mass = physState.inverseMass = 0.0f;	// Infinite
	//	//physState.position.z = -10.0f;
	//	//physState.position.y = 30.0f;
	//	pTempGO->SetPhysState(physState);

	//	sMeshDrawInfo meshInfo;
	//	meshInfo.scale = 1.0f;
	//	meshInfo.debugDiffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
	//	meshInfo.name = "ReallyBigShip";
	//	meshInfo.debugDiffuseColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	//	meshInfo.bUseDebugColour = true;
	//	meshInfo.bDrawAsWireFrame = true;
	//	meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("GuysOnSharkUnicorn.bmp", 1.0f));
	//	meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("Utah_Teapot_xyz_n_uv_Enterprise.bmp", 0.0f));
	//	pTempGO->vecMeshes.push_back(meshInfo);

	//	::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
	//}	


	{
		cGameObject* pTempGO = new cGameObject();
		cPhysicalProperties physState;
		physState.integrationUpdateType = cPhysicalProperties::EXCLUDED_FROM_INTEGRATION;
		physState.mass = physState.inverseMass = 0.0f;	// Infinite
		pTempGO->SetPhysState(physState);

		sMeshDrawInfo meshInfo;
		meshInfo.scale = 1.0f;
		meshInfo.debugDiffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
		meshInfo.name = "Ship_Pack_WIP_mod - command_xyz_n_uv.obj";
		//meshInfo.debugDiffuseColour = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
		//meshInfo.bUseDebugColour = true;
		//meshInfo.bDrawAsWireFrame = true;
		meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("GuysOnSharkUnicorn.bmp", 1.0f));
		meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("Utah_Teapot_xyz_n_uv_Enterprise.bmp", 0.0f));
		pTempGO->vecMeshes.push_back(meshInfo);

		::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
	}	
	
	
	{
		cGameObject* pTempGO = new cGameObject();
		cPhysicalProperties physState;
		physState.integrationUpdateType = cPhysicalProperties::EXCLUDED_FROM_INTEGRATION;
		physState.mass = physState.inverseMass = 0.0f;	// Infinite
		physState.position.x = 4.0f;
		physState.position.y = 25.0f;
//		physState.position.y = -100.0f;
		physState.setRotationalSpeedEuler( glm::vec3(0.0f, 0.5f, 0.0f) );
		pTempGO->SetPhysState(physState);


		sMeshDrawInfo meshInfo;
		meshInfo.debugDiffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
		meshInfo.scale = 350.0f;
		meshInfo.name = "bunny";	

		//meshInfo.scale = 0.65f;
		//meshInfo.name = "teapotUV";	

		// Make bunny reflective...
		meshInfo.bIsEnvirMapped = true;
		meshInfo.reflectBlendRatio = 0.5f;
		meshInfo.refractBlendRatio = 0.5f;
		meshInfo.coefficientRefract = 0.1f;

		// 
		pTempGO->friendlyName = "bugs";

		meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("barberton_etm_2001121_lrg.bmp", 1.0f));
		meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("height_map_norway-height-map-aster-30m.bmp", 0.0f));
		pTempGO->vecMeshes.push_back(meshInfo);

		::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
	}	
	{
		cGameObject* pTempGO = new cGameObject();
		cPhysicalProperties physState;
		physState.integrationUpdateType = cPhysicalProperties::EXCLUDED_FROM_INTEGRATION;
		physState.mass = physState.inverseMass = 0.0f;	// Infinite
		physState.position.y = -100.0f;
		pTempGO->SetPhysState(physState);

		sMeshDrawInfo meshInfo;
		meshInfo.scale = 1.0f;
		meshInfo.debugDiffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
		meshInfo.name = "MeshLabTerrain_xyz_n_uv";	
		meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("GuysOnSharkUnicorn.bmp", 1.0f));
		meshInfo.vecMehs2DTextures.push_back(sTextureBindBlendInfo("Utah_Teapot_xyz_n_uv_Enterprise.bmp", 0.0f));
		pTempGO->vecMeshes.push_back(meshInfo);

		::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
	}
	//{	// Right side plane
	//	cGameObject* pTempGO = new cGameObject();
	//	pTempGO->position.x = 15.0f;	// RIGHTSIDEWALL;		// Where our physics HACK detection is
	//	pTempGO->orientation.z = glm::radians(0.0f);	// Degrees
	//	pTempGO->orientation2.z = glm::radians(90.0f);	// Degrees   
	//	pTempGO->scale = 1.0f;
	//	pTempGO->diffuseColour = glm::vec4( 0.0f, 1.0f, 1.0f, 1.0f );
	//	pTempGO->meshName = "PlaneXZ";		// Was teapot
	//	pTempGO->typeOfObject = eTypeOfObject::PLANE;
	//	// ***
	//	pTempGO->bIsUpdatedInPhysics = false;	
	//	// ***
	//	::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
	//}

	{
		cGameObject* pTempGO = new cGameObject();
		cPhysicalProperties physState;
		physState.position = glm::vec3(-4.0f, 4.0f, 0.0f);
		physState.velocity = glm::vec3(2.0f, 1.0f, 0.0f);
		physState.setOrientationEulerAngles(glm::vec3(0.0f, 0.0f, 0.0f));
		physState.rigidBodyShape = cPhysicalProperties::SPHERE;
		physState.rigidBodyMeasurements.sphere_capsule_radius = 1.0f;
		pTempGO->SetPhysState(physState);
		sMeshDrawInfo meshInfo;
		meshInfo.scale = 1.0f;
		meshInfo.debugDiffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
		meshInfo.name = "SphereRadius1";
		pTempGO->vecMeshes.push_back(meshInfo);
		//
		::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
	}	
	{
		cGameObject* pTempGO = new cGameObject();
		cPhysicalProperties physState;
		physState.position = glm::vec3(-0.5f, 4.0f, 0.0f);
		physState.rigidBodyShape = cPhysicalProperties::SPHERE;
		physState.rigidBodyMeasurements.sphere_capsule_radius = 1.0f;
		pTempGO->SetPhysState(physState);
		sMeshDrawInfo meshInfo;
		meshInfo.scale = 1.0f;
		meshInfo.debugDiffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
		meshInfo.name = "SphereRadius1";			// was dolphin
		pTempGO->vecMeshes.push_back(meshInfo);
		::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
	}
	// Add a bunch of spheres....
	for ( int count = 0; count != 20; count++ )
	{
		cGameObject* pTempGO = new cGameObject();
		cPhysicalProperties physState;
		physState.position.x = getRandInRange<float>(-7.0f, 7.0f);	
		physState.position.y = getRandInRange<float>( 1.0f, 10.0f ); 
		physState.velocity.x = getRandInRange<float>( -3.0f, 3.0f );
		physState.velocity.y = getRandInRange<float>( -1.0f, 2.0f );
		physState.rigidBodyShape = cPhysicalProperties::SPHERE;
		physState.rigidBodyMeasurements.sphere_capsule_radius = 1.0f;
		pTempGO->SetPhysState(physState);
		sMeshDrawInfo meshInfo;
		meshInfo.scale = 1.0f;
		meshInfo.debugDiffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
		meshInfo.name = "SphereRadius1";
		pTempGO->vecMeshes.push_back(meshInfo);
		//
		::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
	}// for ( int count...

	// Teapots at the bottom of vector, so are drawn last...
	{	// Teapot #1
		cGameObject* pTempGO = new cGameObject();
		pTempGO->friendlyName = LEFTTEAPOTNAME;
		cPhysicalProperties physState;
		physState.position.x = 4.0f;
		physState.integrationUpdateType = cPhysicalProperties::ePhysicsIntegrationUpdateTypes::EXCLUDED_FROM_INTEGRATION;
		pTempGO->SetPhysState(physState);
		sMeshDrawInfo meshInfo;
		meshInfo.scale = 0.1f;
		meshInfo.name = "teapotUV";		
		meshInfo.globalAlpha = 0.75f;
		meshInfo.vecMehs2DTextures.push_back( sTextureBindBlendInfo("Utah_Teapot_xyz_n_uv_Enterprise.bmp", 0.0f) );
		meshInfo.vecMehs2DTextures.push_back( sTextureBindBlendInfo("GuysOnSharkUnicorn.bmp", 1.0f) );
		pTempGO->vecMeshes.push_back(meshInfo);
		//
		::g_vecGameObjects.push_back(pTempGO);		// Fastest way to add
	}
	{	// Teapot #2
		cGameObject* pTempGO = new cGameObject();
		pTempGO->friendlyName = "Right Teapot";
		cPhysicalProperties physState;
		physState.position.x = -4.0f;
		physState.integrationUpdateType = cPhysicalProperties::ePhysicsIntegrationUpdateTypes::EXCLUDED_FROM_INTEGRATION;
		pTempGO->SetPhysState(physState);
		sMeshDrawInfo meshInfo;
		meshInfo.scale = 0.1f;
		meshInfo.name = "teapotUV";		
		meshInfo.globalAlpha = 0.75f;
		meshInfo.vecMehs2DTextures.push_back( sTextureBindBlendInfo("Utah_Teapot_xyz_n_uv_Enterprise.bmp", 0.5f) );
		meshInfo.vecMehs2DTextures.push_back( sTextureBindBlendInfo("GuysOnSharkUnicorn.bmp", 0.5f) );
		pTempGO->vecMeshes.push_back(meshInfo);
		//
		::g_vecGameObjects.push_back(pTempGO);		// Fastest way to add

	}

	// Our skybox object
	{
		//cGameObject* pTempGO = new cGameObject();
		::g_pSkyBoxObject = new cGameObject();
		cPhysicalProperties physState;
		::g_pSkyBoxObject->SetPhysState(physState);
		sMeshDrawInfo meshInfo;
		meshInfo.scale = 10000.0f;
		meshInfo.debugDiffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		meshInfo.name = "SmoothSphereRadius1InvertedNormals";			
		meshInfo.vecMeshCubeMaps.push_back(sTextureBindBlendInfo("space", 1.0f));
		meshInfo.bIsSkyBoxObject = true;
		::g_pSkyBoxObject->vecMeshes.push_back(meshInfo);
		// IS SKYBOX
		::g_vecGameObjects.push_back(::g_pSkyBoxObject);		// Fastest way to add
	}


	//// HACK: Set the objects to random colours
	//for ( int index = 0; index != ::g_vecGameObjects.size(); index++ )
	//{
	//	::g_vecGameObjects[index]->diffuseColour.r = getRandInRange(0.0f, 1.0f);
	//	::g_vecGameObjects[index]->diffuseColour.g = getRandInRange(0.0f, 1.0f);
	//	::g_vecGameObjects[index]->diffuseColour.b = getRandInRange(0.0f, 1.0f);
	//}

	//// HACK: Set texture values to something if there isn't a name
	//for (int index = 0; index != ::g_vecGameObjects.size(); index++)
	//{
	//	if (::g_vecGameObjects[index]->textureNames[0] == "")
	//	{
	//		::g_vecGameObjects[index]->textureNames[0] = "Utah_Teapot_xyz_n_uv_Enterprise.bmp";
	//		::g_vecGameObjects[index]->textureNames[1] = "GuysOnSharkUnicorn.bmp";
	//		::g_vecGameObjects[index]->textureBlend[0] = getRandInRange(0.0f, 1.0f);
	//		::g_vecGameObjects[index]->textureBlend[1] = 1.0f - ::g_vecGameObjects[index]->textureBlend[0];
	//	}
	//}//for (int index = 0; index 



//	// Add a bunch more rabbits
//	const float SIZEOFWORLD = 6.0f;	
////	for ( int index = 3; index < MAXNUMBEROFGAMEOBJECTS; index++ )
//	for ( int index = 3; index < 100; index++ )
//	{
//		cGameObject* pTempGO = new cGameObject();
//		pTempGO->position.x = getRandInRange<float>(-SIZEOFWORLD, SIZEOFWORLD );
//		pTempGO->position.y = getRandInRange<float>(-SIZEOFWORLD, SIZEOFWORLD );
//		pTempGO->position.z = getRandInRange<float>(-SIZEOFWORLD, SIZEOFWORLD );
//		//::g_GameObjects[index]->scale = getRandInRange<float>( 7.0f, 15.0f );
//		// Pick a random colour for this bunny
//		pTempGO->diffuseColour.r = getRandInRange<float>(0.0f, 1.0f );
//		pTempGO->diffuseColour.g = getRandInRange<float>(0.0f, 1.0f );
//		pTempGO->diffuseColour.b = getRandInRange<float>(0.0f, 1.0f );
//		pTempGO->meshName = "bunny";
//		::g_vecGameObjects.push_back( pTempGO );
//	}


//  // Add the debug sphere
//  {// STARTOF: Add the debug sphere
//  	::g_pTheDebugSphere = new cGameObject();
//  	::g_pTheDebugSphere->scale = 1.0f;
//  	::g_pTheDebugSphere->diffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
//  	::g_pTheDebugSphere->meshName = "SphereRadius1";
//  	::g_pTheDebugSphere->typeOfObject = eTypeOfObject::SPHERE;
//  	::g_pTheDebugSphere->radius = 1.0f;	
//  	::g_pTheDebugSphere->bIsUpdatedInPhysics = false;
//  	// NOTE: I'm NOT adding it to the vector of objects
//  	//::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
//  }// ENDOF: Add the debug sphere


	//// Add a whole bunch of spheres!
	//float limit = 200.0f;
	//float step = 25.0f;
	//for ( float x = -limit; x <= limit; x += step )
	//{
	//	for ( float y = -limit; y <= limit; y += step )
	//	{
	//		for ( float z = -limit; z <= limit; z += step )
	//		{
	//			cGameObject* pTempGO = new cGameObject();
	//			cPhysicalProperties physState;
	//			physState.position = glm::vec3(x, y, z);
	//			physState.rigidBodyShape = cPhysicalProperties::SPHERE;
	//			physState.rigidBodyMeasurements.sphere_capsule_radius = 1.0f;
	//			pTempGO->SetPhysState(physState);
	//			sMeshDrawInfo meshInfo;
	//			meshInfo.scale = step / 16.0f;
	//			meshInfo.debugDiffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	//			meshInfo.name = "SphereRadius1";			// was dolphin
	//			pTempGO->vecMeshes.push_back(meshInfo);
	//			::g_vecGameObjects.push_back(pTempGO);		// Fastest way to add
	//		}
	//	}
	//}

	
	
	return;
}
