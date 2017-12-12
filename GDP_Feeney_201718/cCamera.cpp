#include "cCamera.h"
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


cCamera::cCamera()
{
	// Default OpenGL 1.1 was on z axis looking at the origin
	this->eye = glm::vec3(0.0f, 0.0f, 1.0f);
	this->target = glm::vec3(0.0f, 0.0f, 0.0f);
	this->up = glm::vec3(0.0f, 1.0f, 0.0f);

	// Set quaternion to some default rotation value
	this->qOrientation = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));

	this->accel = glm::vec3(0.0f);
	this->velocity = glm::vec3(0.0f);

	return;
}


// For following, etc. 
void cCamera::updateTick(float deltaTime)
{

	if ( this->cameraMode == cCamera::FOLLOW_CAMERA )
	{	// update the velocity of the camera 
		this->m_UpdateFollowCamera_GOOD(deltaTime);
	}

	// Explicit forward Euler, like in the physics loop
	// Acceleration comes form velocity
	glm::vec3 accelThisStep = this->accel * deltaTime;
	this->velocity += accelThisStep;

	// Position comes from velocity over time
	glm::vec3 velChangeThisStep = this->velocity * deltaTime;
	this->eye += velChangeThisStep;

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
glm::mat4 cCamera::getViewMatrix(void)
{
	// Based on the mode, calculate the view matrix
	switch (cameraMode)
	{
	case cCamera::eMode::MANUAL:
		// Use LookAT
		break;
	case cCamera::eMode::FOLLOW_CAMERA:
		glm::mat4 matView = glm::lookAt(this->eye,
		                                this->target,
		                                glm::vec3(0.0f, 1.0f, 0.0f) ); // UP
		return matView;
		break;
	case cCamera::eMode::FLY_CAMERA:
		// Use same process as with drawing an object:
		// Combine transform with rotation, and return that
		glm::mat4 matCamView = glm::mat4(1.0f);

		glm::mat4 trans = glm::mat4x4(1.0f);
		trans = glm::translate(trans, this->eye);
		matCamView = matCamView * trans;

		// Like many things in GML, the conversion is done in the constructor
		glm::mat4 postRotQuat = glm::mat4(this->qOrientation);
		matCamView = matCamView * postRotQuat;

		return matCamView;
		break;
	}
	// You need to check what you are doign with your life!
	// return the identity matrix
	return glm::mat4(1.0f);
}

//matView = glm::lookAt( g_cameraXYZ,						// "eye" or "camera" position
//					   g_cameraTarget_XYZ,		// "At" or "target" 
//					   glm::vec3( 0.0f, 1.0f, 0.0f ) );	// "up" vector


// ************************************************************
// For the "fly camera":
// +ve is along z-axis
void cCamera::Fly_moveForward(float distanceAlongRelativeZAxis_PosIsForward)
{
	// Along the z axis
	this->eye.z += distanceAlongRelativeZAxis_PosIsForward;
	return;
}

void cCamera::Fly_moveRightLeft(float distanceAlongRelativeXAxis_PosIsRight)
{
	// Along the x axis
	this->eye.x += distanceAlongRelativeXAxis_PosIsRight;
	return;
}

void cCamera::Fly_moveUpDown(float distanceAlongRelativeYAxis_PosIsUp)
{
	// Along the y
	this->eye.y += distanceAlongRelativeYAxis_PosIsUp;
	return;
}

// +ve is right
void cCamera::Fly_turn_RightLeft(float turnDegreesPosIsRight)
{
	// From camera orientation, Y axis "turns" left and right
	this->adjustQOrientationFormDeltaEuler(
		glm::vec3(0.0f,
				  glm::radians(turnDegreesPosIsRight),	// Y axis
		          0.0f));
	return;
}
// +ve it up
void cCamera::Fly_pitch_UpDown(float pitchDegreesPosIsNoseUp)
{
	// From camera orientation, X axis "pitches" up and down
	this->adjustQOrientationFormDeltaEuler(
		glm::vec3( glm::radians(pitchDegreesPosIsNoseUp),
			       0.0f,
			       0.0f ));
	return;
}

// +ve is Clock-wise rotation (from nose to tail)
void cCamera::Fly_yaw_CWorCCW(float pitchDegreesPosIsClockWise)
{
	// From camera orientation, Z axis "rolls" around
	// (assume z axis is "nose to tail"
	this->adjustQOrientationFormDeltaEuler(
		glm::vec3(0.0f,
			      0.0f,
			      glm::radians(pitchDegreesPosIsClockWise)));
	return;
}
//// ************************************************************
//
//void cCamera::setOrientationFromEuler(glm::vec3 eulerAngles);
//glm::mat4 cCamera::getMat4FromOrientation(void);

void cCamera::overwrtiteQOrientationFormEuler(glm::vec3 eulerAxisOrientation)
{
	// Calcualte the quaternion represnetaiton of this Euler axis
	// NOTE: We are OVERWRITING this..
	this->qOrientation = glm::quat(eulerAxisOrientation);

	return;
}

void cCamera::adjustQOrientationFormDeltaEuler(glm::vec3 eulerAxisOrientChange)
{
	// How do we combine two matrices?
	// That's also how we combine quaternions...

	// So we want to "add" this change in oriention
	glm::quat qRotationChange = glm::quat(eulerAxisOrientChange);

	// Mulitply it by the current orientation;
	this->qOrientation = this->qOrientation * qRotationChange;

	return;
}
