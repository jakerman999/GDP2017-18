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


//#include "Physics/Physics.h"	// Physics collision detection functions
#include "Physics/cPhysicsWorld.h"

#include "cLightManager.h"

// Include all the things that are accessed in other files
#include "globalGameStuff.h"

#include "cCamera.h"

#include "cFBO.h" 

// Here, the scene is rendered in 3 passes:
// 1. Render geometry to G buffer
// 2. Perform deferred pass, rendering to Deferred buffer
// 3. Then post-pass ("2nd pass" to the scree)
//    Copying from the Pass2_Deferred buffer to the final screen
cFBO g_FBO_Pass1_G_Buffer;
cFBO g_FBO_Pass2_Deferred;


void DrawDebugLightingSpheres(void);


cGameObject* g_pSkyBoxObject = NULL;	// (theMain.cpp)


// Remember to #include <vector>...
std::vector< cGameObject* >  g_vecGameObjects;

cCamera* g_pTheCamera = NULL;


cVAOMeshManager* g_pVAOManager = 0;		// or NULL, or nullptr

cShaderManager*		g_pShaderManager = 0;		// Heap, new (and delete)
cLightManager*		g_pLightManager = 0;

CTextureManager*		g_pTextureManager = 0;

cModelAssetLoader*		g_pModelAssetLoader = NULL;


cDebugRenderer*			g_pDebugRenderer = 0;


// This contains the AABB grid for the terrain...
cAABBBroadPhase* g_terrainAABBBroadPhase = 0;

cPhysicsWorld*	g_pPhysicsWorld = NULL;	// (theMain.cpp)

// This is used in the "render a previous pass" object
cGameObject* g_ExampleTexturedQuad;




#include "cFrameBuffer.h"

bool g_IsWindowFullScreen = false;
GLFWwindow* g_pGLFWWindow = NULL;


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


float g_ChromaticAberrationOffset = 0.0f;
float g_CR_Max = 0.1f;
double g_CA_CountDown = 0.0f;

const int RENDER_PASS_0_G_BUFFER_PASS = 0;
const int RENDER_PASS_1_DEFERRED_RENDER_PASS = 1;
const int RENDER_PASS_2_FULL_SCREEN_EFFECT_PASS = 2;


// Moved to GLFW_keyboardCallback.cpp
//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)

int main(void)
{

	//GLFWwindow* pGLFWWindow;		// Moved to allow switch from windowed to full-screen
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
    ::g_pGLFWWindow = glfwCreateWindow( width, height, 
							            title.c_str(), 
							            NULL, NULL);
    if ( ! ::g_pGLFWWindow )
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback( ::g_pGLFWWindow, key_callback );
	// For the FBO to resize when the window changes
	glfwSetWindowSizeCallback( ::g_pGLFWWindow, window_size_callback );

    glfwMakeContextCurrent( ::g_pGLFWWindow );
    gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress );
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
	//fragShader.fileName = "simpleFrag.glsl"; 
	fragShader.fileName = "simpleFragDeferred.glsl"; 

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
	::g_pDebugRenderer = new cDebugRenderer();
	if ( ! ::g_pDebugRenderer->initialize(error) )
	{
		std::cout << "Warning: couldn't init the debug renderer." << std::endl;
		std::cout << "\t" << ::g_pDebugRenderer->getLastError() << std::endl;
	}

