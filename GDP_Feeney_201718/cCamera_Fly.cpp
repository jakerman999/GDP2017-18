#include "cCamera.h"

glm::vec3 cCamera::m_calcDirectionVectorFromTarget(void)
{
	// Get a vector from the target to the eye
	// (i.e. what direction the camera is looking at)
	glm::vec3 vecDirection = this->target - this->eye;
	
	// Scale this to 1.0 (normalize)
	vecDirection = glm::normalize(vecDirection);

	return vecDirection;
}




void cCamera::Fly_moveForward(float distanceAlongRelativeZAxis_PosIsForward)
{
	// Calcualte axis components based on orientation


}

void cCamera::Fly_moveRightLeft(float distanceAlongRelativeXAxis_PosIsRight)
{
	//TODO:
	return;
}

void cCamera::Fly_moveUpDown(float distanceAlongRelativeYAxis_PosIsUp)
{
	//TODO:
	return;
}
// +ve is right
void cCamera::Fly_turn(float turnDegreesPosIsRight)
{
	//TODO:
	return;
}
void cCamera::Fly_turn_RightLeft(float turnDegreesPosIsRight)
{
	//TODO:
	return;
}
// +ve it up
void cCamera::Fly_pitch(float pitchDegreesPosIsNoseUp)
{
	//TODO:
	return;
}
void cCamera::Fly_pitch_UpDown(float pitchDegreesPosIsNoseUp)
{
	//TODO:
	return;
}
// +ve is Clock-wise rotation (from nose to tail)
void cCamera::Fly_yaw(float pitchDegreesPosIsClockWise)
{
	//TODO:
	return;
}
void cCamera::Fly_yaw_CWorCCW(float pitchDegreesPosIsClockWise)
{
	//TODO:
	return;
}
// 
// You can use this to change the target from wherever it is
//	to, say, 1.0 units from the front of the camera. 
void cCamera::RelocateTargetInFrontOfCamera(float howFarAwayFromFront)
{
	//TODO:
	return;
}


////******************************************************************
//// Below is the "garbage" version of the camera. 
//// Please don't use it...
//// ************************************************************
//// For the "fly camera":
//// +ve is along z-axis
//void cCamera::Fly_moveForward(float distanceAlongRelativeZAxis_PosIsForward)
//{
//	// Along the z axis
//	this->eye.z += distanceAlongRelativeZAxis_PosIsForward;
//	return;
//}
//
//void cCamera::Fly_moveRightLeft(float distanceAlongRelativeXAxis_PosIsRight)
//{
//	// Along the x axis
//	this->eye.x += distanceAlongRelativeXAxis_PosIsRight;
//	return;
//}
//
//void cCamera::Fly_moveUpDown(float distanceAlongRelativeYAxis_PosIsUp)
//{
//	// Along the y
//	this->eye.y += distanceAlongRelativeYAxis_PosIsUp;
//	return;
//}
//
//// +ve is right
//void cCamera::Fly_turn_RightLeft(float turnDegreesPosIsRight)
//{
//	// From camera orientation, Y axis "turns" left and right
//	this->adjustQOrientationFormDeltaEuler(
//		glm::vec3(0.0f,
//		glm::radians(turnDegreesPosIsRight),	// Y axis
//		0.0f));
//	return;
//}
//// +ve it up
//void cCamera::Fly_pitch_UpDown(float pitchDegreesPosIsNoseUp)
//{
//	// From camera orientation, X axis "pitches" up and down
//	this->adjustQOrientationFormDeltaEuler(
//		glm::vec3(glm::radians(pitchDegreesPosIsNoseUp),
//		0.0f,
//		0.0f));
//	return;
//}
//
//// +ve is Clock-wise rotation (from nose to tail)
//void cCamera::Fly_yaw_CWorCCW(float pitchDegreesPosIsClockWise)
//{
//	// From camera orientation, Z axis "rolls" around
//	// (assume z axis is "nose to tail"
//	this->adjustQOrientationFormDeltaEuler(
//		glm::vec3(0.0f,
//		0.0f,
//		glm::radians(pitchDegreesPosIsClockWise)));
//	return;
//}
////******************************************************************
