#pragma once


#include "HartEng/Scene/Scene.h"
#include "HartEng/Renderer/Shader.h"
#include <yaml-cpp/yaml.h>

namespace HE
{
    class SceneSerializer
    {
    private:
        std::shared_ptr<Scene> m_Scene;
        std::shared_ptr<ShaderLibrary> m_ShaderLibrary;

        void SerializeEntity(YAML::Emitter& out, Entity* entity);
        void SerializeTransform(YAML::Emitter& out, Entity* entity);
        void SerializeTag(YAML::Emitter& out, Entity* entity);
        void SerializeCamera(YAML::Emitter& out, Entity* entity);
        void SerializeMesh(YAML::Emitter& out, Entity* entity);
        void SerializeLight(YAML::Emitter& out, Entity* entity);
        void SerializeRigidBody(YAML::Emitter& out, Entity* entity);
        void SerializeBoxCollider(YAML::Emitter& out, Entity* entity);
        void SerializeSphereCollider(YAML::Emitter& out, Entity* entity);
        void SerializeCapsuleCollider(YAML::Emitter& out, Entity* entity);
        void SerializeMeshCollider(YAML::Emitter& out, Entity* entity);

        void DeserializeTransform(const YAML::Node& deserializedComponent, Entity* deserializedEntity);
        void DeserializeTag(const YAML::Node& deserializedComponent, Entity* deserializedEntity);
        void DeserializeCamera(const YAML::Node& deserializedComponentt, Entity* deserializedEntity);
        void DeserializeMesh(const YAML::Node& deserializedComponent, Entity* deserializedEntity);
        void DeserializeLight(const YAML::Node& deserializedComponent, Entity* deserializedEntity);
        void DeserializeRigidBody(const YAML::Node& deserializedComponent, Entity* deserializedEntity);
        void DeserializeBoxCollider(const YAML::Node& deserializedComponent, Entity* deserializedEntity);
        void DeserializeSphereCollider(const YAML::Node& deserializedComponent, Entity* deserializedEntity);
        void DeserializeCapsuleCollider(const YAML::Node& deserializedComponent, Entity* deserializedEntity);
        void DeserializeMeshCollider(const YAML::Node& deserializedComponent, Entity* deserializedEntity);
    public:
        SceneSerializer(const std::shared_ptr<Scene>& scene, const std::shared_ptr<ShaderLibrary>& shaderLibrary);

        void Serialize(const std::string& filepath);
        bool Deserialize(const std::string& filepath);
    };
}