//	const float WORLDMAX = 25.0f;
//	::g_pDebugRenderer->addTriangle( glm::vec3( -WORLDMAX, 0.0f, 0.0f ),
//									 glm::vec3(WORLDMAX, 0.0f, 0.0f ),
//									 glm::vec3( 0.0f, WORLDMAX, 0.0f),
//									 glm::vec3( 1.0f, 1.0f, 1.0f ), 20.0f );
//	for (int count = 0; count != 100; count++)
//	{
//		::g_pDebugRenderer->addTriangle(
//			glm::vec3(getRandInRange(-WORLDMAX, WORLDMAX),
//			          getRandInRange(-WORLDMAX, WORLDMAX),
//			          getRandInRange(-WORLDMAX, WORLDMAX)),
//			glm::vec3(getRandInRange(-WORLDMAX, WORLDMAX), 
//			          getRandInRange(-WORLDMAX, WORLDMAX),
//			          getRandInRange(-WORLDMAX, WORLDMAX)),
//			glm::vec3(getRandInRange(-WORLDMAX, WORLDMAX),
//			          getRandInRange(-WORLDMAX, WORLDMAX),
//			          getRandInRange(-WORLDMAX, WORLDMAX)),
//			glm::vec3( 1.0f, 1.0f, 1.0f ), 15.0f );
//	}//for ...
//	::g_pDebugRenderer->addTriangle( glm::vec3( -50.0f, -25.0f, 0.0f ),
//									 glm::vec3( 0.0, 50.0f, 100.0f ),
//									 glm::vec3( 50.0f, -25.0, 0.0f),
//									 glm::vec3( 1.0f, 1.0f, 0.0f ), 1000.0f );




	// Load models
	::g_pModelAssetLoader = new cModelAssetLoader();
	::g_pModelAssetLoader->setBasePath("assets/models/");

	::g_pVAOManager = new cVAOMeshManager();

	GLint currentProgID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");


	GLint sexyShaderID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");

	if ( ! Load3DModelsIntoMeshManager(sexyShaderID, ::g_pVAOManager, ::g_pModelAssetLoader, error ) )
	{
		std::cout << "Not all models were loaded..." << std::endl;
		std::cout << error << std::endl;
	}

	LoadModelsIntoScene();


	// Named unifrom block
	// Now many uniform blocks are there? 
	GLint numberOfUniformBlocks = -1;
	glGetProgramiv(currentProgID, GL_ACTIVE_UNIFORM_BLOCKS, &numberOfUniformBlocks);

	// https://www.opengl.org/wiki/GLAPI/glGetActiveUniformBlock

	// Set aside some buffers for the names of the blocks
	const int BUFFERSIZE = 1000;

	int name_length_written = 0;

	char NUBName_0[BUFFERSIZE] = {0};
	char NUBName_1[BUFFERSIZE] = {0};

	glGetActiveUniformBlockName(currentProgID,
								0,
								BUFFERSIZE,
								&name_length_written,
								NUBName_0);

	glGetActiveUniformBlockName(currentProgID,
								1,
								BUFFERSIZE,
								&name_length_written,
								NUBName_1);

//	NUB_lighting
//	NUB_perFrame

	GLuint NUB_Buffer_0_ID = 0;
	GLuint NUB_Buffer_1_ID = 0;

	glGenBuffers(1, &NUB_Buffer_0_ID);
	glBindBuffer(GL_UNIFORM_BUFFER, NUB_Buffer_0_ID );

	glGenBuffers(1, &NUB_Buffer_1_ID);
	glBindBuffer(GL_UNIFORM_BUFFER, NUB_Buffer_1_ID);




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

	std::cout << "GL_MAX_TEXTURE_IMAGE_UNITS: " << ::g_pTextureManager->getOpenGL_GL_MAX_TEXTURE_IMAGE_UNITS() << std::endl;
	std::cout << "GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS: " << ::g_pTextureManager->getOpenGL_GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS() << std::endl;

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


	// Create an FBO
	if ( ! ::g_FBO_Pass1_G_Buffer.init(1920, 1080, error) )
	{
		std::cout << "::g_FBO_Pass2_Deferred error: " << error << std::endl;
	}
	else
	{
		std::cout << "::g_FBO_Pass2_Deferred is good." << std::endl;
		std::cout << "\t::g_FBO_Pass1_G_Buffer ID = " << ::g_FBO_Pass1_G_Buffer.ID << std::endl;
		std::cout << "\tcolour texture ID = " << ::g_FBO_Pass1_G_Buffer.colourTexture_0_ID << std::endl;
		std::cout << "\tnormal texture ID = " << ::g_FBO_Pass1_G_Buffer.normalTexture_1_ID << std::endl;

		std::cout << "GL_MAX_COLOR_ATTACHMENTS = " << ::g_FBO_Pass1_G_Buffer.getMaxColourAttachments() << std::endl;
		std::cout << "GL_MAX_DRAW_BUFFERS = " << ::g_FBO_Pass1_G_Buffer.getMaxDrawBuffers() << std::endl;

	}//if ( ! ::g_FBO_Pass1_G_Buffer.init

	if ( ! ::g_FBO_Pass2_Deferred.init(1920, 1080, error) )
	{
		std::cout << "::g_FBO_Pass2_Deferred error: " << error << std::endl;
	}
	else
	{
		std::cout << "FBO is good." << std::endl;
		std::cout << "\t::g_FBO_Pass2_Deferred ID = " << ::g_FBO_Pass2_Deferred.ID << std::endl;
		std::cout << "\tcolour texture ID = " << ::g_FBO_Pass2_Deferred.colourTexture_0_ID << std::endl;
		std::cout << "\tnormal texture ID = " << ::g_FBO_Pass2_Deferred.normalTexture_1_ID << std::endl;

		std::cout << "GL_MAX_COLOR_ATTACHMENTS = " << ::g_FBO_Pass2_Deferred.getMaxColourAttachments() << std::endl;
		std::cout << "GL_MAX_DRAW_BUFFERS = " << ::g_FBO_Pass2_Deferred.getMaxDrawBuffers() << std::endl;

	}//if ( ! ::g_FBO_Pass2_Deferred.init




	setWindowFullScreenOrWindowed( ::g_pGLFWWindow, ::g_IsWindowFullScreen );


	// Gets the "current" time "tick" or "step"
	double lastTimeStep = glfwGetTime();

	// Main game or application loop
	while ( ! glfwWindowShouldClose(::g_pGLFWWindow) )
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



