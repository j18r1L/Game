#include "Renderer.h"
#include "Shader.h"
#include "GameObject.h"
#include "ObjectFabric.h"

void addShaders(Renderer& renderer);
void error_callback(int error, const char* description);

int main(int argc, char *argv[])
{

    // TODO load init file
    VideoSettings* videoSettings = VideoSettings::getInstance();
    videoSettings->setWidthHeight(1920, 1080);

    // Create Renderer instance
    Renderer renderer(4, 1, *videoSettings);
    glfwSetErrorCallback(error_callback);

    // Добавляем шейдеры
    addShaders(renderer);

    // Создаем game object-ы
    //TODO можно сделать более фабричный метод
    ObjectFabric objectFabric;
    GameObject* player = objectFabric.createPlayer("../data/planet/planet.obj");
    GameObject* sun = objectFabric.createModel("../data/planet/planet.obj");
    std::vector<GameObject*> gameObjects;
    gameObjects.push_back(player);
    gameObjects.push_back(sun);

    // Временно
    GLfloat deltaTime = 0.0f;
    GLfloat lastFrame = 0.0f;

    // Устанавливаем обработку ввода (player)
    renderer.m_Window.setKeyCallback(player->getInputComponent()->key_callback);
    renderer.m_Window.setCursorPosCallback(player->getInputComponent()->mouse_callback);
    renderer.m_Window.setScrollCallback(player->getInputComponent()->scroll_callback);


    glfwSwapInterval(0);


    // Начинаем GameLoop
    while (!glfwWindowShouldClose(renderer.m_Window.getWindow()))
    {
        glfwPollEvents();
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


        for (int i = 0; i < gameObjects.size(); i++)
        {
            gameObjects[i]->update(renderer, deltaTime);
        }

        glfwSwapBuffers(renderer.m_Window.getWindow());
    }
    glfwTerminate();
    return 0;
}

void addShaders(Renderer& renderer)
{
    // Добавляем шейдеры
    renderer.addShader("../shaders/Model.vert", "../shaders/Model.frag");
    renderer.addShader("../shaders/Editor.vert", "../shaders/Editor.frag");
}

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}


