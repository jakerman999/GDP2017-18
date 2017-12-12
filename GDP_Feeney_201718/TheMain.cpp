// Include glad and GLFW in correct order
#include "globalOpenGL_GLFW.h"


#include <iostream>			// C++ cin, cout, etc.
//#include "linmath.h"
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr
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
#include "cShaderManager.h" 
#include "cGameObject.h"
#include "cVAOMeshManager.h"
#include "cModelAssetLoader.h"


#include "Physics/Physics.h"	// Physics collision detection functions

#include "cLightManager.h"

// Include all the things that are accessed in other files
#include "globalGameStuff.h"

#include "cCamera.h"

// Forward declaration of the function
void DrawObject( cGameObject* pTheGO );



// Used by the light drawing thingy
// Will draw a wireframe sphere at this location with this colour
void DrawDebugSphere(glm::vec3 location, glm::vec4 colour, float scale);
cGameObject* g_pTheDebugSphere;

//	static const int MAXNUMBEROFGAMEOBJECTS = 10;
//	cGameObject* g_GameObjects[MAXNUMBEROFGAMEOBJECTS];

// Remember to #include <vector>...
std::vector< cGameObject* >  g_vecGameObjects;


////glm::vec3 g_cameraXYZ = glm::vec3( 0.0f, 9000.0f, 16000.0f );	
////glm::vec3 g_cameraTarget_XYZ = glm::vec3( 0.0f, -50.0f, 0.0f );
//
//glm::vec3 g_cameraXYZ = glm::vec3( 0.0f, 100.0f, 500.0f );	
////glm::vec3 g_cameraXYZ = glm::vec3( 0.0f, 16.1f, 22.4f );	
//glm::vec3 g_cameraTarget_XYZ = glm::vec3( 0.0f, 0.0f, 0.0f );

cCamera* g_pTheCamera = NULL;


cVAOMeshManager* g_pVAOManager = 0;		// or NULL, or nullptr

//cShaderManager	g_ShaderManager;			// Stack (no new)
cShaderManager*		g_pShaderManager = 0;		// Heap, new (and delete)
cLightManager*		g_pLightManager = 0;

cBasicTextureManager*	g_pTextureManager = 0;	

cDebugRenderer*			g_pDebugRenderer = 0;


// This contains the AABB grid for the terrain...
cAABBBroadPhase* g_terrainAABBBroadPhase = 0;


// Other uniforms:
GLint uniLoc_materialDiffuse = -1;	
GLint uniLoc_materialAmbient = -1;   
GLint uniLoc_ambientToDiffuseRatio = -1; 	// Maybe	// 0.2 or 0.3
GLint uniLoc_materialSpecular = -1;  // rgb = colour of HIGHLIGHT only
							// w = shininess of the 
GLint uniLoc_bIsDebugWireFrameObject = -1;

GLint uniLoc_eyePosition = -1;	// Camera position
GLint uniLoc_mModel = -1;
GLint uniLoc_mView = -1;
GLint uniLoc_mProjection = -1;


static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}



// Moved to GLFW_keyboardCallback.cpp
//static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)

