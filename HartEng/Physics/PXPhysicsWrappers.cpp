#include "HartEng/Physics/PXPhysicsWrappers.h"



#include "HartEng/Scene/Components/CollidersComponent.h"
#include "HartEng/Scene/Components/ScriptComponent.h"
#include "HartEng/Physics/PhysicsActor.h"
#include "HartEng/Physics/Physics.h"
#include "HartEng/Asset/Assets.h"

namespace HE
{
	static PhysicsErrorCallback s_ErrorCallback;
	static PhysicsAssertHandler s_AssertHandler;
	static physx::PxDefaultAllocator s_Allocator;
	static physx::PxFoundation* s_Foundation;
	static physx::PxPvd* s_PVD;
	static physx::PxPhysics* s_Physics;
	static physx::PxCooking* s_CookingFactory;
	static physx::PxOverlapHit s_OverlapBuffer[OVERLAP_MAX_COLLIDERS];

	static ContactListener s_ContactListener;


	static physx::PxBroadPhaseType::Enum HazelToPhysXBroadphaseType(BroadphaseType type)
	{
		switch (type)
		{
		case HE::BroadphaseType::SweepAndPrune: return physx::PxBroadPhaseType::eSAP;
		case HE::BroadphaseType::MultiBoxPrune: return physx::PxBroadPhaseType::eMBP;
		case HE::BroadphaseType::AutomaticBoxPrune: return physx::PxBroadPhaseType::eABP;
		}

		return physx::PxBroadPhaseType::eABP;
	}

	static physx::PxFrictionType::Enum HazelToPhysXFrictionType(FrictionType type)
	{
		switch (type)
		{
		case HE::FrictionType::Patch:			return physx::PxFrictionType::ePATCH;
		case HE::FrictionType::OneDirectional:	return physx::PxFrictionType::eONE_DIRECTIONAL;
		case HE::FrictionType::TwoDirectional:	return physx::PxFrictionType::eTWO_DIRECTIONAL;
		}

		return physx::PxFrictionType::ePATCH;
	}
	
	////////////////////////// PXPhysicsWrapper ////////////////////////////

	physx::PxScene* PXPhysicsWrappers::CreateScene()
	{
		physx::PxSceneDesc sceneDesc(s_Physics->getTolerancesScale());

		const PhysicsSettings& settings = Physics::GetSettings();

		sceneDesc.gravity = ToPhysXVector(settings.Gravity);
		sceneDesc.broadPhaseType = HazelToPhysXBroadphaseType(settings.BroadphaseAlgorithm);
		sceneDesc.cpuDispatcher = physx::PxDefaultCpuDispatcherCreate(1);
		sceneDesc.filterShader = HEFilterShader;
		sceneDesc.simulationEventCallback = &s_ContactListener;
		sceneDesc.frictionType = HazelToPhysXFrictionType(settings.FrictionModel);

		HE_CORE_ASSERT(sceneDesc.isValid(), "PhysX SceneDescription (PhysicsSettings) is not valid!");
		return s_Physics->createScene(sceneDesc);
	}

