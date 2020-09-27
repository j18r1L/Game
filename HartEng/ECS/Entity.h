namespace HE
{
  class Entity
  {
  private:
    Scene* m_SceneHandle;
    std::unordered_map<ComponentType, Component*> m_Components;
    std::string name;
  public:
    Entity(const std::string& object_name):
        name(object_name)
    {
      // m_SceneHandle = SceneManager()->getActiveScene();
    }
    ~Entity() 
    {
      m_SceneHandle->DestroyEntity(name);
    }
    
    bool AddComponent(ComponentType type, Component &component)
    {
      if (HasComponent(type) == false)
      {
          m_Components.push_back(component);
          return true;
      }
      return false;
    }
    
    // Вернуть компонент из текущего геймобжекта
    // API: GraphicsComponent = entity.GetComponent(GraphicsComponent);
    Component& GetComponent(ComponentType type)
    {
      return *(m_Entities.find(type));
    }
    
    // Проверить наличие компонента type в геймобжекте 
    // API: entity.HasComponent(TransformComponent);
    bool HasComponent(ComponentType type)
    {
      return !m_Entities.find(name) == m_Entities.end();
    }
    
    // Удалить компонент type из текущего геймобжекта
    // API: entity.RemoveComponent(TransformComponent);
    void RemoveComponent(ComponentType type)
    {
      m_Components.erase(type);
    }

    void OnUpdate() {
      for (auto &item: m_Components) {
        item.second.OnUpdate();
      }
    }
  };
}