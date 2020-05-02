#include "Camera.h"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch, GLfloat znear, GLfloat zfar):
    Position(position),
    Front(glm::vec3(0.0f, 1.0f, -1.0f)),
    WorldUp(up),
    Yaw(yaw),
    Pitch(pitch),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVTY),
    Zoom(ZOOM),
    zNear(znear),
    zFar(zfar)

{
    this->updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch, GLfloat znear, GLfloat zfar):
    Position(glm::vec3(posX, posY, posZ)),
    Front(glm::vec3(0.0f, 0.0f, -1.0f)),
    WorldUp(glm::vec3(upX, upY, upZ)),
    Yaw(yaw),
    Pitch(pitch),
    MovementSpeed(SPEED),
    MouseSensitivity(SENSITIVTY),
    Zoom(ZOOM),
    zNear(znear),
    zFar(zfar)
{
    this->updateCameraVectors();
}


glm::mat4 Camera::getViewMatrix()
{
    //std::cout << Position.x << " : " << Position.y << " : " << Position.z << std::endl;
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

glm::mat4 Camera::getProjectionMatrix()
{
    // TODO возможно надо исправить
    VideoSettings* videoSettings = VideoSettings::getInstance();
    GLint width = 800, height = 600;
    videoSettings->getWidthHeight(&width, &height);
    return glm::perspective(glm::radians(Zoom), static_cast<GLfloat>(width) / static_cast<GLfloat>(height), zNear, zFar);
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{

    GLfloat velocity = this->MovementSpeed * deltaTime;
    if (direction == FORWARD)
        this->Position += this->Front * velocity;
    if (direction == BACKWARD)
        this->Position -= this->Front * velocity;
    if (direction == LEFT)
        this->Position -= this->Right * velocity;
    if (direction == RIGHT)
        this->Position += this->Right * velocity;
    if (direction == UP)
        this->Position += this->Up * velocity;
    if (direction == DOWN)
        this->Position -= this->Up * velocity;
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw   += xoffset;
    this->Pitch += yoffset;

    if (constrainPitch)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }
    this->updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
    if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
        this->Zoom -= yoffset;
    if (this->Zoom <= 1.0f)
        this->Zoom = 1.0f;
    if (this->Zoom >= 45.0f)
        this->Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    glm::vec3 front(1);
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));
    this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
}
