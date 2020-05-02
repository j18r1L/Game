#include "GraphicsComponents.h"

void PlayerGraphicsComponent::update(Renderer& renderer)
{
    // Настройка параметров рендера
    Shader* shader = renderer.getShader(0);
    Camera* camera = m_CameraComponent->getCamera();

    glm::mat4 mvp = camera->getProjectionMatrix() * camera->getViewMatrix() * (*m_PhysicsComponent->getModelMatrix());

    shader->Use();
    shader->SetMat4("model_view_projection", mvp);

    // TODO создание буфферов в рендере и обращение к ним по индексам
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Рендер
    renderer.Draw(0, m_ModelComponent->getModel());
}

void ModelGraphicsComponent::update(Renderer& renderer)
{
    // Настройка параметров рендера
    Shader* shader = renderer.getShader(0);
    Camera* camera = m_CameraComponent->getCamera();

    glm::mat4 mvp = camera->getProjectionMatrix() * camera->getViewMatrix() * (*m_PhysicsComponent->getModelMatrix());

    shader->Use();
    shader->SetMat4("model_view_projection", mvp);

    // TODO создание буфферов в рендере и обращение к ним по индексам
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Рендер
    renderer.Draw(0, m_ModelComponent->getModel());
}

void EditorGraphicsComponent::update(Renderer& renderer)
{
    // Настройка параметров рендера
    Shader* shader;
    Camera* camera = m_CameraComponent->getCamera();
    // TODO создание буфферов в рендере и обращение к ним по индексам
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glm::mat4 m = *m_PhysicsComponent->getModelMatrix();
    glm::mat4 m_xyz(1.0f);
    glm::mat4 vp = camera->getProjectionMatrix() * camera->getViewMatrix();
    glm::mat4 mvp;

    // Включаем запись тест трафарета
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    glStencilMask(0xFF);
    // Рисуем XYZ стрелки
    shader = renderer.getShader(1);
    shader->Use();
    glBindVertexArray(*m_ModelComponent->getVAOxyz());

    // X
    m_xyz = glm::scale(m, glm::vec3(1.0f, 0.1f, 0.1f));
    m_xyz = glm::translate(m_xyz, glm::vec3(1.1f, 0.0f, 0.0f));
    mvp = vp * m_xyz;
    shader->SetMat4("model_view_projection", mvp);
    shader->SetVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Z
    m_xyz = glm::scale(m, glm::vec3(0.1f, 0.1f, 1.0f));
    m_xyz = glm::translate(m_xyz, glm::vec3(0.0f, 0.0f, 1.1f));
    mvp = vp * m_xyz;
    shader->SetMat4("model_view_projection", mvp);
    shader->SetVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    // Y
    m_xyz = glm::scale(m, glm::vec3(0.1f, 1.0f, 0.1f));
    m_xyz = glm::translate(m_xyz, glm::vec3(0.0f, 0.9f, 0.0f));
    mvp = vp * m_xyz;
    shader->SetMat4("model_view_projection", mvp);
    shader->SetVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    //glDisable(GL_DEPTH_TEST);


    mvp = vp * m;
    shader = renderer.getShader(0);
    shader->Use();
    shader->SetMat4("model_view_projection", mvp);
    renderer.Draw(0, m_ModelComponent->getModel());

    //glStencilMask(0xFF);
    //glEnable(GL_DEPTH_TEST);





}