//    ___                _             _           ___       _            __   __           
//   | _ \ ___  _ _   __| | ___  _ _  | |_  ___   / __| ___ | |__  _  _  / _| / _| ___  _ _ 
//   |   // -_)| ' \ / _` |/ -_)| '_| |  _|/ _ \ | (_ ||___|| '_ \| || ||  _||  _|/ -_)| '_|
//   |_|_\\___||_||_|\__,_|\___||_|    \__|\___/  \___|     |_.__/ \_,_||_|  |_|  \___||_|  
//                                                                        
// In this pass, we render all the geometry to the "G buffer"
// The lighting is NOT done here. 
// 
		::g_pShaderManager->useShaderProgram("mySexyShader");

		// Direct everything to the FBO
		GLint renderPassNumber_LocID = glGetUniformLocation(sexyShaderID, "renderPassNumber");
		glUniform1i( renderPassNumber_LocID, RENDER_PASS_0_G_BUFFER_PASS );

		glBindFramebuffer(GL_FRAMEBUFFER, g_FBO_Pass1_G_Buffer.ID );
		// Clear colour AND depth buffer
		g_FBO_Pass1_G_Buffer.clearBuffers();

		RenderScene( ::g_vecGameObjects, ::g_pGLFWWindow, deltaTime );


//    ___         __                         _   ___                _             ___               
//   |   \  ___  / _| ___  _ _  _ _  ___  __| | | _ \ ___  _ _   __| | ___  _ _  | _ \ __ _  ___ ___
//   | |) |/ -_)|  _|/ -_)| '_|| '_|/ -_)/ _` | |   // -_)| ' \ / _` |/ -_)| '_| |  _// _` |(_-<(_-<
//   |___/ \___||_|  \___||_|  |_|  \___|\__,_| |_|_\\___||_||_|\__,_|\___||_|   |_|  \__,_|/__//__/
//   
// In this pass, we READ from the G-buffer, and calculate the lighting. 
// In this example, we are writing to another FBO, now. 
// 


		// Render it again, but point the the FBO texture... 
