#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

enum Camera_Movement
{
    UPWARD,   // 0
    DOWNWARD, // 1
    LEFT,     // 2
    RIGHT     // 3
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 0.005f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
public:
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;

    glm::vec3 OriginPosition;

    // euler Angles
    float Yaw;
    float Pitch;

    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(
        glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
        float yaw = YAW,
        float pitch = PITCH);

    // constructor with scalar values
    Camera(
        float posX, float posY, float posZ,
        float upX, float upY, float upZ,
        float yaw, float pitch);

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix();

    void ProcessMouseMove(Camera_Movement direction, float deltaTime);
    void ProcessMouseRotate(float xoffset, float yoffset);
    void ProcessMouseScroll(float yoffset);
    void Reset();

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors();
};

#endif
