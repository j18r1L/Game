#include "Entity.h"

namespace HE
{
  class Scene
  {
  private:
      std::unordered_map<std::string, Entity*> m_Entities;
      std::string name;
      uint objectsCount;
  public:
      Scene(const std::string& scene_name):
        objectsCount(0), name(scene_name)
      {}
      ~Scene() = default;

      Entity* CreateEntity()
      {
        std::string entityName = "__obj_" + name + "_" + std::to_string(objectsCount);
        return CreateEntity(entityName);
      }
  
      Entity* CreateEntity(const std::string& name)
      {
          if (objectsCount == std::unordered_map.max_size())
          {
            std::cout << " Достигнуто максимальное количество объектов сцены" << std::endl;
            return nullptr;
          }
          // Нужно проверить стоит ли делать такую проверку (имеется ли такой геймобжект в m_Entities)
          if (m_Entities.find(name) == m_Entities.end())
          {
            Entity* entity = new Entity;
            m_Entities.push_back(entity);
            // Добавляем компененты, которые всегда должны быть в entity, например TransformComponent
            entity->AddComponent<TransformComponent>();
            objectsCount++;
            return entity;
          }
          std::cout << " Такой entity уже существует" << std::endl;
          return nullptr;
      }

      void DestroyEntity(const std::string& name)
      {
        m_Entities.erase(name);
      }
  
      void OnUpdate()
      {
          for (auto& [name, entity]: m_Entities)
          {
              entity->OnUpdate();
          }
      }
  };
}