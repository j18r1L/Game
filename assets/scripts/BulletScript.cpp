#include "BulletScript.h"

BulletScript::BulletScript(HE::Entity* entity) :
	ScriptComponent(entity)
{

}

void BulletScript::OnCreate()
{
    m_EntityHandle->AddComponent<HE::TagComponent>()->SetTag("bullet");
    auto collider = m_EntityHandle->AddComponent<HE::Circle2DColliderComponent>();
    collider->SetRadius(0.1f);
    const glm::vec3& position = m_EntityHandle->GetComponent<HE::TransformComponent>()->GetPosition();
    collider->SetOrigin({ position.x, position.y });
}

void BulletScript::OnUpdate(HE::Timestep& timestep)
{
    UpdateMovement(timestep);
    UpdateCollision(timestep);
}

void BulletScript::OnDestroy()
{
    // destroy entity 
    // TODO HACK!!! dont bind destroing on renderer!
    HE::Renderer::Submit([this]() mutable
        {
            m_EntityHandle->GetScene()->DestroyEntity(m_EntityHandle->GetName());
        });
}

void BulletScript::OnCollision(HE::Entity* entity)
{
    if (entity->GetComponent<HE::TagComponent>()->GetTag() == "obstacle")
    {
        OnDestroy();
    }
}

void BulletScript::UpdateCollision(HE::Timestep& timestep)
{
    auto transformComponent = m_EntityHandle->GetComponent<HE::TransformComponent>();
    glm::vec3 position = transformComponent->GetPosition();
}

void BulletScript::UpdateMovement(HE::Timestep& timestep)
{
    auto transformComponent = m_EntityHandle->GetComponent<HE::TransformComponent>();
    glm::vec3 position = transformComponent->GetPosition();

    glm::vec3 delta_position{ 0.0f };

    m_VerticalSpeed += timestep;
    delta_position.y += m_VerticalSpeed * timestep;


    // Get view and projection matrices
    auto cameraEntity = m_EntityHandle->GetScene()->GetEntity("Camera");
    glm::mat4 projection = cameraEntity->GetComponent<HE::CameraComponent>()->GetCamera().GetProjection();
    glm::mat4 view = glm::inverse(cameraEntity->GetComponent<HE::TransformComponent>()->GetTransform());

    // Calculate position in Clip space
    glm::vec4 clipSpace = projection * view * glm::vec4(position + delta_position, 1.0);

    position += delta_position;

    transformComponent->SetPosition(position);

    m_EntityHandle->GetComponent<HE::Circle2DColliderComponent>()->SetOrigin({ position.x, position.y });

    if (clipSpace.y > 1.0f)
    {
        OnDestroy();
    }
}