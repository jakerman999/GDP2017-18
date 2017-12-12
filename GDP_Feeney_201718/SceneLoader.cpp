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

	{
		cGameObject* pTempGO = new cGameObject();
		pTempGO->scale = 1.0f;
		pTempGO->diffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
		pTempGO->meshName = "MeshLabTerrain_xyz_n_uv";		// Was teapot
		pTempGO->bIsUpdatedInPhysics = false;	
		pTempGO->textureBlend[0] = 1.0f;
		pTempGO->textureNames[0] = "barberton_etm_2001121_lrg.bmp";
//		pTempGO->textureNames[0] = "Seamless_ground_sand_texture.bmp";
//		pTempGO->bIsWireFrame = true;
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
	//{	// Right side plane
	//	cGameObject* pTempGO = new cGameObject();
	//	pTempGO->position.x = -15.0f;		// LEFTSIDEWALL;		// Where our physics HACK detection is
	//	pTempGO->orientation.z = glm::radians(0.0f);	// Degrees
	//	pTempGO->orientation2.z = glm::radians(-90.0f);	// Degrees   
	//	pTempGO->scale = 1.0f;
	//	pTempGO->diffuseColour = glm::vec4( 0.0f, 1.0f, 1.0f, 1.0f );
	//	pTempGO->meshName = "PlaneXZ";		// Was teapot
	//	pTempGO->typeOfObject = eTypeOfObject::PLANE;
	//	// ***
	//	pTempGO->bIsUpdatedInPhysics = false;	
	//	// ***
	//	::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
	//}

	//{	// Back side plane
	//	cGameObject* pTempGO = new cGameObject();
	//	pTempGO->position.z = -15.0f;		// LEFTSIDEWALL;		// Where our physics HACK detection is
	//	pTempGO->orientation.x = glm::radians(0.0f);	// Degrees
	//	pTempGO->orientation2.x = glm::radians(90.0f);	// Degrees   
	//	pTempGO->scale = 1.0f;
	//	pTempGO->diffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
	//	pTempGO->meshName = "PlaneXZ";		// Was teapot
	//	pTempGO->typeOfObject = eTypeOfObject::PLANE;
	//	// ***
	//	pTempGO->bIsUpdatedInPhysics = false;	
	//	// ***
	//	::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
	//}
	{
		cGameObject* pTempGO = new cGameObject();
		pTempGO->position.y = 4.0f;
		pTempGO->position.x = -4.0f;
		pTempGO->vel.x = 2.0f;
		//pTempGO->orientation.z = glm::radians(0.0f);	// Degrees
		//pTempGO->orientation2.z = glm::radians(0.0f);	// Degrees
		pTempGO->overwrtiteQOrientationFormEuler(glm::vec3(0.0f, 0.0f, 0.0f));
		pTempGO->scale = 1.0f;
		pTempGO->diffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
		pTempGO->meshName = "SphereRadius1";
		pTempGO->typeOfObject = eTypeOfObject::SPHERE;
		pTempGO->radius = 1.0f;	
		//
		pTempGO->vel.y = 1.0f;
		//
		::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
	}	
	{
		cGameObject* pTempGO = new cGameObject();
		pTempGO->position.x = -0.5f;
		pTempGO->position.y = 4.0f;
		//pTempGO->orientation.z = glm::radians(0.0f);	// Degrees
		//pTempGO->orientation2.z = glm::radians(0.0f);	// Degrees
		pTempGO->overwrtiteQOrientationFormEuler(glm::vec3(0.0f, 0.0f, 0.0f));
		pTempGO->scale = 1.0f;
		pTempGO->diffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
		pTempGO->meshName = "SphereRadius1";			// was dolphin
		pTempGO->typeOfObject = eTypeOfObject::SPHERE;
		pTempGO->radius = 1.0f;	
		::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
	}
	// Add a bunch of spheres....
	for ( int count = 0; count != 20; count++ )
	{
		cGameObject* pTempGO = new cGameObject();
		pTempGO->position.x = getRandInRange<float>(-7.0f, 7.0f);	// -10 to 10
		pTempGO->position.y = getRandInRange<float>( 1.0f, 10.0f ); // -2 to Infinity
		pTempGO->vel.x = getRandInRange<float>( -3.0f, 3.0f );
		pTempGO->vel.y = getRandInRange<float>( -1.0f, 2.0f );
		pTempGO->overwrtiteQOrientationFormEuler(glm::vec3(0.0f, 0.0f, 0.0f));
		pTempGO->scale = 1.0f;
		pTempGO->diffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
		pTempGO->meshName = "SphereRadius1";
		pTempGO->typeOfObject = eTypeOfObject::SPHERE;
		pTempGO->radius = 1.0f;	
		::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
	}// for ( int count...

	// Teapots at the bottom of vector, so are drawn last...
	{	// Teapot #1
		cGameObject* pTempGO = new cGameObject();
		pTempGO->scale = 0.1f;
		pTempGO->position.x = -4.0f;
//		pTempGO->orientation2.x = glm::radians(-90.0f);
//		pTempGO->overwrtiteQOrientationFormEuler(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));
		pTempGO->meshName = "teapotUV";		// Was teapot
		pTempGO->diffuseColour.a = 0.75f;
		pTempGO->textureNames[0] = "Utah_Teapot_xyz_n_uv_Enterprise.bmp";
		pTempGO->textureNames[1] = "GuysOnSharkUnicorn.bmp";
		pTempGO->textureBlend[0] = 1.0f;
		pTempGO->textureBlend[1] = 0.0f;
		pTempGO->bIsUpdatedInPhysics = false;
//		pTP0 = pTempGO;
		pTempGO->friendlyName = LEFTTEAPOTNAME;
		::g_vecGameObjects.push_back(pTempGO);		// Fastest way to add
	}
	{	// Teapot #2
		cGameObject* pTempGO = new cGameObject();
		pTempGO->scale = 0.1f;
		pTempGO->position.x = 4.0f;
//		pTempGO->orientation2.x = glm::radians(-90.0f);
//		pTempGO->overwrtiteQOrientationFormEuler(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f));
		pTempGO->diffuseColour.a = 0.75f;
		pTempGO->meshName = "teapotUV";		// Was teapot
		pTempGO->textureNames[0] = "Utah_Teapot_xyz_n_uv_Enterprise.bmp";
		pTempGO->textureNames[1] = "GuysOnSharkUnicorn.bmp";
		pTempGO->textureBlend[0] = 0.0f;
		pTempGO->textureBlend[1] = 1.0f;
		pTempGO->bIsUpdatedInPhysics = false;
		::g_vecGameObjects.push_back(pTempGO);		// Fastest way to add
	}

	// Our skybox object
	{
		cGameObject* pTempGO = new cGameObject();
		pTempGO->overwrtiteQOrientationFormEuler(glm::vec3(0.0f, 0.0f, 0.0f));
		pTempGO->scale = 100.0f;
		pTempGO->diffuseColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		pTempGO->meshName = "SmoothSphereRadius1";			
		pTempGO->typeOfObject = eTypeOfObject::SPHERE;
		pTempGO->radius = 1.0f;	//??
		pTempGO->bIsSkyBoxObject = true;
		::g_vecGameObjects.push_back(pTempGO);		// Fastest way to add
	}


	// HACK: Set the objects to random colours
	for ( int index = 0; index != ::g_vecGameObjects.size(); index++ )
	{
		::g_vecGameObjects[index]->diffuseColour.r = getRandInRange(0.0f, 1.0f);
		::g_vecGameObjects[index]->diffuseColour.g = getRandInRange(0.0f, 1.0f);
		::g_vecGameObjects[index]->diffuseColour.b = getRandInRange(0.0f, 1.0f);
	}

	// HACK: Set texture values to something if there isn't a name
	for (int index = 0; index != ::g_vecGameObjects.size(); index++)
	{
		if (::g_vecGameObjects[index]->textureNames[0] == "")
		{
			::g_vecGameObjects[index]->textureNames[0] = "Utah_Teapot_xyz_n_uv_Enterprise.bmp";
			::g_vecGameObjects[index]->textureNames[1] = "GuysOnSharkUnicorn.bmp";
			::g_vecGameObjects[index]->textureBlend[0] = getRandInRange(0.0f, 1.0f);
			::g_vecGameObjects[index]->textureBlend[1] = 1.0f - ::g_vecGameObjects[index]->textureBlend[0];
		}
	}//for (int index = 0; index 



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


	// Add the debug sphere
	{// STARTOF: Add the debug sphere
		::g_pTheDebugSphere = new cGameObject();
		::g_pTheDebugSphere->scale = 1.0f;
		::g_pTheDebugSphere->diffuseColour = glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f );
		::g_pTheDebugSphere->meshName = "SphereRadius1";
		::g_pTheDebugSphere->typeOfObject = eTypeOfObject::SPHERE;
		::g_pTheDebugSphere->radius = 1.0f;	
		::g_pTheDebugSphere->bIsUpdatedInPhysics = false;
		// NOTE: I'm NOT adding it to the vector of objects
		//::g_vecGameObjects.push_back( pTempGO );		// Fastest way to add
	}// ENDOF: Add the debug sphere
	
	
	return;
}
