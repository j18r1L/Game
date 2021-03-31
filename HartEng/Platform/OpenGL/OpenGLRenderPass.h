#pragma once

#include "HartEng/Renderer/RenderPass.h"

namespace HE
{
	class OpenGLRenderPass : public RenderPass
	{
	private:
		RenderPassSpecification m_Specification;
	public:
		OpenGLRenderPass(const RenderPassSpecification& spec);
		virtual ~OpenGLRenderPass();

		RenderPassSpecification& GetSpecification() override;
		const RenderPassSpecification& GetSpecification() const override;
	};
}