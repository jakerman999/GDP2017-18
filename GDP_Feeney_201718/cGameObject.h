#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <queue>

#include "iDebugRenderer.h"

#include "Physics/cPhysicalProperties.h"
#include "Physics/iPhysicalObject.h"	// For the access to things that will update the positions, etc.



class cGameObject : public iPhysicalObject
{
public:
	cGameObject();		// constructor
	~cGameObject();		// destructor
//	cGameObject(const cGameObject &obj);  // copy constructor

	//***** from the iPhysicalObject interface ********************
	virtual void SetPhysProps( cPhysicalProperties &PhysProps );
	virtual cPhysicalProperties GetPhysProps( void );
	virtual void GetPhysProps( cPhysicalProperties &PhysProps );
	//*************************************************************
	glm::vec3 getPosition(void);
	// bOverwritePositionToo effectively stops the object if the "past position" is being used
	void	  overwritePotition( glm::vec3 newPosition, bool bOverwiteOldPositionToo = true );
	glm::quat getQOrientation(void);		// Post-rotation
	void      overwriteQOrientation( glm::quat newOrientation );
	void      overwriteQOrientationEuler( glm::vec3 newOrientationEuler, bool bIsDegrees = true );
	void      adjQOrientation( glm::quat newOrientation );
	void      adjQOrientationEuler( glm::vec3 newOrientationEuler, bool bIsDegrees = true );
	//****************************************************************************************

	//TODO: Add the mediator code
	std::string friendlyName;
	inline unsigned int getUniqueID(void) { return this->m_UniqueID; }

	iDebugRenderer* pDebugRenderer;


	// Mesh information (if drawn)
	// Note: The mesh has a separate orientation and offset from the object.
	//	If you want them to be the same, just ignore this orientaiton
	//	 (i.e. don't touch it) and use the object orientation only.
	std::string meshName;		// mesh I'd like to draw
private: 
	glm::quat m_meshQOrientation;
public:
	glm::quat getMeshQOrientation(void)	{	return this->m_meshQOrientation;	}
	glm::vec3 meshOffset;
	void setMeshOrientationEulerAngles( glm::vec3 newAnglesEuler, bool bIsDegrees = false );
	void setMeshOrientationEulerAngles( float x, float y, float z, bool bIsDegrees = false );
	void adjMeshOrientationEulerAngles( glm::vec3 adjAngleEuler, bool bIsDegrees = false );
	void adjMeshOrientationEulerAngles( float x, float y, float z, bool bIsDegrees = false );
	void adjMeshOrientationQ( glm::quat adjOrientQ );
	// A combination of the orientation and the mesh orientation
	glm::quat getFinalMeshQOrientation(void);


	// The textures used for this mesh
	// This is pretty basic: any textures are modulated (multiplied) 
	//	together using the textureBlendRatio for each. 
	// So all the blend ratios, together, should add to 1.0f
	struct sTextureInfo
	{
	sTextureInfo() : textureBlendRatio(0.0f) {};
		std::string textureName;
		float textureBlendRatio;	// 0.0 to 1.0f
		                            // All blend ratios should add to 1.0
	};
	// If it's just one skybox, but one texture here. 
	//	but if you're going for a "day to night" transition or something, 
	//	you could add multiple cubemaps and blend between
	std::vector<sTextureInfo> vecMehs2DTextures;
	bool bIsMeshASkyBoxObject;
	std::vector<sTextureInfo> vecMeshCubeMaps;

	bool bIsMeshVisible;				// Render ignores objects that are false (not child objects could still be visible)
	bool bIsMeshWireframe;				// Polygon mode is LINES
	glm::vec4 debugDiffuseColour;	// Colour used if wireframe 
	bool bUseDepthBuffer;			// Depth buffer enabled or not
	bool bDisableBackFaceCulling;	// Draw both sides if true
	bool bIsALight;
	int lightID;					// -1 is invalid
	// ************************************

	// Our "child" objects
	std::vector< cGameObject* > vec_pChildObjects;
	void DeleteChildren(void);
	// Returns NULL if not found
	cGameObject* FindChildByFriendlyName( std::string name );
	cGameObject* FindChildByID( unsigned int ID );


	// Used when there is only one game object (like with text), but we're drawing it many times

	void pushRenderingState(void);
	// Ignores call if nothing on stack
	void popRenderingState(void);
	
private:
	unsigned int m_UniqueID;
	// Used when creating objects
	static unsigned int m_nextUniqueID;

	// All the properties of a physical object 
	cPhysicalProperties m_PhysicalProps;

	// Any former render states (that have been pushed)
	struct sRenderState
	{
		glm::vec3 position;
		glm::quat oriention;
		bool bIsWireframe;
		glm::vec4 diffuse;		// Alpha is 4th value
		glm::vec4 ambient;
		glm::vec3 specular;
		float shininess;
		glm::vec3 debugColour;
		bool bUseDebugColour;
		float scale;
		bool bIsVisible;
	};
	std::queue< sRenderState > m_stackRenderState;

};

#endif
