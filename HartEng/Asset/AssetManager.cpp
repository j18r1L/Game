#include "HartEng/Asset/AssetManager.h"

#include <filesystem>

namespace HE
{
	
	void AssetTypes::Init()
	{
		s_Types["he"] = AssetType::Scene;
		s_Types["fbx"] = AssetType::Mesh;
		s_Types["obj"] = AssetType::Mesh;
		s_Types["blend"] = AssetType::Mesh;
		s_Types["png"] = AssetType::Texture;
		s_Types["jpg"] = AssetType::Texture;
		s_Types["hdr"] = AssetType::EnvMap;
		s_Types["pm"] = AssetType::PhysicsMat;
		s_Types["wav"] = AssetType::Audio;
		s_Types["ogg"] = AssetType::Audio;
		s_Types["hs"] = AssetType::Script; 
	}

	UUID AssetTypes::GetAssetTypeID(const std::string& extension)
	{
		
		for (auto& kv : s_Types)
		{
			if (kv.first == extension)
				return UUID(extension);
		}
		return 0;
	}

	AssetType AssetTypes::GetAssetTypeFromExtension(const std::string& extension)
	{
		return s_Types.find(extension) != s_Types.end() ? s_Types[extension] : AssetType::Other;
	}

	std::map<std::string, AssetType> AssetTypes::s_Types;

	void AssetManager::Init()
	{
		ReloadAssets();
	}

	void AssetManager::SetAssetChangeCallback(const std::function<void()>& callback)
	{
		
		s_AssetsChangeCallback = callback;
		
	}

	void AssetManager::Shutdown()
	{
		s_LoadedAssets.clear();
		s_Directories.clear();
	}

	DirectoryInfo& AssetManager::GetDirectoryInfo(int index)
	{
		
		HE_CORE_ASSERT(index >= 0 && index < s_Directories.size(), "AssetManager::GetDirectoryInfo Index is more than directory contains!");
		return s_Directories[index];
		
	}

	std::vector<std::shared_ptr<Asset>> AssetManager::GetAssetsInDirectory(int dirIndex)
	{
		
		std::vector<std::shared_ptr<Asset>> results;

		for (auto& asset : s_LoadedAssets)
		{
			if (asset.second && asset.second->ParentDirectory == dirIndex)
				results.push_back(asset.second);
		}
		return results;
		
	}

	// Utility function to find the parent of an unprocessed directory
	int AssetManager::FindParentIndexInChildren(DirectoryInfo& dir, const std::string& dirName)
	{
		
		if (dir.DirectoryName == dirName)
			return dir.DirectoryIndex;

		for (int childIndex : dir.ChildrenIndices)
		{
			DirectoryInfo& child = AssetManager::GetDirectoryInfo(childIndex);

			int dirIndex = FindParentIndexInChildren(child, dirName);

			if (dirIndex != 0)
				return dirIndex;
		}
		return 0;
		
	}

	int AssetManager::FindParentIndex(const std::string& filepath)
	{
		
		std::vector<std::string> parts = Utils::SplitString(filepath, "/\\");
		std::string parentFolder = parts[parts.size() - 2];
		DirectoryInfo& assetsDirectory = GetDirectoryInfo(0);
		return FindParentIndexInChildren(assetsDirectory, parentFolder);
		
	}

	void AssetManager::OnFileSystemChanged(FileSystemChangedEvent e)
	{
		e.NewName = Utils::RemoveExtension(e.NewName);
		e.OldName = Utils::RemoveExtension(e.OldName);

		int parentIndex = FindParentIndex(e.FilePath);

		if (e.Action == FileSystemAction::Added)
		{
			if (e.IsDirectory)
				ProcessDirectory(e.FilePath, parentIndex);
			else
				ImportAsset(e.FilePath, false, parentIndex);
		}

		if (e.Action == FileSystemAction::Modified)
		{
			if (!e.IsDirectory)
				ImportAsset(e.FilePath, true, parentIndex);
		}

		if (e.Action == FileSystemAction::Rename)
		{
			if (e.IsDirectory)
			{
				for (auto& dir : s_Directories)
				{
					if (dir.DirectoryName == e.OldName)
					{
						dir.FilePath = e.FilePath;
						dir.DirectoryName = e.NewName;
					}
				}
			}
			else
			{
				for (auto it = s_LoadedAssets.begin(); it != s_LoadedAssets.end(); it++)
				{
					if (it->second->FileName == e.OldName)
					{
						it->second->FilePath = e.FilePath;
						it->second->FileName = e.NewName;
					}
				}
			}
		}

		if (e.Action == FileSystemAction::Delete)
		{
			if (e.IsDirectory)
			{
				/*for (auto it = s_Directories.begin(); it != s_Directories.end(); it++)
				{
					if (it->FilePath != e.FilePath)
						continue;

					RemoveDirectory(*it);
					break;
				}*/
			}
			else
			{
				for (auto it = s_LoadedAssets.begin(); it != s_LoadedAssets.end(); it++)
				{
					if (it->second->FilePath != e.FilePath)
						continue;

					s_LoadedAssets.erase(it);
					break;
				}
			}

		}

		s_AssetsChangeCallback();
	}

