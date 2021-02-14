#include "PlayerControllerScript.h"



PlayerControllerScript::PlayerControllerScript(HE::Entity* entity) :
    ScriptComponent(entity),
    m_BulletMesh(new HE::Mesh(std::string(CMAKE_PATH) + "/assets/meshes/obj/quad.obj"))
{
}

void PlayerControllerScript::OnCreate()
{
    m_EntityHandle->AddComponent<HE::TagComponent>()->SetTag("ship");
    auto collider = m_EntityHandle->AddComponent<HE::Circle2DColliderComponent>();
    collider->SetRadius(0.1f);
    collider->SetOrigin({ m_EntityHandle->GetComponent<HE::TransformComponent>()->GetPosition().x, m_EntityHandle->GetComponent<HE::TransformComponent>()->GetPosition().y });
}

void PlayerControllerScript::OnUpdate(HE::Timestep& timestep)
{
	UpdateMovement(timestep);
	UpdateShooting(timestep);
    
}

void PlayerControllerScript::OnDestroy()
{
	auto transformComponent = m_EntityHandle->GetComponent<HE::TransformComponent>();
	transformComponent->SetPosition({ 0.0f, 0.0f, 0.0f });
}

void PlayerControllerScript::OnCollision(HE::Entity* entity)
{
    // if collided entity is obstacle -> gameover
    if (entity->GetComponent<HE::TagComponent>()->GetTag() == "obstacle")
    {
        HE_INFO("Game Over");
    }
}

void PlayerControllerScript::UpdateShooting(HE::Timestep& timestep)
{
	auto transformComponent = m_EntityHandle->GetComponent<HE::TransformComponent>();

	// If left mouse button is pressed then shoot a bullet from current position
	if (HE::Input::IsMouseButtonPressed(HE_MOUSE_BUTTON_1))
	{
        // If timer > m_BulletTime then reset timer and shoot a bullet
        if (m_BulletTimer.ElapsedMillis() > m_BulletTime)
        {
            // Start new timer
            m_BulletTimer.Reset();

            // Create bullet entity
            auto bulletEntity = m_EntityHandle->GetScene()->CreateEntity();
            auto bulletTransformComponent = bulletEntity->GetComponent<HE::TransformComponent>();

            bulletTransformComponent->SetPosition(transformComponent->GetPosition());
            bulletTransformComponent->SetScale({ 0.05f, 0.05f, 0.05f });

            bulletEntity->AddComponent<HE::MeshComponent>()->SetMesh(m_BulletMesh);
            auto script = new BulletScript(bulletEntity);
            bulletEntity->AddComponent<HE::ScriptComponent>(script);
            script->OnCreate();
        }
	}
}

void PlayerControllerScript::UpdateMovement(HE::Timestep& timestep)
{
    auto transformComponent = m_EntityHandle->GetComponent<HE::TransformComponent>();

    glm::vec3 delta_translation{ 0.0f };
    glm::vec3 delta_rotation{ 0.0f };
    if (HE::Input::IsKeyPressed(HE_KEY_D))
    {
        delta_translation.x += m_HorizontalSpeed;
        m_Updated = true;
    }
    if (HE::Input::IsKeyPressed(HE_KEY_A))
    {
        delta_translation.x -= m_HorizontalSpeed;
        m_Updated = true;
    }
    if (HE::Input::IsKeyPressed(HE_KEY_W))
    {
        delta_translation.y += m_VerticalSpeed;
        m_Updated = true;
    }
    if (HE::Input::IsKeyPressed(HE_KEY_S))
    {
        delta_translation.y -= m_VerticalSpeed;
        m_Updated = true;
    }

    delta_translation *= timestep;

    if (m_Updated)
    {
        glm::vec3 position = transformComponent->GetPosition();

        // Get view and projection matrices
        auto cameraEntity = m_EntityHandle->GetScene()->GetEntity("Camera");
        glm::mat4 projection = cameraEntity->GetComponent<HE::CameraComponent>()->GetCamera().GetProjection();
        glm::mat4 view = glm::inverse(cameraEntity->GetComponent<HE::TransformComponent>()->GetTransform());

        // Calculate position in Clip space
        glm::vec4 clipSpace = projection * view * glm::vec4(position + delta_translation, 1.0);

        if (clipSpace.y > 1.0f && delta_translation.y > 0.0f)
        {
            delta_translation.y = 0.0f;
        }
        if (clipSpace.y < -1.0f && delta_translation.y < 0.0f)
        {
            delta_translation.y = 0.0f;
        }
        if (clipSpace.x > 1.0f && delta_translation.x > 0.0f)
        {
            delta_translation.x = 0.0f;
        }
        if (clipSpace.x < -1.0f && delta_translation.x < 0.0f)
        {
            delta_translation.x = 0.0f;
        }

        position += delta_translation;

        transformComponent->SetPosition(position);

        m_EntityHandle->GetComponent<HE::Circle2DColliderComponent>()->SetOrigin({ position.x, position.y });

        m_Updated = false;
    }
}