int main(void)
{
//	cAABBv2 testAABB(glm::vec3(-10.0f, -50.0f, -190.0f), 5.0f /*HALF size*/);

	GLFWwindow* window;
//    GLuint vertex_buffer, vertex_shader, fragment_shader, program;
    GLint mvp_location;	// , vpos_location, vcol_location;
    glfwSetErrorCallback(error_callback);

	//// Other uniforms:
	// Moved to "global" scope 
	// (you might want to place these inside the shader class)
	//GLint uniLoc_materialDiffuse = -1;	
	//GLint uniLoc_materialAmbient = -1;   
	//GLint uniLoc_ambientToDiffuseRatio = -1; 	// Maybe	// 0.2 or 0.3
	//GLint uniLoc_materialSpecular = -1;  // rgb = colour of HIGHLIGHT only
	//							// w = shininess of the 
	//GLint uniLoc_eyePosition = -1;	// Camera position
	//GLint uniLoc_mModel = -1;
	//GLint uniLoc_mView = -1;
	//GLint uniLoc_mProjection = -1;


    if (!glfwInit())
	{
		// exit(EXIT_FAILURE);
		std::cout << "ERROR: Couldn't init GLFW, so we're pretty much stuck; do you have OpenGL??" << std::endl;
		return -1;
	}


	// Print to the console...(if a console is there)
	//std::cout << "Hello" << std::endl;
	//int q = 8;
	//std::cout << "Type a number:";
	//std::cin >> q;
	//std::cout << "You typed " << q << ". Hazzah." << std::endl;

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
    window = glfwCreateWindow( width, height, 
							   title.c_str(), 
							   NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);
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


	::g_pDebugRenderer = new cDebugRenderer();
	if ( ! ::g_pDebugRenderer->initialize(error) )
	{
		std::cout << "Warning: couldn't init the debug renderer." << std::endl;
	}
	//::g_pDebugRenderer->addTriangle( glm::vec3( -1000.0f, 0.0f, 0.0f ), 
	//								 glm::vec3( 1000.0f, 0.0f, 0.0f ),
	//								 glm::vec3( 0.0f, 1000.0f, 0.0f), 
	//								 glm::vec3( 1.0f, 1.0f, 1.0f ), true );
	//for (int count = 0; count != 100; count++)
	//{
	//	::g_pDebugRenderer->addTriangle(
	//		glm::vec3(getRandInRange(-1000.0f, 1000.0f),
	//		          getRandInRange(-1000.0f, 1000.0f),
	//		          getRandInRange(-1000.0f, 1000.0f)),
	//		glm::vec3(getRandInRange(-1000.0f, 1000.0f),
	//		          getRandInRange(-1000.0f, 1000.0f),
	//		          getRandInRange(-1000.0f, 1000.0f)),
	//		glm::vec3(getRandInRange(-1000.0f, 1000.0f),
	//		          getRandInRange(-1000.0f, 1000.0f),
	//		          getRandInRange(-1000.0f, 1000.0f)),
	//		glm::vec3( 1.0f, 1.0f, 1.0f ), true );
	//}//for (int count

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
	mvp_location = glGetUniformLocation(currentProgID, "MVP");		// program, "MVP");
	uniLoc_materialDiffuse = glGetUniformLocation(currentProgID, "materialDiffuse");	
	uniLoc_materialAmbient = glGetUniformLocation(currentProgID, "materialAmbient");   
	uniLoc_ambientToDiffuseRatio = glGetUniformLocation(currentProgID, "ambientToDiffuseRatio");
	uniLoc_materialSpecular = glGetUniformLocation(currentProgID, "materialSpecular");

	uniLoc_bIsDebugWireFrameObject = glGetUniformLocation(currentProgID, "bIsDebugWireFrameObject");

	uniLoc_eyePosition = glGetUniformLocation(currentProgID, "eyePosition");

	uniLoc_mModel = glGetUniformLocation( currentProgID, "mModel" );
	uniLoc_mView = glGetUniformLocation( currentProgID, "mView" );
	uniLoc_mProjection = glGetUniformLocation( currentProgID, "mProjection" );

//	GLint uniLoc_diffuseColour = glGetUniformLocation( currentProgID, "diffuseColour" );

	::g_pLightManager = new cLightManager();

	::g_pLightManager->CreateLights(10);	// There are 10 lights in the shader
	::g_pLightManager->vecLights[0].position = glm::vec3(-4.8f, 570.0f, 212.0f);	
	::g_pLightManager->vecLights[0].attenuation.y = 0.000456922280f;		//0.172113f;
	::g_pLightManager->LoadShaderUniformLocations(currentProgID);


	// Texture 
	::g_pTextureManager = new cBasicTextureManager();
	::g_pTextureManager->SetBasePath("assets/textures");
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

	::g_pTheCamera->setCameraMode(cCamera::FOLLOW_CAMERA);
	::g_pTheCamera->eye = glm::vec3(-100.0f, 150.0f, -300.0f);
	::g_pTheCamera->Follow_SetMaxFollowSpeed(3.0f);
	::g_pTheCamera->Follow_SetDistanceMaxSpeed(50.0f);	// Full speed beyond this distance
	::g_pTheCamera->Follow_SetDistanceMinSpeed(25.0f);	// Zero speed at this distance
	::g_pTheCamera->Follow_SetOrUpdateTarget(glm::vec3(0.0f, 0.0f, 0.0f));
	::g_pTheCamera->Follow_SetIdealCameraLocation(glm::vec3(0.0f, 5.0f, 5.0f));

	// Follow the teapot
	cGameObject* pLeftTeapot = findObjectByFriendlyName(LEFTTEAPOTNAME, ::g_vecGameObjects);
	pLeftTeapot->position.x = 150.0f;
	pLeftTeapot->vel.x = -10.0f;
	pLeftTeapot->vel.y = +25.0f;
	pLeftTeapot->bIsUpdatedInPhysics = true;
	::g_pTheCamera->Follow_SetOrUpdateTarget(pLeftTeapot->position);



	glEnable( GL_DEPTH );

	// Gets the "current" time "tick" or "step"
	double lastTimeStep = glfwGetTime();

	// Main game or application loop
	while ( ! glfwWindowShouldClose(window) )
    {
		// Update camera
		cGameObject* pLeftTeapot = findObjectByFriendlyName(LEFTTEAPOTNAME, ::g_vecGameObjects);
		::g_pTheCamera->Follow_SetOrUpdateTarget(pLeftTeapot->position);


        float ratio;
        int width, height;
		glm::mat4x4 matProjection;			// was "p"

        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);

		// Clear colour AND depth buffer
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
		//glEnable(GL_DEPTH_TEST);

		//        glUseProgram(program);
		::g_pShaderManager->useShaderProgram( "mySexyShader" );
		GLint shaderID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");

		// Update all the light uniforms...
		// (for the whole scene)
		::g_pLightManager->CopyLightInformationToCurrentShader();

		// Projection and view don't change per scene (maybe)
		matProjection = glm::perspective( 0.6f,			// FOV
										  ratio,		// Aspect ratio
										  1.0f,			// Near (as big as possible)
										  100000.0f );	// Far (as small as possible)


		// View or "camera" matrix
		glm::mat4 matView = glm::mat4(1.0f);	// was "v"

		// Now the veiw matrix is taken right from the camera class
		matView = ::g_pTheCamera->getViewMatrix();
		//matView = glm::lookAt( g_cameraXYZ,						// "eye" or "camera" position
		//					   g_cameraTarget_XYZ,		// "At" or "target" 
		//					   glm::vec3( 0.0f, 1.0f, 0.0f ) );	// "up" vector

		glUniformMatrix4fv( uniLoc_mView, 1, GL_FALSE, 
							(const GLfloat*) glm::value_ptr(matView) );
		glUniformMatrix4fv( uniLoc_mProjection, 1, GL_FALSE, 
							(const GLfloat*) glm::value_ptr(matProjection) );

		// Set ALL texture units and binding for ENTIRE SCENE (is faster)
		{
			// 0 
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D,
				::g_pTextureManager->getTextureIDFromName("Utah_Teapot_xyz_n_uv_Enterprise.bmp"));
				//::g_pTextureManager->getTextureIDFromName(pTheGO->textureNames[0]));
			// 1
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D,
				::g_pTextureManager->getTextureIDFromName("GuysOnSharkUnicorn.bmp"));
				//::g_pTextureManager->getTextureIDFromName(pTheGO->textureNames[1]));
			// 2..  and so on... 

			// Set sampler in the shader
			// NOTE: You shouldn't be doing this during the draw call...
			GLint curShaderID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");
			GLint textSampler00_ID = glGetUniformLocation(curShaderID, "myAmazingTexture00");
			GLint textSampler01_ID = glGetUniformLocation(curShaderID, "myAmazingTexture01");
			// And so on (up to 10, or whatever number of textures)... 

			GLint textBlend00_ID = glGetUniformLocation(curShaderID, "textureBlend00");
			GLint textBlend01_ID = glGetUniformLocation(curShaderID, "textureBlend01");

			// This connects the texture sampler to the texture units... 
			glUniform1i(textSampler00_ID, 0);		// Enterprise
			glUniform1i(textSampler01_ID, 1);		// GuysOnSharkUnicorn
		}


		// Enable blend ("alpha") transparency for the scene
		// NOTE: You "should" turn this OFF, then draw all NON-Transparent objects
		//       Then turn ON, sort objects from far to near ACCORDING TO THE CAMERA
		//       and draw them
		glEnable(GL_BLEND);		// Enables "blending"
								//glDisable( GL_BLEND );
								// Source == already on framebuffer
								// Dest == what you're about to draw
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



		// Draw the scene
		unsigned int sizeOfVector = (unsigned int)::g_vecGameObjects.size();	//*****//
		for ( int index = 0; index != sizeOfVector; index++ )
		{
			cGameObject* pTheGO = ::g_vecGameObjects[index];

			DrawObject( pTheGO );

//			}// if ( ::g_pVAOManager->lookupVAOFromName(

		}//for ( int index = 0...




	//DEBUG sphere
	DrawDebugSphere( glm::vec3( 0.0f, 0.0f, 0.0f ),
					 glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ), 1.0f );
	// Light at 95% 
	float scaleAt99 = ::g_pLightManager->vecLights[0].calcApproxDistFromAtten( 0.99f );	
	DrawDebugSphere( ::g_pLightManager->vecLights[0].position,
					 glm::vec4( 1.0f, 1.0f, 1.0f, 1.0f ), scaleAt99 );
	// Light at 50% 
	float scaleAt50 = ::g_pLightManager->vecLights[0].calcApproxDistFromAtten( 0.5f );	
	DrawDebugSphere( ::g_pLightManager->vecLights[0].position,
					 glm::vec4( 0.0f, 1.0f, 0.0f, 1.0f ), scaleAt50 );
	// Light at 25% 
	float scaleAt25 = ::g_pLightManager->vecLights[0].calcApproxDistFromAtten( 0.25f );	
	DrawDebugSphere( ::g_pLightManager->vecLights[0].position,
					 glm::vec4( 1.0f, 0.0f, 0.0f, 1.0f ), scaleAt25 );
	// Light at 1% 
	float scaleAt01 = ::g_pLightManager->vecLights[0].calcApproxDistFromAtten( 0.01f );	
	DrawDebugSphere( ::g_pLightManager->vecLights[0].position,
					 glm::vec4( 0.0f, 1.0f, 1.0f, 1.0f ), scaleAt01 );



	//for (int count = 0; count != 100; count++)
	//{
	//	::g_pDebugRenderer->addTriangle(
	//		glm::vec3(getRandInRange(-1000.0f, 1000.0f),
	//			getRandInRange(-1000.0f, 1000.0f),
	//			getRandInRange(-1000.0f, 1000.0f)),
	//		glm::vec3(getRandInRange(-1000.0f, 1000.0f),
	//			getRandInRange(-1000.0f, 1000.0f),
	//			getRandInRange(-1000.0f, 1000.0f)),
	//		glm::vec3(getRandInRange(-1000.0f, 1000.0f),
	//			getRandInRange(-1000.0f, 1000.0f),
	//			getRandInRange(-1000.0f, 1000.0f)),
	//		glm::vec3(1.0f, 1.0f, 1.0f), false);
	//}//for (int count

	::g_pDebugRenderer->RenderDebugObjects(matView, matProjection);

		//

		std::stringstream ssTitle;
