#include "LoadMesh.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include "HartEng/Scene/Components/MaterialComponent.h"
#include "HartEng/Scene/Components/Texture2DComponent.h"
#include "HartEng/Scene/Components/SubMeshComponent.h"
#include "HartEng/Renderer/VertexArray.h"

namespace HE
{
    struct Vertex
    {
        // position
        glm::vec3 Position;
        // normal
        glm::vec3 Normal;
        // texCoords
        glm::vec2 TexCoords;
        // tangent
        glm::vec3 Tangent;
        // bitangent
        glm::vec3 Bitangent;
    };


    void LoadMesh::CreateMesh(Entity* entity, const std::string& filePath)
    {
        HE_PROFILE_FUNCTION();

        // read file via ASSIMP
        Assimp::Importer importer;

        const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            HE_CORE_ERROR("Cant load mesh using assimp: %s", importer.GetErrorString());
            return;
        }
        // retrieve the directory path of the filepath
        std::string directory = filePath.substr(0, filePath.find_last_of('/'));
        //std::string name = filePath.substr(filePath.find_last_of('/'), filePath.size());

        // process ASSIMP's root node recursively
        ProcessNode(entity, scene->mRootNode, scene, directory);
    }

    void LoadMesh::ProcessNode(Entity* entity, aiNode *node, const aiScene *scene, const std::string& directory)
    {
        HE_PROFILE_FUNCTION();

        // process each mesh located at the current node
        for(unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene.
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(entity->AddComponent(ComponentType::MeshComponent));
            ProcessMesh(entity, *meshComponent, mesh, scene, directory);
        }
        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for(unsigned int i = 0; i < node->mNumChildren; i++)
        {
            ProcessNode(entity, node->mChildren[i], scene, directory);
        }
    }

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void LoadMesh::ProcessMesh(Entity* entity, MeshComponent& meshComponent, aiMesh *mesh, const aiScene *scene, const std::string& directory)
    {
        HE_PROFILE_FUNCTION();

        // data to fill
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        // walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            vertex.Position = vector;
            // normals
            if (mesh->HasNormals())
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;
            }
            // texture coordinates
            if(mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't
                // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                vec.x = mesh->mTextureCoords[0][i].x;
                vec.y = mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
                // tangent
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                vertex.Tangent = vector;
                // bitangent
                vector.x = mesh->mBitangents[i].x;
                vector.y = mesh->mBitangents[i].y;
                vector.z = mesh->mBitangents[i].z;
                vertex.Bitangent = vector;
            }
            else
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);

            vertices.push_back(vertex);
        }
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
                indices.push_back(face.mIndices[j]);
        }
        // process materials
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // 1. diffuse maps
        MaterialComponent* materialComponent = dynamic_cast<MaterialComponent*>(entity->AddComponent(ComponentType::MaterialComponent));
        LoadMaterialTextures(entity, *materialComponent, material, aiTextureType_DIFFUSE, "texture_diffuse", directory);
        // 2. specular maps
        LoadMaterialTextures(entity, *materialComponent, material, aiTextureType_SPECULAR, "texture_specular", directory);
        // 3. normal maps
        LoadMaterialTextures(entity, *materialComponent, material, aiTextureType_HEIGHT, "texture_normal", directory);
        // 4. height maps
        LoadMaterialTextures(entity, *materialComponent, material, aiTextureType_AMBIENT, "texture_height", directory);


        // return a SubMesh object created from the extracted mesh data
        SubMeshComponent* subMeshComponent = dynamic_cast<SubMeshComponent*>(entity->AddComponent(ComponentType::SubMeshComponent));

        // Create vertexArray
        std::shared_ptr<VertexArray> vertexArray = VertexArray::Create();

        // Create vertexBuffer
        std::shared_ptr<VertexBuffer> vertexBuffer = VertexBuffer::Create(&vertices[0], sizeof(Vertex) * vertices.size());
        vertexBuffer->Bind();
        vertexBuffer->SetLayout({
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float3, "a_Normal"},
            {ShaderDataType::Float2, "a_TexCoords"},
            {ShaderDataType::Float3, "a_Tangent"},
            {ShaderDataType::Float3, "a_Bitangent"},
        });
        vertexArray->AddVertexBuffer(vertexBuffer);

        // Create IndexBuffer
        std::shared_ptr<IndexBuffer> indexBuffer;
        indexBuffer = IndexBuffer::Create(&indices[0], indices.size());
        vertexArray->SetIndexBuffer(indexBuffer);

        static uint32_t index = 0;

        subMeshComponent->SetAttribute(vertexArray);
        subMeshComponent->SetName("VertexArray " + std::to_string(index));
        index++;
        subMeshComponent->SetMaterial(materialComponent);
        meshComponent.AddSubMesh(*subMeshComponent);
    }

    void LoadMesh::LoadMaterialTextures(Entity* entity, MaterialComponent& materialComponent, aiMaterial *mat, aiTextureType type, const std::string& typeName, const std::string& directory)
    {
        HE_PROFILE_FUNCTION();

        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);

            std::string filename = directory + '/' + str.C_Str();
            materialComponent.AddTexture(str.C_Str(), filename);
        }
    }
}
