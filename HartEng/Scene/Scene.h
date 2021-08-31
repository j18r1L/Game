#pragma once

#include "HartEng/Core/pch.h"
#include "HartEng/Scene/Entity.h"
#include "HartEng/Core/Timestep.h"
#include "HartEng/Renderer/Cameras.h"
#include "HartEng/Renderer/Shader.h"
#include "HartEng/Renderer/Framebuffer.h"

#include "../assets/shaders/common/Structures.h"

namespace HE
{
    class Entity;

    class Scene
    {
    private:

        std::unordered_map<std::string, Entity*> m_Entities;
        std::string m_Name;
        uint32_t m_ObjectsCount;

        LightStruct m_Lights;
        CameraMatrixStruct m_CameraMatrixStruct;

        bool m_Play = false;
        friend class SceneHierarchyPanel;

        void ProcessLights();

    public:
        Scene();
        Scene(const std::string& scene_name);
        Scene(const Scene& other);

        ~Scene() = default;

        Entity* CreateEntity();
        Entity* CreateEntity(const Entity& other);
        Entity* CreateEntity(const std::string& name);

        Entity* GetEntity(const std::string& name);
        Entity* GetEntity(uint32_t entityID);
        Entity GetMainCameraEntity();
        const std::unordered_map<std::string, Entity*>& GetEntities();
        const std::unordered_map<std::string, Entity*>& GetEntities() const;
        const std::string& GetName() const;
        const LightStruct& GetLights() const;

        Entity* FindEntityByTag(const std::string& tag);

        void DestroyEntity(const std::string& name);
        void Clear();

        // Runtime
        void OnRuntimeStart();
        void OnRuntimeStop();

        void OnUpdate(Timestep& ts);
        void OnRenderRuntime(Timestep& ts);
        void OnRenderEditor(Timestep& ts, PerspectiveCamera& camera); // This used only in levelEditor with non-runtime camera
        
        void OnViewportResize(uint32_t width, uint32_t height);

        void RenameEntity(std::string oldName, std::string newName);
    };
}

