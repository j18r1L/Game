#include <HartEng.h>

class ExampleLayer: public HE::Layer
{
public:
    ExampleLayer():
        Layer("Example")
    {}

    void OnUpdate() override
    {
        HE_INFO("ExampleLayer::Update");
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
