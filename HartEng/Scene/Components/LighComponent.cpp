#include "HartEng/Scene/Components/LightComponent.h"

namespace HE
{
	LightComponent::LightComponent(Entity* entityHandle)
	{
		m_EntityHandle = entityHandle;
		m_Type = ComponentType::LightComponent;

	}
	void LightComponent::SetLightType(const LightType& type)
	{
		m_LightType = type;
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
		m_InnerConeAngle = glm::cos(glm::radians(angle));
	}
	void LightComponent::SetOuterConeAngle(float angle)
	{
		m_OuterConeAngle = glm::cos(glm::radians(angle));
	}

	const LightType& LightComponent::GetLightType() const
	{
		return m_LightType;
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