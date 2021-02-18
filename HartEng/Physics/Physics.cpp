#include "HartEng/Physics/Physics.h"

#include "HartEng/Physics/PXPhysicsWrappers.h"
#include "HartEng/Physics/PhysicsLayer.h"
#include "HartEng/Physics/PhysicsActor.h"

#include <PhysX/extensions/PxBroadPhaseExt.h>
#include <PhysX/PxPhysicsAPI.h>

namespace HE
{
	static physx::PxScene* s_Scene;
	static std::vector<std::shared_ptr<PhysicsActor>> s_Actors;
	static float s_SimulationTime = 0.0F;

	static PhysicsSettings s_Settings;

	void Physics::Init()
	{
		PXPhysicsWrappers::Initialize();
		PhysicsLayerManager::AddLayer("Default");
	}

	void Physics::Shutdown()
	{
		PXPhysicsWrappers::Shutdown();
	}

	void Physics::CreateScene() 
	{
		HE_CORE_ASSERT(s_Scene == nullptr, "Scene already has a Physics Scene!");
		s_Scene = PXPhysicsWrappers::CreateScene();

		if (s_Settings.BroadphaseAlgorithm != BroadphaseType::AutomaticBoxPrune)
		{
			physx::PxBounds3* regionBounds;
			physx::PxBounds3 globalBounds(ToPhysXVector(s_Settings.WorldBoundsMin), ToPhysXVector(s_Settings.WorldBoundsMax));
			uint32_t regionCount = physx::PxBroadPhaseExt::createRegionsFromWorldBounds(regionBounds, globalBounds, s_Settings.WorldBoundsSubdivisions);

			for (uint32_t i = 0; i < regionCount; i++)
			{
				physx::PxBroadPhaseRegion region;
				region.bounds = regionBounds[i];
				s_Scene->addBroadPhaseRegion(region);
			}
		}
	}

	std::shared_ptr<PhysicsActor> Physics::CreateActor(const Entity& entity)
	{
		HE_CORE_ASSERT(s_Scene, "PhysX scene is NULL");
		auto actor = std::make_shared<PhysicsActor>(entity);
		s_Actors.push_back(actor);
		actor->Spawn();
		return actor;
	}

	std::shared_ptr<PhysicsActor> Physics::GetActorForEntity(const Entity& entity)
	{
		for (auto& actor : s_Actors)
		{
			if (actor->GetEntity() == entity)
			{
				return actor;
			}
				
		}

		return nullptr;
	}

	void Physics::Simulate(Timestep ts) 
	{
		s_SimulationTime += ts.GetMilliseconds();

		if (s_SimulationTime < s_Settings.FixedTimestep)
			return;

		s_SimulationTime -= s_Settings.FixedTimestep;

		for (auto& actor : s_Actors)
		{
			actor->Update(s_Settings.FixedTimestep);
		}
			

		s_Scene->simulate(s_Settings.FixedTimestep);
		s_Scene->fetchResults(true);

		for (auto& actor : s_Actors)
		{
			actor->SynchronizeTransform();
		}
			
	}

	void Physics::DestroyScene()
	{
		HE_CORE_ASSERT(s_Scene, "PhysX scene is nullptr");

		s_Actors.clear();
		s_Scene->release();
		s_Scene = nullptr;
	}

	void* Physics::GetPhysicsScene()
	{
		return s_Scene;
	}

	PhysicsSettings& Physics::GetSettings()
	{
		return s_Settings;
	}
}