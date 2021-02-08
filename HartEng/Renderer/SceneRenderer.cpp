#include "HartEng/Renderer/SceneRenderer.h"

#include "HartEng/Core/Application.h"
#include "HartEng/Renderer/Framebuffer.h"
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Renderer/Material.h"
#include "HartEng/Renderer/Mesh.h"
#include "HartEng/Scene/Scene.h"
#include "HartEng/Core/Timer.h"

namespace HE
{
	struct SceneRendererData
	{
		const Scene* ActiveScene = nullptr;
		

		struct SceneInfo
		{
			SceneRendererCamera SceneCamera;
		} SceneData;
		
		std::shared_ptr<RenderPass> GeoPass;

		bool EnableBloom = false;
		float BloomThreshold = 1.5f;
		
		struct DrawCommand
		{
			std::shared_ptr<Mesh> Mesh;
			std::shared_ptr<MaterialInstance> Material;
			glm::mat4 Transform;
		};
		std::vector<DrawCommand> DrawList;
		

		SceneRendererOptions Options;
	};

	struct SceneRendererStats
	{
		float GeometryPass = 0.0f;

		Timer GeometryPassTimer;
	};

	static SceneRendererData s_SceneRendererData;
	static SceneRendererStats s_Stats;

	void SceneRenderer::Init()
	{
		// Geometry RenderPass
		// geometry gramebuffer
		FrameBufferSpecification geoFramebufferSpec;
		geoFramebufferSpec.Width = Application::Get().GetWindow().GetWidth();
		geoFramebufferSpec.Height = Application::Get().GetWindow().GetHeight();
		geoFramebufferSpec.Attachemtns = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH24STENCIL8 };
		geoFramebufferSpec.Samples = 1;
		geoFramebufferSpec.ClearColor = { 1.0f, 0.0f, 1.0f, 1.0f };

		// geometry renderpass specification
		RenderPassSpecification geoRenderPassSpec;
		geoRenderPassSpec.TargetFramebuffer = FrameBuffer::Create(geoFramebufferSpec);
		s_SceneRendererData.GeoPass = RenderPass::Create(geoRenderPassSpec);
		


		
	}
	void SceneRenderer::SetViewportSize(uint32_t width, uint32_t height)
	{
		s_SceneRendererData.GeoPass->GetSpecification().TargetFramebuffer->Resize(width, height);
	}
	void SceneRenderer::BeginScene(const Scene* scene, const SceneRendererCamera& camera)
	{
		HE_CORE_ASSERT(!s_SceneRendererData.ActiveScene, "Already have active scene in SceneRenderer::BeginScene,do you call EndScene after last BeginScene?");
	
		s_SceneRendererData.ActiveScene = scene;
		s_SceneRendererData.SceneData.SceneCamera = camera;

		//s_Data.ActiveLight = scene->m_Light;
	}
	void SceneRenderer::EndScene()
	{
		HE_CORE_ASSERT(s_SceneRendererData.ActiveScene, "There is no active scene when SceneRenderer::EndScene was called! Do you call BeginScene first?");

		s_SceneRendererData.ActiveScene = nullptr;

		FlushDrawList();
	}

	void SceneRenderer::SubmitMesh(std::shared_ptr<Mesh> mesh, const glm::mat4& transform, std::shared_ptr<MaterialInstance> overrideMaterial)
	{
		s_SceneRendererData.DrawList.push_back({ mesh, overrideMaterial, transform });
		//s_Data.ShadowPassDrawList.push_back({});
	}

	std::shared_ptr<RenderPass> SceneRenderer::GetFinalRenderPass()
	{
		return s_SceneRendererData.GeoPass;
	}
	std::shared_ptr<Texture2D> SceneRenderer::GetFinalColorBuffer()
	{
		HE_CORE_ASSERT(false, "Currently not supported!");
		return nullptr;
	}
	SceneRendererOptions& SceneRenderer::GetOptions()
	{
		return s_SceneRendererData.Options;
	}
	void SceneRenderer::FlushDrawList()
	{
		HE_CORE_ASSERT(!s_SceneRendererData.ActiveScene, "Already have active scene in SceneRenderer::FlushDrawList,do you call EndScene after last BeginScene?");
		memset(&s_Stats, 0, sizeof(SceneRendererStats));

		{
			
			Renderer::Submit([]
				{
					s_Stats.GeometryPass = s_Stats.GeometryPassTimer.ElapsedMillis();
				});
			
		}
		{
			
			Renderer::Submit([]()
			{
				s_Stats.GeometryPassTimer.Reset();
			});
			
			GeometryPass();
			
			Renderer::Submit([]()
			{
				s_Stats.GeometryPass = s_Stats.GeometryPassTimer.ElapsedMillis();
			});
			
		}

		s_SceneRendererData.DrawList.clear();
		s_SceneRendererData.SceneData = {};


	}
	void SceneRenderer::GeometryPass()
	{
		
		Renderer::BeginRenderPass(s_SceneRendererData.GeoPass);

		auto& sceneCamera = s_SceneRendererData.SceneData.SceneCamera;

		auto viewProjection = sceneCamera.Camera.GetProjection() * sceneCamera.ViewMatrix;
		glm::vec3 cameraPosition = glm::inverse(s_SceneRendererData.SceneData.SceneCamera.ViewMatrix)[3]; // TODO: Negate instead

		// Render entities
		for (auto& dc : s_SceneRendererData.DrawList)
		{
			auto& baseMaterial = dc.Mesh->GetMaterial();
			baseMaterial->Set("u_ViewProjectionMatrix", viewProjection);
			baseMaterial->Set("u_ViewMatrix", sceneCamera.ViewMatrix);

			auto overrideMaterial = nullptr; // dc.Material;
			Renderer::SubmitMesh(dc.Mesh, dc.Transform, overrideMaterial);
		}
		Renderer::EndRenderPass();
	}
	void SceneRenderer::CompositePass()
	{
	}
	void SceneRenderer::BloomBlurPass()
	{
	}
	void SceneRenderer::ShadowMapPass()
	{
	}
}