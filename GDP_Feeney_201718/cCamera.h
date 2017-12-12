#ifndef _cCamera_HG_
#define _cCamera_HG_

#include <glm/vec3.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

class cCamera
{
public:
	cCamera();

	glm::vec3 eye;			// position
	glm::vec3 target;
	glm::vec3 up;

	glm::vec3 velocity;		// For "fly camera", like in blender
	glm::vec3 accel;		// For "thruster" like with rockets

	// For following, etc. 
	void updateTick(float deltaTime);

	enum eMode
	{
		MANUAL,			// Move along the axes (lookat)
		FOLLOW_CAMERA,	// Follows a target (lookat)
		FLY_CAMERA		// Movement based on direction of gaze
						// Use quaternion orientation
						// "catch"  is no LOOKAT
	};

	void setCameraMode(eMode cameraMode);
	eMode cameraMode;

	// ************************************************************
	// For the "fly camera":
	// +ve is along z-axis
	void Fly_moveForward(float distanceAlongRelativeZAxis_PosIsForward);
	void Fly_moveRightLeft(float distanceAlongRelativeXAxis_PosIsRight);
	void Fly_moveUpDown(float distanceAlongRelativeYAxis_PosIsUp);
	// +ve is right
	void Fly_turn(float turnDegreesPosIsRight);
	void Fly_turn_RightLeft(float turnDegreesPosIsRight);
	// +ve it up
	void Fly_pitch(float pitchDegreesPosIsNoseUp);
	void Fly_pitch_UpDown(float pitchDegreesPosIsNoseUp);
	// +ve is Clock-wise rotation (from nose to tail)
	void Fly_yaw(float pitchDegreesPosIsClockWise);
	void Fly_yaw_CWorCCW(float pitchDegreesPosIsClockWise);
	// ************************************************************

	void overwrtiteQOrientationFormEuler(glm::vec3 eulerAxisOrientation);
	// NOTE: Use THIS, not just setting the values
	void adjustQOrientationFormDeltaEuler(glm::vec3 eulerAxisOrientChange);

//	glm::mat4 getMat4FromOrientation(void);
	glm::mat4 getViewMatrix(void);


	// 
	glm::quat qOrientation;

	glm::vec3 EulerAngles;	// Ya get gimbal lock, yo.
};

#endif
