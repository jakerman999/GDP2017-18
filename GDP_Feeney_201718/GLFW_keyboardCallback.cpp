#include "globalOpenGL_GLFW.h"
#include "globalGameStuff.h"

#include <iostream>

bool isShiftKeyDown( int mods, bool bByItself = true );
bool isCtrlKeyDown( int mods, bool bByItself = true );
bool isAltKeyDown( int mods, bool bByItself = true );

/*static*/ void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);

	if ( key == GLFW_KEY_SPACE  )
	{
//		::g_GameObjects[1]->position.y += 0.01f;
		::g_vecGameObjects[1]->position.y += 0.01f;
	}

	cGameObject* pLeftTeapot = findObjectByFriendlyName(LEFTTEAPOTNAME, ::g_vecGameObjects);

	const float CAMERASPEED = 10.0f;

	const float CAM_ACCELL_THRUST = 100.0f;

//	const float CAMERASPEED = 100.0f;
	switch (key)
	{
	// HACK: Change orientation of pLeftTeapot
	// 5,6 - rotation around x
	// 7,8 - rotation around y 
	// 9,0 - rotation around z
	case GLFW_KEY_5:
		//pLeftTeapot->qOrientation.x += 1.0f;		// NOT Euler x axis!!
//		pLeftTeapot->adjustQOrientationFormDeltaEuler(glm::vec3(+0.1f, 0.0f, 0.0f));
		break;
	case GLFW_KEY_6:
		pLeftTeapot->adjustQOrientationFormDeltaEuler(glm::vec3(-0.1f, 0.0f, 0.0f));
		break;
	case GLFW_KEY_7:
		pLeftTeapot->adjustQOrientationFormDeltaEuler(glm::vec3(0.0f, +0.1f, 0.0f));
		break;
	case GLFW_KEY_8:
		pLeftTeapot->adjustQOrientationFormDeltaEuler(glm::vec3(0.0f, -0.1f, 0.0f));
		break;
	case GLFW_KEY_9:
		pLeftTeapot->adjustQOrientationFormDeltaEuler(glm::vec3(0.0f, 0.0f, +0.1f));
		break;
	case GLFW_KEY_0:
		pLeftTeapot->adjustQOrientationFormDeltaEuler(glm::vec3(0.0f, 0.0f, -0.1f));
		break;

	case GLFW_KEY_N:
		if (pLeftTeapot)
		{
			pLeftTeapot->textureBlend[0] -= 0.01f;
			if (pLeftTeapot->textureBlend[0] <= 0.0f)
			{
				pLeftTeapot->textureBlend[0] = 0.0f;
			}
			pLeftTeapot->textureBlend[1] = 1.0f - pLeftTeapot->textureBlend[0];
		}
		break;
	case GLFW_KEY_M:
		if (pLeftTeapot)
		{
			pLeftTeapot->textureBlend[0] += 0.01f;
			if (pLeftTeapot->textureBlend[0] > 1.0f)
			{
				pLeftTeapot->textureBlend[0] = 1.0f;
			}
			pLeftTeapot->textureBlend[1] = 1.0f - pLeftTeapot->textureBlend[0];
		}
		break;

	// CAMERA and lighting
	case GLFW_KEY_A:		// Left
//		g_cameraTarget_XYZ.x -= CAMERASPEED;
		if ( isShiftKeyDown(mods, true) )	
		{	::g_pLightManager->vecLights[0].position.x -= CAMERASPEED;	}
		else								
		{	
			//::g_cameraXYZ.x -= CAMERASPEED;
			if (isCtrlKeyDown(mods, true))
			{
				// Move camera (ctrl)
				::g_pTheCamera->Fly_moveRightLeft(-CAMERASPEED);	// strafe
			}
			else if (isAltKeyDown(mods, true))
			{	// F=ma, so changing the accel REALLY is like putting a force
				//	on an object
				::g_pTheCamera->accel.x = -CAM_ACCELL_THRUST;	// Force to the left!
			}
			else
			{	// Turn camera 
				::g_pTheCamera->Fly_turn_RightLeft(-1.0f);
			}
		}
		break;
	case GLFW_KEY_D:		// Right
//		g_cameraTarget_XYZ.x += CAMERASPEED;
		if ( isShiftKeyDown(mods, true) )	
		{	::g_pLightManager->vecLights[0].position.x += CAMERASPEED;	}
		else
		{	
			//::g_cameraXYZ.x += CAMERASPEED;
			if (isCtrlKeyDown(mods, true))
			{
				// Move camera (ctrl)
				::g_pTheCamera->Fly_moveRightLeft(+CAMERASPEED);	// strafe
			}
			else if (isAltKeyDown(mods, true))
			{	// F=ma, so changing the accel REALLY is like putting a force
				//	on an object
				::g_pTheCamera->accel.x = +CAM_ACCELL_THRUST;	// Force to the left!
			}
			else
			{	// Turn camera 
				::g_pTheCamera->Fly_turn_RightLeft(+1.0f);
			}
		}
		break;
	case GLFW_KEY_W:		// Forward (along z)
		if ( isShiftKeyDown(mods, true) )	
		{	::g_pLightManager->vecLights[0].position.z += CAMERASPEED;	}
		else
		{	
			//::g_cameraXYZ.z += CAMERASPEED;
			if (isCtrlKeyDown(mods, true))
			{
				// Move camera (ctrl)
				::g_pTheCamera->Fly_moveForward(+CAMERASPEED);
			}
			else if (isAltKeyDown(mods, true))
			{	// F=ma, so changing the accel REALLY is like putting a force
				//	on an object
				::g_pTheCamera->accel.z = +CAM_ACCELL_THRUST;	// Force to the left!
			}
			else
			{	// Pitch down
				::g_pTheCamera->Fly_pitch_UpDown(-1.0f);
			}
		}
		break;
	case GLFW_KEY_S:		// Backwards (along z)
		if ( isShiftKeyDown(mods, true) )	
		{	::g_pLightManager->vecLights[0].position.z -= CAMERASPEED;	}
		else
		{	
			//::g_cameraXYZ.z -= CAMERASPEED;	
			if (isCtrlKeyDown(mods, true))
			{
				// Move camera (ctrl)
				::g_pTheCamera->Fly_moveForward(-CAMERASPEED);		// Backwards
			}
			else if (isAltKeyDown(mods, true))
			{	// F=ma, so changing the accel REALLY is like putting a force
				//	on an object
				::g_pTheCamera->accel.z = -CAM_ACCELL_THRUST;	// Force to the left!
			}
			else
			{	// Pitch down
				::g_pTheCamera->Fly_pitch_UpDown(+1.0f);
			}
		}
		break;
	case GLFW_KEY_Q:		// "Down" (along y axis)
		if ( isShiftKeyDown(mods, true) )	
		{	::g_pLightManager->vecLights[0].position.y -= CAMERASPEED;	}
		else
		{	
			//::g_cameraXYZ.y -= CAMERASPEED;	
			if (isCtrlKeyDown(mods, true))
			{
				// Move camera (ctrl)
				::g_pTheCamera->Fly_moveUpDown(-CAMERASPEED);	// "Z minus 10000 meters, Mr. Sulu!"
			}
			else
			{	// "roll" left (counter-clockwise)
				::g_pTheCamera->Fly_yaw_CWorCCW(-1.0f);
			}
		}
		break;
	case GLFW_KEY_E:		// "Up" (along y axis)
		if ( isShiftKeyDown(mods, true) )	
		{	::g_pLightManager->vecLights[0].position.y += CAMERASPEED;	}
		else
		{	
			//::g_cameraXYZ.y += CAMERASPEED;	
			if (isCtrlKeyDown(mods, true))
			{
				// Move camera (ctrl)
				::g_pTheCamera->Fly_moveUpDown(+CAMERASPEED);	// "Z minus 10000 meters, Mr. Sulu!"
			}
			else
			{	// "roll" left (counter-clockwise)
				::g_pTheCamera->Fly_yaw_CWorCCW(+1.0f);
			}
		}
		break;

	case GLFW_KEY_1:
		::g_pLightManager->vecLights[0].attenuation.y *= 0.99f;	// less 1%
		break;
	case GLFW_KEY_2:
		::g_pLightManager->vecLights[0].attenuation.y *= 1.01f; // more 1%
		if ( ::g_pLightManager->vecLights[0].attenuation.y <= 0.0f )
		{
			::g_pLightManager->vecLights[0].attenuation.y = 0.001f;	// Some really tiny value
		}
		break;
	case GLFW_KEY_3:	// Quad
		::g_pLightManager->vecLights[0].attenuation.z *= 0.99f;	// less 1%
		break;
	case GLFW_KEY_4:	//  Quad
		::g_pLightManager->vecLights[0].attenuation.z *= 1.01f; // more 1%
		if ( ::g_pLightManager->vecLights[0].attenuation.z <= 0.0f )
		{
			::g_pLightManager->vecLights[0].attenuation.z = 0.001f;	// Some really tiny value
		}
		break;

	}
	// HACK: print output to the console
