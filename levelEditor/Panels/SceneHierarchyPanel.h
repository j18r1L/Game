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
        void DrawEntityNode(std::string name, Entity* entity);


    public:
        SceneHierarchyPanel() = default;
        SceneHierarchyPanel(const std::shared_ptr<Scene>& scene);

        void SetScene(const std::shared_ptr<Scene>& scene);

        void OnImGuiRender();
    };
}


#endif // SCENEHIERARCHYPANEL_H