	void PXPhysicsWrappers::AddBoxCollider(PhysicsActor& actor)
	{
		auto collider = actor.GetEntity().GetComponent<BoxColliderComponent>();

		if (!collider->GetPhysicsMaterial())
			collider->SetPhysicsMaterial(std::make_shared<PhysicsMaterial>(0.6F, 0.6F, 0.0F));

		glm::vec3 size = actor.GetEntity().GetComponent<TransformComponent>()->GetScale();
		glm::vec3 colliderSize = collider->GetSize();

		if (size.x != 0.0F) colliderSize.x *= size.x;
		if (size.y != 0.0F) colliderSize.y *= size.y;
		if (size.z != 0.0F) colliderSize.z *= size.z;

		physx::PxBoxGeometry boxGeometry = physx::PxBoxGeometry(colliderSize.x / 2.0F, colliderSize.y / 2.0F, colliderSize.z / 2.0F);
		physx::PxMaterial* material = s_Physics->createMaterial(collider->GetPhysicsMaterial()->StaticFriction, collider->GetPhysicsMaterial()->DynamicFriction, collider->GetPhysicsMaterial()->Bounciness);
		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(*actor.m_ActorInternal, boxGeometry, *material);
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !collider->GetTrigger());
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, collider->GetTrigger());
		shape->setLocalPose(ToPhysXTransform(glm::translate(glm::mat4(1.0F), collider->GetOffset())));
	}

	bool PXPhysicsWrappers::OverlapBox(const glm::vec3& origin, const glm::vec3& halfSize, std::array<physx::PxOverlapHit, OVERLAP_MAX_COLLIDERS>& buffer, uint32_t* count)
	{
		physx::PxScene* scene = static_cast<physx::PxScene*>(Physics::GetPhysicsScene());

		memset(s_OverlapBuffer, 0, sizeof(s_OverlapBuffer));
		physx::PxOverlapBuffer buf(s_OverlapBuffer, OVERLAP_MAX_COLLIDERS);
		physx::PxBoxGeometry geometry = physx::PxBoxGeometry(halfSize.x, halfSize.y, halfSize.z);
		physx::PxTransform pose = ToPhysXTransform(glm::translate(glm::mat4(1.0F), origin));

		bool result = scene->overlap(geometry, pose, buf);

		if (result)
		{
			uint32_t bodyCount = buf.nbTouches >= OVERLAP_MAX_COLLIDERS ? OVERLAP_MAX_COLLIDERS : buf.nbTouches;
			memcpy(buffer.data(), buf.touches, bodyCount * sizeof(physx::PxOverlapHit));
			*count = bodyCount;
		}

		return result;
	}


	void PXPhysicsWrappers::Initialize()
	{
		HE_CORE_ASSERT(!s_Foundation, "PXPhysicsWrappers::Initializer shouldn't be called more than once!");

		s_Foundation = PxCreateFoundation(PX_PHYSICS_VERSION, s_Allocator, s_ErrorCallback);
		HE_CORE_ASSERT(s_Foundation, "PxCreateFoundation Failed!");

		s_PVD = PxCreatePvd(*s_Foundation);
		if (s_PVD)
		{
			physx::PxPvdTransport* transport = physx::PxDefaultPvdSocketTransportCreate("localhost", 5425, 10);
			s_PVD->connect(*transport, physx::PxPvdInstrumentationFlag::eALL);
		}

		physx::PxTolerancesScale scale = physx::PxTolerancesScale();
		scale.length = 10;
		s_Physics = PxCreatePhysics(PX_PHYSICS_VERSION, *s_Foundation, scale, true, s_PVD);
		HE_CORE_ASSERT(s_Physics, "PxCreatePhysics Failed!");

		s_CookingFactory = PxCreateCooking(PX_PHYSICS_VERSION, *s_Foundation, s_Physics->getTolerancesScale());
		HE_CORE_ASSERT(s_CookingFactory, "PxCreatePhysics Failed!");

		PxSetAssertHandler(s_AssertHandler);
	}

	void PXPhysicsWrappers::Shutdown()
	{
		s_CookingFactory->release();
		s_Physics->release();
		s_Foundation->release();
	}

	physx::PxPhysics& PXPhysicsWrappers::GetPhysics()
	{
		return *s_Physics;
	}

	physx::PxAllocatorCallback& PXPhysicsWrappers::GetAllocator()
	{
		return s_Allocator;
	}




	/////////////////////////////////////// CONTACT LISTENER //////////////////////////////////////

	void ContactListener::onConstraintBreak(physx::PxConstraintInfo* constraints, physx::PxU32 count)
	{
		PX_UNUSED(constraints);
		PX_UNUSED(count);
	}

	void ContactListener::onWake(physx::PxActor** actors, physx::PxU32 count)
	{
		for (uint32_t i = 0; i < count; i++)
		{
			physx::PxActor& actor = *actors[i];
			Entity& entity = *(Entity*)actor.userData;

			HE_CORE_INFO("PhysX Actor waking up: ID: {0}, Name: {1}", entity.GetID(), entity.GetName());
		}
	}

	void ContactListener::onSleep(physx::PxActor** actors, physx::PxU32 count)
	{
		for (uint32_t i = 0; i < count; i++)
		{
			physx::PxActor& actor = *actors[i];
			Entity& entity = *(Entity*)actor.userData;

			HE_CORE_INFO("PhysX Actor going to sleep: ID: {0}, Name: {1}", entity.GetID(), entity.GetName());
		}
	}

	void ContactListener::onContact(const physx::PxContactPairHeader& pairHeader, const physx::PxContactPair* pairs, physx::PxU32 nbPairs)
	{
		Entity& a = *(Entity*)pairHeader.actors[0]->userData;
		Entity& b = *(Entity*)pairHeader.actors[1]->userData;

		if (pairs->flags == physx::PxContactPairFlag::eACTOR_PAIR_HAS_FIRST_TOUCH)
		{
			if (a.HasComponent<ScriptComponent>())
			{
				a.GetComponent<ScriptComponent>()->OnCollisionBegin(b);
			}
			if (b.HasComponent<ScriptComponent>())
			{
				b.GetComponent<ScriptComponent>()->OnCollisionBegin(a);
			}
		}
		else if (pairs->flags == physx::PxContactPairFlag::eACTOR_PAIR_LOST_TOUCH)
		{
			if (a.HasComponent<ScriptComponent>())
			{
				a.GetComponent<ScriptComponent>()->OnCollisionEnd(b);
			}
			if (b.HasComponent<ScriptComponent>())
			{
				b.GetComponent<ScriptComponent>()->OnCollisionEnd(a);
			}
		}
	}

	void ContactListener::onTrigger(physx::PxTriggerPair* pairs, physx::PxU32 count)
	{
		Entity& a = *(Entity*)pairs->triggerActor->userData;
		Entity& b = *(Entity*)pairs->otherActor->userData;

		if (pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_FOUND)
		{
			if (a.HasComponent<ScriptComponent>())
			{
				a.GetComponent<ScriptComponent>()->OnTriggerBegin(b);
			}
			if (b.HasComponent<ScriptComponent>())
			{
				b.GetComponent<ScriptComponent>()->OnTriggerBegin(a);
			}
		}
		else if (pairs->status == physx::PxPairFlag::eNOTIFY_TOUCH_LOST)
		{
			if (a.HasComponent<ScriptComponent>())
			{
				a.GetComponent<ScriptComponent>()->OnTriggerEnd(b);
			}
			if (b.HasComponent<ScriptComponent>())
			{
				b.GetComponent<ScriptComponent>()->OnTriggerEnd(a);
			}
		}
	}

	void ContactListener::onAdvance(const physx::PxRigidBody* const* bodyBuffer, const physx::PxTransform* poseBuffer, const physx::PxU32 count)
	{
		PX_UNUSED(bodyBuffer);
		PX_UNUSED(poseBuffer);
		PX_UNUSED(count);
	}

	//////////////////////////////////// PHYSX ERROR CALLBACK //////////////////////////////////////

	void PhysicsErrorCallback::reportError(physx::PxErrorCode::Enum code, const char* message, const char* file, int line)
	{
		const char* errorMessage = NULL;

		switch (code)
		{
		case physx::PxErrorCode::eNO_ERROR:				errorMessage = "No Error"; break;
		case physx::PxErrorCode::eDEBUG_INFO:			errorMessage = "Info"; break;
		case physx::PxErrorCode::eDEBUG_WARNING:		errorMessage = "Warning"; break;
		case physx::PxErrorCode::eINVALID_PARAMETER:	errorMessage = "Invalid Parameter"; break;
		case physx::PxErrorCode::eINVALID_OPERATION:	errorMessage = "Invalid Operation"; break;
		case physx::PxErrorCode::eOUT_OF_MEMORY:		errorMessage = "Out Of Memory"; break;
		case physx::PxErrorCode::eINTERNAL_ERROR:		errorMessage = "Internal Error"; break;
		case physx::PxErrorCode::eABORT:				errorMessage = "Abort"; break;
		case physx::PxErrorCode::ePERF_WARNING:			errorMessage = "Performance Warning"; break;
		case physx::PxErrorCode::eMASK_ALL:				errorMessage = "Unknown Error"; break;
		}

		switch (code)
		{
		case physx::PxErrorCode::eNO_ERROR:
		case physx::PxErrorCode::eDEBUG_INFO:
			HE_CORE_INFO("[PhysX]: {0}: {1} at {2} ({3})", errorMessage, message, file, line);
			break;
		case physx::PxErrorCode::eDEBUG_WARNING:
		case physx::PxErrorCode::ePERF_WARNING:
			HE_CORE_WARN("[PhysX]: {0}: {1} at {2} ({3})", errorMessage, message, file, line);
			break;
		case physx::PxErrorCode::eINVALID_PARAMETER:
		case physx::PxErrorCode::eINVALID_OPERATION:
		case physx::PxErrorCode::eOUT_OF_MEMORY:
		case physx::PxErrorCode::eINTERNAL_ERROR:
			HE_CORE_ERROR("[PhysX]: {0}: {1} at {2} ({3})", errorMessage, message, file, line);
			break;
		case physx::PxErrorCode::eABORT:
		case physx::PxErrorCode::eMASK_ALL:
			HE_CORE_ERROR("[PhysX]: {0}: {1} at {2} ({3})", errorMessage, message, file, line);
			HE_CORE_ASSERT(false, "[PhysX]: eMASK_ALL");
			break;
		}
	}

	void PhysicsAssertHandler::operator()(const char* exp, const char* file, int line, bool& ignore)
	{
		HE_CORE_ERROR("[PhysX Error]: {0}:{1} - {2}", file, line, exp);

#if 0
		HZ_CORE_ASSERT(false);
#endif
	}
}