#include "cCamera.h"

cCamera::cCamera()
{
	// Default OpenGL 1.1 was on z axis looking at the origin
	this->eye = glm::vec3(0.0f, 0.0f, 1.0f);
	this->target = glm::vec3(0.0f, 0.0f, 0.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);

	return;
}


// For following, etc. 
void cCamera::updateTick(float deltaTime)
{
	// TODO:

	return;
}

void cCamera::setCameraMode(eMode cameraMode)
{
	// Yes, it's an enum, but you can pass anything, so double-check
	switch (cameraMode)
	{
	case cCamera::eMode::FLY_CAMERA:
	case cCamera::eMode::FOLLOW_CAMERA:
	case cCamera::eMode::MANUAL:
		this->cameraMode = cameraMode;
		return;
	}

	// Something is wrong, so set to manual
	this->cameraMode = cCamera::eMode::MANUAL;
	return;
}


// ************************************************************
//// For the "fly camera":
//// +ve is along z-axis
//void cCamera::FC_moveForward(float distanceAlongRelativeZAxis_PosIsForward);
//void cCamera::FC_moveRightLeft(float distanceAlongRelativeXAxis_PosIsRight);
//void cCamera::FC_moveUpDown(float distanceAlongRelativeYAxis_PosIsUp);
//// +ve is right
//void cCamera::FC_turn(float turnDegreesPosIsRight);
//void cCamera::FC_turn_RightLeft(float turnDegreesPosIsRight);
//// +ve it up
//void cCamera::FC_pitch(float pitchDegreesPosIsNoseUp);
//void cCamera::FC_pitch_UpDown(float pitchDegreesPosIsNoseUp);
//// +ve is Clock-wise rotation (from nose to tail)
//void cCamera::FC_yaw(float pitchDegreesPosIsClockWise);
//void cCamera::FC_yaw_CWorCCW(float pitchDegreesPosIsClockWise);
//// ************************************************************
//
//void cCamera::setOrientationFromEuler(glm::vec3 eulerAngles);
//glm::mat4 cCamera::getMat4FromOrientation(void);
