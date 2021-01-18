#include "HartEng/Platform/OpenGL/OpenGLRenderPass.h"

namespace HE
{
	OpenGLRenderPass::OpenGLRenderPass(const RenderPassSpecification& spec):
		m_Specification(spec)
	{

	}

	OpenGLRenderPass::~OpenGLRenderPass()
	{

	}

	RenderPassSpecification& OpenGLRenderPass::GetSpecification()
	{
		return m_Specification;
	}

	const RenderPassSpecification& OpenGLRenderPass::GetSpecification() const
	{
		return m_Specification;
	}
}