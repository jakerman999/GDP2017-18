#ifndef _cGameObject_HG_
#define _cGameObject_HG_

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <string>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "iDebugRenderer.h"

// 1. Place physical properties into GO
// 2. Add a separate Physical Properties 
//    (i.e. a separate list of physical objects we are simulating)
//    -> Do we include GO into the Physical Property object?
//    -> Do we include the Physical Property into the GO?
//    -> Do we list them sep. in a 3rd list of "pairs"?

//class cGOPhysPair
//{
//	cPhysProps* pPhyOB;
//	cGameObject* pGO;
//}
//
//class cPhysProps
//{
//	glm::vec3 vel;			// Velocity
//	glm::vec3 accel;		// Acceleration'
//}

enum eTypeOfObject
{	// Ok people, here's the deal:
	SPHERE = 0,		// - it's a sphere
	PLANE = 1,		// - it's a plane
	CAPSULE = 2,    // - it's a capsule
	AABB = 3,		// 3- it's an axis-aligned box
	UNKNOWN = 99	// I don't know
};

class cGameObject
{
public:
	cGameObject();		// constructor
	~cGameObject();		// destructor
//	cGameObject(const cGameObject &obj);  // copy constructor
	glm::vec3 position;
	//glm::vec3 orientation;		// Is really Euler axis
	//glm::vec3 orientation2;		// (Euler) HACK (will elimiate this with)

	// Now orientation 
	void overwrtiteQOrientationFormEuler(glm::vec3 eulerAxisOrientation);
	// NOTE: Use THIS, not just setting the values
	void adjustQOrientationFormDeltaEuler(glm::vec3 eulerAxisOrientChange);
	glm::quat qOrientation;
//	glm::mat4 matOrientation;
//	glm::mat3 matOrientation;
	float scale;

	bool bIsSkyBoxObject;

	// **********************************
	// Add some physics things
	glm::vec3 vel;			// Velocity
	glm::vec3 accel;		// Acceleration
	bool bIsUpdatedInPhysics;		// 
//	float mass;		INFINITY				// 
//	float inverseMass;	// = 0.0f	// 
	// Refers to the enum above
	eTypeOfObject typeOfObject;		// (really an int)
	float radius;
	// **********************************

	bool bIsWireFrame;

	glm::vec4 diffuseColour;	// 

	std::string meshName;		// mesh I'd like to draw
	
	// The texture of this object
	static const unsigned int NUMTEXTURES = 10;
	std::string textureNames[NUMTEXTURES];
	float textureBlend[NUMTEXTURES];			// 0 - 1

	std::string friendlyName;
	inline unsigned int getUniqueID(void) { return this->m_UniqueID; }

	iDebugRenderer* pDebugRenderer;
private:
	unsigned int m_UniqueID;
	// Used when creating objects
	static unsigned int m_nextUniqueID;

};

#endif
