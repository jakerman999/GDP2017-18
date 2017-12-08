#include "cDebugRenderer.h"

#include "globalOpenGL_GLFW.h"

#include "cShaderManager.h"

#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr


/*static*/ 
const std::string cDebugRenderer::DEFALUT_VERT_SHADER_SOURCE = "\
        #version 420                                    \n \
		                                                \n \
        uniform mat4 mModel;                            \n \
        uniform mat4 mView;                             \n \
        uniform mat4 mProjection;                       \n \
		                                                \n \
        in vec4 vPosition;                              \n \
        in vec4 vColour;                                \n \
		                                                \n \
        out vec4 vertColour;                            \n \
		                                                \n \
        void main()                                     \n \
        {                                               \n \
            mat4 MVP = mProjection * mView * mModel;    \n \
            gl_Position = MVP * vPosition;              \n \
		                                                \n \
            vertColour = vColour;                       \n \
        }\n";

/*static*/ 
const std::string cDebugRenderer::DEFAULT_FRAG_SHADER_SOURCE = "\
        #version 420                                \n \
                                                    \n \
        in vec4 vertColour;                         \n \
                                                    \n \
        void main()                                 \n \
        {                                           \n \
            gl_FragColor.rgb = vertColour.rgb;      \n \
            gl_FragColor.a = vertColour.a;          \n \
        }\n	";




class cDebugRenderer::cShaderProgramInfo
{
public:
	cShaderProgramInfo() :
		shaderProgramID(0), 
		matProjectionUniformLoc(-1), 
		matViewUniformLoc(-1),
		matModelUniformLoc(-1)
	{};
	cShaderManager::cShader vertShader;
	cShaderManager::cShader fragShader;
	unsigned int shaderProgramID;
	// Uniforms in the shader
	int matProjectionUniformLoc;
	int matViewUniformLoc;
	int matModelUniformLoc;
};


bool cDebugRenderer::initialize(std::string &error)
{
	cShaderManager shaderManager;

	this->m_pShaderProg->vertShader.parseStringIntoMultiLine(this->m_vertexShaderSource);
	this->m_pShaderProg->fragShader.parseStringIntoMultiLine(this->m_fragmentShaderSource);

	if ( ! shaderManager.createProgramFromSource( "debugShader", 
												  this->m_pShaderProg->vertShader, 
												  this->m_pShaderProg->fragShader ) )
	{
		error = "Could not create the debug shader program.\n" + shaderManager.getLastError();
		return false;
	}
	// The shader was compiled, so get the shader program number
	this->m_pShaderProg->shaderProgramID = shaderManager.getIDFromFriendlyName("debugShader");

	// Get the uniform variable locations 
	glUseProgram(this->m_pShaderProg->shaderProgramID);
	this->m_pShaderProg->matModelUniformLoc = glGetUniformLocation(this->m_pShaderProg->shaderProgramID, "mModel");
	this->m_pShaderProg->matViewUniformLoc = glGetUniformLocation(this->m_pShaderProg->matViewUniformLoc, "mView");
	this->m_pShaderProg->matProjectionUniformLoc = glGetUniformLocation(this->m_pShaderProg->shaderProgramID, "mProjection");
	glUseProgram(0);

	return true;
}

