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
	~cCamera();

	// For following, etc. 
	void updateTick(double deltaTime);

	glm::mat4 getViewMatrix(void);

	enum eMode
	{
		MODE_MANUAL,				// Move along the axes (lookat)
		MODE_FOLLOW,				// Follows a target (lookat)
		MODE_FLY_USING_LOOK_AT		// Here, you use the "target" as direction
									// you want to go. This allows you to transition
									// from the FOLLOW_CAMERA to FLY seamlessly
	};

	void setCameraMode(eMode cameraMode);
	eMode getCameraMode(void);
	std::string getCameraModeString(void);

	glm::vec3 getEyePosition(void);

	// These are used to simplify the interface for the programmer, 
	//	breaking the various modes of the camera into groups
	class cManualCameraRedirect
	{
	public:
		void setEyePosition(glm::vec3 newPos);
		void adjustEyePosition(glm::vec3 deltaPos);
		void setTargetInWorld(glm::vec3 worldLocation);
		void adjustTargetInWorld(glm::vec3 deltaWorldLocation);
		void setUpVector(glm::vec3 up);

		glm::vec3 getTargetPosition(void);
		glm::vec3 getUpVector(void);
	private:
		friend cCamera;
		cManualCameraRedirect(cCamera* pTheCamera);
		cManualCameraRedirect();		// Don't call
		cCamera* pParentCamera;
		// called BY camera
		void m_updateTick(double deltaTime);
	};

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
		// called by camera
		void m_updateTick(double deltaTime);
	};

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
		// called by camera
		void m_updateTick(double deltaTime);
	};




	// The public variables for the various modes
	cManualCameraRedirect*	ManualCam;
	cFollowCameraRedirect*	FollowCam;
	cFlyCameraRedirect*		FlyCam;


private:

	eMode m_cameraMode;

	// connect this to the physics updater??
	void m_EulerIntegrate(double deltaTime);

	// State:
	glm::quat m_qOrientation;
	glm::vec3 m_eye;			// position
	glm::vec3 m_target;
	glm::vec3 m_up;
	glm::vec3 m_velocity;		// For "fly camera", like in blender
	glm::vec3 m_accel;			// For "thruster" like with rockets


};

#endif
