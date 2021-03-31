#pragma once

#include "HartEng/Scene/Component.h"
#include <glm/glm.hpp>

namespace HE
{
	enum LightType
	{
		Directional,
		Point,
		Spot,
	};

	class LightComponent : public Component
	{
	private:
		LightType m_LightType = Point;
		bool m_CastShadow = false;

		glm::vec3 m_Direction{ 0.0f, 0.0f, 1.0f };
		glm::vec3 m_Color{ 1.0f };
		float m_Intensity = 1.0f;
		float m_Range = 0.0f;
		float m_InnerConeAngle = 0.0f;
		float m_OuterConeAngle = 0.0f;
	public:
		LightComponent(Entity* entityHandle);
		virtual ~LightComponent() = default;

		void Copy(Component* other_base) override;

		void SetLightType(const LightType& type);
		void SetCastShadow(bool castShadow);
		void SetDirection(const glm::vec3& direction);
		void SetColor(const glm::vec3& color);
		void SetIntensity(float intensity);
		void SetRange(float range);
		void SetInnerConeAngle(float angle);
		void SetOuterConeAngle(float angle);

		const LightType& GetLightType() const;
		bool GetCastShadow() const;
		const glm::vec3& GetDirection() const;
		const glm::vec3& GetColor() const;
		float GetIntensity() const;
		float GetRange() const;
		float GetInnerConeAngle() const;
		float GetOuterConeAngle() const;


	};

}