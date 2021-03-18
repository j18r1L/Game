#include "AssetManagerPanel.h"

#include "HartEng/Asset/AssetManager.h"

namespace HE
{
	void AssetManagerPanel::RenderAssets()
	{
		ImGui::Begin("AssetManagerPanel");
		for (auto& asset : AssetManager::GetAssets())
		{
			switch (asset.second->Type)
			{
			case AssetType::Mesh: DrawMesh(asset.second); break;
			case AssetType::Texture: DrawTexture(asset.second); break;
			}
			
		}
		ImGui::End();
	}



	void AssetManagerPanel::DrawMesh(const std::shared_ptr<Asset>& asset)
	{

	}

	void AssetManagerPanel::DrawTexture(const std::shared_ptr<Asset>& asset)
	{
		auto texture = std::dynamic_pointer_cast<Texture2D>(asset);
		ImGui::Image((ImTextureID)texture->GetRendererID(), { 128, 128 });
	}
}