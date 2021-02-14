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
		std::shared_ptr<RenderPass> EntityIDPass;
		
		struct DrawCommand
		{
			std::shared_ptr<Mesh> Mesh;
			std::shared_ptr<MaterialInstance> Material;
			glm::mat4 Transform;
		};
		std::vector<DrawCommand> DrawList;
		std::vector<DrawCommand> EntityIDDrawList;
		

		SceneRendererOptions Options;
	};

	struct SceneRendererStats
	{
		float GeometryPass = 0.0f;
		float EntityIDPass = 0.0f;

		Timer GeometryPassTimer;
		Timer EntityIDPassTimer;
	};

	static SceneRendererData s_SceneRendererData;
	static SceneRendererStats s_Stats;

	void SceneRenderer::Init()
	{
		// Geometry RenderPass
		{
			// geometry gramebuffer
			FrameBufferSpecification geoFramebufferSpec;
			geoFramebufferSpec.Width = Application::Get().GetWindow().GetWidth();
			geoFramebufferSpec.Height = Application::Get().GetWindow().GetHeight();
			geoFramebufferSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::DEPTH24STENCIL8 };
			geoFramebufferSpec.Samples = 1;
			geoFramebufferSpec.ClearColor = { 1.0f, 0.0f, 1.0f, 1.0f };

			// geometry renderpass specification
			RenderPassSpecification geoRenderPassSpec;
			geoRenderPassSpec.TargetFramebuffer = FrameBuffer::Create(geoFramebufferSpec);
			s_SceneRendererData.GeoPass = RenderPass::Create(geoRenderPassSpec);
		}
		

		// EntityID RenderPass
		{
			// EntityID gramebuffer
			FrameBufferSpecification entityIdFramebufferSpec;
			entityIdFramebufferSpec.Width = Application::Get().GetWindow().GetWidth();
			entityIdFramebufferSpec.Height = Application::Get().GetWindow().GetHeight();
			entityIdFramebufferSpec.Attachments = { FramebufferTextureFormat::R32I };
			entityIdFramebufferSpec.Samples = 1;
			entityIdFramebufferSpec.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

			// EntityID renderpass specification
			RenderPassSpecification entityIDRenderPassSpec;
			entityIDRenderPassSpec.TargetFramebuffer = FrameBuffer::Create(entityIdFramebufferSpec);
			s_SceneRendererData.EntityIDPass = RenderPass::Create(entityIDRenderPassSpec);
		}

		


		
	}
	void SceneRenderer::SetViewportSize(uint32_t width, uint32_t height)
	{
		s_SceneRendererData.GeoPass->GetSpecification().TargetFramebuffer->Resize(width, height);
		s_SceneRendererData.EntityIDPass->GetSpecification().TargetFramebuffer->Resize(width, height);
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

	void SceneRenderer::SubmitEntityIDMesh(std::shared_ptr<Mesh> mesh, const glm::mat4& transform, std::shared_ptr<MaterialInstance> overrideMaterial)
	{
		s_SceneRendererData.EntityIDDrawList.push_back({ mesh, overrideMaterial, transform });
	}

	std::shared_ptr<RenderPass> SceneRenderer::GetGeometryRenderPass()
	{
		return s_SceneRendererData.GeoPass;
	}

	std::shared_ptr<RenderPass> SceneRenderer::GetEntityIDRenderPass()
	{
		return s_SceneRendererData.EntityIDPass;
	}

	SceneRendererOptions& SceneRenderer::GetOptions()
	{
		return s_SceneRendererData.Options;
	}

	void SceneRenderer::FlushDrawList()
	{
		HE_CORE_ASSERT(!s_SceneRendererData.ActiveScene, "Already have active scene in SceneRenderer::FlushDrawList,do you call EndScene after last BeginScene?");
		memset(&s_Stats, 0, sizeof(SceneRendererStats));

		// Maybe sort by distance from camera, frustum culling and so on here..

		// Do the submiting to Renderer

		// Geometry Pass
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

		// EntityID Pass
		{
			Renderer::Submit([]()
				{
					s_Stats.EntityIDPassTimer.Reset();
				});
			EntityIDPass();
			Renderer::Submit([]()
				{
					s_Stats.EntityIDPass = s_Stats.EntityIDPassTimer.ElapsedMillis();
				});
		}

		for (auto& drawlist : s_SceneRendererData.DrawList)
		{
			drawlist.Mesh.reset();
			drawlist.Material.reset();
		}
		for (auto& drawlist : s_SceneRendererData.EntityIDDrawList)
		{
			drawlist.Mesh.reset();
			drawlist.Material.reset();
		}
		s_SceneRendererData.DrawList.clear();
		s_SceneRendererData.EntityIDDrawList.clear();
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
			// Get base material from mesh
			auto& baseMaterial = dc.Mesh->GetMaterial();

			// Set values
			baseMaterial->Set("u_ViewProjectionMatrix", viewProjection);
			baseMaterial->Set("u_ViewMatrix", sceneCamera.ViewMatrix);

			Renderer::SubmitMesh(dc.Mesh, dc.Transform, nullptr);
		}
		Renderer::EndRenderPass();
	}

	void SceneRenderer::EntityIDPass()
	{
		Renderer::BeginRenderPass(s_SceneRendererData.EntityIDPass);

		auto& sceneCamera = s_SceneRendererData.SceneData.SceneCamera;

		auto viewProjection = sceneCamera.Camera.GetProjection() * sceneCamera.ViewMatrix;
		glm::vec3 cameraPosition = glm::inverse(s_SceneRendererData.SceneData.SceneCamera.ViewMatrix)[3]; // TODO: Negate instead

		// Render entities
		for (auto& dc : s_SceneRendererData.EntityIDDrawList)
		{
			// Get overriden material 
			auto& overrideMaterial = dc.Material;

			// Set values
			overrideMaterial->Set("u_ViewProjectionMatrix", viewProjection);
			overrideMaterial->Set("u_ViewMatrix", sceneCamera.ViewMatrix);

			Renderer::SubmitMesh(dc.Mesh, dc.Transform, overrideMaterial);
		}
		Renderer::EndRenderPass();
	}

}