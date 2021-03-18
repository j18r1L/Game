#pragma once 

#include "HartEng/Asset/Assets.h"

#include <memory>

namespace HE
{
	// AssetsSerializer serialize all assets to save AssetID into meta file, SceneSerializer serialize all Entities & components, mesh and other components saving Assets ID 
	class AssetSerializer
	{
	public:
		template<typename T>
		static void SerializeAsset(const std::shared_ptr<T>& asset)
		{
			static_assert(std::is_base_of<Asset, T>::value, "SerializeAsset only accepts types that inherit from Asset");
			SerializeAsset(asset, asset->Type);
		}

		static std::shared_ptr<Asset> Deserialize(const std::string& filepath, int parentIndex, bool reimport, AssetType type);

		static void LoadMetaFile(const std::string& filepath, int parentIndex, bool reimport, AssetType type);
	private:
		static void SerializeAsset(const std::shared_ptr<Asset>& asset, AssetType type);
		static std::shared_ptr<Asset> DeserializeYAML(const std::string& filepath, AssetType type);
		static void LoadMetaData(std::shared_ptr<Asset>& asset);
		
		static void CreateMeta(const std::shared_ptr<Asset>& asset);
		friend class AssetManager;
	};
}