bool cDebugRenderer::m_InitBuffer(unsigned int numberOfVertices, sVAOInfo &VAOInfo)
{

	// Create a Vertex Array Object (VAO)
	glGenVertexArrays( 1, &(VAOInfo.VAO_ID) );
	glBindVertexArray( VAOInfo.VAO_ID );


    glGenBuffers(1, &(VAOInfo.vertex_buffer_ID) );
    glBindBuffer(GL_ARRAY_BUFFER, VAOInfo.vertex_buffer_ID);

	sVertex_xyz_rgb* pVertices = new sVertex_xyz_rgb[numberOfVertices];
	
	// Clear buffer
	unsigned int sizeOfBufferInBytes = sizeof(sVertex_xyz_rgb) * numberOfVertices;
	memset(pVertices, 0, sizeOfBufferInBytes);


	// Copy the local vertex array into the GPUs memory
    glBufferData(GL_ARRAY_BUFFER, 
				 sizeOfBufferInBytes,		// sizeof(vertices), 
				 pVertices, 
				 GL_DYNAMIC_DRAW);

	// Get rid of local vertex array
	delete [] pVertices;

	// Now set up the vertex layout (for this shader):
	//
    //	in vec4 vPosition;                           
    //	in vec4 vColour;                               
	//
	GLuint vpos_location = glGetAttribLocation(VAOInfo.shaderID, "vPosition");		// program, "vPos");	// 6
    GLuint vcol_location = glGetAttribLocation(VAOInfo.shaderID, "vColour");		// program, "vCol");	// 13

	// Size of the vertex we are using in the array.
	// This is called the "stride" of the vertices in the vertex buffer
	const unsigned int VERTEX_SIZE_OR_STRIDE_IN_BYTES = sizeof(sVertex_xyz_rgb);

    glEnableVertexAttribArray(vpos_location);
	const unsigned int OFFSET_TO_X_IN_CVERTEX = offsetof( sVertex_xyz_rgb, x );
    glVertexAttribPointer(vpos_location, 
						  4,					//	in vec4 vPosition; 	
						  GL_FLOAT, 
						  GL_FALSE,
                          VERTEX_SIZE_OR_STRIDE_IN_BYTES,
						  reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_X_IN_CVERTEX)) );	// 64-bit


    glEnableVertexAttribArray(vcol_location);
	const unsigned int OFFSET_TO_R_IN_CVERTEX = offsetof( sVertex_xyz_rgb, r );
    glVertexAttribPointer(vcol_location, 
						  4,					//	in vec4 vColour; 
						  GL_FLOAT, 
						  GL_FALSE,
                          VERTEX_SIZE_OR_STRIDE_IN_BYTES,		
						  reinterpret_cast<void*>(static_cast<uintptr_t>(OFFSET_TO_R_IN_CVERTEX)) );	// 64-bit
	// *******************************************************************

	// CRITICAL 
	// Unbind the VAO first!!!!
	glBindVertexArray( 0 );	// 

	// Unbind (release) everything
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	glDisableVertexAttribArray(vcol_location);
	glDisableVertexAttribArray(vpos_location);

	return true;
}

bool cDebugRenderer::IsOK(void)
{
	//TODO
	return true;
}


cDebugRenderer::cDebugRenderer()
{
	this->m_vertexShaderSource = cDebugRenderer::DEFALUT_VERT_SHADER_SOURCE;
	this->m_fragmentShaderSource = cDebugRenderer::DEFAULT_FRAG_SHADER_SOURCE;

	this->m_pShaderProg = new cShaderProgramInfo();
	return;
}

cDebugRenderer::~cDebugRenderer()
{
	if (this->m_pShaderProg)
	{
		delete this->m_pShaderProg;
	}
	return;
}


bool cDebugRenderer::resizeBufferForTriangels(unsigned int newNumberOfTriangles)
{
	// TODO
	return false;
}

bool cDebugRenderer::resizeBufferForLines(unsigned int newNumberOfLines)
{
	//TODO
	return false;
}

bool cDebugRenderer::resizeBufferForPoints(unsigned int newNumberOfPoints)
{
	//TODO
	return false;
}

void cDebugRenderer::RenderDebugObjects(glm::mat4 matCameraView, glm::mat4 matProjection)
{
	glUseProgram(this->m_pShaderProg->shaderProgramID);

	glUniformMatrix4fv( this->m_pShaderProg->matViewUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(matCameraView) );
	glUniformMatrix4fv( this->m_pShaderProg->matProjectionUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(matProjection) );
	// Model matrix is just set to identity. 
	// In other words, the values in the buffers are in WORLD coordinates (untransformed)
	glUniformMatrix4fv( this->m_pShaderProg->matModelUniformLoc, 1, GL_FALSE, 
					(const GLfloat*) glm::value_ptr(glm::mat4(1.0f)) );
	

	//TODO: Right now, the objects are drawn WITHOUT the depth buffer
	//      To be added is the ability to draw objects with and without depth
	//      (like some objects are draw "in the scene" and others are drawn 
	//       "on top of" the scene)
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );	// Default
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);		// Test for z and store in z buffer


	glBindVertexArray( this->m_VAOBufferInfoTriangles.VAO_ID );

	glDrawArrays( GL_TRIANGLES, 
				  0,		// 1st vertex
				  this->m_VAOBufferInfoTriangles.numberOfTriangles * 3 );

	// Unbind that VAO
	glBindVertexArray( 0 );

	glUseProgram(0);

	// Put everything back as it was 
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );	// Default
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);		

	return;
}

cDebugRenderer::sDebugTri::sDebugTri()
{
	this->v[0] = glm::vec3(0.0f); this->v[1] = glm::vec3(0.0f); this->v[2] = glm::vec3(0.0f);
	this->colour = glm::vec3(1.0f);	// white
	this->bPersist = false;
	this->bIgnorDepthBuffer = false;
	return;
}

