#ifndef CAMERAS_H
#define CAMERAS_H


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace HE
{
    // This camera we using for camera component (runtime)
    class Camera
    {
    protected:
        glm::mat4 m_Projection = glm::mat4(1.0f);

    public:
        Camera() = default;
        Camera(const glm::mat4& projection);
        virtual ~Camera() = default;

        const glm::mat4& GetProjection() const;
    };


    // These cameras we using for levelEditor camera
    class OrthographicCamera
    {
    private:
        glm::mat4 m_Projection = glm::mat4(1.0f);
        glm::mat4 m_View = glm::mat4(1.0f);
        glm::mat4 m_ProjectionView = glm::mat4(1.0f);

        glm::vec3 m_Position = glm::vec3(0.0f);
        float m_Rotation = 0.0f;

        void RecalculateView();
    public:
        OrthographicCamera(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);

        void SetProjection(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);
        void SetPosition(const glm::vec3& position);
        void SetRotation(float rotation);

        const glm::mat4& GetProjection() const { return m_Projection; }
        const glm::mat4& GetView() const { return m_View; }
        const glm::mat4& GetProjectionView() const { return m_ProjectionView; }
        const glm::vec3& GetPosition() const { return m_Position; }
        float GetRotation() const { return m_Rotation; }
    };


    class PerspectiveCamera
    {
    private:
        glm::mat4 m_Projection = glm::mat4(1.0f);
        glm::mat4 m_View = glm::mat4(1.0f);
        glm::mat4 m_ProjectionView = glm::mat4(1.0f);

        glm::vec3 m_Position = {0.0f, 0.0f, 0.0f};
        glm::quat m_Rotation = glm::quat(glm::vec4(0.0f));
        //glm::vec3 m_Rotation = glm::vec3(0.0f); // Euler realization

        void RecalculateView();
    public:
        PerspectiveCamera(float fov, float aspectRatio, float zNear, float zFar);
        PerspectiveCamera(float fov, float width, float height, float zNear, float zFar);

        void RecalculateView(glm::vec3 front, glm::vec3 up);

        void SetProjection(float fov, float aspectRatio, float zNear, float zFar);
        void SetProjection(float fov, float width, float height, float zNear, float zFar);
        void SetPosition(const glm::vec3& position);
        void SetRotation(const glm::vec3& angles);
        void SetRotation(const glm::quat& rotation);

        const glm::mat4& GetProjection() const { return m_Projection; }
        const glm::mat4& GetView() const { return m_View; }
        const glm::mat4& GetProjectionView() const { return m_ProjectionView; }
        const glm::vec3& GetPosition() const { return m_Position; }
        const glm::quat& GetRotation() const { return m_Rotation; }

    };
}

#endif // CAMERAS_H
