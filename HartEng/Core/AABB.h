#pragma once

#include <GLM/glm.hpp>

namespace HE
{
	class AABB
	{
	private:
		// Min Max in local space
		glm::vec3 m_Min;
		glm::vec3 m_Max;

	public:
		AABB();
		AABB(const glm::vec3& min, const glm::vec3& max);

		// Return min and max in local space, need to multiply it by model matrix manually
		const glm::vec3& GetMin() const { return m_Min; }
		const glm::vec3& GetMax() const { return m_Max; }

		void SetMin(const glm::vec3& min) { m_Min = min; }
		void SetMax(const glm::vec3& max) { m_Max = max; }
		void SetMinMax(const glm::vec3& min, const glm::vec3& max) { m_Min = min; m_Max = max; }
	};
}