cDebugRenderer::sDebugTri::sDebugTri(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 colour, bool bPersist/*=false*/)
{
	this->v[0] = v1;
	this->v[1] = v2;
	this->v[2] = v3;
	this->colour = colour;
	this->bPersist = bPersist;
	this->bIgnorDepthBuffer = false;
	return;
}

cDebugRenderer::sDebugTri::sDebugTri(glm::vec3 v[3], glm::vec3 colour, bool /*bPersist=false*/)
{
	this->v[0] = v[0];
	this->v[1] = v[1];
	this->v[2] = v[2];
	this->colour = colour;
	this->bPersist = bPersist;
	this->bIgnorDepthBuffer = false;
	return;
}

cDebugRenderer::sDebugLine::sDebugLine()
{
	this->points[0] = glm::vec3(0.0f);
	this->points[0] = glm::vec3(0.0f);
	this->colour = glm::vec3(1.0f);		// white
	this->bPersist = false;
	this->bIgnorDepthBuffer = false;
	return;
}

cDebugRenderer::sDebugLine::sDebugLine(glm::vec3 start, glm::vec3 end, glm::vec3 colour, bool /*bPersist=false*/)
{
	this->points[0] = start;
	this->points[1] = end;
	this->colour = colour;
	this->bPersist = bPersist;
	this->bIgnorDepthBuffer = false;
	return;
}

cDebugRenderer::sDebugLine::sDebugLine(glm::vec3 points[2], glm::vec3 colour, bool /*bPersist=false*/)
{
	this->points[0] = points[0];
	this->points[1] = points[1];
	this->colour = colour;
	this->bPersist = bPersist;
	this->bIgnorDepthBuffer = false;
	return;
}

/*static*/ 
const float cDebugRendererDEFAULT_POINT_SIZE = 1.0f;


cDebugRenderer::sDebugPoint::sDebugPoint()
{
	this->xyz = glm::vec3(0.0f);
	this->colour = glm::vec3(1.0f);	// white
	this->bPersist = false;
	this->pointSize = cDebugRendererDEFAULT_POINT_SIZE;
	this->bIgnorDepthBuffer = false;
	return;
}

cDebugRenderer::sDebugPoint::sDebugPoint(glm::vec3 xyz, glm::vec3 colour, bool bPersist/*=false*/)
{
	this->xyz = xyz;
	this->colour = colour;
	this->bPersist = bPersist;
	this->pointSize = cDebugRendererDEFAULT_POINT_SIZE;
	this->bIgnorDepthBuffer = false;
	return;
}

cDebugRenderer::sDebugPoint::sDebugPoint(glm::vec3 xyz, glm::vec3 colour, float pointSize, bool bPersist/*=false*/)
{
	this->xyz = xyz;
	this->colour = colour;
	this->pointSize = pointSize;
	this->bPersist = bPersist;
	this->bIgnorDepthBuffer = false;
	return;
}


void cDebugRenderer::addTriangle(glm::vec3 v1XYZ, glm::vec3 v2XYZ, glm::vec3 v3XYZ, glm::vec3 colour, bool bPersist /*=false*/)
{
	cDebugRenderer::sDebugTri tempTri(v1XYZ, v2XYZ, v3XYZ, colour, bPersist);
	this->addTriangle(tempTri);
	return;
}


void cDebugRenderer::addTriangle(sDebugTri &tri)
{
	this->m_vecTriangles.push_back(tri);
	return;
}

void cDebugRenderer::addLine(glm::vec3 startXYZ, glm::vec3 endXYZ, glm::vec3 colour, bool bPersist /*=false*/)
{
	cDebugRenderer::sDebugLine tempLine(startXYZ, endXYZ, colour, bPersist);
	this->addLine(tempLine);
	return;
}

void cDebugRenderer::addLine(sDebugLine &line)
{
	this->m_vecLines.push_back(line);
	return;
}

void cDebugRenderer::addPoint(glm::vec3 xyz, glm::vec3 colour, bool bPersist /*=false*/)
{
	cDebugRenderer::sDebugPoint tempPoint(xyz, colour, bPersist);
	this->addPoint(tempPoint);
	return;
}

void cDebugRenderer::addPoint(sDebugPoint &point)
{
	this->m_vecPoints.push_back(point);
	return;
}

void cDebugRenderer::addPoint(glm::vec3 xyz, glm::vec3 colour, float pointSize, bool bPersist /*=false*/)
{
	cDebugRenderer::sDebugPoint tempPoint(xyz, colour, pointSize, bPersist);
	this->addPoint(tempPoint);
	return;
}








