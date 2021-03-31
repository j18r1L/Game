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

			LightEnvironment SceneLightEnvironment;
		} SceneData;
		
		// Geometry passes
		std::shared_ptr<RenderPass> GeoPass;
		std::shared_ptr<RenderPass> EntityIDPass;
		
		// Draw lists
		struct DrawCommand
		{
			std::shared_ptr<Mesh> Mesh;
			std::shared_ptr<MaterialInstance> Material;
			glm::mat4 Transform;
		};
		std::vector<DrawCommand> DrawList;
		std::vector<DrawCommand> EntityIDDrawList;
		std::vector<DrawCommand> ColliderDrawList;
		

		SceneRendererOptions Options;


		// Materials
		std::shared_ptr<MaterialInstance> ColliderMaterial;
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
			entityIdFramebufferSpec.Attachments = { FramebufferTextureFormat::R32I, FramebufferTextureFormat::DEPTH24STENCIL8 };
			entityIdFramebufferSpec.Samples = 1;
			entityIdFramebufferSpec.ClearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

			// EntityID renderpass specification
			RenderPassSpecification entityIDRenderPassSpec;
			entityIDRenderPassSpec.TargetFramebuffer = FrameBuffer::Create(entityIdFramebufferSpec);
			s_SceneRendererData.EntityIDPass = RenderPass::Create(entityIDRenderPassSpec);
		}


		// Create materials
		// Collider
		std::string pathToProject = CMAKE_PATH;
		auto colliderShader = Shader::Create(pathToProject + "/assets/shaders/Collider.glsl");
		s_SceneRendererData.ColliderMaterial = MaterialInstance::Create(Material::Create(colliderShader));
		s_SceneRendererData.ColliderMaterial->SetFlag(MaterialFlag::DepthTest, false);

		


		
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

		s_SceneRendererData.SceneData.SceneLightEnvironment = scene->GetLightEnvironment();
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

	void SceneRenderer::SubmitColliderMesh(std::shared_ptr<Mesh> mesh, const glm::mat4& transform, std::shared_ptr<MaterialInstance> overrideMaterial)
	{
		s_SceneRendererData.ColliderDrawList.push_back({ mesh, overrideMaterial, transform });
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

		// Maybe sort by distance from camera, frustum culling and so on here?

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
		s_SceneRendererData.ColliderDrawList.clear();
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
			// If changing mesh of meshCOmponent -> pointer to mesh in DrawList is nullptr
			if (dc.Mesh)
			{
				// Get base material from mesh
				auto& baseMaterial = dc.Mesh->GetMaterial();

				// Set values
				baseMaterial->Set("u_ViewProjectionMatrix", viewProjection);
				baseMaterial->Set("u_ViewMatrix", sceneCamera.ViewMatrix);

				// Set lights (TODO: move to light environment and don't do per mesh)
				baseMaterial->Set("u_DirectionalLights", s_SceneRendererData.SceneData.SceneLightEnvironment.DirectionalLights);
				baseMaterial->Set("u_PointLights", s_SceneRendererData.SceneData.SceneLightEnvironment.PointLights);
				baseMaterial->Set("u_SpotLights", s_SceneRendererData.SceneData.SceneLightEnvironment.SpotLights);

				Renderer::SubmitMesh(dc.Mesh, dc.Transform, nullptr);
			}
		}



		// Render colliders
		bool hasColliders = s_SceneRendererData.ColliderDrawList.size() > 0;
		if (hasColliders)
		{
			s_SceneRendererData.ColliderMaterial->Set("u_ViewProjection", viewProjection);
			s_SceneRendererData.ColliderMaterial->SetFlag(MaterialFlag::DepthTest, false);
			s_SceneRendererData.ColliderMaterial->SetFlag(MaterialFlag::LineMode, true);
			s_SceneRendererData.ColliderMaterial->SetFlag(MaterialFlag::TwoSided, true);
			for (auto& dc : s_SceneRendererData.ColliderDrawList)
			{
				if (dc.Mesh)
					Renderer::SubmitMesh(dc.Mesh, dc.Transform, s_SceneRendererData.ColliderMaterial);
			}
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
			if (dc.Mesh)
			{
				// Get overriden material 
				auto& overrideMaterial = dc.Material;

				// Set values
				overrideMaterial->Set("u_ViewProjectionMatrix", viewProjection);
				overrideMaterial->Set("u_ViewMatrix", sceneCamera.ViewMatrix);

				Renderer::SubmitMesh(dc.Mesh, dc.Transform, overrideMaterial);
			}
			
		}
		Renderer::EndRenderPass();
	}

}