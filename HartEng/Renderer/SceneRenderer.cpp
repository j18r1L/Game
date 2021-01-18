#include "HartEng/Renderer/SceneRenderer.h"


#include "HartEng/Renderer/Framebuffer.h"
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Scene/Scene.h"
#include "HartEng/Core/Timer.h"

namespace HE
{
	struct SceneRendererData
	{
		const Scene* ActiveScene = nullptr;
		
		//std::shared_ptr<Shader> CompositeShader;
		
		std::shared_ptr<RenderPass> GeoPass;
		//std::shared_ptr<RenderPass> CompositePass;

		bool EnableBloom = false;
		float BloomThreshold = 1.5f;
		/*
		struct DrawCommand
		{
			std::shared_ptr<Mesh> Mesh;
			std::shared_ptr<MaterialInstance> Material;
			glm::mat4 tansform;
		};
		std::vector<DrawCommand> DrawList;
		std::vector<DrawCommand> ShadowPassDrawList;
		*/

		SceneRendererOptions Options;
	};

	struct SceneRendererStats
	{
		float GeometryPass = 0.0f;
		//float CompositePass = 0.0f;

		Timer GeometryPassTimer;
		//Timer CompositePassTimer;
	};

	static SceneRendererData s_Data;
	static SceneRendererStats s_Stats;

	void SceneRenderer::Init()
	{
		// Geometry RenderPass
		// geometry gramebuffer
		FrameBufferSpecification geoFramebufferSpec;
		geoFramebufferSpec.Attachemtns = { FramebufferTextureFormat::RGBA16F, FramebufferTextureFormat::DEPTH };
		geoFramebufferSpec.Samples = 8;
		geoFramebufferSpec.ClearColor = { 1.0f, 0.0f, 1.0f, 1.0f };

		// geometry renderpass specification
		RenderPassSpecification geoRenderPassSpec;
		geoRenderPassSpec.TargetFramebuffer = FrameBuffer::Create(geoFramebufferSpec);
		s_Data.GeoPass = RenderPass::Create(geoRenderPassSpec);
		/*
		// Composite RenderPass
		// composite framebuffer
		FrameBufferSpecification compFramebufferSpec;
		compFramebufferSpec.Attachemtns = { FramebufferTextureFormat::RGBA8 };
		compFramebufferSpec.ClearColor = { 1.0f, 0.0f, 1.0f, 1.0f };

		// composite renderpass specification
		RenderPassSpecification compRenderPassSpec;
		compRenderPassSpec.TargetFramebuffer = FrameBuffer::Create(compFramebufferSpec);
		s_Data.CompositePass = RenderPass::Create(compRenderPassSpec);

		s_Data.CompositeShader = Shader::Create("");
		*/


		
	}
	void SceneRenderer::SetViewportSize(uint32_t width, uint32_t height)
	{
		s_Data.GeoPass->GetSpecification().TargetFramebuffer->Resize(width, height);
		//s_Data.CompositePass->GetSpecification().TargetFramebuffer->Resize(width, height);
	}
	void SceneRenderer::BeginScene(const Scene* scene)
	{
		HE_CORE_ASSERT(!s_Data.ActiveScene, "Already have active scene in SceneRenderer::BeginScene,do you call EndScene after last BeginScene?");
	
		s_Data.ActiveScene = scene;

		//s_Data.ActiveLight = scene->m_Light;
	}
	void SceneRenderer::EndScene()
	{
		HE_CORE_ASSERT(s_Data.ActiveScene, "There is no active scene when SceneRenderer::EndScene was called! Do you call BeginScene first?");

		s_Data.ActiveScene = nullptr;

		FlushDrawList();
	}

	std::shared_ptr<RenderPass> SceneRenderer::GetFinalRenderPass()
	{
		return s_Data.GeoPass;
	}
	std::shared_ptr<Texture2D> SceneRenderer::GetFinalColorBuffer()
	{
		HE_CORE_ASSERT(false, "Currently not supported!");
		return nullptr;
	}
	SceneRendererOptions& SceneRenderer::GetOptions()
	{
		return s_Data.Options;
	}
	void SceneRenderer::FlushDrawList()
	{
		HE_CORE_ASSERT(!s_Data.ActiveScene, "Already have active scene in SceneRenderer::FlushDrawList,do you call EndScene after last BeginScene?");
		memset(&s_Stats, 0, sizeof(SceneRendererStats));

		{
			/*
			Renderer::Submit([]
				{
					s_Stats.GeometryPass = s_Stats.GeometryPassTimer.ElapsedMillis();
				});
			*/
		}
		{
			/*
			Renderer::Submit([]()
			{
				s_Stats.GeometryPassTimer.Reset();
			});
			*/
			GeometryPass();
			/*
			Renderer::Submit([]()
			{
				s_Stats.GeometryPass = s_Stats.GeometryPassTimer.ElapsedMillis();
			});
			*/
		}

		//s_Data.DrawList.clear();
		//s_Data.SceneData = {};


	}
	void SceneRenderer::GeometryPass()
	{
		
		Renderer::BeginRenderPass(s_Data.GeoPass);
		// Draw entities, outlien, grid
		/*
		for (auto& dc : s_Data.DrawList)
		{
			Renderer::SubmitMesh(dc.Mesh, dc.Transform, );
		}
		*/
		//bool outline = s_Data.SelectedMeshDrawList.size() > 0;
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