//		ssTitle << "Camera (xyz): " 
//			<< g_cameraXYZ.x << ", " 
//			<< g_cameraXYZ.y << ", " 
//			<< g_cameraXYZ.z;
		// 
		glm::vec4 EulerAngle;
		//g_pTheCamera->qOrientation.eularAngles(EulerAngle);
		//ssTitle << "Camera (xyz): "
		//	<< EulerAngle.x << ", "
		//	<< EulerAngle.y << ", "
		//	<< EulerAngle.z;

		glfwSetWindowTitle( window, ssTitle.str().c_str() );

		// "Presents" what we've drawn
		// Done once per scene 
        glfwSwapBuffers(window);
        glfwPollEvents();


		// Essentially the "frame time"
		// Now many seconds that have elapsed since we last checked
		double curTime = glfwGetTime();
		double deltaTime =  curTime - lastTimeStep;

		PhysicsStep( deltaTime );

		// Update camera, too
		::g_pTheCamera->updateTick(deltaTime);
		::g_pTheCamera->accel = glm::vec3(0.0f, 0.0f, 0.0f);

		lastTimeStep = curTime;

    }// while ( ! glfwWindowShouldClose(window) )


    glfwDestroyWindow(window);
    glfwTerminate();

	// 
	delete ::g_pShaderManager;
	delete ::g_pVAOManager;

