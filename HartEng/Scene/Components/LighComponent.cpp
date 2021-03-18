#include "HartEng/Scene/Components/LightComponent.h"

namespace HE
{
	LightComponent::LightComponent(Entity* entityHandle)
	{
		m_EntityHandle = entityHandle;
	}
	void LightComponent::Copy(Component* other_base)
	{
		LightComponent* other = dynamic_cast<LightComponent*>(other_base);
		m_LightType = other->m_LightType;
		m_CastShadow = other->m_CastShadow;

		m_Direction = other->m_Direction;
		m_Color = other->m_Color;
		m_Intensity = other->m_Intensity;
		m_Range = other->m_Range;
		m_InnerConeAngle = other->m_InnerConeAngle;
		m_OuterConeAngle = other->m_OuterConeAngle;
	}
	void LightComponent::SetLightType(const LightType& type)
	{
		m_LightType = type;
	}
	void LightComponent::SetCastShadow(bool castShadow)
	{
		m_CastShadow = castShadow;
	}
	void LightComponent::SetDirection(const glm::vec3& direction)
	{
		m_Direction = direction;
	}
	void LightComponent::SetColor(const glm::vec3& color)
	{
		m_Color = color;
	}
	void LightComponent::SetIntensity(float intensity)
	{
		m_Intensity = intensity;
	}
	void LightComponent::SetRange(float range)
	{
		m_Range = range;
	}
	void LightComponent::SetInnerConeAngle(float angle)
	{
		m_InnerConeAngle = angle;
	}
	void LightComponent::SetOuterConeAngle(float angle)
	{
		m_OuterConeAngle = angle;
	}

	const LightType& LightComponent::GetLightType() const
	{
		return m_LightType;
	}
	bool LightComponent::GetCastShadow() const
	{
		return m_CastShadow;
	}
	const glm::vec3& LightComponent::GetDirection() const
	{
		return m_Direction;
	}
	const glm::vec3& LightComponent::GetColor() const
	{
		return m_Color;
	}
	float LightComponent::GetIntensity() const
	{
		return m_Intensity;
	}
	float LightComponent::GetRange() const
	{
		return m_Range;
	}
	float LightComponent::GetInnerConeAngle() const
	{
		return m_InnerConeAngle;
	}
	float LightComponent::GetOuterConeAngle() const
	{
		return m_OuterConeAngle;
	}
}