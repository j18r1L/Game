#pragma once

#include "HartEng/Scene/Scene.h"
#include "HartEng/Renderer/RenderPass.h"
#include "HartEng/Renderer/Texture.h"

namespace HE
{
	struct SceneRendererOptions
	{
		bool ShowBoundingBoxes = false;
		bool ShowGrid = true;
	};

	struct SceneRendererCamera
	{
		
	};

	class SceneRenderer
	{
	private:
		static void FlushDrawList();
		static void GeometryPass();
		static void CompositePass();
		static void BloomBlurPass();
		static void ShadowMapPass();
	public:
		static void Init();

		static void SetViewportSize(uint32_t width, uint32_t height);

		static void BeginScene(const Scene* scene);
		static void EndScene();

		static std::shared_ptr<RenderPass> GetFinalRenderPass();
		static std::shared_ptr<Texture2D> GetFinalColorBuffer();

		static SceneRendererOptions& GetOptions();

	
	};
}