//    exit(EXIT_SUCCESS);
	return 0;
}



// Used by the light drawing thingy
// Will draw a wireframe sphere at this location with this colour
void DrawDebugSphere(glm::vec3 location, glm::vec4 colour, 
					 float scale)
{
	// TODO: the magic
	glm::vec3 oldPosition = ::g_pTheDebugSphere->position;
	glm::vec4 oldDiffuse = ::g_pTheDebugSphere->diffuseColour;
	bool bOldIsWireFrame = ::g_pTheDebugSphere->bIsWireFrame;

	::g_pTheDebugSphere->position = location;
	::g_pTheDebugSphere->diffuseColour = colour;
	::g_pTheDebugSphere->bIsWireFrame = true;
	::g_pTheDebugSphere->scale = scale;

	DrawObject( ::g_pTheDebugSphere );

	::g_pTheDebugSphere->position = oldPosition;
	::g_pTheDebugSphere->diffuseColour = oldDiffuse;
	::g_pTheDebugSphere->bIsWireFrame = bOldIsWireFrame;

	return;
}

// Draw a single object
void DrawObject( cGameObject* pTheGO )
{
	// Is there a game object? 
	if ( pTheGO == 0 )	//if ( ::g_GameObjects[index] == 0 )
	{	// Nothing to draw
		return;		// Skip all for loop code and go to next
	}

	// Was near the draw call, but we need the mesh name
	std::string meshToDraw = pTheGO->meshName;		//::g_GameObjects[index]->meshName;

	sVAOInfo VAODrawInfo;
	if ( ::g_pVAOManager->lookupVAOFromName( meshToDraw, VAODrawInfo ) == false )
	{	// Didn't find mesh
		std::cout << "WARNING: Didn't find mesh " << meshToDraw << " in VAO Manager" << std::endl;
		return;
	}
	
	// There IS something to draw

	// 'model' or 'world' matrix
	glm::mat4x4 mModel = glm::mat4x4(1.0f);	//		mat4x4_identity(m);

	//// Euler orientation stuff, which is so "Fall 2017"
	//// ******************************************************
	//// PRE-Rotation
	//glm::mat4 matRreRotZ = glm::mat4x4(1.0f);
	//matRreRotZ = glm::rotate( matRreRotZ, pTheGO->orientation.z, 
	//							glm::vec3(0.0f, 0.0f, 1.0f) );
	//mModel = mModel * matRreRotZ;
	//// ******************************************************

	glm::mat4 trans = glm::mat4x4(1.0f);
	trans = glm::translate( trans, 
							pTheGO->position );
	mModel = mModel * trans; 

	//// Euler orientation stuff, which is so "Fall 2017"
	//// ***************************
	//// POST-Rotation
	//glm::mat4 matPostRotZ = glm::mat4x4(1.0f);
	//matPostRotZ = glm::rotate( matPostRotZ, pTheGO->orientation2.z, 
	//							glm::vec3(0.0f, 0.0f, 1.0f) );
	//mModel = mModel * matPostRotZ;
//
	//glm::mat4 matPostRotY = glm::mat4x4(1.0f);
	//matPostRotY = glm::rotate( matPostRotY, pTheGO->orientation2.y, 
	//							glm::vec3(0.0f, 1.0f, 0.0f) );
	//mModel = mModel * matPostRotY;
//
	//glm::mat4 matPostRotX = glm::mat4x4(1.0f);
	//matPostRotX = glm::rotate( matPostRotX, pTheGO->orientation2.x, 
	//							glm::vec3(1.0f, 0.0f, 0.0f) );
	//mModel = mModel * matPostRotX;
	//// ***************************
	
	// Now with quaternion rotation
	// Like many things in GML, the conversion is done in the constructor
	glm::mat4 postRotQuat = glm::mat4(pTheGO->qOrientation);
	mModel = mModel * postRotQuat;

//	mModel = mModel * trans * postRotQuat;


	// assume that scale to unit bounding box
	// ************* BEWARE *****************
//			float finalScale = VAODrawInfo.scaleForUnitBBox * ::g_vecGameObjects[index]->scale;
	// We have taken out the scale adjustment so the scale is AS IT IS FROM THE MODEL
	float finalScale = pTheGO->scale;

	glm::mat4 matScale = glm::mat4x4(1.0f);
	matScale = glm::scale( matScale, 
							glm::vec3( finalScale,
								        finalScale,
								        finalScale ) );
	mModel = mModel * matScale;


//	//mat4x4_ortho(p, -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
////			p = glm::ortho( -1.0f, 1.0f, -1.0f, 1.0f );
//	p = glm::perspective( 0.6f,			// FOV
//							ratio,		// Aspect ratio
//							0.1f,			// Near (as big as possible)
//							1000.0f );	// Far (as small as possible)
//
//	// View or "camera" matrix
//	glm::mat4 v = glm::mat4(1.0f);	// identity
//
//	//glm::vec3 cameraXYZ = glm::vec3( 0.0f, 0.0f, 5.0f );	// 5 units "down" z
//	v = glm::lookAt( g_cameraXYZ,						// "eye" or "camera" position
//						g_cameraTarget_XYZ,		// "At" or "target" 
//						glm::vec3( 0.0f, 1.0f, 0.0f ) );	// "up" vector
//
//
////        glUseProgram(program);
//	::g_pShaderManager->useShaderProgram( "mySexyShader" );
//	GLint shaderID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");

//			mvp = p * v * m;			// This way (sort of backwards)
//			glUniformMatrix4fv(mvp_location, 1, GL_FALSE, 
//							   (const GLfloat*) glm::value_ptr(mvp) );

	// All of these shouldn't be in the render loop, as they 
	//	aren't changing if we are using the same shader.
	//GLint mModel_locID = glGetUniformLocation( shaderID, "mModel" );
	//GLint mView_locID = glGetUniformLocation( shaderID, "mView" );
	//GLint mProjection = glGetUniformLocation( shaderID, "mProjection" );
	glUniformMatrix4fv( uniLoc_mModel, 1, GL_FALSE, 
						(const GLfloat*) glm::value_ptr(mModel) );
	//glUniformMatrix4fv( uniLoc_mView, 1, GL_FALSE, 
	//					(const GLfloat*) glm::value_ptr(v) );
	//glUniformMatrix4fv( uniLoc_mProjection, 1, GL_FALSE, 
	//					(const GLfloat*) glm::value_ptr(p) );

	glm::mat4 mWorldInTranpose = glm::inverse(glm::transpose(mModel));

	//GLint myLight_00_diffuse = glGetUniformLocation( shaderID, "myLight[0].diffuse" );
	//GLint myLight_01_diffuse = glGetUniformLocation( shaderID, "myLight[1].diffuse" );
	//GLint myLight_02_diffuse = glGetUniformLocation( shaderID, "myLight[2].diffuse" );
	// 
	//glUniform4f( myLight_00_diffuse, 1.0f, 1.0f, 1.0f, 1.0f );

//			GLint diffuseColour_loc = glGetUniformLocation( shaderID, "diffuseColour" );

	glUniform4f( uniLoc_materialDiffuse, 
				pTheGO->diffuseColour.r, 
				pTheGO->diffuseColour.g, 
				pTheGO->diffuseColour.b, 
				pTheGO->diffuseColour.a );
	//...and all the other object material colours

	if ( pTheGO->bIsWireFrame )
	{
		glUniform1f( uniLoc_bIsDebugWireFrameObject, 1.0f );	// TRUE
	}
	else
	{
		glUniform1f( uniLoc_bIsDebugWireFrameObject, 0.0f );	// FALSE
	}


	// Set up the textures
	std::string textureName = pTheGO->textureNames[0];
	GLuint texture00Number
		= ::g_pTextureManager->getTextureIDFromName(textureName);
	// Texture binding... (i.e. set the 'active' texture
	GLuint texture00Unit = 13;							// Texture units go from 0 to 79 (at least)
	glActiveTexture( texture00Unit + GL_TEXTURE0 );		// GL_TEXTURE0 = 33984
	glBindTexture( GL_TEXTURE_2D, texture00Number);

	// 0 
	glActiveTexture(GL_TEXTURE0);
	glBindTexture( GL_TEXTURE_2D, 
		           ::g_pTextureManager->getTextureIDFromName(pTheGO->textureNames[0]));
		           //::g_pTextureManager->getTextureIDFromName("Utah_Teapot_xyz_n_uv_Enterprise.bmp"));
	// 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture( GL_TEXTURE_2D, 
					::g_pTextureManager->getTextureIDFromName(pTheGO->textureNames[1]));
					//::g_pTextureManager->getTextureIDFromName("GuysOnSharkUnicorn.bmp"));
	// 2..  and so on... 

	// Set sampler in the shader
	// NOTE: You shouldn't be doing this during the draw call...
	GLint curShaderID = ::g_pShaderManager->getIDFromFriendlyName("mySexyShader");
	GLint textSampler00_ID = glGetUniformLocation( curShaderID, "myAmazingTexture00" );
	GLint textSampler01_ID = glGetUniformLocation( curShaderID, "myAmazingTexture01" );
	//// And so on (up to 10, or whatever number of textures)... 

	GLint textBlend00_ID = glGetUniformLocation( curShaderID, "textureBlend00" );
	GLint textBlend01_ID = glGetUniformLocation( curShaderID, "textureBlend01" );

	//// This connects the texture sampler to the texture units... 
	//glUniform1i( textSampler00_ID, 0  );		// Enterprise
	//glUniform1i( textSampler01_ID, 1  );		// GuysOnSharkUnicorn
	// .. and so on

	// And the blending values
	glUniform1f(textBlend00_ID, pTheGO->textureBlend[0]);
	glUniform1f(textBlend01_ID, pTheGO->textureBlend[1]);
	// And so on...


	

//			glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );
//			glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	if ( pTheGO->bIsWireFrame )
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	// Default
//		glEnable(GL_DEPTH_TEST);		// Test for z and store in z buffer
		glDisable( GL_CULL_FACE );
	}
	else
	{
		glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );	// Default
		glEnable(GL_DEPTH_TEST);		// Test for z and store in z buffer
		glEnable( GL_CULL_FACE );
	}

	glCullFace( GL_BACK );


	glBindVertexArray( VAODrawInfo.VAO_ID );

	glDrawElements( GL_TRIANGLES, 
					VAODrawInfo.numberOfIndices,		// testMesh.numberOfTriangles * 3,	// How many vertex indices
					GL_UNSIGNED_INT,					// 32 bit int 
					0 );
	// Unbind that VAO
	glBindVertexArray( 0 );

	return;
}
