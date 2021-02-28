#include "HartEng/Asset/AssetSerializer.h"

#include "HartEng/Renderer/Texture.h"
#include "HartEng/Core/FileSystem.h"
#include "HartEng/Renderer/Mesh.h"
#include "HartEng/Core/Utils.h"


#include <yaml-cpp/yaml.h>



namespace HE
{
	void AssetSerializer::SerializeAsset(const std::shared_ptr<Asset>& asset, AssetType type)
	{
		YAML::Emitter out;

		out << YAML::BeginMap;

		switch (type)
		{
		case AssetType::PhysicsMat:
		{
			//std::shared_ptr<PhysicsMaterial> material = static_cast<std::shared_ptr<PhysicsMaterial>>(asset);
			auto material = dynamic_cast<PhysicsMaterial*>(asset.get());
			out << YAML::Key << "StaticFriction" << material->StaticFriction;
			out << YAML::Key << "DynamicFriction" << material->DynamicFriction;
			out << YAML::Key << "Bounciness" << material->Bounciness;
			break;
		}
		}
		out << YAML::EndMap;

		std::ofstream fout(asset->FilePath);
		fout << out.c_str();
	}

	std::shared_ptr<Asset> AssetSerializer::Deserialize(const std::string& filepath, int parentIndex, bool reimport, AssetType type)
	{
		std::shared_ptr<Asset> asset;
		std::string extension = Utils::GetExtension(filepath);

		bool loadYAMLData = true;
		bool loaded = false;

		switch (type)
		{
		case AssetType::Mesh:
		{
			if (extension != "blend")
			{
				asset = std::make_shared<Mesh>(filepath);
				if (asset)
				{
					loaded = true;
				}
			}
				
			loadYAMLData = false;
			
			break;
		}
		case AssetType::Texture:
		{
			asset = Texture2D::Create(filepath);
			if (!asset)
			{
				std::string pathToProject = CMAKE_PATH;
				asset = Texture2D::Create(pathToProject + "/assets/textures/red.png");
			}
			loaded = true;
			loadYAMLData = false;
			break;
		}
		case AssetType::EnvMap:
		{
			// TODO
			//auto [radiance, irradiance] = SceneRenderer::CreateEnvironmentMap(filepath);
			//asset = std::shared_ptr<Environment>::Create(radiance, irradiance);
			loadYAMLData = false;
			break;
		}
		case AssetType::Scene:
		case AssetType::Audio:
		case AssetType::Script:
		case AssetType::Other:
		{
			loadYAMLData = false;
			break;
		}
		}

		if (loadYAMLData)
		{
			asset = DeserializeYAML(filepath, type);
			HE_CORE_ASSERT(asset, "Failed to load asset");
			loaded = true;
		}
		if (loaded)
		{
			asset->FilePath = filepath;
			std::replace(asset->FilePath.begin(), asset->FilePath.end(), '\\', '/');

			bool hasMeta = FileSystem::Exists(asset->FilePath + ".meta");
			if (hasMeta)
			{
				LoadMetaData(asset);
			}
			else
			{
				UUID uuid(0);
				uuid.GenerateFromString(filepath);

				asset->Handle = uuid;
				asset->FileName = Utils::RemoveExtension(Utils::GetFilename(filepath));
				asset->Extension = Utils::GetExtension(filepath);
				asset->ParentDirectory = parentIndex;
				asset->Type = type;
			}

			if (!hasMeta)
				CreateMetaFile(asset);
		}
		

		return asset;
	}

	std::shared_ptr<Asset> AssetSerializer::DeserializeYAML(const std::string& filepath, AssetType type)
	{
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());

		if (type == AssetType::PhysicsMat)
		{
			float staticFriction = data["StaticFriction"].as<float>();
			float dynamicFriction = data["DynamicFriction"].as<float>();
			float bounciness = data["Bounciness"].as<float>();

			return std::make_shared<PhysicsMaterial>(staticFriction, dynamicFriction, bounciness);
		}

		return nullptr;
	}



	void AssetSerializer::LoadMetaData(std::shared_ptr<Asset>& asset)
	{
		std::ifstream stream(asset->FilePath + ".meta");
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::Load(strStream.str());
		if (!data["Asset"])
			HE_CORE_ASSERT(false, "Invalid File Format");

		asset->Handle = UUID(data["Asset"].as<std::string>());
		asset->FileName = data["FileName"].as<std::string>();
		asset->FilePath = data["FilePath"].as<std::string>();
		asset->Extension = data["Extension"].as<std::string>();
		asset->ParentDirectory = data["Directory"].as<int>();
		asset->Type = (AssetType)data["Type"].as<int>();
	}

	void AssetSerializer::CreateMetaFile(const std::shared_ptr<Asset>& asset)
	{
		YAML::Emitter out;
		out << YAML::BeginMap;
		out << YAML::Key << "Asset" << YAML::Value << asset->Handle;
		out << YAML::Key << "FileName" << YAML::Value << asset->FileName;
		out << YAML::Key << "FilePath" << YAML::Value << asset->FilePath;
		out << YAML::Key << "Extension" << YAML::Value << asset->Extension;
		out << YAML::Key << "Directory" << YAML::Value << asset->ParentDirectory;
		out << YAML::Key << "Type" << YAML::Value << (int)asset->Type;
		out << YAML::EndMap;

		std::ofstream fout(asset->FilePath + ".meta");
		fout << out.c_str();
	}
}