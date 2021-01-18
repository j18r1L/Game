#ifndef SCENE_H
#define SCENE_H

#include "HartEng/Core/pch.h"
#include "HartEng/Scene/Entity.h"
#include "HartEng/Core/Timestep.h"
#include "HartEng/Renderer/Cameras.h"
#include "HartEng/Renderer/Shader.h"
#include "HartEng/Renderer/Framebuffer.h"

namespace HE
{
    class Entity;

    class Scene
    {
    private:

        std::unordered_map<std::string, Entity*> m_Entities;
        std::string m_Name;
        uint32_t m_ObjectsCount;

        bool m_Play = false;
        friend class SceneHierarchyPanel;

    public:
        Scene();
        Scene(const std::string& scene_name);

        ~Scene() = default;

        Entity* CreateEntity();
        Entity* CreateEntity(const std::string& name);

        Entity* GetEntity(const std::string& name);
        Entity* GetEntity(uint32_t entityID);
        const std::unordered_map<std::string, Entity*>& GetEntities();
        const std::string& GetName() const;

        void DestroyEntity(const std::string& name);
        void Clear();

        void OnScenePlay();
        void OnSceneStop();
        void OnUpdate(Timestep& ts);
        void OnRenderRuntime(Timestep& ts);
        void OnRenderEditor(Timestep& ts, PerspectiveCamera& camera); // This used only in levelEditor with non-runtime camera
        void OnRenderShader(std::shared_ptr<Shader> shader, PerspectiveCamera& camera); // Render scene with given shader 

        void OnViewportResize(uint32_t width, uint32_t height);

        void RenameEntity(std::string oldName, std::string newName);
    };
}

#endif // SCENE_H
