#include "HartEng/Renderer/RenderPass.h"
#include "HartEng/Renderer/Renderer.h"
#include "HartEng/Platform/OpenGL/OpenGLRenderPass.h"

namespace HE
{
	std::shared_ptr<RenderPass> RenderPass::Create(const RenderPassSpecification& spec)
	{
        RendererAPI::API api = Renderer::GetAPI();
        if (api == RendererAPI::API::OpenGL)
        {
            return std::shared_ptr<OpenGLRenderPass>(new OpenGLRenderPass(spec));
        }
        else if (api == RendererAPI::API::None)
        {
            HE_CORE_ASSERT(false, "RendererAPI::None currently not supported!");
            return nullptr;
        }
        HE_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
	}
}