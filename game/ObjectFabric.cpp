#include "ObjectFabric.h"

ObjectFabric::ObjectFabric()
{
    createXYZ();

}

void ObjectFabric::createXYZ()
{
    float vertices[] = {
        // back face
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
        -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
        // front face
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
        -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
        -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
        // left face
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        -1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
        -1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
        // right face
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
         1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
         1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left
        // bottom face
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
         1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
         1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
        -1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
        -1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
        // top face
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
         1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
         1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right
         1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
        -1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
        -1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left
    };
    // Создаем VAO для xyz стрелок в editor-е
    GLuint VBO;
    glGenVertexArrays(1, &m_xyzVAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindVertexArray(m_xyzVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glDeleteBuffers(1, &VBO);

}

GameObject* ObjectFabric::createPlayer(std::string path)
{
    // Проверяем создана ли уже модель
    GLint i = checkCreatedModels(path);
    PlayerModelComponent *modelComponent;
    if (i == -1)
    {
        Model model(path);
        m_Models.push_back(model);
        m_ModelsPath.push_back(path);
        modelComponent = new PlayerModelComponent(&m_Models.back());
    }
    else
        modelComponent = new PlayerModelComponent(&m_Models[i]);

    // Создаем остальные компоненты

    // Созадем компонент камеры
    PlayerCameraComponent *cameraComponent = new PlayerCameraComponent();
    // Созадем компонент физики
    PlayerPhysicsComponent *physicsComponent = new PlayerPhysicsComponent();
    // Созадем компонент ввода
    PlayerInputComponent *inputComponent = new PlayerInputComponent(cameraComponent);

    //Создаем компонент графики
    PlayerGraphicsComponent *graphicsComponent = new PlayerGraphicsComponent(modelComponent, cameraComponent, physicsComponent);

    // Сохраняем общие для нескольких компонентов компоненты
    m_CameraComponent = cameraComponent;
    m_PhysicsComponent = physicsComponent;

    // Создаем объект
    return new GameObject(inputComponent, graphicsComponent, modelComponent, cameraComponent, physicsComponent);
}

GameObject* ObjectFabric::createModel(std::string path)
{
    // Проверяем создана ли уже модель
    GLint i = checkCreatedModels(path);
    PlayerModelComponent *modelComponent;
    if (i == -1)
    {
        Model model(path);
        m_Models.push_back(model);
        m_ModelsPath.push_back(path);
        modelComponent = new PlayerModelComponent(&m_Models.back());
    }
    else
        modelComponent = new PlayerModelComponent(&m_Models[i]);

    // Создаем остальные компоненты

    // Созадем компонент физики
    ModelPhysicsComponent *physicsComponent = new ModelPhysicsComponent();

    //Создаем компонент графики
    ModelGraphicsComponent *graphicsComponent = new ModelGraphicsComponent(modelComponent, m_CameraComponent, physicsComponent);

    // Создаем объект
    return new GameObject(nullptr, graphicsComponent, modelComponent, nullptr, physicsComponent);
}

GameObject* ObjectFabric::createModelEditor(std::string path)
{
    // Проверяем создана ли уже модель
    GLint i = checkCreatedModels(path);
    EditorModelComponent *modelComponent;
    if (i == -1)
    {
        Model model(path);
        m_Models.push_back(model);
        m_ModelsPath.push_back(path);
        modelComponent = new EditorModelComponent(&m_Models.back(), &m_xyzVAO);
    }
    else
        modelComponent = new EditorModelComponent(&m_Models[i], &m_xyzVAO);

    // Создаем остальные компоненты

    // Созадем компонент физики
    ModelPhysicsComponent *physicsComponent = new ModelPhysicsComponent();

    //Создаем компонент графики
    EditorGraphicsComponent *graphicsComponent = new EditorGraphicsComponent(modelComponent, m_CameraComponent, physicsComponent);

    // Создаем объект
    return new GameObject(nullptr, graphicsComponent, modelComponent, nullptr, physicsComponent);
}

GLint ObjectFabric::checkCreatedModels(std::string path) const
{
    GLuint size = m_ModelsPath.size();
    GLint i = 0;
    bool find = false;
    for (i = 0; i < size && find == false; ++i)
    {
        if (m_ModelsPath[i] == path)
            find = true;
    }
    if (find == false)
        i = 0;
    return i - 1;
}
