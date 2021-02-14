#include "ObstacleScript.h"

ObstacleScript::ObstacleScript(HE::Entity* entity) :
    ScriptComponent(entity)
{

}

void ObstacleScript::OnCreate()
{
    m_EntityHandle->AddComponent<HE::TagComponent>()->SetTag("obstacle");

    auto transformComponent = m_EntityHandle->GetComponent<HE::TransformComponent>();

    // Get view and projection matrices
    auto cameraEntity = m_EntityHandle->GetScene()->GetEntity("Camera");
    glm::mat4 projection = cameraEntity->GetComponent<HE::CameraComponent>()->GetCamera().GetProjection();
    glm::mat4 view = glm::inverse(cameraEntity->GetComponent<HE::TransformComponent>()->GetTransform());

    glm::vec4 position = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) * glm::inverse(view) * glm::inverse(projection);
    transformComponent->SetPosition({ position .x, position .y, position .z});

    auto collider = m_EntityHandle->AddComponent<HE::Circle2DColliderComponent>();
    collider->SetRadius(0.1f);
    collider->SetOrigin({ position.x, position.y });

}

void ObstacleScript::OnUpdate(HE::Timestep& timestep)
{
    auto transformComponent = m_EntityHandle->GetComponent<HE::TransformComponent>();
    glm::vec3 position = transformComponent->GetPosition();
    glm::vec3 rotation = glm::degrees(transformComponent->GetRotation());

    glm::vec3 delta_position{ 0.0f };
    glm::vec3 delta_rotation{ 0.0f };

    delta_position.y -= m_VerticalSpeed * timestep;

    if (m_RotateSide)
        delta_rotation.y += m_RotationSpeed * timestep;
    else
        delta_rotation.y -= m_RotationSpeed * timestep;




    // Get view and projection matrices
    auto cameraEntity = m_EntityHandle->GetScene()->GetEntity("Camera");
    glm::mat4 projection = cameraEntity->GetComponent<HE::CameraComponent>()->GetCamera().GetProjection();
    glm::mat4 view = glm::inverse(cameraEntity->GetComponent<HE::TransformComponent>()->GetTransform());

    // Calculate position in Clip space
    glm::vec4 clipSpace = projection * view * glm::vec4(position + delta_position, 1.0);

    if (clipSpace.y < -1.0f)
    {
        position.y = (glm::vec4(1.0f) * glm::inverse(view) * glm::inverse(projection)).y;
        delta_position.y = 0.0f;
        m_RotateSide = !m_RotateSide;
    }

    if (clipSpace.x < -1.0f)
    {
        delta_position.x = 0.0f;
    }
    if (clipSpace.x > 1.0f)
    {
        delta_position.x = 0.0f;
    }

    position += delta_position;
    rotation += delta_rotation;

    transformComponent->SetPosition(position);
    transformComponent->SetRotation(rotation);

    m_EntityHandle->GetComponent<HE::Circle2DColliderComponent>()->SetOrigin({ position.x, position.y });

}

void ObstacleScript::OnDestroy()
{
    // Get view and projection matrices
    auto cameraEntity = m_EntityHandle->GetScene()->GetEntity("Camera");
    glm::mat4 projection = cameraEntity->GetComponent<HE::CameraComponent>()->GetCamera().GetProjection();
    glm::mat4 view = glm::inverse(cameraEntity->GetComponent<HE::TransformComponent>()->GetTransform());
    float position_y = (glm::vec4(1.0f) * glm::inverse(view) * glm::inverse(projection)).y;


    auto transformComponent = m_EntityHandle->GetComponent<HE::TransformComponent>();
    transformComponent->SetPosition({ 0.0f, position_y, 0.0f });
}

void ObstacleScript::OnCollision(HE::Entity* entity)
{
    if (entity->GetComponent<HE::TagComponent>()->GetTag() == "bullet")
    {
        OnDestroy();
    }
}