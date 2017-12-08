#ifndef _cCamera_HG_
#define _cCamera_HG_

#include <glm/vec3.hpp>
#define GLM_ENABLE_EXPERIMENTAL
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

	void setCameraMode(eMode cameraMode);
	eMode cameraMode;

	// ************************************************************
	// For the "fly camera":
	// +ve is along z-axis
	void FC_moveForward(float distanceAlongRelativeZAxis_PosIsForward);
	void FC_moveRightLeft(float distanceAlongRelativeXAxis_PosIsRight);
	void FC_moveUpDown(float distanceAlongRelativeYAxis_PosIsUp);
	// +ve is right
	void FC_turn(float turnDegreesPosIsRight);
	void FC_turn_RightLeft(float turnDegreesPosIsRight);
	// +ve it up
	void FC_pitch(float pitchDegreesPosIsNoseUp);
	void FC_pitch_UpDown(float pitchDegreesPosIsNoseUp);
	// +ve is Clock-wise rotation (from nose to tail)
	void FC_yaw(float pitchDegreesPosIsClockWise);
	void FC_yaw_CWorCCW(float pitchDegreesPosIsClockWise);
	// ************************************************************

	void overwriteOrientationFromEuler(glm::vec3 newEulerAngles);
	void updateOrientationFromEuler(glm::vec3 deltaEulerAngleChange);
	glm::mat4 getMat4FromOrientation(void);

	glm::quat orientation;
};

#endif
