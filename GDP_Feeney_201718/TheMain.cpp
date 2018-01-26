// Include glad and GLFW in correct order
#include "globalOpenGL_GLFW.h"


#include <iostream>			// C++ cin, cout, etc.
//#include "linmath.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/quaternion.hpp>


#include <stdlib.h>
#include <stdio.h>
// Add the file stuff library (file stream>
#include <fstream>
#include <sstream>		// "String stream"
#include <string>

#include <vector>		//  smart array, "array" in most languages
#include "Utilities.h"
#include "ModelUtilities.h"
#include "cMesh.h"
#include "sMeshDrawInfo.h"
#include "cShaderManager.h" 
#include "cGameObject.h"
#include "cVAOMeshManager.h"
#include "cModelAssetLoader.h"


//#include "Physics/Physics.h"	// Physics collision detection functions
#include "Physics/cPhysicsWorld.h"

#include "cLightManager.h"

// Include all the things that are accessed in other files
#include "globalGameStuff.h"

#include "cCamera.h"


void DrawDebugLightingSpheres(void);


cGameObject* g_pSkyBoxObject = NULL;	// (theMain.cpp)


// Remember to #include <vector>...
std::vector< cGameObject* >  g_vecGameObjects;

cCamera* g_pTheCamera = NULL;


cVAOMeshManager* g_pVAOManager = 0;		// or NULL, or nullptr

cShaderManager*		g_pShaderManager = 0;		// Heap, new (and delete)
cLightManager*		g_pLightManager = 0;

CTextureManager*		g_pTextureManager = 0;

cDebugRenderer*			g_pDebugRenderer = 0;


// This contains the AABB grid for the terrain...
cAABBBroadPhase* g_terrainAABBBroadPhase = 0;

cPhysicsWorld*	g_pPhysicsWorld = NULL;	// (theMain.cpp)


#include "cFrameBuffer.h"


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


// Moved to GLFW_keyboardCallback.cpp
//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)

