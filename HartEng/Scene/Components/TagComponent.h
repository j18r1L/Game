#pragma once

#include "HartEng/Scene/Component.h"

namespace HE
{
	class TagComponent : public Component
	{
	private:
		std::string m_Tag;
	public:
		TagComponent(Entity* entityHandle);
		virtual ~TagComponent() = default;

		void SetTag(const std::string& tag) { m_Tag = tag; }
		const std::string& GetTag() { return m_Tag; };
	};
}