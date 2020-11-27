// https://github.com/CedricGuillemet/ImGuizmo
// v 1.61 WIP
//
// The MIT License(MIT)
// 
// Copyright(c) 2016 Cedric Guillemet
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
// 
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// -------------------------------------------------------------------------------------------
#pragma once

#include <glm/glm.hpp>

namespace ImGuizmo
{
   // call inside your own window and before Manipulate() in order to draw gizmo to that window.
   // Or pass a specific ImDrawList to draw to (e.g. ImGui::GetForegroundDrawList()).
   IMGUI_API void SetDrawlist(ImDrawList* drawlist = nullptr);

   // call BeginFrame right after ImGui_XXXX_NewFrame();
   IMGUI_API void BeginFrame();

   // return true if mouse cursor is over any gizmo control (axis, plan or screen component)
   IMGUI_API bool IsOver();

   // return true if mouse IsOver or if the gizmo is in moving state
   IMGUI_API bool IsUsing();

   // enable/disable the gizmo. Stay in the state until next call to Enable.
   // gizmo is rendered with gray half transparent color when disabled
   IMGUI_API void Enable(bool enable);

   // helper functions for manualy editing translation/rotation/scale with an input float
   // translation, rotation and scale float points to 3 floats each
   // Angles are in degrees (more suitable for human editing)
   // example:
   // float matrixTranslation[3], matrixRotation[3], matrixScale[3];
   // ImGuizmo::DecomposeMatrixToComponents(gizmoMatrix.m16, matrixTranslation, matrixRotation, matrixScale);
   // ImGui::InputFloat3("Tr", matrixTranslation, 3);
   // ImGui::InputFloat3("Rt", matrixRotation, 3);
   // ImGui::InputFloat3("Sc", matrixScale, 3);
   // ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, gizmoMatrix.m16);
   //
   // These functions have some numerical stability issues for now. Use with caution.
   IMGUI_API void DecomposeMatrixToComponents(const glm::mat4& tramsformMatrix, glm::vec3& translation, glm::vec3& rotation, glm::vec3& scale);
   IMGUI_API void RecomposeMatrixFromComponents(const float* translation, const float* rotation, const float* scale, float* matrix);

   IMGUI_API void SetRect(float x, float y, float width, float height);
   // default is false
   IMGUI_API void SetOrthographic(bool isOrthographic);

   // Render a cube with face color corresponding to face normal. Usefull for debug/tests
   IMGUI_API void DrawCubes(const float* view, const float* projection, const float* matrices, int matrixCount);
   IMGUI_API void DrawGrid(const float* view, const float* projection, const float* matrix, const float gridSize);

   // call it when you want a gizmo
   // Needs view and projection matrices. 
   // matrix parameter is the source matrix (where will be gizmo be drawn) and might be transformed by the function. Return deltaMatrix is optional
   // translation is applied in world space
   enum OPERATION
   {
      TRANSLATE,
      ROTATE,
      SCALE,
      BOUNDS,
   };

   enum MODE
   {
      LOCAL,
      WORLD
   };

   IMGUI_API bool Manipulate(const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, OPERATION operation, MODE mode, glm::mat4& transformMatrix, float* deltaMatrix = NULL, const float* snap = NULL, const float* localBounds = NULL, const float* boundsSnap = NULL);
   //
   // Please note that this cubeview is patented by Autodesk : https://patents.google.com/patent/US7782319B2/en
   // It seems to be a defensive patent in the US. I don't think it will bring troubles using it as
   // other software are using the same mechanics. But just in case, you are now warned!
   //
   IMGUI_API void ViewManipulate(float* view, float length, ImVec2 position, ImVec2 size, ImU32 backgroundColor);

   IMGUI_API void SetID(int id);

   // return true if the cursor is over the operation's gizmo
   IMGUI_API bool IsOver(OPERATION op);
   IMGUI_API void SetGizmoSizeClipSpace(float value);
};
