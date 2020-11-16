#include "HartEng/Scene/SceneSerializer.h"


#include <fstream>
#include "HartEng/Core/Log.h"
#include "HartEng/Scene/LoadMesh.h"
#include "HartEng/Scene/Components/TransformComponent.h"
#include "HartEng/Scene/Components/MaterialComponent.h"
#include "HartEng/Scene/Components/Texture2DComponent.h"
#include "HartEng/Scene/Components/MeshComponent.h"
#include "HartEng/Scene/Components/SubMeshComponent.h"
#include "HartEng/Scene/Components/CameraComponent.h"

namespace YAML
{
    template<>
    struct convert<glm::vec3>
    {
        static Node encode(const glm::vec3& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec3& rhs)
        {
            if (!node.IsSequence() || node.size() != 3)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            return true;
        }
    };

    template<>
    struct convert<glm::vec4>
    {
        static Node encode(const glm::vec4& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::vec4& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
    template<>
    struct convert<glm::quat>
    {
        static Node encode(const glm::quat& rhs)
        {
            Node node;
            node.push_back(rhs.x);
            node.push_back(rhs.y);
            node.push_back(rhs.z);
            node.push_back(rhs.w);
            node.SetStyle(EmitterStyle::Flow);
            return node;
        }

        static bool decode(const Node& node, glm::quat& rhs)
        {
            if (!node.IsSequence() || node.size() != 4)
                return false;

            rhs.x = node[0].as<float>();
            rhs.y = node[1].as<float>();
            rhs.z = node[2].as<float>();
            rhs.w = node[3].as<float>();
            return true;
        }
    };
}

namespace HE
{
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec3& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;
        return out;
    }
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::vec4& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }
    YAML::Emitter& operator<<(YAML::Emitter& out, const glm::quat& v)
    {
        out << YAML::Flow;
        out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;
        return out;
    }

    SceneSerializer::SceneSerializer(const std::shared_ptr<Scene>& scene, const std::shared_ptr<ShaderLibrary>& shaderLibrary):
        m_Scene(scene),
        m_ShaderLibrary(shaderLibrary)
    {
    }

    void SceneSerializer::SerializeTransform(YAML::Emitter& out, Entity* entity)
    {
        TransformComponent* transformComponent = dynamic_cast<TransformComponent*>(entity->GetComponent(ComponentType::TransformComponent));
        out << YAML::Key << "TransformComponent";
        {
            out << YAML::BeginMap; // Transform component

            auto& position = transformComponent->GetPosition();
            out << YAML::Key << "Position" << YAML::Value << position;

            auto& rotation = transformComponent->GetRotation();
            out << YAML::Key << "Rotation" << YAML::Value << rotation;

            auto& scale = transformComponent->GetScale();
            out << YAML::Key << "Scale" << YAML::Value << scale;

            out << YAML::EndMap; // Transform component
        }
    }
    void SceneSerializer::SerializeCamera(YAML::Emitter& out, Entity* entity)
    {
        CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(entity->GetComponent(ComponentType::CameraComponent));
        out << YAML::Key << "CameraComponent";
        {
            out << YAML::BeginMap; // Camera component

            auto& camera = cameraComponent->GetCamera();
            out << YAML::Key << "Camera" << YAML::Value;
            {
                out << YAML::BeginMap; // Camera
                out << YAML::Key << "ProjectionType" << YAML::Value << (int)camera.GetProjectionType();
                out << YAML::Key << "PerspectiveFOV" << YAML::Value << camera.GetPerspectiveFov();
                out << YAML::Key << "PerspectiveNear" << YAML::Value << camera.GetPerspectiveNearClip();
                out << YAML::Key << "PerspectiveFar" << YAML::Value << camera.GetPerspectiveFarClip();
                out << YAML::Key << "OrthographicSize" << YAML::Value << camera.GetOrthographicSize();
                out << YAML::Key << "OrthographicNear" << YAML::Value << camera.GetOrthographicNearClip();
                out << YAML::Key << "OrthographicFar" << YAML::Value << camera.GetOrthographicFarClip();
                out << YAML::EndMap; // Camera
            }

            out << YAML::Key << "Primary" << YAML::Value << cameraComponent->GetPrimary();
            out << YAML::Key << "FixedAspectRatio" << YAML::Value << cameraComponent->GetFixedAspectRatio();

            out << YAML::EndMap; // Camera component
        }
    }
    void SceneSerializer::SerializeMesh(YAML::Emitter& out, Entity* entity)
    {
        MeshComponent* meshComponent = dynamic_cast<MeshComponent*>(entity->GetComponent(ComponentType::MeshComponent));
        out << YAML::Key << "MeshComponent";
        {
            out << YAML::BeginMap; // Mesh component
            out << YAML::Key << "FilePath" << YAML::Value << meshComponent->GetPath();
            out << YAML::EndMap; // Mesh component
        }
    }
    void SceneSerializer::SerializeMaterial(YAML::Emitter& out, Entity* entity)
    {
        MaterialComponent* materialComponent = dynamic_cast<MaterialComponent*>(entity->GetComponent(ComponentType::MaterialComponent));
        out << YAML::Key << "MaterialComponent";
        {
            out << YAML::BeginMap; // Material component
            out << YAML::Key << "ShaderName" << YAML::Value << materialComponent->GetShaderName();
            out << YAML::Key << "FilePath" << YAML::Value << materialComponent->GetShader()->GetFilePath();
            out << YAML::EndMap; // Material component
        }
    }
    
