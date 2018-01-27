#ifndef _FBO_HG_
#define _FBO_HG_

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class cFBO
{
public:
	cFBO() : 
		ID(0), 
		colourTexture_ID(0), 
		depthTexture_ID(0), 
		width(-1), height(-1) {};

	GLuint ID;				// g_FBO	// = 0;
	GLuint colourTexture_ID;		// = 0;
	GLuint depthTexture_ID;		// = 0;
	GLint width;		// = 512 the WIDTH of the framebuffer, in pixels;
	GLint height;

	// Inits the FBP
	bool init(int width, int height, std::string &error);
	
	void clearBuffers(bool bClearColour = true, bool bClearDepth = true);
};

#endif