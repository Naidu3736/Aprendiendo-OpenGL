#ifndef CAMERA_HPP
#define CAMERA_HPP

#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

namespace engine::graphics {
    class Camera {
        private: 
        glm::vec3 m_position;
        glm::vec3 m_forward;
        glm::vec3 m_right;
        glm::vec3 m_up;
        glm::vec3 m_worldUp;

        float m_yaw;
        float m_pitch;
        float m_movementSpeed;
        float m_mouseSensitivity;
        float m_zoomSensitivity;
        float m_fov;
        float m_minFov;
        float m_maxFov;
        float m_zNear;
        float m_zFar;
        
        void updateVectors();

        public:
        Camera(const glm::vec3& position = {0,0,0},
                float yaw = -90.0f,
                float pitch = 0.0f,
                const glm::vec3& up = {0,1,0},
                float movementSpeed = 2.5f,
                float mouseSensitivity = 0.05f,
                float zoomSensitivity = 0.5f,
                float fov = 45.0f,
                float minFov = 1.0f,
                float maxFov = 120.0f,
                float zNear = 0.0001f,
                float zFar = 100.0f);

        glm::mat4 getViewMatrix() const;
        glm::mat4 getProjectionMatrix(float aspectRatio) const;

        void move(const glm::vec3& offset);
        void moveForward(float distance);
        void moveRight(float distance);
        void moveUp(float distance);
        void rotate(float yawOffset, float pitchOffset);
        void zoom(float offset);
        
        glm::vec3 position() const;
        glm::vec3 forward() const;
        glm::vec3 up() const;
        glm::vec3 right() const;
        float yaw() const;
        float pitch() const;
        float movementSpeed() const;
        float mouseSensitivity() const;
        float zoomSensitivity() const;
        float fov() const;
        float minFov() const;
        float maxFov() const;
        float zNear() const;
        float zFar() const;

        void setMovementSpeed(float movementSpeed);
        void setMouseSensitivity(float sensitivity);
        void setZoomSensitivity(float sensitivity);
        void setFov(float fov);
        void setFovLimits(float minFov, float maxFov);
        void setZNear(float zNear);
        void setZFar(float zFar);

        void reset();
    };
}

#endif // CAMERA_HPP