#include "Camera.h"


//defaults
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 0.1f;
const glm::vec3 POSITION = glm::vec3(0, 0, 3);
const glm::vec3 UP = glm::vec3(0, 1, 0);
const glm::vec3 LOOKAT = glm::vec3(0, 0, 0);

void Camera::updateVectors()
{
    glm::vec3 lookAt;
    lookAt.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    lookAt.y = sin(glm::radians(m_pitch));
    lookAt.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));

    m_lookAt = glm::normalize(lookAt);
    m_right = glm::normalize(glm::cross(m_lookAt, glm::vec3(0.0f, 1.0f, 0.0f)));
    m_up = glm::normalize(glm::cross(m_right, m_lookAt));
}

Camera::Camera():
    m_yaw(YAW),
    m_pitch(PITCH),
    m_speed(SPEED),
    m_position(POSITION),
    m_up(UP),
    m_lookAt(LOOKAT)
{
    updateVectors();
}

Camera::Camera(float yaw = YAW, float pitch = PITCH, float speed = SPEED, glm::vec3 position = POSITION, glm::vec3 up = UP, glm::vec3 lookAt = LOOKAT) :
    m_yaw(yaw),
    m_pitch(pitch),
    m_speed(speed),
    m_position(position),
    m_up(up),
    m_lookAt(lookAt)
{
    updateVectors();
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(m_position, m_position + m_lookAt, m_up);
}

void Camera::handleKeyboardInput(DIRECTION direction)
{
    if (direction == FORWARD)
        m_position += m_lookAt * m_speed;
    if (direction == BACKWARD)
        m_position -= m_lookAt * m_speed;
    if (direction == LEFT)
        m_position -= m_right * m_speed;
    if (direction == RIGHT)
        m_position += m_right * m_speed;
    if (direction == UPWARD)
        m_position += m_up * m_speed;
}

void Camera::handleMouseInput(float mousemoveX, float mousemoveY)
{
    m_yaw += mousemoveX * 0.1f;
    m_pitch += mousemoveY * 0.1f;

    if (m_pitch > 89.0f)
    {
        m_pitch = 89.0f;
    }
    else if (m_pitch < -89.0f)
    {
        m_pitch = -89.0f;
    }
    updateVectors();
}
