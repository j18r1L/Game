#include <HartEng.h>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
#include <glm/ext/matrix_clip_space.hpp> // glm::perspective
#include <glm/ext/scalar_constants.hpp> // glm::pi

glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
    glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
    glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
    View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
    View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
    return Projection * View * Model;
}

class ExampleLayer: public HE::Layer
{
public:
    ExampleLayer():
        Layer("Example")
    {
        float a = 5.0f;
        sizeof(a) * 3;
        cout << sizeof(camera(5.0f, {0.5f, 0.5f}));

    }

    void OnUpdate() override
    {
        //HE_INFO("ExampleLayer::Update");

        if (HE::Input::IsKeyPressed(HE_KEY_G))
            HE_TRACE("G key is pressed!");
    }

    void OnEvent(HE::Event &event) override
    {
        HE_TRACE("{0}", event);
    }

};

class SandBox: public HE::Application
{
public:
    SandBox()
    {
        PushLayer(new ExampleLayer());
        PushOverlay(new HE::ImGuiLayer());
    }

    ~SandBox() {}
};


HE::Application* HE::CreateApplication()
{
    return new SandBox();
}


/*
m for members
c for constants/readonlys
p for pointer (and pp for pointer to pointer)
v for volatile
s for static
i for indexes and iterators
e for events
*/
