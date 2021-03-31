#include "HartEng/Core/AABB.h"

namespace HE
{
	AABB::AABB():
		m_Min(0.0f),
		m_Max(0.0f)
	{

	}

	AABB::AABB(const glm::vec3& min, const glm::vec3& max):
		m_Min(min),
		m_Max(max)
	{

	}
}
