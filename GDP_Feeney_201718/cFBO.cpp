#include "cFBO.h"

bool cFBO::init( int width, int height, std::string &error )
{
	this->width = width;
	this->height = height;

	glCreateFramebuffers(1, &( this->ID ) );			//g_FBO
	glBindFramebuffer(GL_FRAMEBUFFER, this->ID);

	// Create the colour buffer (texture)
	glGenTextures(1, &(this->colourTexture_ID) );		//g_FBO_colourTexture
	glBindTexture(GL_TEXTURE_2D, this->colourTexture_ID);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 
				   this->width,				// g_FBO_SizeInPixes
				   this->height);			// g_FBO_SizeInPixes

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Create the depth buffer (texture)
	glGenTextures(1, &( this->depthTexture_ID ));			//g_FBO_depthTexture
	glBindTexture(GL_TEXTURE_2D, this->depthTexture_ID);

	glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, 
				   this->width,		//g_FBO_SizeInPixes
				   this->height);

	glFramebufferTexture(GL_FRAMEBUFFER,
						 GL_COLOR_ATTACHMENT0,
						 this->colourTexture_ID, 0);
	glFramebufferTexture(GL_FRAMEBUFFER,
						 GL_DEPTH_ATTACHMENT,
						 this->depthTexture_ID, 0);

	static const GLenum draw_bufers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_bufers);


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


