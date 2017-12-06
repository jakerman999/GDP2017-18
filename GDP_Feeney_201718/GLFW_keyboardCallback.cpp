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

	const float CAMERASPEED = 0.1f;
	switch (key)
	{
	case GLFW_KEY_N:
		pTP0->textureBlend[0] -= 0.01f;
		if (pTP0->textureBlend[0] <= 0.0f)
		{
			pTP0->textureBlend[0] = 0.0f;
		}
		pTP0->textureBlend[1] = 1.0f - pTP0->textureBlend[0];
		break;
	case GLFW_KEY_M:
		pTP0->textureBlend[0] += 0.01f;
		if (pTP0->textureBlend[0] > 1.0f)
		{
			pTP0->textureBlend[0] = 1.0f;
		}
		pTP0->textureBlend[1] = 1.0f - pTP0->textureBlend[0];
		break;

	case GLFW_KEY_A:		// Left
//		g_cameraTarget_XYZ.x -= CAMERASPEED;
		if ( isShiftKeyDown(mods, true) )	
		{	::g_pLightManager->vecLights[0].position.x -= CAMERASPEED;	}
		else								
		{	::g_cameraXYZ.x -= CAMERASPEED;		}
		break;
	case GLFW_KEY_D:		// Right
//		g_cameraTarget_XYZ.x += CAMERASPEED;
		if ( isShiftKeyDown(mods, true) )	
		{	::g_pLightManager->vecLights[0].position.x += CAMERASPEED;	}
		else
		{	::g_cameraXYZ.x += CAMERASPEED;	}		
		break;
	case GLFW_KEY_W:		// Forward (along z)
		if ( isShiftKeyDown(mods, true) )	
		{	::g_pLightManager->vecLights[0].position.z += CAMERASPEED;	}
		else
		{	::g_cameraXYZ.z += CAMERASPEED;	}
		break;
	case GLFW_KEY_S:		// Backwards (along z)
		if ( isShiftKeyDown(mods, true) )	
		{	::g_pLightManager->vecLights[0].position.z -= CAMERASPEED;	}
		else
		{	::g_cameraXYZ.z -= CAMERASPEED;	}
		break;
	case GLFW_KEY_Q:		// "Down" (along y axis)
		if ( isShiftKeyDown(mods, true) )	
		{	::g_pLightManager->vecLights[0].position.y -= CAMERASPEED;	}
		else
		{	::g_cameraXYZ.y -= CAMERASPEED;	}
		break;
	case GLFW_KEY_E:		// "Up" (along y axis)
		if ( isShiftKeyDown(mods, true) )	
		{	::g_pLightManager->vecLights[0].position.y += CAMERASPEED;	}
		else
		{	::g_cameraXYZ.y += CAMERASPEED;	}
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

//bool isCtrlKeyDown( int mods, bool bByItself = true );
//bool isAltKeyDown( int mods, bool bByItself = true );