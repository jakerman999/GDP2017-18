#include "cFBO.h"

bool cFBO::init( int width, int height, std::string &error )
{
	this->width = width;
	this->height = height;

	glCreateFramebuffers(1, &( this->ID ) );			//g_FBO
	glBindFramebuffer(GL_FRAMEBUFFER, this->ID);

//************************************************************
	// Create the colour buffer (texture)
	glGenTextures(1, &(this->colourTexture_0_ID ) );		//g_FBO_colourTexture
	glBindTexture(GL_TEXTURE_2D, this->colourTexture_0_ID);

//	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8,		// 8 bits per colour
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F,		// 8 bits per colour
				   this->width,				// g_FBO_SizeInPixes
				   this->height);			// g_FBO_SizeInPixes

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//***************************************************************
//************************************************************
// Create the NORMAL buffer (texture)
	glGenTextures(1, &( this->normalTexture_1_ID ));		//g_FBO_colourTexture
	glBindTexture(GL_TEXTURE_2D, this->normalTexture_1_ID);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB32F,		// 32 bits per "colour"
				   this->width,				// g_FBO_SizeInPixes
				   this->height);			// g_FBO_SizeInPixes

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//***************************************************************


	// Create the depth buffer (texture)
	glGenTextures(1, &( this->depthTexture_ID ));			//g_FBO_depthTexture
	glBindTexture(GL_TEXTURE_2D, this->depthTexture_ID);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, 
				   this->width,		//g_FBO_SizeInPixes
				   this->height);

// ***************************************************************

	glFramebufferTexture(GL_FRAMEBUFFER,
						 GL_COLOR_ATTACHMENT0,			// Colour goes to #0
						 this->colourTexture_0_ID, 0);

	glFramebufferTexture(GL_FRAMEBUFFER,
						 GL_COLOR_ATTACHMENT1,			// Normal goes to #1
						 this->normalTexture_1_ID, 0);

	glFramebufferTexture(GL_FRAMEBUFFER,
						 GL_DEPTH_ATTACHMENT,
						 this->depthTexture_ID, 0);

	static const GLenum draw_bufers[] = 
	{ 
		GL_COLOR_ATTACHMENT0, 
		GL_COLOR_ATTACHMENT1
	};
	glDrawBuffers(2, draw_bufers);		// There are 2 outputs now

	// ***************************************************************




	// ADD ONE MORE THING...
	bool bFrameBufferIsGoodToGo = true; 

	switch ( glCheckFramebufferStatus(GL_FRAMEBUFFER) )
	{
	case GL_FRAMEBUFFER_COMPLETE:
		bFrameBufferIsGoodToGo = true; 
		break;

	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		error = "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT";
		bFrameBufferIsGoodToGo = false; 
		break;
//	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
	case GL_FRAMEBUFFER_UNSUPPORTED:
	default:
		bFrameBufferIsGoodToGo = false; 
		break;
	}//switch ( glCheckFramebufferStatus(GL_FRAMEBUFFER) )

	// Point back to default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return bFrameBufferIsGoodToGo;
}

void cFBO::clearBuffers(bool bClearColour, bool bClearDepth)
{
	glViewport(0, 0, this->width, this->height);
	GLfloat	zero = 0.0f;
	GLfloat one = 1.0f;
	if ( bClearColour )
	{
		glClearBufferfv(GL_COLOR, 0, &zero);
	}
	if ( bClearDepth )
	{
		glClearBufferfv(GL_DEPTH, 0, &one);
	}

	return;
}


int cFBO::getMaxColourAttachments(void)
{
	//  void glGetIntegerv(GLenum pname,
	// 				       GLint * data);
	
	int maxColourAttach = 0;
	glGetIntegerv( GL_MAX_COLOR_ATTACHMENTS, &maxColourAttach );

	return maxColourAttach;
}

int cFBO::getMaxDrawBuffers(void)
{
	int maxDrawBuffers = 0;
	glGetIntegerv( GL_MAX_DRAW_BUFFERS, &maxDrawBuffers );
	
	return maxDrawBuffers;
}
