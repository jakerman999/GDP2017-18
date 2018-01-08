#ifndef _cCamera_HG_
#define _cCamera_HG_

#include <glm/vec3.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <string>

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
	void updateTick(double deltaTime);

	// These are used to simplify the interface for the programmer, 
	//	breaking the various modes of the camera into groups
	class cFollowCameraRedirect
	{
	public:
		void setOrUpdateTarget(glm::vec3 target);
		void setIdealCameraLocation(glm::vec3 relativeToTarget);
		void setMaxFollowSpeed(float speed);
		void setDistanceMaxSpeed(float distanceToTarget);
		void setDistanceZeroSpeed(float distanceToTarget);
		void setAll(glm::vec3 target, glm::vec3 idealCamLocRelToTarget, 
					float maxSpeed, float maxSpeedDistance, float zeroSpeedDistance);
	private:
		friend cCamera;
		cFollowCameraRedirect(cCamera *pTheCamera);
		cFollowCameraRedirect();	// Don't call
		cCamera* pParentCamera;
	};
	cFollowCameraRedirect* FollowCam;

	class cFlyCameraRedirect
	{
	public:
		void moveForward(float distance);
		void moveRight(float distance);
		void moveUp(float distance);
		void move(glm::vec3 direction_Zforward_Yup_Xright);
		void yawOrTurnRight(float angle, bool isDegrees = true);
		void pitchUp(float angle, bool isDegrees = true);
		void rollClockWise(float angle, bool isDegrees = true);
		// 
		void setTargetInWorld(glm::vec3 worldLocation);
		void setDirectionRelative(glm::vec3 relativeDirection);

	private:
		friend cCamera;
		cFlyCameraRedirect(cCamera* pTheCamera);
		cFlyCameraRedirect();		// Don't call
		cCamera* pParentCamera;
	};
	cFlyCameraRedirect* FlyCam;

	enum eMode
	{
		MODE_MANUAL,				// Move along the axes (lookat)
		MODE_FOLLOW,				// Follows a target (lookat)
		MODE_FLY_USING_LOOK_AT,		// Here, you use the "target" as direction
									// you want to go. This allows you to transition
									// from the FOLLOW_CAMERA to FLY seamlessly

		MODE_FLY_CAMERA_GARBAGE_DONT_USE		// Movement based on direction of gaze
												// Use quaternion orientation
												// "catch"  is no LOOKAT
	};

	void setCameraMode(eMode cameraMode);
	eMode getCameraMode(void);
	std::string getCameraModeString(void);

private:

	eMode m_cameraMode;

	// Follow camera
	void m_Follow_SetOrUpdateTarget(glm::vec3 target);
	void m_Follow_SetIdealCameraLocation(glm::vec3 relativeToTarget);
	void m_Follow_SetMaxFollowSpeed(float speed);
	void m_Follow_SetDistanceMaxSpeed(float distanceToTarget);
	void m_Follow_SetDistanceZeroSpeed(float distanceToTarget);

	glm::vec3 follow_idealCameraLocationRelToTarget;
	float follow_max_speed;
	float follow_distance_max_speed;
	float follow_distance_zero_speed;

	void m_UpdateFollowCamera_SUCKS(double deltaTime);
	void m_UpdateFollowCamera_GOOD(double deltaTime);

	// ************************************************************
	// For the "fly camera":
	// +ve is along z-axis
	void Fly_moveForward(float distanceAlongRelativeZAxis_PosIsForward);
	void Fly_moveRightLeft(float distanceAlongRelativeXAxis_PosIsRight);
	void Fly_moveUpDown(float distanceAlongRelativeYAxis_PosIsUp);
	void Fly_move(glm::vec3 directionIWantToMove_Zforward_Yup_Xleftright);
	// +ve is right
	void Fly_turn_RightLeft(float turnDegreesPosIsRight);
	// +ve it up
	void Fly_pitch_UpDown(float pitchDegreesPosIsNoseUp);
	// +ve is Clock-wise rotation (from nose to tail)
	void Fly_yaw_CWorCCW(float pitchDegreesPosIsClockWise);
	// 
	// You can use this to change the target from wherever it is
	//	to, say, 1.0 units from the front of the camera. 
	void RelocateTargetInFrontOfCamera(float howFarAwayFromFront);
private:
	// Used for the directional calculations to move the camera
	void m_calcDirectionVectorFromTarget(glm::vec3 &vEyeToTargetDirection, float &eyeToTargetLength);
	void m_calcDirectionVectorFromTarget(glm::vec3 &vEyeToTargetDirection);
public:
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
