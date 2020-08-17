#ifndef CAMERAS_H
#define CAMERAS_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace HE
{

    class Camera
    {
    protected:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ProjectionViewMatrix;



    public:
        virtual ~Camera() = default;

        void RecalculateViewMatrix(glm::vec3 position, glm::quat rotation);

        virtual const glm::mat4& GetProjectionMatrix() const;
        virtual const glm::mat4& GetViewMatrix() const;
        virtual const glm::mat4& GetProjectionViewMatrix() const;
    };


    class OrthographicCamera: public Camera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);

        void SetProjection(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);
    };


    class PerspectiveCamera: public Camera
    {
    public:
        PerspectiveCamera(float fov, float aspectRatio, float zNear, float zFar);
        PerspectiveCamera(float fov, float width, float heigth, float zNear, float zFar);

        void SetProjection(float fov, float aspectRatio, float zNear, float zFar);

    };
}

#endif // CAMERAS_H
