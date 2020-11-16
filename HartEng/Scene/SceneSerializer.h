#ifndef SCENESERIALIZER_H
#define SCENESERIALIZER_H


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

        //void SerializeTransform(Entity)
        void SerializeEntity(YAML::Emitter& out, Entity* entity);
        void SerializeTransform(YAML::Emitter& out, Entity* entity);
        void SerializeCamera(YAML::Emitter& out, Entity* entity);
        void SerializeMesh(YAML::Emitter& out, Entity* entity);
        void SerializeMaterial(YAML::Emitter& out, Entity* entity);
    public:
        SceneSerializer(const std::shared_ptr<Scene>& scene, const std::shared_ptr<ShaderLibrary>& shaderLibrary);

        void Serialize(const std::string& filepath);
        bool Deserialize(const std::string& filepath);
    };
}


#endif // SCENESERIALIZER_H