//	std::cout << "Light[0] linear atten: "
//		<< ::g_pLightManager->vecLights[0].attenuation.y << ", "
//		<< ::g_pLightManager->vecLights[0].attenuation.z << std::endl;
	return;
}



// Helper functions
bool isShiftKeyDown( int mods, bool bByItself /*=true*/ )
{
	if ( bByItself )
	{	// shift by itself?
		if ( mods == GLFW_MOD_SHIFT )	{ return true; }
		else							{ return false; }
	}
	else
	{	// shift with anything else, too
		if ( ( mods && GLFW_MOD_SHIFT ) == GLFW_MOD_SHIFT )	{	return true;	}
		else												{	return false;	}
	}
	// Shouldn't never get here, so return false? I guess?
	return false;
}

bool isCtrlKeyDown( int mods, bool bByItself /*=true*/ )
{
	if ( bByItself )
	{	// shift by itself?
		if ( mods == GLFW_MOD_CONTROL )	{ return true; }
		else							{ return false; }
	}
	else
	{	// shift with anything else, too
		if ( ( mods && GLFW_MOD_CONTROL) == GLFW_MOD_CONTROL)	{	return true;	}
		else												{	return false;	}
	}
	// Shouldn't never get here, so return false? I guess?
	return false;
}

bool isAltKeyDown( int mods, bool bByItself /*=true*/ )
{
	if (bByItself)
	{	// shift by itself?
		if (mods == GLFW_MOD_ALT) { return true; }
		else { return false; }
	}
	else
	{	// shift with anything else, too
		if ((mods && GLFW_MOD_ALT) == GLFW_MOD_ALT) { return true; }
		else { return false; }
	}
	// Shouldn't never get here, so return false? I guess?
	return false;
}