namespace HE
{
  enum ComponentType
  {
    UndefinedComponent,
    TransformComponent,
    GraphicsComponent,
  };

  class Component
  {
  protected:
      Entity* m_EntityHandle;
      ComponentType type;
  public:
      virtual ~Component()
      {
        m_EntityHandle->RemoveComponent();
      }
      virtual void OnUpdate() = 0;
      ComponentType getType()
      {
        return type;
      }
  };
  
  class TransformComponent: public Component
  {
  private:
    glm::mat4 Transform(1.0f);
  public:
    TransformComponent():
      type(ComponentType) {}
    TransformComponent(const glm::mat4& transform):
      type(ComponentType), Transform(transform) {}
    const glm::mat4& GetTransform();
    
    // хз нужен ли здесь OnUpdate, так как просто хранятся данные
    void OnUpdate() override;
  }

  class GraphicsComponent: public Component
  {
  private:
    glm::vec4 Color(1.0f);
  public:
    GraphicsComponent():
      type(GraphicsComponent) {}
    GraphicsComponent(const glm::vec4& color):
      GraphicsComponent, Color(color) {}
      
    const glm::vec4& GetColor();
    
    void OnUpdate() override
    {
        std::cout << "Graphics: " << Color.r << ", " << Color.g << ", " << Color.b << std::endl;
        // Допустим нужно узнать X из Transform компонента:
        float x = m_EntityHandle->GetComponent(TransformComponent).GetTransform()[0][0];
        //std::cout << "transform_x from Graphics: " << x << std::endl;
    }
  }
}