//		glBindFramebuffer(GL_FRAMEBUFFER, g_FBO_Pass2_Deferred.ID );
//		g_FBO_Pass2_Deferred.clearBuffers();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		::g_pShaderManager->useShaderProgram("mySexyShader");

		glUniform1i( renderPassNumber_LocID, RENDER_PASS_1_DEFERRED_RENDER_PASS );
		
		//uniform sampler2D texFBONormal2D;
		//uniform sampler2D texFBOVertexWorldPos2D;

		GLint texFBOColour2DTextureUnitID = 10;
		GLint texFBOColour2DLocID = glGetUniformLocation(sexyShaderID, "texFBOColour2D");
		GLint texFBONormal2DTextureUnitID = 11;
		GLint texFBONormal2DLocID = glGetUniformLocation(sexyShaderID, "texFBONormal2D");
		GLint texFBOWorldPosition2DTextureUnitID = 12;
		GLint texFBOWorldPosition2DLocID = glGetUniformLocation(sexyShaderID, "texFBOVertexWorldPos2D");

		// Pick a texture unit... 
		glActiveTexture(GL_TEXTURE0 + texFBOColour2DTextureUnitID);
		glBindTexture(GL_TEXTURE_2D, g_FBO_Pass1_G_Buffer.colourTexture_0_ID);
		glUniform1i(texFBOColour2DLocID, texFBOColour2DTextureUnitID);

		glActiveTexture(GL_TEXTURE0 + texFBONormal2DTextureUnitID);
		glBindTexture(GL_TEXTURE_2D, g_FBO_Pass1_G_Buffer.normalTexture_1_ID);
		glUniform1i(texFBONormal2DLocID, texFBONormal2DTextureUnitID);
		
		glActiveTexture(GL_TEXTURE0 + texFBOWorldPosition2DTextureUnitID);
		glBindTexture(GL_TEXTURE_2D, g_FBO_Pass1_G_Buffer.vertexWorldPos_2_ID);
		glUniform1i(texFBOWorldPosition2DLocID, texFBOWorldPosition2DTextureUnitID);
		
		// Set the sampler in the shader to the same texture unit (20)

		glfwGetFramebufferSize(::g_pGLFWWindow, &width, &height);

		GLint screenWidthLocID = glGetUniformLocation(sexyShaderID, "screenWidth");
		GLint screenHeightLocID = glGetUniformLocation(sexyShaderID, "screenHeight");
		glUniform1f(screenWidthLocID, (float)width);
		glUniform1f(screenHeightLocID, (float)height);

		// Adjust the CA:
		//float g_ChromaticAberrationOffset = 0.0f;
		// 0.1
		if ( ::g_CA_CountDown > 0.0 )
		{
			float CRChange = getRandInRange( (-g_CR_Max/10.0f), (::g_CR_Max/10.0f) );
			::g_ChromaticAberrationOffset += CRChange;
			::g_CA_CountDown -= deltaTime;
		}
		else
		{
			::g_ChromaticAberrationOffset = 0.0f;
		}

		GLint CROffset_LocID = glGetUniformLocation(sexyShaderID, "CAoffset" );
		glUniform1f( CROffset_LocID, g_ChromaticAberrationOffset);

		std::vector< cGameObject* >  vecCopy2ndPass;
		// Push back a SINGLE quad or GIANT triangle that fills the entire screen
		// Here we will use the skybox (as it fills the entire screen)
		vecCopy2ndPass.push_back( ::g_pSkyBoxObject );
		RenderScene(vecCopy2ndPass, ::g_pGLFWWindow, deltaTime );

//    ___  _              _   ___  ___    ___               
//   | __|(_) _ _   __ _ | | |_  )|   \  | _ \ __ _  ___ ___
//   | _| | || ' \ / _` || |  / / | |) | |  _// _` |(_-<(_-<
//   |_|  |_||_||_|\__,_||_| /___||___/  |_|  \__,_|/__//__/
//                                                          	
// Here, we read from the off screen buffer, the one that 
// has all the lighting, etc. 
// This is where we can do the "2nd pass effects", so the 
// full-screen 2D effects.
//
// NOTE: In this example, we are only using this to render to an offscreen object

		// Now the final pass (in this case, only rendering to a quad)
		//RENDER_PASS_2_FULL_SCREEN_EFFECT_PASS
