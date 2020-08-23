#include <HartEng.h>

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

// Entry point
#include "HartEng/EntryPoint.h"
#include "TestLayer.h"


class ExampleLayer: public HE::Layer
{
private:


public:
    ExampleLayer():
        Layer("Example")
    {
    }

    void OnUpdate(HE::Timestep& ts) override
    {


    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Setting");

        //ImGui::InputFloat("input scientific", &m_CameraPosition.z, 0.0, 0.0, "%e");
        //ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));

        ImGui::End();
    }

    void OnEvent(HE::Event &e) override
    {

    }

    bool OnKeyPressedEvent(HE::KeyPressedEvent& e)
    {
        return false;
    }

};

class SandBox: public HE::Application
{
public:
    SandBox()
    {

        PushLayer(new TestLayer());
        //PushLayer(new ExampleLayer());
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
