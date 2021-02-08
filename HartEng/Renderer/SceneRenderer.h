#pragma once

#include "HartEng/Scene/Scene.h"
#include "HartEng/Renderer/RenderPass.h"
#include "HartEng/Renderer/Texture.h"
#include "HartEng/Renderer/Mesh.h"

namespace HE
{
	struct SceneRendererOptions
	{
		bool ShowBoundingBoxes = false;
		bool ShowGrid = true;
	};

	struct SceneRendererCamera
	{
		Camera Camera;
		glm::mat4 ViewMatrix;
		float Near, Far;
		float FOV;
	};

	class SceneRenderer
	{
	public:
		static void Init();

		static void SetViewportSize(uint32_t width, uint32_t height);

		static void BeginScene(const Scene* scene, const SceneRendererCamera& camera);
		static void EndScene();

		static void SubmitMesh(std::shared_ptr<Mesh> mesh, const glm::mat4& transform = glm::mat4(1.0f), std::shared_ptr<MaterialInstance> overrideMaterial = nullptr);

		static std::shared_ptr<RenderPass> GetFinalRenderPass();
		static std::shared_ptr<Texture2D> GetFinalColorBuffer();

		static SceneRendererOptions& GetOptions();

	private:
		static void FlushDrawList();
		static void GeometryPass();
		static void CompositePass();
		static void BloomBlurPass();
		static void ShadowMapPass();
	
	};
}