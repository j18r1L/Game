#include "HartEng/ECS/Scene.h"

#include "HartEng/ECS/Components/TransformComponent.h"
#include "HartEng/ECS/Components/MaterialComponent.h"
#include "HartEng/ECS/Components/MeshComponent.h"
#include "HartEng/ECS/Components/SubMeshComponent.h"
#include "HartEng/ECS/Components/Texture2DComponent.h"
#include "HartEng/Renderer/Renderer.h"

namespace HE
{
    Scene::Scene(const std::string& scene_name):
        m_Entities(),
        name(scene_name),
        objectsCount(0)

    {

    }


    Entity* Scene::CreateEntity()
    {
        std::string entityName = "__obj_" + name + "_" + std::to_string(objectsCount);
        return CreateEntity(entityName);
    }

    Entity* Scene::CreateEntity(const std::string& name)
    {
        float size = m_Entities.max_size();
        HE_CORE_INFO("{0}", size);
        if (objectsCount == m_Entities.max_size())
        {
            HE_CORE_WARN("Max scene capacity is reached!");
            return nullptr;
        }

        // Нужно проверить стоит ли делать такую проверку (имеется ли такой геймобжект в m_Entities)
        if (m_Entities.find(name) == m_Entities.end())
        {
            Entity* entity = new Entity(this, name);
            m_Entities[name] = entity;

            // Добавляем компененты, которые всегда должны быть в entity, например TransformComponent
            //entity->AddComponent<TransformComponent>();

            Component* transformComponent = new TransformComponent(entity);
            entity->AddComponent(ComponentType::TransformComponent, *transformComponent);
            objectsCount++;

            return entity;
        }
        HE_CORE_WARN("Entity with name: {0} already exists!", name);
        return nullptr;
    }

    Entity* Scene::getEntity(const std::string& name)
    {
        std::unordered_map<std::string, Entity*>::const_iterator entity = m_Entities.find(name);
        if (entity == m_Entities.end())
        {
            HE_CORE_ASSERT(false, "There is no entity with name: " + name);
            return nullptr;
        }
        return entity->second;
    }

    void Scene::DestroyEntity(const std::string& name)
    {
        m_Entities.erase(name);
    }

    void Scene::OnUpdate(Timestep& ts)
    {
        for (auto& [name, entity]: m_Entities)
        {
            // quad with texture
            MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(entity->GetComponent(ComponentType::MeshComponent));
            TransformComponent* transformComponent = dynamic_cast<TransformComponent*>(entity->GetComponent(ComponentType::TransformComponent));
            auto squareSubMeshes = meshComponent->GetSubMeshes();
            for (auto& subMesh: squareSubMeshes)
            {
                auto material = subMesh->GetMaterial();
                auto shader = subMesh->GetShader();
                shader->Bind();
                auto attribute = subMesh->GetAttribute();
                for (auto& [name, VAO]: attribute)
                {
                    Renderer::Submit(shader, VAO, transformComponent->GetTransform(), material);
                }
            }
            // TODO
            // Update transform / physics first...

            // Then update graphics...

            //entity->OnUpdate();
        }
    }
}
