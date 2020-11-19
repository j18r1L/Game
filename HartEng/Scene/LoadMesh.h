#ifndef LOADMESH_H
#define LOADMESH_H

#include "HartEng/Core/pch.h"
#include "HartEng/Scene/Entity.h"
#include "HartEng/Scene/Components/MaterialComponent.h"
#include "HartEng/Scene/Components/MeshComponent.h"

#include <assimp/scene.h>


namespace HE
{
    class LoadMesh
    {
    private:
        static void ProcessNode(Entity* entity, aiNode *node, const aiScene *scene, const std::string& directory, const std::string& filePath);
        static void ProcessMesh(Entity* entity, MeshComponent& meshComponent, aiMesh *mesh, const aiScene *scene, const std::string& directory);
        static void LoadMaterialTextures(Entity* entity, MaterialComponent& materialComponent, aiMaterial *mat, aiTextureType type, const std::string& typeName, const std::string& directory);
    public:
        static bool CreateMesh(Entity* entity, const std::string& filePath);
    };
}


#endif // LOADMESH_H
