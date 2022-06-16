#pragma once
#include "../../include/LowRenderer/Camera.h"

Camera::Camera(float fovy, float aspect, float near, float far)
{
	projMatrix = lm::mat4::perspectiveProjection(fovy, aspect, near, far);
	Position = lm::vec3(0, 2, 0);
	WorldUp = lm::vec3::up;
	Yaw = -90.0f;
	Pitch = 0.0f;
	_near = near;
	_far = far;
	updateCameraVectors();
}

void Camera::setHRotationDirection(int direction)
{
	this->rotationHDirection = direction;
}

lm::vec3 Camera::setFrontDirection()
{
	lm::vec3 front;
	front.X() = cos(Yaw * (M_PI / HALF_CIRCLE));
	front.Z() = sin(Yaw * (M_PI / HALF_CIRCLE));
	front.normalized();

	return front;
}

void Camera::setVRotationDirection(int direction)
{
	this->rotationVDirection = direction;
}

void Camera::update()
{

	viewMatrix = GetViewMatrix();

	viewProjMatrix = projMatrix * viewMatrix;

	if (this->Position.Y() > 2)
		inAir = true;
	else
		inAir = false;
}

lm::mat4& Camera::GetVPMatrix()
{
	return viewProjMatrix;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (Pitch > 89.0f)
			Pitch = 89.0f;
		if (Pitch < -89.0f)
			Pitch = -89.0f;
	}

	// update Front, Right and Up Vectors using the updated Euler angles
	updateCameraVectors();
}

void Camera::updateCameraVectors()
{
	// calculate the new Front vector
	lm::vec3 front;
	front.X() = cos(Yaw * (M_PI / HALF_CIRCLE)) * cos(Pitch * (M_PI / HALF_CIRCLE));
	front.Y() = sin(Pitch * (M_PI / HALF_CIRCLE));
	front.Z() = sin(Yaw * (M_PI / HALF_CIRCLE)) * cos(Pitch * (M_PI / HALF_CIRCLE));
	Front = front.normalized();
	// also re-calculate the Right and Up vector
	Right = Front.crossProduct(WorldUp).normalized();  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	Up = Right.crossProduct(Front).normalized();
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	if (direction == FORWARD)
		_frontInput = 1;
	else if (direction == BACKWARD)
		_frontInput = -1;

	if (direction == LEFT)
		_rightInput = -1;
	else if (direction == RIGHT)
		_rightInput = 1;

	if (direction == SPEED)
		MovementSpeed = SpeedModifier;
	if (direction == WALK)
		MovementSpeed = NormalSpeed;

	if (direction == JUMP)
		_upInput = 1;

	else if (direction == JUMPSTOP)
		_upInput = 0;

	/*
	lm::vec3 front = setFrontDirection(deltaTime);
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		Position += front * velocity;
	if (direction == BACKWARD)
		Position -= front * velocity;
	if (direction == LEFT)
		Position -= Right * velocity;
	if (direction == RIGHT)
		Position += Right * velocity;
	if (direction == SPEED)
		MovementSpeed = SpeedModifier;
	if (direction == WALK)
		MovementSpeed = NormalSpeed;*/
}

// returns the view matrix calculated using Euler Angles and the LookAt Matrix
lm::mat4 Camera::GetViewMatrix()
{
	return lm::mat4::lookAt(Position, Position + Front, Up);
}

lm::vec3& Camera::GetPosition()
{
	return this->Position;
}

lm::vec3& Camera::GetFoward()
{
	return this->Front;
}

lm::vec3& Camera::GetUp()
{
	return this->Up;
}
