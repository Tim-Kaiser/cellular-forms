#pragma once
#include <glm.hpp>
#include <ext/matrix_transform.hpp>

enum DIRECTION
{
	FORWARD,
	BACKWARD,
	UPWARD,
	LEFT,
	RIGHT
	
};


class Camera
{
private:
	float m_yaw;
	float m_pitch;
	float m_speed;

	glm::vec3 m_position;
	glm::vec3 m_up;
	glm::vec3 m_lookAt;
	glm::vec3 m_right;

	void updateVectors();
public:
	Camera();
	Camera(float yaw, float pitch, float speed, glm::vec3 position, glm::vec3 up, glm::vec3 lookAt);
	glm::mat4 getViewMatrix();
	void handleKeyboardInput(DIRECTION direction);
	void handleMouseInput(float mousemoveX, float mousemoveY);

};