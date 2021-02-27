#pragma once

#include "HartEng/Asset/AssetSerializer.h"
#include "HartEng/Core/FileSystem.h"
#include "HartEng/Asset/Assets.h"
#include "HartEng/Core/Utils.h"
#include "HartEng/Core/UUID.h"
#include "HartEng/Core/Log.h"
#include "HartEng/Renderer/Mesh.h"


#include <vector>
#include <unordered_map>
#include <map>
#include <functional>
#include <memory>

namespace HE
{
	//template<class Mesh> //template parameter could be anything!
	//void dynamic_pointer_cast(); //ADD this. NO NEED TO DEFINE IT


	class AssetTypes
	{
	private:
		static std::map<std::string, AssetType> s_Types;

	public:
		static void Init();
		static UUID GetAssetTypeID(const std::string& extension);
		static AssetType GetAssetTypeFromExtension(const std::string& extension);
	};

	struct DirectoryInfo
	{
		std::string DirectoryName;
		std::string FilePath;
		int DirectoryIndex;
		int ParentIndex;
		std::vector<int> ChildrenIndices;
	};

	struct SearchResults
	{
		std::vector<DirectoryInfo> Directories;
		std::vector<std::shared_ptr<Asset>> Assets;
	};

	class AssetManager
	{
	private:
		static std::unordered_map<UUID, std::shared_ptr<Asset>> s_LoadedAssets;
		static std::vector<DirectoryInfo> s_Directories;
		static std::function<void()> s_AssetsChangeCallback;

	public:
		static void Init();
		static void SetAssetChangeCallback(const std::function<void()>& callback);
		static void Shutdown();

		static DirectoryInfo& GetDirectoryInfo(int index);
		static std::vector<std::shared_ptr<Asset>> GetAssetsInDirectory(int dirIndex);

		static SearchResults SearchFiles(const std::string& query, const std::string& searchPath);
		static std::string GetParentPath(const std::string& path);

		static bool IsDirectory(const std::string& filepath);

		static UUID GetAssetIDForFile(const std::string& newName);
		static bool IsAssetHandleValid(UUID assetHandle);

		static void Rename(std::shared_ptr<Asset>& asset, const std::string& newName);
		static void Rename(int directoryIndex, const std::string& newName);

		template<typename T, typename... Args>
		static std::shared_ptr<T> CreateAsset(const std::string& filename, AssetType type, int directoryIndex, Args&&... args)
		{
			static_assert(std::is_base_of<Asset, T>::value, "CreateAsset only works for types derived from Asset");

			auto& directory = GetDirectoryInfo(directoryIndex);

			std::shared_ptr<T> asset = std::make_shared<T>(std::forward<Args>(args)...);
			asset->Type = type;
			asset->FilePath = directory.FilePath + "/" + filename;
			asset->FileName = Utils::RemoveExtension(Utils::GetFilename(asset->FilePath));
			asset->Extension = Utils::GetFilename(filename);
			asset->ParentDirectory = directoryIndex;
			asset->Handle = UUID(asset->FilePath);
			s_LoadedAssets[asset->Handle] = asset;

			AssetSerializer::SerializeAsset(asset);

			return asset;
		}

		template<typename T>
		static std::shared_ptr<T> GetAsset(UUID assetHandle)
		{

			HE_CORE_ASSERT(s_LoadedAssets.find(assetHandle) != s_LoadedAssets.end(), "AssetManager::GetAsset AssetID is not valid!");

			return std::dynamic_pointer_cast<T>(s_LoadedAssets[assetHandle]);
		}

		static bool IsAssetType(UUID assetHandle, AssetType type)
		{
			return (s_LoadedAssets.find(assetHandle) != s_LoadedAssets.end()) && (s_LoadedAssets[assetHandle]->Type == type);
		}

		static std::string StripExtras(const std::string& filename);
	private:
		static void ImportAsset(const std::string& filepath, bool reimport = false, int parentIndex = -1);
		static void ConvertAsset(const std::string& assetPath, const std::string& conversionType);
		static int ProcessDirectory(const std::string& directoryPath, int parentIndex = -1);
		static void ReloadAssets();

		static void OnFileSystemChanged(FileSystemChangedEvent e);

		static int FindParentIndexInChildren(DirectoryInfo& dir, const std::string& dirName);
		static int FindParentIndex(const std::string& filepath);
	};
}