#ifndef _cCamera_HG_
#define _cCamera_HG_

#include <glm/vec3.hpp>
#include <glm/gtx/quaternion.hpp>

class cCamera
{
public:
	cCamera();

	glm::vec3 eye;
	glm::vec3 target;
	glm::vec3 up;

	// For following, etc. 
	void updateTick(float deltaTime);

	enum eMode
	{
		MANUAL,			// Move along the axes
		FLY_CAMERA,		// Movement based on direction of gaze
		FOLLOW_CAMERA	// Follows a target
	};

	// +ve is along z-axis
	void moveForward(float amount);
	// +ve is right
	void turnRightLeft(float degrees);
	// +ve it up
	void turnUpDown(float degrees);


	glm::quaternion orientation; 
};

#endif
