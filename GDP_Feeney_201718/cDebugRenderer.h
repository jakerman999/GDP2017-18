#ifndef _cDebugRenderer_HG_
#define _cDebugRenderer_HG_

// This is a complete debug thing for rendering wireframe lines
#include <glm/glm.hpp>
#include <string>
#include <vector>
#include "cVAOMeshManager.h"		


class cDebugRenderer
{
public:
	cDebugRenderer();
	~cDebugRenderer();

	bool initialize(std::string &error);
	bool IsOK(void);

	bool resizeBufferForTriangels(unsigned int newNumberOfTriangles);
	bool resizeBufferForLines(unsigned int newNumberOfLines);
	bool resizeBufferForPoints(unsigned int newNumberOfPoints);

	// Renders scene
	void RenderDebugObjects(glm::mat4 matCameraView, glm::mat4 matProjection);

	struct sDebugTri
	{
		sDebugTri();
		sDebugTri(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 colour, bool bPersist=false);
		sDebugTri(glm::vec3 v[3], glm::vec3 colour, bool bPersist=false);
		glm::vec3 v[3];		glm::vec3 colour;
		bool bPersist;	
		bool bIgnorDepthBuffer;
	};

	struct sDebugLine
	{
		sDebugLine();
		sDebugLine(glm::vec3 start, glm::vec3 end, glm::vec3 colour, bool bPersist=false);
		sDebugLine(glm::vec3 points[2], glm::vec3 colour, bool bPersist=false);
		glm::vec3 points[2];		glm::vec3 colour;
		bool bPersist;
		bool bIgnorDepthBuffer;
	};

	struct sDebugPoint
	{
		sDebugPoint();
		sDebugPoint(glm::vec3 xyz, glm::vec3 colour, bool bPersist=false);
		sDebugPoint(glm::vec3 xyz, glm::vec3 colour, float pointSize, bool bPersist=false);
		glm::vec3 xyz;		glm::vec3 colour;	float pointSize;
		bool bPersist;
		bool bIgnorDepthBuffer;
	};

	static const float DEFAULT_POINT_SIZE;	// = 1.0f;

	void addTriangle(glm::vec3 v1XYZ, glm::vec3 v2XYZ, glm::vec3 v3XYZ, glm::vec3 colour, bool bPersist = false);
	void addTriangle(sDebugTri &tri);
	void addLine(glm::vec3 startXYZ, glm::vec3 endXYZ, glm::vec3 colour, bool bPersist = false);
	void addLine(sDebugLine &line);
	void addPoint(glm::vec3 xyz, glm::vec3 colour, bool bPersist = false);
	void addPoint(sDebugPoint &point);
	void addPoint(glm::vec3 xyz, glm::vec3 colour, float pointSize, bool bPersist = false);

	unsigned int getTriangleBufferSizeInTriangles(void)	{ return this->m_TriBufSizeInTriangles; }
	unsigned int getTriangleBufferSizeInBytes(void)		{ return this->m_TriBufSizeBytes; }
	unsigned int getLineBufferSizeInLines(void)			{ return this->m_LineBufSizeInLines; }
	unsigned int getLineBufferSizeInBytes(void)			{ return this->m_LineBufSizeInBytes; }
	unsigned int getPointBufferSizeInPoints(void)		{ return this->m_PointBufSizeInPoints; }
	unsigned int getPointBufferSizeInBytes(void)		{ return this->m_PointBufSizeInBytes; }

	bool getShadersUsed(std::string &vertexShaderSource, std::string &fragmentShaderSource);
	bool setVertexShader(std::string vertexShaderSource);
	bool setFragmentShader(std::string fragmentShaderSource);
	bool setShaders(std::string vertexShaderSource, std::string fragmentShaderSource);

private:
	unsigned int m_TriBufSizeInTriangles;
	unsigned int m_TriBufSizeBytes;
	unsigned int m_LineBufSizeInLines;
	unsigned int m_LineBufSizeInBytes;
	unsigned int m_PointBufSizeInPoints;
	unsigned int m_PointBufSizeInBytes;

	std::string m_vertexShaderSource;
	std::string m_fragmentShaderSource;

	std::vector<sDebugTri> m_vecTriangles;
	std::vector<sDebugLine> m_vecLines;
	std::vector<sDebugPoint> m_vecPoints;

	static const std::string DEFALUT_VERT_SHADER_SOURCE;
	static const std::string DEFAULT_FRAG_SHADER_SOURCE;

	sVAOInfo m_VAOBufferInfoTriangles;
	sVAOInfo m_VAOBufferInfoLines;
	sVAOInfo m_VAOBufferInfoPoints;
	// This is used to create all the buffers
	bool m_InitBuffer(unsigned int numberOfVertices, sVAOInfo &VAOInfo);

	// This is the point that's inside the vertex buffer
	struct sVertex_xyz_rgb
	{
		sVertex_xyz_rgb() : 
			x(0.0f), y(0.0f), z(0.0f),
			r(0.0f), g(0.0f), b(0.0f) {};
		float x, y, z;
		float r, g, b;
	};
	sVertex_xyz_rgb* m_pTriangleVertexArray;
	sVertex_xyz_rgb* m_pLineVertexArray;
	sVertex_xyz_rgb* m_pPointVertexArray;

	// Used to hold the shader information
	// Note: it's set up this way so that we don't have to include the shader manager 
	//	in this file or set up a pimpl, etc. The cShaderProgramInfo is defined in
	//	the implementation file for the cDebugRenderer.cpp
	class cShaderProgramInfo;
	cShaderProgramInfo* m_pShaderProg;
};

#endif
