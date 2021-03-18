#include "HartEng/Physics/PhysicsActor.h"

#include "HartEng/Scene/Components/CollidersComponent.h"
#include "HartEng/Scene/Components/TransformComponent.h"
#include "HartEng/Scene/Components/ScriptComponent.h"
#include "HartEng/Physics/PXPhysicsWrappers.h"
#include "HartEng/Physics/PhysicsLayer.h"
#include "HartEng/Physics/PhysicsUtils.h"

#include <glm/gtx/compatibility.hpp>

namespace HE
{
	PhysicsActor::PhysicsActor(Entity entity): 
		m_Entity(entity),
		m_RigidBody(*entity.GetComponent<RigidBodyComponent>())
	{
		Initialize();
	}

	PhysicsActor::~PhysicsActor()
	{
		if (m_ActorInternal)
		{
			if (m_ActorInternal->isReleasable())
			{
				m_ActorInternal->release();
				//m_ActorInternal = nullptr;
			}
		}
	}

	glm::vec3 PhysicsActor::GetPosition()
	{
		return FromPhysXVector(m_ActorInternal->getGlobalPose().p);
	}

	glm::quat PhysicsActor::GetRotation()
	{
		return FromPhysXQuat(m_ActorInternal->getGlobalPose().q);
	}

	void PhysicsActor::SetPosition(const glm::vec3 position)
	{
		physx::PxTransform transform = m_ActorInternal->getGlobalPose();
		transform.p = ToPhysXVector(position);
		m_ActorInternal->setGlobalPose(transform);
	}

	void PhysicsActor::SetRotation(const glm::quat& rotation)
	{
		physx::PxTransform transform = m_ActorInternal->getGlobalPose();
		transform.q = ToPhysXQuat(rotation);
		m_ActorInternal->setGlobalPose(transform);
	}

	void PhysicsActor::Rotate(const glm::vec3& rotation)
	{
		physx::PxTransform transform = m_ActorInternal->getGlobalPose();
		transform.q *= (physx::PxQuat(glm::radians(rotation.x), { 1.0f, 0.0f, 0.0f })
			* physx::PxQuat(glm::radians(rotation.y), { 0.0f, 1.0f, 0.0f })
			* physx::PxQuat(glm::radians(rotation.z), { 0.0f, 0.0f, 1.0f }));
		m_ActorInternal->setGlobalPose(transform);
	}