///////glBindFramebuffer(GL_FRAMEBUFFER, 0);
///////
///////glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
///////
///////::g_pShaderManager->useShaderProgram("mySexyShader");
///////
///////glUniform1i(renderPassNumber_LocID, RENDER_PASS_2_FULL_SCREEN_EFFECT_PASS );
///////
///////// The "deferred pass" FBO has a colour texture with the entire rendered scene
///////// (including lighting, etc.)
///////GLint fullRenderedImage2D_LocID = glGetUniformLocation(sexyShaderID, "fullRenderedImage2D");
///////
///////// Pick a texture unit... 
///////unsigned int pass2unit = 50;
///////glActiveTexture( GL_TEXTURE0 + pass2unit);
///////glBindTexture(GL_TEXTURE_2D, ::g_FBO_Pass2_Deferred.colourTexture_0_ID);
///////glUniform1i(fullRenderedImage2D_LocID, pass2unit);
///////
///////
///////std::vector< cGameObject* >  vecCopySingleLonelyQuad;
///////// Push back a SINGLE quad or GIANT triangle that fills the entire screen
///////vecCopySingleLonelyQuad.push_back( ::g_ExampleTexturedQuad );
///////RenderScene(vecCopySingleLonelyQuad, ::g_pGLFWWindow, deltaTime);





//		RenderScene(::g_vecTransparentObjectsOnly, ::g_pGLFWWindow, deltaTime);

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

		glfwSetWindowTitle( ::g_pGLFWWindow, ssTitle.str().c_str() );

		// "Presents" what we've drawn
		// Done once per scene 
        glfwSwapBuffers(::g_pGLFWWindow);
        glfwPollEvents();


    }// while ( ! glfwWindowShouldClose(window) )


    glfwDestroyWindow(::g_pGLFWWindow);
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



void setWindowFullScreenOrWindowed( GLFWwindow* pTheWindow, bool IsFullScreen )
{
	// Set full-screen or windowed
	if ( ::g_IsWindowFullScreen )
	{	
		// Find the size of the primary monitor
		GLFWmonitor* pPrimaryScreen = glfwGetPrimaryMonitor();
		const GLFWvidmode* pPrimMonVidMode = glfwGetVideoMode( pPrimaryScreen );
		// Set this window to full screen, matching the size of the monitor:
		glfwSetWindowMonitor( pTheWindow, pPrimaryScreen, 
							  0, 0,				// left, top corner 
							  pPrimMonVidMode->width, pPrimMonVidMode->height, 
							  GLFW_DONT_CARE );	// refresh rate

		std::cout << "Window now fullscreen at ( " 
			<< pPrimMonVidMode->width << " x " 
			<< pPrimMonVidMode->height << " )" << std::endl;
	}
	else
	{
		// Make the screen windowed. (i.e. It's CURRENTLY full-screen)
		// NOTE: We aren't saving the "old" windowed size - you might want to do that...
		// HACK: Instead, we are taking the old size and mutiplying it by 75% 
		// (the thinking is: the full-screen switch always assumes we want the maximum
		//	resolution - see code above - but when we make that maximum size windowed,
		//  it's going to be 'too big' for the screen)
		GLFWmonitor* pPrimaryScreen = glfwGetPrimaryMonitor();
		const GLFWvidmode* pPrimMonVidMode = glfwGetVideoMode( pPrimaryScreen );

		// Put the top, left corner 10% of the size of the full-screen
		int topCornerTop = (int)( (float)pPrimMonVidMode->height * 0.1f );
		int topCornerLeft = (int)( (float)pPrimMonVidMode->width * 0.1f );
		// Make the width and height 75% of the full-screen resolution
		int height = (int)( (float)pPrimMonVidMode->height * 0.75f );
		int width = (int)( (float)pPrimMonVidMode->width * 0.75f );

		glfwSetWindowMonitor( pTheWindow, NULL,		// This NULL makes the screen windowed
							  topCornerLeft, topCornerTop,				// left, top corner 
							  width, height, 
							  GLFW_DONT_CARE );	// refresh rate

		std::cout << "Window now windowed at ( " 
			<< width << " x " << height << " )" 
			<< " and offset to ( "
			<< topCornerLeft << ", " << topCornerTop << " )"
			<< std::endl;
	}
	return;
}
























