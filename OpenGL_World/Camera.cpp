#include "Camera.h"

Camera::Camera() {}

Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, GLfloat startYaw, GLfloat startPitch, GLfloat startMoveSpeed, GLfloat startTurnSpeed)
{
	position = startPosition;
	worldUp = startUp;
	yaw = startYaw;
	pitch = startPitch;
	front = glm::vec3(0.0f, 0.0f, -1.0f);

	moveSpeed = startMoveSpeed;
	turnSpeed = startTurnSpeed;

	update();
}

void Camera::keyControl(bool* keys) 
{
	if (keys[GLFW_KEY_W])
	{
		position += front * moveSpeed;
		printf("Camera::keyControl called. W key = %d\n", keys[GLFW_KEY_W]);

		printf("Moved Forward, Position: (%f,%f,%f)\n", position.x, position.y, position.z);
	}

	if (keys[GLFW_KEY_S])
	{
		position -= front * moveSpeed;
		printf("Moved Backwards, Position: (%f,%f,%f)\n", position.x, position.y, position.z);
	}

	if (keys[GLFW_KEY_A])
	{
		position -= right * moveSpeed;
		printf("Moved Left, Position: (%f,%f,%f)\n", position.x, position.y, position.z);
	}

	if (keys[GLFW_KEY_D])
	{
		position += right * moveSpeed;
		printf("Moved Right, Position: (%f,%f,%f)\n", position.x, position.y, position.z);
	}

}

glm::mat4 Camera::calculateViewMatrix()
{
	return glm::lookAt(position, position + front, up);
}

void Camera::update()
{ 
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw) * cos(glm::radians(pitch)));
	front = glm::normalize(front);

	right = glm::normalize(glm::cross(front, worldUp));
	up = glm::normalize(glm::cross(right, front));
}

Camera::~Camera()
{

}
