#ifndef _cFBO_Shadow_HG_
#define _cFBO_Shadow_HG_


#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class cFBO_Shadow
{
public:
	cFBO_Shadow() :
		ID(0),
		depthTexture_ID(0),
		vertexWorldPos_ID(0),			 // Alternatively, you can use the world location 	
		width(-1), height(-1) {};

	GLuint ID;				// g_FBO	// = 0;
	GLuint depthTexture_ID;		// = 0;
	GLuint vertexWorldPos_ID;		// = 0;
	GLint width;		// = 512 the WIDTH of the framebuffer, in pixels;
	GLint height;

	// Takes width and height and return ratio
	float CalculateAspectRatio(void);

	// Inits the FBP
	bool init(int width, int height, std::string &error);
	bool shutdown(void);
	// Calls shutdown(), then init()
	bool reset(int width, int height, std::string &error);

	void clear( bool bClearDepth, bool bClearWorldXYZ );
	void clearDepth(void);
	void clearWorldXYZ(void);

	int getMaxColourAttachments(void);
	int getMaxDrawBuffers(void);
};

#endif