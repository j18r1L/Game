#include "HartEng/Scene/SceneSerializer.h"


#include <fstream>
#include "HartEng/Core/Log.h"
#include "HartEng/Scene/SceneCamera.h"
#include "HartEng/Scene/Components/TransformComponent.h"
#include "HartEng/Scene/Components/Texture2DComponent.h"
#include "HartEng/Scene/Components/MeshComponent.h"
#include "HartEng/Scene/Components/CameraComponent.h"
#include "HartEng/Scene/Components/LightComponent.h"
#include "HartEng/Scene/Components/CollidersComponent.h"
#include "HartEng/Asset/AssetManager.h"

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

    void SceneSerializer::Serialize(const std::string& filepath)
    {
        YAML::Emitter out;

        out << YAML::BeginMap;
        out << YAML::Key << "Scene";
        out << YAML::Value << m_Scene->GetName();
        out << YAML::Key << "Entities";
        out << YAML::Value << YAML::BeginSeq;
        auto& entities = m_Scene->GetEntities();
        for (auto& [name, entity] : entities)
        {
            SerializeEntity(out, entity);
        }
        out << YAML::EndSeq;
        out << YAML::EndMap;

        std::ofstream fout(filepath);
        fout << out.c_str();
    }

    void SceneSerializer::SerializeEntity(YAML::Emitter& out, Entity* entity)
    {
        out << YAML::BeginMap; // Entity
        std::string name = entity->GetName();
        out << YAML::Key << "Entity" << YAML::Value << name;

        if (entity->HasComponent<TransformComponent>())
            SerializeTransform(out, entity);
        if (entity->HasComponent<CameraComponent>())
            SerializeCamera(out, entity);
        if (entity->HasComponent<MeshComponent>())
            SerializeMesh(out, entity);
        if (entity->HasComponent<LightComponent>())
            SerializeLight(out, entity);
        if (entity->HasComponent<RigidBodyComponent>())
            SerializeRigidBody(out, entity);
        if (entity->HasComponent<BoxColliderComponent>())
            SerializeBoxCollider(out, entity);
        if (entity->HasComponent<SphereColliderComponent>())
            SerializeSphereCollider(out, entity);
        if (entity->HasComponent<CapsuleColliderComponent>())
            SerializeCapsuleCollider(out, entity);
        if (entity->HasComponent<MeshColliderComponent>())
            SerializeMeshCollider(out, entity);

        out << YAML::EndMap; // Entity
    }

    void SceneSerializer::SerializeTransform(YAML::Emitter& out, Entity* entity)
    {
        TransformComponent* transformComponent = entity->GetComponent<TransformComponent>();
        out << YAML::Key << "TransformComponent";
        {
            out << YAML::BeginMap; // Transform component

            out << YAML::Key << "Position" << YAML::Value << transformComponent->GetPosition();
            out << YAML::Key << "Rotation" << YAML::Value << glm::degrees(transformComponent->GetRotation());
            out << YAML::Key << "Scale" << YAML::Value << transformComponent->GetScale();

            out << YAML::EndMap; // Transform component
        }
    }
    void SceneSerializer::SerializeCamera(YAML::Emitter& out, Entity* entity)
    {
        CameraComponent* cameraComponent = entity->GetComponent<CameraComponent>();
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
        MeshComponent* meshComponent = entity->GetComponent<MeshComponent>();
        out << YAML::Key << "MeshComponent";
        {
            out << YAML::BeginMap; // Mesh component
            std::string assetID = meshComponent->GetMesh().get()->Handle;
            out << YAML::Key << "AssetID" << YAML::Value << assetID;
            out << YAML::EndMap; // Mesh component
        }
    }

    void SceneSerializer::SerializeLight(YAML::Emitter& out, Entity* entity)
    {
        LightComponent* lightComponent = entity->GetComponent<LightComponent>();
        out << YAML::Key << "LightComponent";
        {
            out << YAML::BeginMap; // Light component

            out << YAML::Key << "LightType" << YAML::Value << lightComponent->GetLightType();
            out << YAML::Key << "CastShadow" << YAML::Value << lightComponent->GetCastShadow();
            out << YAML::Key << "Direction" << YAML::Value << lightComponent->GetDirection();
            out << YAML::Key << "Color" << YAML::Value << lightComponent->GetColor();
            out << YAML::Key << "Intensity" << YAML::Value << lightComponent->GetIntensity();
            out << YAML::Key << "Range" << YAML::Value << lightComponent->GetRange();
            out << YAML::Key << "InnerConeAngle" << YAML::Value << lightComponent->GetInnerConeAngle();
            out << YAML::Key << "OuterConeAngle" << YAML::Value << lightComponent->GetOuterConeAngle();

            out << YAML::EndMap; // Light component
        }
    }

    void SceneSerializer::SerializeRigidBody(YAML::Emitter& out, Entity* entity)
    {
        auto rigidBodyComponent = entity->GetComponent<RigidBodyComponent>();
        out << YAML::Key << "RigidBodyComponent";
        {
            out << YAML::BeginMap; // RigidBodyComponent

            out << YAML::Key << "BodyType" << YAML::Value << (int)rigidBodyComponent->GetBodyType();
            out << YAML::Key << "Mass" << YAML::Value << rigidBodyComponent->GetMass();
            out << YAML::Key << "LinearDrag" << YAML::Value << rigidBodyComponent->GetLinearDrag();
            out << YAML::Key << "AngularDrag" << YAML::Value << rigidBodyComponent->GetAngularDrag();
            out << YAML::Key << "DisableGravity" << YAML::Value << rigidBodyComponent->GetDisableGravity();
            out << YAML::Key << "IsKinematic" << YAML::Value << rigidBodyComponent->GetIsKinematic();
            out << YAML::Key << "Layer" << YAML::Value << rigidBodyComponent->GetLayer();
            out << YAML::Key << "LockPositionX" << YAML::Value << rigidBodyComponent->GetLockPositionX();
            out << YAML::Key << "LockPositionY" << YAML::Value << rigidBodyComponent->GetLockPositionY();
            out << YAML::Key << "LockPositionZ" << YAML::Value << rigidBodyComponent->GetLockPositionZ();
            out << YAML::Key << "LockRotationX" << YAML::Value << rigidBodyComponent->GetLockRotationX();
            out << YAML::Key << "LockRotationY" << YAML::Value << rigidBodyComponent->GetLockRotationY();
            out << YAML::Key << "LockRotationZ" << YAML::Value << rigidBodyComponent->GetLockRotationZ();

            out << YAML::EndMap; // RigidBodyComponent
        }
    }

    void SceneSerializer::SerializeBoxCollider(YAML::Emitter& out, Entity* entity)
    {
        auto boxColliderComponent = entity->GetComponent<BoxColliderComponent>();
        out << YAML::Key << "BoxColliderComponent";
        {
            out << YAML::BeginMap; // BoxCollider Component

            out << YAML::Key << "Size" << YAML::Value << boxColliderComponent->GetSize();
            out << YAML::Key << "Offset" << YAML::Value << boxColliderComponent->GetOffset();
            out << YAML::Key << "IsTrigger" << YAML::Value << boxColliderComponent->GetTrigger();

            auto& debugMesh = boxColliderComponent->GetMesh();
            if (debugMesh)
                out << YAML::Key << "DebugMeshAssetID" << YAML::Value << debugMesh->Handle;
            else
                out << YAML::Key << "DebugMeshAssetID" << YAML::Value << UUID(0);

            auto& physicsMaterial = boxColliderComponent->GetPhysicsMaterial();
            if (physicsMaterial)
                out << YAML::Key << "PhysicsMaterialAssetID" << YAML::Value << physicsMaterial->Handle;
            else
                out << YAML::Key << "PhysicsMaterialAssetID" << YAML::Value << UUID(0);

            out << YAML::EndMap; // BoxCollider Component
        }
    }

    void SceneSerializer::SerializeSphereCollider(YAML::Emitter& out, Entity* entity)
    {
        auto sphereColliderComponent = entity->GetComponent<SphereColliderComponent>();
        out << YAML::Key << "SphereColliderComponent";
        {
            out << YAML::BeginMap; // sphereCollider Component

            out << YAML::Key << "Radius" << YAML::Value << sphereColliderComponent->GetRadius();
            out << YAML::Key << "IsTrigger" << YAML::Value << sphereColliderComponent->GetTrigger();

            auto& debugMesh = sphereColliderComponent->GetMesh();
            if (debugMesh)
                out << YAML::Key << "DebugMeshAssetID" << YAML::Value << debugMesh->Handle;
            else
                out << YAML::Key << "DebugMeshAssetID" << YAML::Value << UUID(0);

            auto& physicsMaterial = sphereColliderComponent->GetPhysicsMaterial();
            if (physicsMaterial)
                out << YAML::Key << "PhysicsMaterialAssetID" << YAML::Value << physicsMaterial->Handle;
            else
                out << YAML::Key << "PhysicsMaterialAssetID" << YAML::Value << UUID(0);

            out << YAML::EndMap; // sphereCollider Component
        }
    }

    void SceneSerializer::SerializeCapsuleCollider(YAML::Emitter& out, Entity* entity)
    {
        auto capsuleColliderComponent = entity->GetComponent<CapsuleColliderComponent>();
        out << YAML::Key << "CapsuleColluderComponent";
        {
            out << YAML::BeginMap; // capsuleCollider Component

            out << YAML::Key << "Radius" << YAML::Value << capsuleColliderComponent->GetRadius();
            out << YAML::Key << "Height" << YAML::Value << capsuleColliderComponent->GetHeight();
            out << YAML::Key << "IsTrigger" << YAML::Value << capsuleColliderComponent->GetTrigger();

            auto& debugMesh = capsuleColliderComponent->GetMesh();
            if (debugMesh)
                out << YAML::Key << "DebugMeshAssetID" << YAML::Value << debugMesh->Handle;
            else
                out << YAML::Key << "DebugMeshAssetID" << YAML::Value << UUID(0);

            auto& physicsMaterial = capsuleColliderComponent->GetPhysicsMaterial();
            if (physicsMaterial)
                out << YAML::Key << "PhysicsMaterialAssetID" << YAML::Value << physicsMaterial->Handle;
            else
                out << YAML::Key << "PhysicsMaterialAssetID" << YAML::Value << UUID(0);

            out << YAML::EndMap; // capsuleCollider Component
        }
    }

    void SceneSerializer::SerializeMeshCollider(YAML::Emitter& out, Entity* entity)
    {
        auto meshColliderComponent = entity->GetComponent<MeshColliderComponent>();
        out << YAML::Key << "MeshColliderComponent";
        {
            out << YAML::BeginMap; // MeshCollider Component

            out << YAML::Key << "Convex" << YAML::Value << meshColliderComponent->GetConvex();
            out << YAML::Key << "Override" << YAML::Value << meshColliderComponent->GetOverrideMesh();
            out << YAML::Key << "IsTrigger" << YAML::Value << meshColliderComponent->GetTrigger();

            out << YAML::Key << "CollisionMeshAssetID" << YAML::Value << meshColliderComponent->GetCollisionMesh()->Handle;

            auto& physicsMaterial = meshColliderComponent->GetPhysicsMaterial();
            if (physicsMaterial)
                out << YAML::Key << "PhysicsMaterialAssetID" << YAML::Value << physicsMaterial->Handle;
            else
                out << YAML::Key << "PhysicsMaterialAssetID" << YAML::Value << UUID(0);

            out << YAML::EndMap; // MeshCollider Component
        }
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

        auto entities = data["Entities"];
        if (entities)
        {
            for (auto entity : entities)
            {
                std::string name = entity["Entity"].as<std::string>();
                HE_CORE_TRACE("Deserialized entity with name: '{0}'", name);
                Entity* deserializedEntity = m_Scene->CreateEntity(name);

                auto deserializedTransformComponent = entity["TransformComponent"];
                if (deserializedTransformComponent)
                    DeserializeTransform(deserializedTransformComponent, deserializedEntity);

                auto deserializedCameraComponent = entity["CameraComponent"];
                if (deserializedCameraComponent)
                    DeserializeCamera(deserializedCameraComponent, deserializedEntity);

                auto deserializedMeshComponent = entity["MeshComponent"];
                if (deserializedMeshComponent)
                    DeserializeMesh(deserializedMeshComponent, deserializedEntity);
                
                auto deserializedLightComponent = entity["LightComponent"];
                if (deserializedLightComponent)
                    DeserializeLight(deserializedLightComponent, deserializedEntity);

                auto deserializedRigidBodyComponent = entity["RigidBodyComponent"];
                if (deserializedRigidBodyComponent)
                    DeserializeRigidBody(deserializedRigidBodyComponent, deserializedEntity);

                auto deserializedBoxColliderComponent = entity["BoxColliderComponent"];
                if (deserializedBoxColliderComponent)
                    DeserializeBoxCollider(deserializedBoxColliderComponent, deserializedEntity);

                auto deserializedSphereColliderComponent = entity["SphereColliderComponent"];
                if (deserializedSphereColliderComponent)
                    DeserializeSphereCollider(deserializedSphereColliderComponent, deserializedEntity);

                auto deserializedCapsuleColliderComponent = entity["CapsuleColliderComponent"];
                if (deserializedCapsuleColliderComponent)
                    DeserializeCapsuleCollider(deserializedCapsuleColliderComponent, deserializedEntity);

                auto deserializedMeshColliderComponent = entity["MeshColliderComponent"];
                if (deserializedMeshColliderComponent)
                    DeserializeMeshCollider(deserializedMeshColliderComponent, deserializedEntity);

            }
        }
        return true;
    }

    void SceneSerializer::DeserializeTransform(const YAML::Node& deserializedComponent, Entity* deserializedEntity)
    {
        // Entities always have transform
        TransformComponent* transformComponent = deserializedEntity->GetComponent<TransformComponent>();
        transformComponent->SetPosition(deserializedComponent["Position"].as<glm::vec3>());
        transformComponent->SetRotation(deserializedComponent["Rotation"].as<glm::vec3>());
        transformComponent->SetScale(deserializedComponent["Scale"].as<glm::vec3>());
    }
    void SceneSerializer::DeserializeCamera(const YAML::Node& deserializedComponent, Entity* deserializedEntity)
    {
        CameraComponent* cameraComponent = deserializedEntity->AddComponent<CameraComponent>();
        auto cameraProps = deserializedComponent["Camera"];
        auto& camera = cameraComponent->GetCamera();

        camera.SetProjectionType((SceneCamera::ProjectionType)cameraProps["ProjectionType"].as<int>());
        camera.SetPerspectiveFov(cameraProps["PerspectiveFOV"].as<float>());
        camera.SetPerspectiveNearClip(cameraProps["PerspectiveNear"].as<float>());
        camera.SetPerspectiveFarClip(cameraProps["PerspectiveFar"].as<float>());

        camera.SetOrthographicSize(cameraProps["OrthographicSize"].as<float>());
        camera.SetOrthographicNearClip(cameraProps["OrthographicNear"].as<float>());
        camera.SetOrthographicFarClip(cameraProps["OrthographicFar"].as<float>());

        cameraComponent->SetPrimary(deserializedComponent["Primary"].as<bool>());
        cameraComponent->SetFixedAspectRatio(deserializedComponent["FixedAspectRatio"].as<bool>());
    }
    void SceneSerializer::DeserializeMesh(const YAML::Node& deserializedComponent, Entity* deserializedEntity)
    {
        std::string assetID = deserializedComponent["AssetID"].as<std::string>();
        UUID uuid(assetID);
        if (AssetManager::IsAssetHandleValid(uuid))
        {
            if (!deserializedEntity->HasComponent<MeshComponent>())
            {
                auto meshComponent = deserializedEntity->AddComponent<MeshComponent>();
                meshComponent->SetMesh(AssetManager::GetAsset<Mesh>(assetID));
            }
            else
            {
                HE_CORE_ERROR("Serialized entity already has mesh component!");
            }
        }
        else
        {
            HE_CORE_ERROR("Serialized mesh AssetID is not valid!");
        }
    }

    void SceneSerializer::DeserializeLight(const YAML::Node& deserializedComponent, Entity* deserializedEntity)
    {
        LightComponent* lightComponent = deserializedEntity->AddComponent<LightComponent>();

        LightType lightType = (LightType)deserializedComponent["LightType"].as<int>();
        lightComponent->SetLightType(lightType);
        lightComponent->SetCastShadow(deserializedComponent["CastShadow"].as<bool>());
        lightComponent->SetDirection(deserializedComponent["Direction"].as<glm::vec3>());
        lightComponent->SetColor(deserializedComponent["Color"].as<glm::vec3>());
        lightComponent->SetIntensity(deserializedComponent["Intensity"].as<float>());
        lightComponent->SetRange(deserializedComponent["Range"].as<float>());
        lightComponent->SetInnerConeAngle(deserializedComponent["InnerConeAngle"].as<float>());
        lightComponent->SetOuterConeAngle(deserializedComponent["OuterConeAngle"].as<float>());
    }

    void SceneSerializer::DeserializeRigidBody(const YAML::Node& deserializedComponent, Entity* deserializedEntity)
    {
        auto rigidBodyComponent = deserializedEntity->AddComponent<RigidBodyComponent>();

        RigidBodyComponent::Type bodyType = (RigidBodyComponent::Type)deserializedComponent["BodyType"].as<int>();
        rigidBodyComponent->SetBodyType(bodyType);
        rigidBodyComponent->SetMass(deserializedComponent["Mass"].as<float>());
        rigidBodyComponent->SetLinearDrag(deserializedComponent["LinearDrag"].as<float>());
        rigidBodyComponent->SetAngularDrag(deserializedComponent["AngularDrag"].as<float>());
        rigidBodyComponent->SetDisableGravity(deserializedComponent["DisableGravity"].as<bool>());
        rigidBodyComponent->SetIsKinematic(deserializedComponent["IsKinematic"].as<bool>());
        rigidBodyComponent->SetLayer(deserializedComponent["Layer"].as<uint32_t>());

        rigidBodyComponent->SetLockPositionX(deserializedComponent["LockPositionX"].as<bool>());
        rigidBodyComponent->SetLockPositionY(deserializedComponent["LockPositionY"].as<bool>());
        rigidBodyComponent->SetLockPositionZ(deserializedComponent["LockPositionZ"].as<bool>());

        rigidBodyComponent->SetLockRotationX(deserializedComponent["LockRotationX"].as<bool>());
        rigidBodyComponent->SetLockRotationY(deserializedComponent["LockRotationY"].as<bool>());
        rigidBodyComponent->SetLockRotationZ(deserializedComponent["LockRotationZ"].as<bool>());
    }

    void SceneSerializer::DeserializeBoxCollider(const YAML::Node& deserializedComponent, Entity* deserializedEntity)
    {
        auto boxColliderComponent = deserializedEntity->AddComponent<BoxColliderComponent>();

        boxColliderComponent->SetSize(deserializedComponent["Size"].as<glm::vec3>());
        boxColliderComponent->SetOffset(deserializedComponent["Offset"].as<glm::vec3>());
        boxColliderComponent->SetTrigger(deserializedComponent["IsTrigger"].as<bool>());
        

        std::string meshAssetID = deserializedComponent["DebugMeshAssetID"].as<std::string>();
        UUID meshuuid(meshAssetID);
        if (AssetManager::IsAssetHandleValid(meshuuid))
        {
            boxColliderComponent->SetMesh(AssetManager::GetAsset<Mesh>(meshuuid));
        }
        else
        {
            HE_CORE_ERROR("Serialized debugMesh AssetID is not valid!");
        }

        std::string physicsMaterialAssetID = deserializedComponent["PhysicsMaterialAssetID"].as<std::string>();
        UUID physicsMaterialuuid(physicsMaterialAssetID);
        if (AssetManager::IsAssetHandleValid(physicsMaterialuuid))
        {
            boxColliderComponent->SetPhysicsMaterial(AssetManager::GetAsset<PhysicsMaterial>(physicsMaterialuuid));
        }
        else
        {
            HE_CORE_ERROR("Serialized physicsMaterial AssetID is not valid!");
        }
    }

    void SceneSerializer::DeserializeSphereCollider(const YAML::Node& deserializedComponent, Entity* deserializedEntity)
    {
        auto sphereColliderComponent = deserializedEntity->AddComponent<SphereColliderComponent>();

        sphereColliderComponent->SetRadius(deserializedComponent["Radius"].as<float>());
        sphereColliderComponent->SetTrigger(deserializedComponent["IsTrigger"].as<bool>());


        std::string meshAssetID = deserializedComponent["DebugMeshAssetID"].as<std::string>();
        UUID meshuuid(meshAssetID);
        if (AssetManager::IsAssetHandleValid(meshuuid))
        {
            sphereColliderComponent->SetMesh(AssetManager::GetAsset<Mesh>(meshuuid));
        }
        else
        {
            HE_CORE_ERROR("Serialized debugMesh AssetID is not valid!");
        }

        std::string physicsMaterialAssetID = deserializedComponent["PhysicsMaterialAssetID"].as<std::string>();
        UUID physicsMaterialuuid(physicsMaterialAssetID);
        if (AssetManager::IsAssetHandleValid(physicsMaterialuuid))
        {
            sphereColliderComponent->SetPhysicsMaterial(AssetManager::GetAsset<PhysicsMaterial>(physicsMaterialuuid));
        }
        else
        {
            HE_CORE_ERROR("Serialized physicsMaterial AssetID is not valid!");
        }
    }

    void SceneSerializer::DeserializeCapsuleCollider(const YAML::Node& deserializedComponent, Entity* deserializedEntity)
    {
        auto capsuleColliderComponent = deserializedEntity->AddComponent<CapsuleColliderComponent>();

        capsuleColliderComponent->SetRadius(deserializedComponent["Radius"].as<float>());
        capsuleColliderComponent->SetHeight(deserializedComponent["Height"].as<float>());
        capsuleColliderComponent->SetTrigger(deserializedComponent["IsTrigger"].as<bool>());


        std::string meshAssetID = deserializedComponent["DebugMeshAssetID"].as<std::string>();
        UUID meshuuid(meshAssetID);
        if (AssetManager::IsAssetHandleValid(meshuuid))
        {
            capsuleColliderComponent->SetMesh(AssetManager::GetAsset<Mesh>(meshuuid));
        }
        else
        {
            HE_CORE_ERROR("Serialized debugMesh AssetID is not valid!");
        }

        std::string physicsMaterialAssetID = deserializedComponent["PhysicsMaterialAssetID"].as<std::string>();
        UUID physicsMaterialuuid(physicsMaterialAssetID);
        if (AssetManager::IsAssetHandleValid(physicsMaterialuuid))
        {
            capsuleColliderComponent->SetPhysicsMaterial(AssetManager::GetAsset<PhysicsMaterial>(physicsMaterialuuid));
        }
        else
        {
            HE_CORE_ERROR("Serialized physicsMaterial AssetID is not valid!");
        }
    }

    void SceneSerializer::DeserializeMeshCollider(const YAML::Node& deserializedComponent, Entity* deserializedEntity)
    {
        auto meshColliderComponent = deserializedEntity->AddComponent<MeshColliderComponent>();

        meshColliderComponent->SetConvex(deserializedComponent["Convex"].as<bool>());
        meshColliderComponent->SetOverrideMesh(deserializedComponent["Override"].as<bool>());
        meshColliderComponent->SetTrigger(deserializedComponent["IsTrigger"].as<bool>());
        
        std::string collisionMeshAssetID = deserializedComponent["CollisionMeshAssetID"].as<std::string>();
        UUID collisionMeshUUID(collisionMeshAssetID);
        if (AssetManager::IsAssetHandleValid(collisionMeshUUID))
        {
            meshColliderComponent->SetCollisionMesh(AssetManager::GetAsset<Mesh>(collisionMeshUUID));
        }
        else
        {
            HE_CORE_ERROR("Serialized debugMesh AssetID is not valid!");
        }

        std::string physicsMaterialAssetID = deserializedComponent["PhysicsMaterialAssetID"].as<std::string>();
        UUID physicsMaterialuuid(physicsMaterialAssetID);
        if (AssetManager::IsAssetHandleValid(physicsMaterialuuid))
        {
            meshColliderComponent->SetPhysicsMaterial(AssetManager::GetAsset<PhysicsMaterial>(physicsMaterialuuid));
        }
        else
        {
            HE_CORE_ERROR("Serialized physicsMaterial AssetID is not valid!");
        }
    }
}

