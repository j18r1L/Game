
#include "imGUILayer.h"


namespace HE
{
    ImGuiLayer::ImGuiLayer():
        Layer("ImGUILayer")
    {

    }
    ImGuiLayer::~ImGuiLayer()
    {

    }

    void ImGuiLayer::OnUpdate()
    {


        ImGuiIO& io = ImGui::GetIO();
        Application& app = Application::Get();
        io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());

        float time = (float)glfwGetTime();
        io.DeltaTime = m_Time > 0.0 ? (time - m_Time) : (1.0 / 60.0f);
        m_Time = time;

        ImGui_ImplOpenGL3_NewFrame();
        ImGui::NewFrame();

        static bool show = true;
        ImGui::ShowDemoWindow(&show);

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    }

    void ImGuiLayer::OnAttach()
    {
        // TODO очень временная реализация до появления рендерера
        ImGui::CreateContext();
        ImGui::StyleColorsDark();

        ImGuiIO& io = ImGui::GetIO();
        io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
        io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;

        // TODO убрать GLFW кейкоды, поставить HE_KEY
        // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
        io.KeyMap[ImGuiKey_Tab] = HE_KEY_TAB;
        io.KeyMap[ImGuiKey_LeftArrow] = HE_KEY_LEFT;
        io.KeyMap[ImGuiKey_RightArrow] = HE_KEY_RIGHT;
        io.KeyMap[ImGuiKey_UpArrow] = HE_KEY_UP;
        io.KeyMap[ImGuiKey_DownArrow] = HE_KEY_DOWN;
        io.KeyMap[ImGuiKey_PageUp] = HE_KEY_PAGE_UP;
        io.KeyMap[ImGuiKey_PageDown] = HE_KEY_PAGE_DOWN;
        io.KeyMap[ImGuiKey_Home] = HE_KEY_HOME;
        io.KeyMap[ImGuiKey_End] = HE_KEY_END;
        io.KeyMap[ImGuiKey_Insert] = HE_KEY_INSERT;
        io.KeyMap[ImGuiKey_Delete] = HE_KEY_DELETE;
        io.KeyMap[ImGuiKey_Backspace] = HE_KEY_BACKSPACE;
        io.KeyMap[ImGuiKey_Space] = HE_KEY_SPACE;
        io.KeyMap[ImGuiKey_Enter] = HE_KEY_ENTER;
        io.KeyMap[ImGuiKey_Escape] = HE_KEY_ESCAPE;
        io.KeyMap[ImGuiKey_KeyPadEnter] = HE_KEY_KP_ENTER;
        io.KeyMap[ImGuiKey_A] = HE_KEY_A;
        io.KeyMap[ImGuiKey_C] = HE_KEY_C;
        io.KeyMap[ImGuiKey_V] = HE_KEY_V;
        io.KeyMap[ImGuiKey_X] = HE_KEY_X;
        io.KeyMap[ImGuiKey_Y] = HE_KEY_Y;
        io.KeyMap[ImGuiKey_Z] = HE_KEY_Z;

        ImGui_ImplOpenGL3_Init("#version 410");


    }

    void ImGuiLayer::OnDetach()
    {

    }

    void ImGuiLayer::OnEvent(Event &event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<MouseButtonPressedEvent>(HE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonPressedEvent));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(HE_BIND_EVENT_FN(ImGuiLayer::OnMouseButtonReleasedEvent));
        dispatcher.Dispatch<MouseMovedEvent>(HE_BIND_EVENT_FN(ImGuiLayer::OnMouseMovedEvent));
        dispatcher.Dispatch<MouseScrolledEvent>(HE_BIND_EVENT_FN(ImGuiLayer::OnMouseScrolledEvent));
        dispatcher.Dispatch<KeyPressedEvent>(HE_BIND_EVENT_FN(ImGuiLayer::OnKeyPressedEvent));
        dispatcher.Dispatch<KeyReleasedEvent>(HE_BIND_EVENT_FN(ImGuiLayer::OnKeyReleasedEvent));
        dispatcher.Dispatch<KeyTypedEvent>(HE_BIND_EVENT_FN(ImGuiLayer::OnKeyTypedEvent));
        dispatcher.Dispatch<WindowResizeEvent>(HE_BIND_EVENT_FN(ImGuiLayer::OnWindwResizedEvent));
    }

    bool ImGuiLayer::OnMouseButtonPressedEvent(MouseButtonPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = true;

        //Чтобы другие эвенты могли обработать этот запрос мы возвращаем false
        //В будущем, если будет кнопка и было нажатие, то возвращаем true
        return false;
    }
    bool ImGuiLayer::OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[e.GetMouseButton()] = false;

        return false;
    }
    bool ImGuiLayer::OnMouseMovedEvent(MouseMovedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MousePos = ImVec2(e.GetX(), e.GetY());

        return false;
    }
    bool ImGuiLayer::OnMouseScrolledEvent(MouseScrolledEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseWheel += e.GetXOffset();
        io.MouseWheelH += e.GetYOffset();

        return false;
    }

    bool ImGuiLayer::OnKeyPressedEvent(KeyPressedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.KeysDown[e.GetKeyCode()] = true;

        // TODO Убрать GLFW key-коды, нужно поставить HE_KEY-коды, которые будут platform независимые
        io.KeyCtrl = io.KeysDown[HE_KEY_LEFT_CONTROL] || io.KeysDown[HE_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[HE_KEY_LEFT_SHIFT] || io.KeysDown[HE_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[HE_KEY_LEFT_ALT] || io.KeysDown[HE_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[HE_KEY_LEFT_SUPER] || io.KeysDown[HE_KEY_RIGHT_SUPER];

        return false;
    }
    bool ImGuiLayer::OnKeyReleasedEvent(KeyReleasedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();

        io.KeysDown[e.GetKeyCode()] = false;

        // TODO Убрать GLFW key-коды, нужно поставить HE_KEY-коды, которые будут platform независимые
        io.KeyCtrl = io.KeysDown[HE_KEY_LEFT_CONTROL] || io.KeysDown[HE_KEY_RIGHT_CONTROL];
        io.KeyShift = io.KeysDown[HE_KEY_LEFT_SHIFT] || io.KeysDown[HE_KEY_RIGHT_SHIFT];
        io.KeyAlt = io.KeysDown[HE_KEY_LEFT_ALT] || io.KeysDown[HE_KEY_RIGHT_ALT];
        io.KeySuper = io.KeysDown[HE_KEY_LEFT_SUPER] || io.KeysDown[HE_KEY_RIGHT_SUPER];

        return false;
    }
    bool ImGuiLayer::OnKeyTypedEvent(KeyTypedEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        int keycode = e.GetKeyCode();
        // Смотрим на кейкоды TODO кириллица не работает
        if (keycode > 0 && keycode < 0x10000)
            io.AddInputCharacter((unsigned short)keycode);
    }

    bool ImGuiLayer::OnWindwResizedEvent(WindowResizeEvent& e)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.DisplaySize = ImVec2(e.GetWidth(), e.GetHeight());
        io.DisplayFramebufferScale = ImVec2(1.0f, 1.0f);

        // TODO Убрать glViewPort, тут должен быть platform non specific код
        glViewport(0, 0, e.GetWidth(), e.GetHeight());
        return false;
    }
}
