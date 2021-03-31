#pragma once

#include "HartEng/Renderer/Framebuffer.h"

namespace HE
{
	struct RenderPassSpecification
	{
		std::shared_ptr<FrameBuffer> TargetFramebuffer;
	};
	class RenderPass
	{
	public:
		virtual ~RenderPass() = default;

		virtual RenderPassSpecification& GetSpecification() = 0;
		virtual const RenderPassSpecification& GetSpecification() const = 0;

		static std::shared_ptr<RenderPass> Create(const RenderPassSpecification& spec);

	};

}