	float PhysicsActor::GetMass() const
	{
		if (!IsDynamic())
		{
			HE_CORE_WARN("Trying to set mass of non-dynamic PhysicsActor.");
			return 0.0F;
		}

		physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*)m_ActorInternal;
		return actor->getMass();
	}

	void PhysicsActor::SetMass(float mass)
	{
		if (!IsDynamic())
		{
			HE_CORE_WARN("Trying to set mass of non-dynamic PhysicsActor.");
			return;
		}

		physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*)m_ActorInternal;
		physx::PxRigidBodyExt::setMassAndUpdateInertia(*actor, mass);
		m_RigidBody.SetMass(mass);
	}

	void PhysicsActor::AddForce(const glm::vec3& force, ForceMode forceMode)
	{
		if (!IsDynamic())
		{
			HE_CORE_WARN("Trying to add force to non-dynamic PhysicsActor.");
			return;
		}

		physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*)m_ActorInternal;
		actor->addForce(ToPhysXVector(force), (physx::PxForceMode::Enum)forceMode);
	}

	void PhysicsActor::AddTorque(const glm::vec3& torque, ForceMode forceMode)
	{
		if (!IsDynamic())
		{
			HE_CORE_WARN("Trying to add torque to non-dynamic PhysicsActor.");
			return;
		}

		physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*)m_ActorInternal;
		actor->addTorque(ToPhysXVector(torque), (physx::PxForceMode::Enum)forceMode);
	}

	glm::vec3 PhysicsActor::GetLinearVelocity() const
	{
		if (!IsDynamic())
		{
			HE_CORE_WARN("Trying to get velocity of non-dynamic PhysicsActor.");
			return glm::vec3(0.0F);
		}

		physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*)m_ActorInternal;
		return FromPhysXVector(actor->getLinearVelocity());
	}

	void PhysicsActor::SetLinearVelocity(const glm::vec3& velocity)
	{
		if (!IsDynamic())
		{
			HE_CORE_WARN("Trying to set velocity of non-dynamic PhysicsActor.");
			return;
		}

		if (!glm::all(glm::isfinite(velocity)))
			return;

		physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*)m_ActorInternal;
		actor->setLinearVelocity(ToPhysXVector(velocity));
	}

	glm::vec3 PhysicsActor::GetAngularVelocity() const
	{
		if (!IsDynamic())
		{
			HE_CORE_WARN("Trying to get angular velocity of non-dynamic PhysicsActor.");
			return glm::vec3(0.0F);
		}

		physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*)m_ActorInternal;
		return FromPhysXVector(actor->getAngularVelocity());
	}

	void PhysicsActor::SetAngularVelocity(const glm::vec3& velocity)
	{
		if (!IsDynamic())
		{
			HE_CORE_WARN("Trying to set angular velocity of non-dynamic PhysicsActor.");
			return;
		}

		if (!glm::all(glm::isfinite(velocity)))
			return;

		physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*)m_ActorInternal;
		actor->setAngularVelocity(ToPhysXVector(velocity));
	}

	void PhysicsActor::SetLinearDrag(float drag) const
	{
		if (!IsDynamic())
			return;

		physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*)m_ActorInternal;
		actor->setLinearDamping(drag);
	}

	void PhysicsActor::SetAngularDrag(float drag) const
	{
		if (!IsDynamic())
			return;

		physx::PxRigidDynamic* actor = (physx::PxRigidDynamic*)m_ActorInternal;
		actor->setAngularDamping(drag);
	}

	void PhysicsActor::SetLayer(uint32_t layerId)
	{
		physx::PxAllocatorCallback& allocator = PXPhysicsWrappers::GetAllocator();
		const PhysicsLayer& layerInfo = PhysicsLayerManager::GetLayer(layerId);

		if (layerInfo.CollidesWith == 0)
			return;

		physx::PxFilterData filterData;
		filterData.word0 = layerInfo.BitValue;
		filterData.word1 = layerInfo.CollidesWith;

		const physx::PxU32 numShapes = m_ActorInternal->getNbShapes();
		physx::PxShape** shapes = (physx::PxShape**)allocator.allocate(sizeof(physx::PxShape*) * numShapes, "", "", 0);
		m_ActorInternal->getShapes(shapes, numShapes);

		for (physx::PxU32 i = 0; i < numShapes; i++)
			shapes[i]->setSimulationFilterData(filterData);

		allocator.deallocate(shapes);
	}

	void PhysicsActor::Initialize()
	{
		physx::PxPhysics& physics = PXPhysicsWrappers::GetPhysics();

		//std::shared_ptr<Scene> scene = std::make_shared<Scene>(m_Entity.GetScene());
		
		glm::mat4 transform = m_Entity.GetComponent<TransformComponent>()->GetTransform();
		// TODO if tree system in involved -> scene.GetTransformRelativeToParent(m_Entity)

		if (m_RigidBody.GetBodyType() == RigidBodyComponent::Type::Static)
		{
			m_ActorInternal = physics.createRigidStatic(ToPhysXTransform(transform));
		}
		else
		{
			const PhysicsSettings& settings = Physics::GetSettings();

			physx::PxRigidDynamic* actor = physics.createRigidDynamic(ToPhysXTransform(transform));
			actor->setLinearDamping(m_RigidBody.GetLinearDrag());
			actor->setAngularDamping(m_RigidBody.GetAngularDrag());
			actor->setRigidBodyFlag(physx::PxRigidBodyFlag::eKINEMATIC, m_RigidBody.GetIsKinematic());
			actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_X, m_RigidBody.GetLockPositionX());
			actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Y, m_RigidBody.GetLockPositionY());
			actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_LINEAR_Z, m_RigidBody.GetLockPositionZ());
			actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_X, m_RigidBody.GetLockRotationX());
			actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Y, m_RigidBody.GetLockRotationY());
			actor->setRigidDynamicLockFlag(physx::PxRigidDynamicLockFlag::eLOCK_ANGULAR_Z, m_RigidBody.GetLockRotationZ());
			actor->setActorFlag(physx::PxActorFlag::eDISABLE_GRAVITY, m_RigidBody.GetDisableGravity());
			actor->setSolverIterationCounts(settings.SolverIterations, settings.SolverVelocityIterations);

			physx::PxRigidBodyExt::setMassAndUpdateInertia(*actor, m_RigidBody.GetMass());
			m_ActorInternal = actor;
		}

		if (m_Entity.HasComponent<BoxColliderComponent>()) PXPhysicsWrappers::AddBoxCollider(*this);
		if (m_Entity.HasComponent<SphereColliderComponent>()) PXPhysicsWrappers::AddSphereCollider(*this);
		if (m_Entity.HasComponent<CapsuleColliderComponent>()) PXPhysicsWrappers::AddCapsuleCollider(*this);
		if (m_Entity.HasComponent<MeshColliderComponent>()) PXPhysicsWrappers::AddMeshCollider(*this);

		if (!PhysicsLayerManager::IsLayerValid(m_RigidBody.GetLayer()))
			m_RigidBody.SetLayer(0);

		SetLayer(m_RigidBody.GetLayer());
		m_ActorInternal->userData = &m_Entity;
	}

	void PhysicsActor::Spawn()
	{
		((physx::PxScene*)Physics::GetPhysicsScene())->addActor(*m_ActorInternal);
	}

	void PhysicsActor::Update(float fixedTimestep)
	{
		// TODO maybe add OnPhysicsUpdate to entity script
		if (m_Entity.HasComponent<ScriptComponent>())
		{
			m_Entity.GetComponent<ScriptComponent>()->OnPhysicsUpdate(fixedTimestep);
		}
		
	}

	void PhysicsActor::SynchronizeTransform()
	{
		if (IsDynamic())
		{
			auto transform = m_Entity.GetComponent<TransformComponent>();
			physx::PxTransform actorPose = m_ActorInternal->getGlobalPose();
			transform->SetPosition(FromPhysXVector(actorPose.p));
			transform->SetRotation(glm::degrees(glm::eulerAngles(FromPhysXQuat(actorPose.q))));
		}
		else
		{
			// Synchronize Physics Actor with static Entity
			// std::shared_ptr<Scene> scene = std::make_shared<Scene>(m_Entity.GetScene());
			// TODO if tree system in involved -> scene.GetTransformRelativeToParent(m_Entity)
			m_ActorInternal->setGlobalPose(ToPhysXTransform(m_Entity.GetComponent<TransformComponent>()->GetTransform()));
		}
	}

	void PhysicsActor::AddCollisionShape(physx::PxShape* shape)
	{
		bool status = m_ActorInternal->attachShape(*shape);
		shape->release();
		if (!status)
			shape = nullptr;
	}
}