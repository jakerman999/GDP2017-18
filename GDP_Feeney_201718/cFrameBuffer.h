#ifndef _cFrameBuffer_HG_
#define _cFrameBuffer_HG_

struct sScreenDesc
{
	sScreenDesc() : width(0), height(0) {};
	unsigned int width;
	unsigned int height;
};

class cFrameBuffer
{
public:
	cFrameBuffer();
	~cFrameBuffer();

	unsigned int FBO_ID;
	unsigned int FBO_ColourTexture_ID;
	unsigned int FBO_DepthTexture_ID;
	int width_pixels;
	int height_pixels;
};

#endif 
