#include "cCamera.h"

void cCamera::m_calcDirectionVectorFromTarget(glm::vec3 &eyeToTargetDirection, float &eyeToTargetLength)
{
	// Get a vector from the target to the eye
	// (i.e. what direction the camera is looking at)
	glm::vec3 vDirectionAndDistanceTarget = this->target - this->eye;
	
	eyeToTargetLength = glm::length(vDirectionAndDistanceTarget);
	// Scale this to 1.0 (normalize)
	eyeToTargetDirection = glm::normalize(vDirectionAndDistanceTarget);
}

void cCamera::m_calcDirectionVectorFromTarget(glm::vec3 &vEyeToTargetDirection)
{
	float eyeToTargetLength = 0.0f;
	return this->m_calcDirectionVectorFromTarget( vEyeToTargetDirection, eyeToTargetLength );
}



void cCamera::Fly_moveForward(float distanceAlongRelativeZAxis_PosIsForward)
{
	// Calcualte axis components based on orientation
	glm::vec3 vDirection;
	this->m_calcDirectionVectorFromTarget(vDirection);

	// Scale the direction we want to go towards the direction the camera is looking at
	glm::vec3 finalDeltaPosition = vDirection * distanceAlongRelativeZAxis_PosIsForward;

	// Update the position
	this->eye += finalDeltaPosition;

	return;
}

void cCamera::Fly_moveRightLeft(float distanceAlongRelativeXAxis_PosIsRight)
{
	// Calcualte axis components based on orientation
	glm::vec3 vDirection;
	this->m_calcDirectionVectorFromTarget(vDirection);

	// Scale the direction we want to go towards the direction the camera is looking at
	glm::vec3 moveDelta = glm::vec3( distanceAlongRelativeXAxis_PosIsRight, 0.0f, 0.0f );
	glm::vec3 finalDeltaPosition = vDirection * moveDelta;

	// Update the position
	this->eye += finalDeltaPosition;

	return;
}

void cCamera::Fly_moveUpDown(float distanceAlongRelativeYAxis_PosIsUp)
{
	// Calcualte axis components based on orientation
	glm::vec3 vDirection;
	this->m_calcDirectionVectorFromTarget(vDirection);

	// Scale the direction we want to go towards the direction the camera is looking at
	glm::vec3 moveDelta = glm::vec3( 0.0f, distanceAlongRelativeYAxis_PosIsUp, 0.0f );
	glm::vec3 finalDeltaPosition = vDirection * moveDelta;

	// Update the position
	this->eye += finalDeltaPosition;

	return;
}
// +ve is right
void cCamera::Fly_turn(float turnDegreesPosIsRight)
{
	// Get direction vector and length
	glm::vec3 vEyeToTargetDirection;
	float eyeToTargetLength = 0.0f;
	this->m_calcDirectionVectorFromTarget(vEyeToTargetDirection, eyeToTargetLength);

	// Convert vector into a quaternion
	glm::quat qAdjust = glm::quat(vEyeToTargetDirection);

	// Rotate with quaternion

	// Scale target back to original location

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
