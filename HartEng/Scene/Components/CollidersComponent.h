#pragma once

#include "HartEng/Scene/Component.h"
#include "HartEng/Renderer/Mesh.h"
#include "HartEng/Asset/Assets.h"

namespace HE
{
	class BoxColliderComponent : public Component
	{
	private:
		glm::vec3 m_Size{ 1.0f };
		glm::vec3 m_Offset{ 0.0f };

		bool IsTrigger = false;

		std::shared_ptr<PhysicsMaterial> m_PhysicsMaterial;
		std::shared_ptr<Mesh> m_DebugMesh; // Mesh that will be drawn in editor to show collision bounds

	public:
		BoxColliderComponent(Entity* entityHandle);
		virtual ~BoxColliderComponent() = default;

		void SetSize(const glm::vec3& size)			{ m_Size = size; }
		void SetOffset(const glm::vec3& offset)		{ m_Offset = offset; }
		void SetTrigger(bool trigger)				{ IsTrigger = trigger; }
		void SetMesh(std::shared_ptr<Mesh> mesh)	{ m_DebugMesh = mesh; }
		void SetPhysicsMaterial(std::shared_ptr<PhysicsMaterial> physicsMaterial) { m_PhysicsMaterial = physicsMaterial; }

		const glm::vec3& GetSize() { return m_Size; }
		const glm::vec3& GetOffset() { return m_Offset; }
		bool GetTrigger() { return IsTrigger; }
		std::shared_ptr<Mesh> GetMesh() { return m_DebugMesh; }
		std::shared_ptr<PhysicsMaterial> GetPhysicsMaterial() { return m_PhysicsMaterial; }
	};



	class RigidBodyComponent: public Component
	{
	public:
		enum class Type
		{
			Static,
			Dynamic
		};
	private:
		Type m_BodyType;

		float m_Mass = 1.0f;
		float m_LinearDrag = 0.0f;
		float m_AngularDrag = 0.05f;

		bool m_DisableGravity = false;
		bool m_IsKinematic = false;

		uint32_t m_Layer = 0;

		bool m_LockPositionX = false;
		bool m_LockPositionY = false;
		bool m_LockPositionZ = false;
		bool m_LockRotationX = false;
		bool m_LockRotationY = false;
		bool m_LockRotationZ = false;

	public:
		RigidBodyComponent(Entity* entityHandle);
		virtual ~RigidBodyComponent() = default;


		void SetBodyType(Type type)					{ m_BodyType = type; }
		void SetMass(float mass)					{ m_Mass = mass; }
		void SetLinearDrag(float linearDrag)		{ m_LinearDrag = linearDrag; }
		void SetAngularDrag(float angularDrag)		{ m_AngularDrag = angularDrag; }
		void SetDisableGravity(bool disableGravity) { m_DisableGravity = disableGravity; }
		void SetIsKinematic(bool isKinematic)		{ m_IsKinematic = isKinematic; }
		void SetLayer(uint32_t layer)				{ m_Layer = layer; }
		void SetLockPositionX(bool lockPositionX)	{ m_LockPositionX = lockPositionX; }
		void SetLockPositionY(bool lockPositionY)	{ m_LockPositionY = lockPositionY; }
		void SetLockPositionZ(bool lockPositionZ)	{ m_LockPositionZ = lockPositionZ; }
		void SetLockRotationX(bool lockRotationX)	{ m_LockRotationX = lockRotationX; }
		void SetLockRotationY(bool lockRotationY)	{ m_LockRotationY = lockRotationY; }
		void SetLockRotationZ(bool lockRotationZ)	{ m_LockRotationZ = lockRotationZ; }
		
		Type GetBodyType() { return m_BodyType; }
		float GetMass() { return m_Mass; }
		float GetLinearDrag() { return m_LinearDrag; }
		float GetAngularDrag() { return m_AngularDrag; }
		bool GetDisableGravity() { return m_DisableGravity; }
		bool GetIsKinematic() { return m_IsKinematic; }
		uint32_t GetLayer() { return m_Layer; }
		bool GetLockPositionX() { return m_LockPositionX; }
		bool GetLockPositionY() { return m_LockPositionY; }
		bool GetLockPositionZ() { return m_LockPositionZ; }
		bool GetLockRotationX() { return m_LockRotationX; }
		bool GetLockRotationY() { return m_LockRotationY; }
		bool GetLockRotationZ() { return m_LockRotationZ; }
	};
}