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
        glm::mat4 m_Projection = glm::mat4(1.0f);
        glm::mat4 m_View = glm::mat4(1.0f);
        glm::mat4 m_ProjectionView = glm::mat4(1.0f);



    public:
        Camera() = default;
        Camera(const glm::mat4& projection);
        virtual ~Camera() = default;

        void RecalculateView(glm::vec3 position, glm::quat rotation);
        void RecalculateView(glm::vec3 position, glm::vec3 front, glm::vec3 up);

        virtual const glm::mat4& GetProjection() const;
        virtual const glm::mat4& GetView() const;
        virtual const glm::mat4& GetProjectionView() const;

        virtual void SetViewportSize(uint32_t width, uint32_t height) = 0;
    };


    class OrthographicCamera: public Camera
    {
    private:
        float m_Left = -5.0f;
        float m_Right = 5.0f;
        float m_Bottom = -5.0f;
        float m_Top = 5.0f;
        float m_Near = -1.0f;
        float m_Far = 1.0f;
        float m_AspectRatio = 0.0f;
    public:
        OrthographicCamera(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);

        void SetProjection(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);
        void SetViewportSize(uint32_t width, uint32_t height) override;
    };


    class PerspectiveCamera: public Camera
    {
    private:
        float m_Fov = 45.0f;
        float m_Near = 0.5f;
        float m_Far = 10.0f;
        float m_AspectRatio = 0.0f;
    public:
        PerspectiveCamera(float fov, float aspectRatio, float zNear, float zFar);
        PerspectiveCamera(float fov, float width, float heigth, float zNear, float zFar);

        void SetProjection(float fov, float aspectRatio, float zNear, float zFar);
        void SetViewportSize(uint32_t width, uint32_t height) override;

    };
}

#endif // CAMERAS_H
