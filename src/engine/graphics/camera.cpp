#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include <iostream>
#include <algorithm>

#include "engine/graphics/camera.hpp"
#include "engine/input/mouse.hpp"
#include "engine/core/timer.hpp"

using namespace engine::graphics;
using namespace engine::input;
using namespace engine::core;

void Camera::updateVectors()
{
    float cosPitch = std::cos(glm::radians(m_pitch));
    glm::vec3 newForward = glm::vec3(
        std::cos(glm::radians(m_yaw)) * cosPitch,
        std::sin(glm::radians(m_pitch)),
        std::sin(glm::radians(m_yaw)) * cosPitch
    );

    m_forward = glm::normalize(newForward);
    m_right = glm::normalize(glm::cross(m_forward, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_forward));
}

Camera::Camera(const glm::vec3& position,
               float yaw,
               float pitch,
               const glm::vec3& up,
               float movementSpeed,
               float mouseSensitivity,
               float zoomSensitivity,
               float fov,
               float minFov,
               float maxFov,
               float zNear,
               float zFar)
    : m_position(position)
    , m_worldUp(up)
    , m_yaw(yaw)
    , m_pitch(pitch)
    , m_movementSpeed(movementSpeed)
    , m_mouseSensitivity(mouseSensitivity)
    , m_zoomSensitivity(zoomSensitivity)
    , m_minFov(std::max(1.0f, minFov))
    , m_maxFov(std::min(120.0f, maxFov))
    , m_zNear(zNear)
    , m_zFar(zFar)
{    
    if (m_minFov >= m_maxFov) {
        std::swap(m_minFov, m_maxFov);
    }
    
    m_fov = std::clamp(fov, m_minFov, m_maxFov);
    updateVectors();
}

glm::mat4 Camera::getViewMatrix() const
{
    return glm::lookAt(m_position, m_position + m_forward, m_up);
}

glm::mat4 Camera::getProjectionMatrix(float aspectRatio) const
{
    return glm::perspective(glm::radians(m_fov), aspectRatio, m_zNear, m_zFar);
}

void Camera::move(const glm::vec3& offset)
{
    m_position += m_movementSpeed * offset;
}

void Camera::moveForward(float deltaTime)
{
    m_position += deltaTime * m_movementSpeed * glm::normalize(glm::vec3(m_forward.x, 0.0f, m_forward.z));
}

void Camera::moveRight(float deltaTime)
{
    m_position += deltaTime * m_movementSpeed * m_right;
}

void Camera::moveUp(float deltaTime)
{
    m_position += deltaTime * m_movementSpeed * m_worldUp;
}

void Camera::rotate(float yawOffset, float pitchOffset)
{
    yawOffset *= m_mouseSensitivity;
    pitchOffset *= m_mouseSensitivity;

    m_yaw += yawOffset;
    m_pitch += pitchOffset;
    m_pitch = std::clamp(m_pitch, -89.0f, 89.0f);

    updateVectors();
}

void Camera::zoom(float offset)
{
    setFov(m_fov - (offset * m_zoomSensitivity));
}

// Getters
glm::vec3 Camera::position() const
{
    return m_position;
}

glm::vec3 Camera::forward() const
{
    return m_forward;
}

glm::vec3 Camera::up() const
{
    return m_up;
}

glm::vec3 Camera::right() const
{
    return m_right;
}

float Camera::yaw() const
{
    return m_yaw;
}

float Camera::pitch() const
{
    return m_pitch;
}

float Camera::movementSpeed() const
{
    return m_movementSpeed;
}

float Camera::mouseSensitivity() const
{
    return m_mouseSensitivity;
}

float Camera::zoomSensitivity() const
{
    return m_zoomSensitivity;
}

float Camera::fov() const
{
    return m_fov;
}

float Camera::minFov() const
{
    return m_minFov;
}

float Camera::maxFov() const
{
    return m_maxFov;
}

float Camera::zNear() const
{
    return m_zNear;
}

float Camera::zFar() const
{
    return m_zFar;
}

// Setters
void Camera::setMovementSpeed(float movementSpeed)
{
    m_movementSpeed = movementSpeed;
}

void Camera::setMouseSensitivity(float sensitivity)
{
    m_mouseSensitivity = sensitivity;
}

void Camera::setZoomSensitivity(float sensitivity)
{
    m_zoomSensitivity = std::clamp(sensitivity, 0.0001f, 2.0f);
}

void Camera::setFov(float fov)
{
    m_fov = std::clamp(fov, m_minFov, m_maxFov);
}

void Camera::setFovLimits(float minFov, float maxFov)
{
    m_minFov = std::max(1.0f, minFov);
    m_maxFov = std::min(120.0f, maxFov);

    if (m_minFov > m_maxFov) {
        std::swap(m_minFov, m_maxFov);
    }
    
    m_fov = std::clamp(m_fov, m_minFov, m_maxFov);
}

void Camera::setZNear(float zNear)
{
    m_zNear = zNear;
}

void Camera::setZFar(float zFar)
{
    m_zFar = zFar;
}