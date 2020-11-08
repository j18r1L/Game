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


        void DrawEntityNode(std::string name, Entity* entity);
        void DrawComponents(Entity* entity);


    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const std::shared_ptr<Scene>& scene, std::shared_ptr<ShaderLibrary> shaderLibrary = nullptr);

        void SetScene(const std::shared_ptr<Scene>& scene);
        void SetShaderLibrary(std::shared_ptr<ShaderLibrary> shaderLibrary);

        void OnImGuiRender();
    };
}


#endif // SCENEHIERARCHYPANEL_H
