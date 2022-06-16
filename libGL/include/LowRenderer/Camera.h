#pragma once
#include "../../libmaths/LibMaths/Mat4/Mat4.h"
#include "../../libmaths/LibMaths/Vec3/Vec3.h"
#include "../../libmaths/LibMaths/Vec2/Vec2.h"

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	SPEED,
	WALK,
	JUMP,
	JUMPSTOP
};

class Camera
{
protected:
	lm::vec3 Position;
	lm::vec3 Front;
	lm::vec3 Up;
	lm::vec3 Right;
	lm::vec3 WorldUp;
	float _near;
	float _far;

	bool inAir = 0;
	// euler Angles
	float Yaw;
	float Pitch;
	// camera options
	float MovementSpeed = 1.f;
	float SpeedModifier = 10.f;
	float NormalSpeed = 5.f;
	float MouseSensitivity = 0.1f;

	lm::mat4 viewMatrix;
	lm::mat4 projMatrix;
	lm::mat4 viewProjMatrix;

	float sensitity = 0.01f;
	int rotationHDirection = 0;
	int rotationVDirection = 0;

	void updateCameraVectors();

	//Input
	int _frontInput = 0;
	int _rightInput = 0;
	int _upInput = 0;

public:
	Camera() {};
	Camera(float fovy, float aspect, float near, float far);

	void			setHRotationDirection(int direction);

	lm::vec3		setFrontDirection();
	void			setVRotationDirection(int direction);
	lm::mat4&		GetProjMatrix() { return projMatrix; }
	lm::mat4&		GetVPMatrix();
	void			update();

	void			ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
	virtual void	ProcessKeyboard(Camera_Movement direction, float deltaTime);

	lm::mat4		GetViewMatrix();
	lm::mat4		GetViewProj() { return viewProjMatrix; }
	lm::vec3&		GetPosition();
	lm::vec3&		GetFoward();
	lm::vec3&		GetUp();
	lm::vec3&		GetRight() { return Right; }
	float&			GetNear() { return _near; }
	float&			GetFar() { return _far; }
	float&			GetMovementSpeed() { return MovementSpeed; }

	int& GetFrontInput() { return _frontInput; }
	int& GetRightInput() { return _rightInput; }
	int& GetUpInput() { return _upInput;  }
	bool& GetInAir() { return inAir; }
	float& GetYaw() { return Yaw; }
	float& GetPitch() { return Pitch; }
};