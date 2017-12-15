#ifndef _CPhysProps_HG_
#define _CPhysProps_HG_

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// These are some of the physical properties of the object
// the ones that are only really needed by the Physics updater thingy

#include <string>

class cGameObject;	// for friend declaration

class cPhysicalProperties		
{
public:
	friend cGameObject;	// to access gameObjectID

	// Render doesn't need to be able to update these
	cPhysicalProperties();
	glm::vec3 position;			// Both render and physics need this...
	glm::vec3 positionLast;		// Previous location a "moment" ago
	glm::vec3 velocity;			// velocity_X, velocity_Y, velocity_Z     
	glm::vec3 accel;			// accel_X, accel_Y, accel_Z;

	// Objects world matrix
	glm::mat4 matWorld;	

	glm::quat qOrientation;

	glm::vec4 directedVelocity;
	glm::vec4 directedAccel;
	glm::quat qRotationalSpeed;

	float mass;					// If zero, assume infinite mass
	float inverseMass;			

	// Mainly used for collisions
	enum eCollisionTypes
	{
		SPHERE, 
		PLANE, 
		BOX, 
		AABB,
		CAPSULE,
		MESH,
		POINT_CLOUD,				// Like mesh. Is a set of points to test
		EXCLUDED_FROM_COLLISION		// Things that can pass through other things
		                            // (cameras, lights, etc.)
	};
	eCollisionTypes collisionType;


	enum ePhysicsUpdateTypes
	{	// These are like the Bullet Physics types
		DYNAMIC,		// Full physical siluation
		KINEMATIC,		// Possibly included in collision, but not updated by physics
		INFINITEMASS,	// Possibly included in collision, but ignored by physics update
		EXCLUDED_FROM_INTEGRATION		// Not part of the physics update, but might be in collision
	};
	ePhysicsUpdateTypes physUpdateType;


	// "set" over-writes the existing values
	// "adj" adjusts ("adds"/updates) the angles
	void setOrientationEulerAngles( glm::vec3 newAnglesEuler, bool bIsDegrees = false );
	void adjOrientationEulerAngles( glm::vec3 adjAngleEuler, bool bIsDegrees = false );
	void setOrientationQ( glm::quat newOrientQ );
	void adjOrientationQ( glm::quat adjOrientQ );

	void setRotationalSpeedEuler( glm::vec3 newRotSpeedEuler, bool bIsDegrees = false );
	void adjRotationalSpeedEuler( glm::vec3 adjRotSpeedEuler, bool bIsDegrees = false );
	void setRotationalSpeedQ( glm::quat newRotSpeedQ );
	void adjRotationalSpeedQ( glm::quat adjRotSpeedQ );

	// These are vec4s so that we can directly tranform by qOrientation matrix

	std::string DEBUG_getString_position(void);
	std::string DEBUG_getString_positionLast(void);
	std::string DEBUG_getString_velocity(void);
	std::string DEBUG_getString_accel(void);
	std::string DEBUG_getString_directedVelocity(void);
	std::string DEBUG_getString_directedAccel(void);


	unsigned int getGameObjectID(void) { return this->m_GameObjectID; }
private:
	// To be able to match up to the game objects. Can be set by friend CGameObject
	unsigned int m_GameObjectID;	
};

// Shorter name for this class
typedef cPhysicalProperties cPhysProps;

#endif

