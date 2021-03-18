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

		bool m_IsTrigger = false;

		std::shared_ptr<PhysicsMaterial> m_PhysicsMaterial;
		std::shared_ptr<Mesh> m_DebugMesh; // Mesh that will be drawn in editor to show collision bounds

	public:
		BoxColliderComponent(Entity* entityHandle);
		virtual ~BoxColliderComponent() = default;

		void Copy(Component* other_base) override;

		void SetSize(const glm::vec3& size)			{ m_Size = size; }
		void SetOffset(const glm::vec3& offset)		{ m_Offset = offset; }
		void SetTrigger(bool trigger)				{ m_IsTrigger = trigger; }
		void SetMesh(const std::shared_ptr<Mesh>& mesh)	{ m_DebugMesh = mesh; }
		void SetPhysicsMaterial(const std::shared_ptr<PhysicsMaterial>& physicsMaterial) { m_PhysicsMaterial = physicsMaterial; }

		const glm::vec3& GetSize() { return m_Size; }
		const glm::vec3& GetOffset() { return m_Offset; }
		bool GetTrigger() { return m_IsTrigger; }
		std::shared_ptr<Mesh> GetMesh() { return m_DebugMesh; }
		std::shared_ptr<PhysicsMaterial> GetPhysicsMaterial() { return m_PhysicsMaterial; }
	};


	class SphereColliderComponent : public Component
	{
	private:
		float m_Radius = 0.5F;
		bool m_IsTrigger = false;

		std::shared_ptr<PhysicsMaterial> m_PhysicsMaterial;
		std::shared_ptr<Mesh> m_DebugMesh;

	public:
		SphereColliderComponent(Entity* entityHandle);
		virtual ~SphereColliderComponent() = default;

		void Copy(Component* other_base) override;

		void SetRadius(float radius) { m_Radius = radius; }
		void SetTrigger(bool trigger) { m_IsTrigger = trigger; }
		void SetMesh(const std::shared_ptr<Mesh>& mesh) { m_DebugMesh = mesh; }
		void SetPhysicsMaterial(const std::shared_ptr<PhysicsMaterial>& physicsMaterial) { m_PhysicsMaterial = physicsMaterial; }

		float GetRadius() { return m_Radius; }
		bool GetTrigger() { return m_IsTrigger; }
		std::shared_ptr<Mesh> GetMesh() { return m_DebugMesh; }
		std::shared_ptr<PhysicsMaterial> GetPhysicsMaterial() { return m_PhysicsMaterial; }
	};

	class CapsuleColliderComponent : public Component
	{
	private:
		float m_Radius = 0.5F;
		float m_Height = 1.0f;
		bool m_IsTrigger = false;

		std::shared_ptr<PhysicsMaterial> m_PhysicsMaterial;
		std::shared_ptr<Mesh> m_DebugMesh;

	public:
		CapsuleColliderComponent(Entity* entityHandle);
		virtual ~CapsuleColliderComponent() = default;

		void Copy(Component* other_base) override;

		void SetRadius(float radius) { m_Radius = radius; }
		void SetHeight(float height) { m_Height = height; }
		void SetTrigger(bool trigger) { m_IsTrigger = trigger; }
		void SetMesh(const std::shared_ptr<Mesh>& mesh) { m_DebugMesh = mesh; }
		void SetPhysicsMaterial(const std::shared_ptr<PhysicsMaterial>& physicsMaterial) { m_PhysicsMaterial = physicsMaterial; }

		float GetRadius() { return m_Radius; }
		float GetHeight() { return m_Height; }
		bool GetTrigger() { return m_IsTrigger; }
		std::shared_ptr<Mesh> GetMesh() { return m_DebugMesh; }
		std::shared_ptr<PhysicsMaterial> GetPhysicsMaterial() { return m_PhysicsMaterial; }
	};

	class MeshColliderComponent : public Component
	{
	private:
		std::shared_ptr<Mesh> m_CollisionMesh;
		std::vector<std::shared_ptr<Mesh>> m_ProcessedMeshes;
		bool m_IsConvex = false;
		bool m_IsTrigger = false;
		bool m_OverrideMesh = false;
		std::shared_ptr<PhysicsMaterial> m_PhysicsMaterial;

	public:
		MeshColliderComponent(Entity* entityHandle);
		virtual ~MeshColliderComponent() = default;

		void Copy(Component* other_base) override;

		void SetCollisionMesh(std::shared_ptr<Mesh> collisionMesh);
		void SetProcessedMeshes(const std::vector<std::shared_ptr<Mesh>>& processedMeshes) { m_ProcessedMeshes = processedMeshes; }
		void SetConvex(bool convex) { m_IsConvex = convex; }
		void SetTrigger(bool trigger) { m_IsTrigger = trigger; }
		void SetOverrideMesh(bool overrideMesh) { m_OverrideMesh = overrideMesh; }
		void SetPhysicsMaterial(const std::shared_ptr<PhysicsMaterial>& physicsMaterial) { m_PhysicsMaterial = physicsMaterial; }

		std::shared_ptr<Mesh> GetCollisionMesh() { return m_CollisionMesh; }
		std::vector<std::shared_ptr<Mesh>>& GetProcessedMeshes() { return m_ProcessedMeshes; }
		bool GetConvex() { return m_IsConvex; }
		bool GetTrigger() { return m_IsTrigger; }
		bool GetOverrideMesh() { return m_OverrideMesh; }
		std::shared_ptr<PhysicsMaterial> GetPhysicsMaterial() { return m_PhysicsMaterial; }

		operator std::shared_ptr<Mesh>() { return m_CollisionMesh; }
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
		Type m_BodyType = Type::Static;

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

		void Copy(Component* other_base) override;

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