    void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity* entity)
    {
        out << YAML::BeginMap; // Entity
        out << YAML::Key << "Entity" << YAML::Value << entity->GetName(); // TODO: Entity ID goes here



        if (entity->HasComponent(ComponentType::TransformComponent))
            SerializeTransform(out, entity);
        if (entity->HasComponent(ComponentType::CameraComponent))
            SerializeCamera(out, entity);
        if (entity->HasComponent(ComponentType::MeshComponent))
            SerializeMesh(out, entity);
        if (entity->HasComponent(ComponentType::MaterialComponent))
            SerializeMaterial(out, entity);

        out << YAML::EndMap; // Entity
    }
    
    void SceneSerializer::Serialize(const std::string& filepath)
    {
        YAML::Emitter out;
        
        out << YAML::BeginMap;
        out << YAML::Key << "Scene";
        out << YAML::Value << m_Scene->GetName();
        out << YAML::Key << "Entities";
        out << YAML::Value << YAML::BeginSeq;
        auto& entities = m_Scene->GetEntities();
        for (auto& [name, entity]: entities)
        {
            SerializeEntity(out, entity);
        }
        out << YAML::EndSeq;
        out << YAML::EndMap;
        
        std::ofstream fout(filepath);
        fout << out.c_str();
    }

    bool SceneSerializer::Deserialize(const std::string& filepath)
    {
        std::ifstream stream(filepath);
        std::stringstream strStream;
        strStream << stream.rdbuf();

        YAML::Node data = YAML::Load(strStream.str());
        if (!data["Scene"])
            return false;
        
        std::string sceneName = data["Scene"].as<std::string>();
        if (m_Scene)
        {
            if (m_Scene->GetName() != sceneName)
            {
                // TODO maybe there can be multiple scenes 
            }
            m_Scene->Clear();
        }
        else
        {
            m_Scene = std::make_shared<Scene>(Scene(sceneName));
        }
        HE_CORE_TRACE("Deserializing scene '{0}'", sceneName);

        auto& entities = data["Entities"];
        if (entities)
        {
            for (auto& entity : entities)
            {
                std::string name = entity["Entity"].as<std::string>();
                HE_CORE_TRACE("Deserialized entity with name: '{0}'", name);
                Entity* deserializedEntity = m_Scene->CreateEntity(name);

                auto deserializedTransformComponent = entity["TransformComponent"];
                if (deserializedTransformComponent)
                {
                    // Entities always have transform
                    TransformComponent* transformComponent = dynamic_cast<TransformComponent*>(deserializedEntity->GetComponent(ComponentType::TransformComponent));
                    transformComponent->SetPosition(deserializedTransformComponent["Position"].as<glm::vec3>());
                    transformComponent->SetRotation(deserializedTransformComponent["Rotation"].as<glm::quat>());
                    transformComponent->SetScale(deserializedTransformComponent["Scale"].as<glm::vec3>());
                }

                auto deserializedCameraComponent = entity["CameraComponent"];
                if (deserializedCameraComponent)
                {
                    CameraComponent* cameraComponent = dynamic_cast<CameraComponent*>(deserializedEntity->AddComponent(ComponentType::CameraComponent));
                    auto& cameraProps = deserializedCameraComponent["Camera"];
                    auto& camera = cameraComponent->GetCamera();

                    camera.SetPerspectiveFov(cameraProps["PerspectiveFOV"].as<float>());
                    camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
                    camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

                    camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
                    camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
                    camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

                    cameraComponent->SetPrimary(deserializedCameraComponent["Primary"].as<bool>());
                    cameraComponent->SetFixedAspectRatio(deserializedCameraComponent["FixedAspectRatio"].as<bool>());
                }

                auto deserializedMeshComponent = entity["MeshComponent"];
                if (deserializedMeshComponent)
                {
                    auto& filePath = deserializedMeshComponent["FilePath"].as<std::string>();
                    LoadMesh::CreateMesh(deserializedEntity, filePath);
                }

                auto deserializedMaterialComponent = entity["MaterialComponent"];
                if (deserializedMaterialComponent)
                {
                    auto& shaderName = deserializedMaterialComponent["ShaderName"].as<std::string>();
                    auto& shaderPath = deserializedMaterialComponent["FilePath"].as<std::string>();
                    MaterialComponent* materialComponent = dynamic_cast<MaterialComponent*>(deserializedEntity->GetComponent(ComponentType::MaterialComponent));
                    m_ShaderLibrary->Load(shaderPath);
                    materialComponent->SetShader(m_ShaderLibrary, shaderName);

                }

            }
        }
        return true;
    }

}