int main(void)
{

	GLFWwindow* pGLFWWindow;
    glfwSetErrorCallback(error_callback);



    if (!glfwInit())
	{
		// exit(EXIT_FAILURE);
		std::cout << "ERROR: Couldn't init GLFW, so we're pretty much stuck; do you have OpenGL??" << std::endl;
		return -1;
	}

	int height = 480;	/* default */
	int width = 640;	// default
	std::string title = "OpenGL Rocks";

	std::ifstream infoFile("config.txt");
	if ( ! infoFile.is_open() )
	{	// File didn't open...
		std::cout << "Can't find config file" << std::endl;
		std::cout << "Using defaults" << std::endl;
	}   
	else
	{	// File DID open, so read it... 
		std::string a;	
		
		infoFile >> a;	// "Game"	//std::cin >> a;
		infoFile >> a;	// "Config"
		infoFile >> a;	// "width"

		infoFile >> width;	// 1080

		infoFile >> a;	// "height"

		infoFile >> height;	// 768

		infoFile >> a;		// Title_Start

		std::stringstream ssTitle;		// Inside "sstream"
		bool bKeepReading = true;
		do
		{
			infoFile >> a;		// Title_End??
			if ( a != "Title_End" )
			{
				ssTitle << a << " ";
			}
			else
			{	// it IS the end! 
				bKeepReading  = false;
				title = ssTitle.str();
			}
		}while( bKeepReading );


	}//if ( ! infoFile.is_open() )




    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

	// C++ string
	// C no strings. Sorry. char    char name[7] = "Michael\0";
    pGLFWWindow = glfwCreateWindow( width, height, 
							        title.c_str(), 
							        NULL, NULL);
    if ( ! pGLFWWindow )
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(pGLFWWindow, key_callback);
    glfwMakeContextCurrent(pGLFWWindow);
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
    glfwSwapInterval(1);

	std::cout << glGetString(GL_VENDOR) << " " 
		<< glGetString(GL_RENDERER) << ", " 
		<< glGetString(GL_VERSION) << std::endl;
	std::cout << "Shader language version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	// General error string, used for a number of items during start up
	std::string error;

	::g_pShaderManager = new cShaderManager();

	cShaderManager::cShader vertShader;
	cShaderManager::cShader fragShader;

	vertShader.fileName = "simpleVert.glsl";	
	fragShader.fileName = "simpleFrag.glsl"; 

	::g_pShaderManager->setBasePath( "assets//shaders//" );

	// Shader objects are passed by reference so that
	//	we can look at the results if we wanted to. 
	if ( ! ::g_pShaderManager->createProgramFromFile(
		        "mySexyShader", vertShader, fragShader ) )
	{
		std::cout << "Oh no! All is lost!!! Blame Loki!!!" << std::endl;
		std::cout << ::g_pShaderManager->getLastError() << std::endl;
		// Should we exit?? 
		return -1;	
//		exit(
	}
	std::cout << "The shaders comipled and linked OK" << std::endl;


// Triangle debug renderer test...
	//::g_pDebugRenderer = new cDebugRenderer();
	//if ( ! ::g_pDebugRenderer->initialize(error) )
	//{
	//	std::cout << "Warning: couldn't init the debug renderer." << std::endl;
	//}

	//const float WORLDMAX = 25.0f;
	//::g_pDebugRenderer->addTriangle( glm::vec3( -WORLDMAX, 0.0f, 0.0f ),
	//								 glm::vec3(WORLDMAX, 0.0f, 0.0f ),
	//								 glm::vec3( 0.0f, WORLDMAX, 0.0f),
	//								 glm::vec3( 1.0f, 1.0f, 1.0f ), 20.0f );
	//for (int count = 0; count != 100; count++)
	//{
	//	::g_pDebugRenderer->addTriangle(
	//		glm::vec3(getRandInRange(-WORLDMAX, WORLDMAX),
	//		          getRandInRange(-WORLDMAX, WORLDMAX),
	//		          getRandInRange(-WORLDMAX, WORLDMAX)),
	//		glm::vec3(getRandInRange(-WORLDMAX, WORLDMAX), 
	//		          getRandInRange(-WORLDMAX, WORLDMAX),
	//		          getRandInRange(-WORLDMAX, WORLDMAX)),
	//		glm::vec3(getRandInRange(-WORLDMAX, WORLDMAX),
	//		          getRandInRange(-WORLDMAX, WORLDMAX),
	//		          getRandInRange(-WORLDMAX, WORLDMAX)),
	//		glm::vec3( 1.0f, 1.0f, 1.0f ), 15.0f );
	//}//for ...

	// Load models
	::g_pModelAssetLoader = new cModelAssetLoader();
	::g_pModelAssetLoader->setBasePath("assets/models/");


	::g_pVAOManager = new cVAOMeshManager();

	GLint sexyShaderID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");

	if ( ! Load3DModelsIntoMeshManager(sexyShaderID, ::g_pVAOManager, ::g_pModelAssetLoader, error ) )
	{
		std::cout << "Not all models were loaded..." << std::endl;
		std::cout << error << std::endl;
	}

	LoadModelsIntoScene();

	GLint currentProgID = ::g_pShaderManager->getIDFromFriendlyName( "mySexyShader" );

	// Get the uniform locations for this shader
//	mvp_location = glGetUniformLocation(currentProgID, "MVP");		// program, "MVP");


//	GLint uniLoc_diffuseColour = glGetUniformLocation( currentProgID, "diffuseColour" );

	::g_pLightManager = new cLightManager();

	::g_pLightManager->CreateLights(10);	// There are 10 lights in the shader
	::g_pLightManager->vecLights[0].position = glm::vec3(-4.8f, 570.0f, 212.0f);	
	::g_pLightManager->vecLights[0].attenuation.y = 0.000456922280f;		//0.172113f;
	::g_pLightManager->LoadShaderUniformLocations(currentProgID);

	::g_pLightManager->vecLights[0].setLightParamType(cLight::SPOT);
	// Point spot straight down at the ground
	::g_pLightManager->vecLights[0].direction = glm::vec3(0.0f, -1.0f, 0.0f );
	::g_pLightManager->vecLights[0].setLightParamSpotPrenumAngleInner( glm::radians(15.0f) );
	::g_pLightManager->vecLights[0].setLightParamSpotPrenumAngleOuter( glm::radians(45.0f) );
	::g_pLightManager->vecLights[0].position = glm::vec3(0.0f, 50.0f, 0.0f);	


	// Texture 
	::g_pTextureManager = new CTextureManager();

	std::cout << ::g_pTextureManager->getOpenGL_GL_MAX_TEXTURE_IMAGE_UNITS() << std::endl;
	std::cout << ::g_pTextureManager->getOpenGL_GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS() << std::endl;

	::g_pTextureManager->setBasePath("assets/textures");
	if ( ! ::g_pTextureManager->Create2DTextureFromBMPFile("Utah_Teapot_xyz_n_uv_Checkerboard.bmp", true) )
	{
		std::cout << "Didn't load the texture. Oh no!" << std::endl;
	}
	else
	{
		std::cout << "Texture is loaded! Hazzah!" << std::endl;
	}
	::g_pTextureManager->Create2DTextureFromBMPFile("Utah_Teapot_xyz_n_uv_Enterprise.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("GuysOnSharkUnicorn.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("GuysOnSharkUnicorn.bmp", true);
//	::g_pTextureManager->Create2DTextureFromBMPFile("Seamless_ground_sand_texture.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("barberton_etm_2001121_lrg.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("height_map_norway-height-map-aster-30m.bmp", true);
	::g_pTextureManager->Create2DTextureFromBMPFile("A_height_map_norway-height-map-aster-30m.bmp", true);

	::g_pTextureManager->setBasePath("assets/textures/skybox");
	if (!::g_pTextureManager->CreateCubeTextureFromBMPFiles(
			"space",
			"SpaceBox_right1_posX.bmp",
			"SpaceBox_left2_negX.bmp",
			"SpaceBox_top3_posY.bmp",
			"SpaceBox_bottom4_negY.bmp",
			"SpaceBox_front5_posZ.bmp",
			"SpaceBox_back6_negZ.bmp", true, true))
	{
		std::cout << "Didn't load skybox" << std::endl;
	}


	      
///***********************************************************
	// About the generate the AABB for the terrain
	::g_terrainAABBBroadPhase = new cAABBBroadPhase();
	// Perhaps you'd like something more sophisticated than this...
	::g_terrainAABBBroadPhase->pDebugRenderer = ::g_pDebugRenderer;
	// 
	cMesh terrainMesh; 
	if (::g_pVAOManager->lookupMeshFromName("MeshLabTerrain_xyz_n_uv", terrainMesh))
	{
		std::cout << "Generating the terrain AABB grid. This will take a moment..." << std::endl;
		
		::g_terrainAABBBroadPhase->genAABBGridFromMesh(terrainMesh);

	}//if (::g_pVAOManager->lookupMeshFromName
///***********************************************************


	::g_pTheCamera = new cCamera();
//	::g_pTheCamera->setCameraMode(cCamera::FLY_CAMERA);
//	::g_pTheCamera->eye = glm::vec3(0.0f, 5.0f, -10.0f);

	//::g_pTheCamera->setCameraMode(cCamera::FOLLOW_CAMERA);
	//::g_pTheCamera->eye = glm::vec3(-100.0f, 150.0f, -300.0f);
	//::g_pTheCamera->Follow_SetMaxFollowSpeed(3.0f);
	//::g_pTheCamera->Follow_SetDistanceMaxSpeed(50.0f);	// Full speed beyond this distance
	//::g_pTheCamera->Follow_SetDistanceMinSpeed(25.0f);	// Zero speed at this distance
	//::g_pTheCamera->Follow_SetOrUpdateTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	//::g_pTheCamera->Follow_SetIdealCameraLocation(glm::vec3(0.0f, 5.0f, 5.0f));

	::g_pTheCamera->setCameraMode(cCamera::MODE_FLY_USING_LOOK_AT);
	::g_pTheCamera->FlyCamLA->setEyePosition(glm::vec3(0.0f, 10.0f, 100.0f));
	::g_pTheCamera->FlyCamLA->setTargetInWorld(glm::vec3(0.0f, 20.0f, 0.0f));
	::g_pTheCamera->FlyCamLA->setUpVector(glm::vec3(0.0f, 1.0f, 0.0f));

//	::g_pTheCamera->FollowCam->SetOrUpdateTarget(glm::vec3(1.0f));


	// Follow the teapot
	cGameObject* pLeftTeapot = findObjectByFriendlyName(LEFTTEAPOTNAME, ::g_vecGameObjects);
	cPhysicalProperties physState;
	pLeftTeapot->GetPhysState(physState);
	physState.position.x = 150.0f;
	physState.velocity.x = -10.0f;
	physState.velocity.y = +25.0f;
	physState.integrationUpdateType = cPhysicalProperties::DYNAMIC;	//	pLeftTeapot->bIsUpdatedInPhysics = true;
	//pLeftTeapot->SetPhysState(physState);
	//::g_pTheCamera->Follow_SetOrUpdateTarget(physState.position);


	::g_pPhysicsWorld = new cPhysicsWorld();


	// All loaded!
	std::cout << "And we're good to go! Staring the main loop..." << std::endl;


	glEnable( GL_DEPTH );

	// Gets the "current" time "tick" or "step"
	double lastTimeStep = glfwGetTime();

	// Main game or application loop
	while ( ! glfwWindowShouldClose(pGLFWWindow) )
    {
		// Essentially the "frame time"
		// Now many seconds that have elapsed since we last checked
		double curTime = glfwGetTime();
		double deltaTime =  curTime - lastTimeStep;
		lastTimeStep = curTime;


		::g_pPhysicsWorld->IntegrationStep(deltaTime);

		::g_pTheCamera->updateTick(deltaTime);

		// *********************************************
		//    ___ _        ___              __     ___                          
		//   / __| |___  _| _ ) _____ __  __\ \   / __|__ _ _ __  ___ _ _ __ _  
		//   \__ \ / / || | _ \/ _ \ \ / |___> > | (__/ _` | '  \/ -_) '_/ _` | 
		//   |___/_\_\\_, |___/\___/_\_\    /_/   \___\__,_|_|_|_\___|_| \__,_| 
		//            |__/                                                      
		cPhysicalProperties skyBoxPP;
		::g_pSkyBoxObject->GetPhysState(skyBoxPP);
		skyBoxPP.position = ::g_pTheCamera->getEyePosition();
		::g_pSkyBoxObject->SetPhysState(skyBoxPP);


		RenderScene( ::g_vecGameObjects, pGLFWWindow, deltaTime );



	

		std::stringstream ssTitle;
//		ssTitle << "Camera (xyz): " 
//			<< g_cameraXYZ.x << ", " 
//			<< g_cameraXYZ.y << ", " 
//			<< g_cameraXYZ.z;
		// 
//		glm::vec4 EulerAngle;
		//g_pTheCamera->qOrientation.eularAngles(EulerAngle);
		//ssTitle << "Camera (xyz): "
		//	<< EulerAngle.x << ", "
		//	<< EulerAngle.y << ", "
		//	<< EulerAngle.z;

		glfwSetWindowTitle( pGLFWWindow, ssTitle.str().c_str() );

		// "Presents" what we've drawn
		// Done once per scene 
        glfwSwapBuffers(pGLFWWindow);
        glfwPollEvents();


    }// while ( ! glfwWindowShouldClose(window) )


    glfwDestroyWindow(pGLFWWindow);
    glfwTerminate();

	// 
	delete ::g_pShaderManager;
	delete ::g_pVAOManager;

//    exit(EXIT_SUCCESS);
	return 0;
}


void DrawDebugLightingSpheres(void)
{
	//DEBUG sphere
	::g_pDebugRenderer->addDebugSphere( glm::vec3( 0.0f, 0.0f, 0.0f ), 
										glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ), 1.0f );
	// Light at 95% 
	float scaleAt99 = ::g_pLightManager->vecLights[0].calcApproxDistFromAtten( 0.99f );	
	::g_pDebugRenderer->addDebugSphere( ::g_pLightManager->vecLights[0].position, 
										glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ), scaleAt99 );

	// Light at 50% 
	float scaleAt50 = ::g_pLightManager->vecLights[0].calcApproxDistFromAtten( 0.5f );	
	::g_pDebugRenderer->addDebugSphere( ::g_pLightManager->vecLights[0].position, 
										glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ), scaleAt50 );

	// Light at 25% 
	float scaleAt25 = ::g_pLightManager->vecLights[0].calcApproxDistFromAtten( 0.25f );	
	::g_pDebugRenderer->addDebugSphere( ::g_pLightManager->vecLights[0].position, 
										glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ), scaleAt25 );

	// Light at 1% 
	float scaleAt01 = ::g_pLightManager->vecLights[0].calcApproxDistFromAtten( 0.01f );	
	::g_pDebugRenderer->addDebugSphere( ::g_pLightManager->vecLights[0].position, 
										glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ), scaleAt01 );
	return;
}

//// Used by the light drawing thingy
//// Will draw a wireframe sphere at this location with this colour
//void DrawDebugSphere(glm::vec3 location, glm::vec4 colour, 
//					 float scale)
//{
//	// TODO: the magic
//	glm::vec3 oldPosition = ::g_pTheDebugSphere->position;
//	glm::vec4 oldDiffuse = ::g_pTheDebugSphere->diffuseColour;
//	bool bOldIsWireFrame = ::g_pTheDebugSphere->bIsWireFrame;
//
//	::g_pTheDebugSphere->position = location;
//	::g_pTheDebugSphere->diffuseColour = colour;
//	::g_pTheDebugSphere->bIsWireFrame = true;
//	::g_pTheDebugSphere->scale = scale;
//
//	DrawObject( ::g_pTheDebugSphere );
//
//	::g_pTheDebugSphere->position = oldPosition;
//	::g_pTheDebugSphere->diffuseColour = oldDiffuse;
//	::g_pTheDebugSphere->bIsWireFrame = bOldIsWireFrame;
//
//	return;
//}


























