#pragma once

#include "HartEng/HartEng.h"

namespace HE
{
	class AssetManagerPanel
	{
	public:
		static void RenderAssets();

	private:
		static void DrawMesh(const std::shared_ptr<Asset>& asset);
		static void DrawTexture(const std::shared_ptr<Asset>& asset);
	};
}