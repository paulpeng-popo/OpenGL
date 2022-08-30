#include "../myheaders/Camera.h"

Camera::Camera(
    glm::vec3 position,
    glm::vec3 up,
    float yaw,
    float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
                   MovementSpeed(SPEED),
                   MouseSensitivity(SENSITIVITY),
                   Zoom(ZOOM)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    OriginPosition = position;

    updateCameraVectors();
}

Camera::Camera(
    float posX, float posY, float posZ,
    float upX, float upY, float upZ,
    float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)),
                              MovementSpeed(SPEED),
                              MouseSensitivity(SENSITIVITY),
                              Zoom(ZOOM)
{
    Position = glm::vec3(posX, posY, posZ);
    WorldUp = glm::vec3(upX, upY, upZ);
    Yaw = yaw;
    Pitch = pitch;
    OriginPosition = Position;

    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessMouseMove(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == UPWARD)
        Position += Up * velocity;
    if (direction == DOWNWARD)
        Position -= Up * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
}

void Camera::ProcessMouseRotate(float xoffset, float yoffset)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset)
{
    Zoom -= (float)yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 80.0f)
        Zoom = 80.0f;
}

void Camera::Reset()
{
    Position = OriginPosition;
    Yaw = YAW;
    Pitch = PITCH;
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    // calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);

    // also re-calculate the Right and Up vector
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}
