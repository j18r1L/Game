#include "Gizmo.h"
#include "ImGuizmo.h"
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/euler_angles.hpp>

#define GLM_ENABLE_EXPERIMENTAL

namespace HE
{
    void Gizmo::Draw(Entity* entity)
    {
        TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
        glm::mat4 viewMatrix = m_CameraController->GetCamera().GetView();
        glm::mat4 projectionMatrix = m_CameraController->GetCamera().GetProjection();
        glm::mat4 transformMatrix = transformComponent->GetTransform();

        
        static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::ROTATE);
        static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::WORLD);
        static float snap = 1.0f;
        static bool useSnap = false;

        

        ImGuizmo::SetDrawlist();
        ImVec2 windowPosition = ImGui::GetWindowPos();
        ImGuizmo::SetRect(windowPosition.x, windowPosition.y, m_CameraController->GetWidth(), m_CameraController->GetHeight());
        if (ImGuizmo::Manipulate(viewMatrix, projectionMatrix, mCurrentGizmoOperation, mCurrentGizmoMode, transformMatrix, NULL, useSnap ? &snap : NULL))
        {
            glm::vec3 position(0.0f);
            glm::vec3 rotation(0.0f);
            glm::vec3 scale(1.0f);
            ImGuizmo::DecomposeMatrixToComponents(transformMatrix, position, rotation, scale);

            glm::vec3 newRotation = transformComponent->GetRotation();
            glm::vec3 deltaRotation = rotation - newRotation;
            newRotation += deltaRotation;

            std::swap(newRotation.y, newRotation.z); // TODO fix gizmo & UI 
            transformComponent->SetPRS(position, newRotation, scale);
            transformComponent->SetTransform(transformMatrix);
        }

        ImGui::Begin("Gizmo");


        if (Input::IsKeyPressed(HE_KEY_T) && Input::IsKeyPressed(HE_KEY_LEFT_SHIFT))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        if (Input::IsKeyPressed(HE_KEY_R) && Input::IsKeyPressed(HE_KEY_LEFT_SHIFT))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        if (Input::IsKeyPressed(HE_KEY_S) && Input::IsKeyPressed(HE_KEY_LEFT_SHIFT))
            mCurrentGizmoOperation = ImGuizmo::SCALE;

        if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
            mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
            mCurrentGizmoOperation = ImGuizmo::ROTATE;
        ImGui::SameLine();
        if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
            mCurrentGizmoOperation = ImGuizmo::SCALE;

        if (mCurrentGizmoOperation != ImGuizmo::SCALE)
        {
            if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
                mCurrentGizmoMode = ImGuizmo::LOCAL;
            ImGui::SameLine();
            if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
                mCurrentGizmoMode = ImGuizmo::WORLD;
        }

        ImGui::Checkbox("Snap", &useSnap);
        ImGui::SameLine();

        switch (mCurrentGizmoOperation)
        {
        case ImGuizmo::TRANSLATE:
            ImGui::InputFloat3("Snap", &snap);
            break;
        case ImGuizmo::ROTATE:
            ImGui::InputFloat("Angle Snap", &snap);
            break;
        case ImGuizmo::SCALE:
            ImGui::InputFloat("Scale Snap", &snap);
            break;
        }

        ImGui::End();

        
    } 

    void Gizmo::SetCamera(PerspectiveCameraController* cameraController)
    {
        m_CameraController = cameraController;
    }

    bool Gizmo::IsUsing() const
    {
        return ImGuizmo::IsUsing();
    }

    bool Gizmo::IsOver() const
    {
        return ImGuizmo::IsOver();
    }

}