	SearchResults AssetManager::SearchFiles(const std::string& query, const std::string& searchPath)
	{
		
		SearchResults results;

		if (!searchPath.empty())
		{
			for (const auto& dir : s_Directories)
			{
				if (dir.DirectoryName.find(query) != std::string::npos && dir.FilePath.find(searchPath) != std::string::npos)
				{
					results.Directories.push_back(dir);
				}
			}
			for (const auto& [key, asset] : s_LoadedAssets)
			{
				if (asset->FileName.find(query) != std::string::npos && asset->FilePath.find(searchPath) != std::string::npos)
				{
					results.Assets.push_back(asset);
				}
			}
		}
		return results;
		
	}

	std::string AssetManager::GetParentPath(const std::string& path)
	{
		return std::filesystem::path(path).parent_path().string();
	}

	bool AssetManager::IsDirectory(const std::string& filepath)
	{
		
		for (auto& dir : s_Directories)
		{
			if (dir.FilePath == filepath)
				return true;
		}
		return false;
	}

	UUID AssetManager::GetAssetIDForFile(const std::string& filepath)
	{
		
		for (auto& [id, asset] : s_LoadedAssets)
		{
			if (asset->FilePath == filepath)
				return id; 
		}
		HE_CORE_ERROR("There is no asset with filepath: {0}", filepath);
		return 0;
		
	}

	bool AssetManager::IsAssetHandleValid(UUID assetHandle)
	{
		
		return !assetHandle.IsNil() && s_LoadedAssets.find(assetHandle) != s_LoadedAssets.end();
		
	}

	void AssetManager::Rename(std::shared_ptr<Asset>& asset, const std::string& newName)
	{
		HE_CORE_ASSERT(false, "AssetManager::Rename Currently not implemented!");
	}

	void AssetManager::Rename(int directoryIndex, const std::string& newName)
	{
		HE_CORE_ASSERT(false, "AssetManager::Rename Currently not implemented!");
	}

	std::string AssetManager::StripExtras(const std::string& filename)
	{
		
		std::vector<std::string> out;
		size_t start;
		size_t end = 0;

		while ((start = filename.find_first_not_of(".", end)) != std::string::npos)
		{
			end = filename.find(".", start);
			out.push_back(filename.substr(start, end - start));
		}

		if (out[0].length() >= 10)
		{
			auto cutFilename = out[0].substr(0, 9) + "...";
			return cutFilename;
		}

		auto filenameLength = out[0].length();
		auto paddingToAdd = 9 - filenameLength;

		std::string newFileName;

		for (int i = 0; i <= paddingToAdd; i++)
		{
			newFileName += " ";
		}

		newFileName += out[0];

		return newFileName;
		
	}

	void AssetManager::ImportAsset(const std::string& filepath, bool reimport, int parentIndex)
	{
		
		std::string extension = Utils::GetExtension(filepath);
		if (extension == "meta")
			return;

		AssetType type = AssetTypes::GetAssetTypeFromExtension(extension);
		std::shared_ptr<Asset> asset = AssetSerializer::Deserialize(filepath, parentIndex, reimport, type);
		if (asset)
		{
			s_LoadedAssets[asset->Handle] = asset;
		}		
	}

	int AssetManager::ProcessDirectory(const std::string& directoryPath, int parentIndex)
	{
		
		DirectoryInfo dirInfo;
		dirInfo.DirectoryName = std::filesystem::path(directoryPath).filename().string();
		dirInfo.ParentIndex = parentIndex;
		dirInfo.FilePath = directoryPath;
		s_Directories.push_back(dirInfo);
		int currentIndex = s_Directories.size() - 1;
		s_Directories[currentIndex].DirectoryIndex = currentIndex;

		if (parentIndex != -1)
			s_Directories[parentIndex].ChildrenIndices.push_back(currentIndex);

		for (auto entry : std::filesystem::directory_iterator(directoryPath))
		{
			if (entry.is_directory())
				ProcessDirectory(entry.path().string(), currentIndex);
			else
				ImportAsset(entry.path().string(), false, currentIndex);
		}

		return dirInfo.DirectoryIndex;
		
	}

	void AssetManager::ReloadAssets()
	{
		std::string pathToProject = CMAKE_PATH;
		ProcessDirectory(pathToProject + "/assets");
	}
	

	std::unordered_map<UUID, std::shared_ptr<Asset>> AssetManager::s_LoadedAssets;
	std::vector<DirectoryInfo> AssetManager::s_Directories;
	std::function<void()> AssetManager::s_AssetsChangeCallback;
	
}