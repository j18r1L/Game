#ifndef SCENEHIERARCHYPANEL_H
#define SCENEHIERARCHYPANEL_H

#include "HartEng/HartEng.h"

namespace HE
{
    class SceneHierarchyPanel
    {
    private:
        std::shared_ptr<Scene> m_Scene;
        Entity* m_SelectionContext = nullptr;
        std::shared_ptr<ShaderLibrary> m_ShaderLibrary = nullptr;
        std::string m_DeletedEntity = "";
        std::string m_PathToNewMesh = "";
        bool m_CreateChild = false; // Create child for selected entity


        void DrawEntityNode(std::string name, Entity* entity);
        void DrawComponents(Entity* entity);

        void DrawTransform(Entity* entity);
        void DrawCamera(Entity* entity);
        void DrawMaterial(Entity* entity);
        void DrawMesh(Entity* entity);
        void DrawLight(Entity* entity);


    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const std::shared_ptr<Scene>& scene, std::shared_ptr<ShaderLibrary> shaderLibrary = nullptr);

        void SetScene(const std::shared_ptr<Scene>& scene);
        void SetShaderLibrary(std::shared_ptr<ShaderLibrary> shaderLibrary);
        void SetSelectedEntity(Entity* entity);

        Entity* GetSelectedEntity() const;

        void OnImGuiRender();
    };
}


#endif // SCENEHIERARCHYPANEL_H
