#include "HartEng/Physics/PXPhysicsWrappers.h"



#include "HartEng/Scene/Components/ScriptComponent.h"
#include "HartEng/Physics/PhysicsActor.h"
#include "HartEng/Physics/Physics.h"
#include "HartEng/Asset/Assets.h"


#include <glm/gtx/matrix_decompose.hpp>

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

	void PXPhysicsWrappers::AddSphereCollider(PhysicsActor& actor)
	{
		auto collider = actor.GetEntity().GetComponent<SphereColliderComponent>();

		if (!collider->GetPhysicsMaterial())
			collider->SetPhysicsMaterial(std::make_shared<PhysicsMaterial>(0.6F, 0.6F, 0.0F));

		float colliderRadius = collider->GetRadius();
		glm::vec3 size = actor.GetEntity().GetComponent<TransformComponent>()->GetScale();
		if (size.x != 0.0f)
		{
			colliderRadius *= size.x;
		}
			

		physx::PxSphereGeometry sphereGeometry = physx::PxSphereGeometry(colliderRadius);
		physx::PxMaterial* material = s_Physics->createMaterial(collider->GetPhysicsMaterial()->StaticFriction, collider->GetPhysicsMaterial()->DynamicFriction, collider->GetPhysicsMaterial()->Bounciness);
		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(*actor.m_ActorInternal, sphereGeometry, *material);
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !collider->GetTrigger());
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, collider->GetTrigger());

	}

	void PXPhysicsWrappers::AddCapsuleCollider(PhysicsActor& actor)
	{
		auto collider = actor.GetEntity().GetComponent<CapsuleColliderComponent>();

		if (!collider->GetPhysicsMaterial())
			collider->SetPhysicsMaterial(std::make_shared<PhysicsMaterial>(0.6F, 0.6F, 0.0F));

		float colliderRadius = collider->GetRadius();
		float colliderHeight = collider->GetHeight();
		glm::vec3 size = actor.GetEntity().GetComponent<TransformComponent>()->GetScale();
		if (size.x != 0.0F) colliderRadius *= (size.x / 2.0F);
		if (size.y != 0.0F) colliderHeight *= size.y;

		physx::PxCapsuleGeometry capsuleGeometry = physx::PxCapsuleGeometry(colliderRadius, colliderHeight / 2.0F);
		physx::PxMaterial* material = s_Physics->createMaterial(collider->GetPhysicsMaterial()->StaticFriction, collider->GetPhysicsMaterial()->DynamicFriction, collider->GetPhysicsMaterial()->Bounciness);
		physx::PxShape* shape = physx::PxRigidActorExt::createExclusiveShape(*actor.m_ActorInternal, capsuleGeometry, *material);
		shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !collider->GetTrigger());
		shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, collider->GetTrigger());
		shape->setLocalPose(physx::PxTransform(physx::PxQuat(physx::PxHalfPi, physx::PxVec3(0, 0, 1))));

	}

	void PXPhysicsWrappers::AddMeshCollider(PhysicsActor& actor)
	{
		auto collider = actor.GetEntity().GetComponent<MeshColliderComponent>();

		if (!collider->GetPhysicsMaterial())
			collider->SetPhysicsMaterial(std::make_shared<PhysicsMaterial>(0.6F, 0.6F, 0.0F));

		glm::vec3 size = actor.m_Entity.GetComponent<TransformComponent>()->GetScale();
		physx::PxMaterial* material = s_Physics->createMaterial(collider->GetPhysicsMaterial()->StaticFriction, collider->GetPhysicsMaterial()->DynamicFriction, collider->GetPhysicsMaterial()->Bounciness);
		physx::PxMaterial* materials[] = { material };

		if (collider->GetConvex())
		{
			// Remove any potential triangle meshes from this actor
			std::vector<physx::PxShape*> shapes = CreateConvexMesh(*collider, size);

			for (auto shape : shapes)
			{
				shape->setMaterials(materials, 1);
				shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !collider->GetTrigger());
				shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, collider->GetTrigger());
				actor.AddCollisionShape(shape);
			}
		}
		else
		{
			// Remove any potential convex meshes from this actor
			std::vector<physx::PxShape*> shapes = CreateTriangleMesh(*collider, size);

			for (auto shape : shapes)
			{
				shape->setMaterials(materials, 1);
				shape->setFlag(physx::PxShapeFlag::eSIMULATION_SHAPE, !collider->GetTrigger());
				shape->setFlag(physx::PxShapeFlag::eTRIGGER_SHAPE, collider->GetTrigger());
				actor.AddCollisionShape(shape);
			}
		}
	}

	std::vector<physx::PxShape*> PXPhysicsWrappers::CreateConvexMesh(MeshColliderComponent& collider, const glm::vec3& size, bool invalidateOld)
	{
		std::vector<physx::PxShape*> shapes;

		collider.GetProcessedMeshes().clear();

		const physx::PxCookingParams& currentParams = s_CookingFactory->getParams();
		physx::PxCookingParams newParams = currentParams;
		newParams.planeTolerance = 0.0F;
		newParams.meshPreprocessParams = physx::PxMeshPreprocessingFlags(physx::PxMeshPreprocessingFlag::eWELD_VERTICES);
		newParams.meshWeldTolerance = 0.01f;
		s_CookingFactory->setParams(newParams);

		auto& collisionMesh = collider.GetCollisionMesh();
		auto& processedMeshes = collider.GetProcessedMeshes();
		if (invalidateOld)
			PhysicsMeshSerializer::DeleteIfSerialized(collisionMesh->GetFilePath());

		if (!PhysicsMeshSerializer::IsSerialized(collisionMesh->GetFilePath()))
		{
			const std::vector<Vertex>& vertices = collisionMesh->GetStaticVertices();
			const std::vector<Index>& indices = collisionMesh->GetIndices();

			for (const auto& submesh : collisionMesh->GetSubmeshes())
			{
				physx::PxConvexMeshDesc convexDesc;
				convexDesc.points.count = submesh.VertexCount;
				convexDesc.points.stride = sizeof(Vertex);
				convexDesc.points.data = &vertices[submesh.BaseVertex];
				convexDesc.indices.count = submesh.IndexCount / 3;
				convexDesc.indices.data = &indices[submesh.BaseIndex / 3];
				convexDesc.indices.stride = sizeof(Index);
				convexDesc.flags = physx::PxConvexFlag::eCOMPUTE_CONVEX | physx::PxConvexFlag::eSHIFT_VERTICES;

				physx::PxDefaultMemoryOutputStream buf;
				physx::PxConvexMeshCookingResult::Enum result;
				if (!s_CookingFactory->cookConvexMesh(convexDesc, buf, &result))
				{
					HE_CORE_ERROR("Failed to cook convex mesh {0}", submesh.MeshName);
					continue;
				}

				PhysicsMeshSerializer::SerializeMesh(collisionMesh->GetFilePath(), buf, submesh.MeshName);
				physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
				physx::PxConvexMesh* convexMesh = s_Physics->createConvexMesh(input);
				physx::PxConvexMeshGeometry convexGeometry = physx::PxConvexMeshGeometry(convexMesh, physx::PxMeshScale(ToPhysXVector(size)));
				convexGeometry.meshFlags = physx::PxConvexMeshGeometryFlag::eTIGHT_BOUNDS;
				physx::PxMaterial* material = s_Physics->createMaterial(0, 0, 0); // Dummy material, will be replaced at runtime.
				physx::PxShape* shape = s_Physics->createShape(convexGeometry, *material, true);
				shape->setLocalPose(ToPhysXTransform(submesh.Transform));
				shapes.push_back(shape);
			}
		}
		else
		{
			for (const auto& submesh : collisionMesh->GetSubmeshes())
			{
				physx::PxDefaultMemoryInputData meshData = PhysicsMeshSerializer::DeserializeMesh(collisionMesh->GetFilePath(), submesh.MeshName);
				physx::PxConvexMesh* convexMesh = s_Physics->createConvexMesh(meshData);
				physx::PxConvexMeshGeometry convexGeometry = physx::PxConvexMeshGeometry(convexMesh, physx::PxMeshScale(ToPhysXVector(size)));
				convexGeometry.meshFlags = physx::PxConvexMeshGeometryFlag::eTIGHT_BOUNDS;
				physx::PxMaterial* material = s_Physics->createMaterial(0, 0, 0); // Dummy material, will be replaced at runtime.
				physx::PxShape* shape = s_Physics->createShape(convexGeometry, *material, true);
				shape->setLocalPose(ToPhysXTransform(submesh.Transform));
				shapes.push_back(shape);
			}
		}

		if (processedMeshes.size() <= 0)
		{
			for (auto shape : shapes)
			{
				physx::PxConvexMeshGeometry convexGeometry;
				shape->getConvexMeshGeometry(convexGeometry);
				physx::PxConvexMesh* mesh = convexGeometry.convexMesh;

				// https://github.com/EpicGames/UnrealEngine/blob/release/Engine/Source/ThirdParty/PhysX3/NvCloth/samples/SampleBase/renderer/ConvexRenderMesh.cpp
				const uint32_t nbPolygons = mesh->getNbPolygons();
				const physx::PxVec3* convexVertices = mesh->getVertices();
				const physx::PxU8* convexIndices = mesh->getIndexBuffer();

				uint32_t nbVertices = 0;
				uint32_t nbFaces = 0;

				std::vector<Vertex> collisionVertices;
				std::vector<Index> collisionIndices;
				uint32_t vertCounter = 0;
				uint32_t indexCounter = 0;

				for (uint32_t i = 0; i < nbPolygons; i++)
				{
					physx::PxHullPolygon polygon;
					mesh->getPolygonData(i, polygon);
					nbVertices += polygon.mNbVerts;
					nbFaces += (polygon.mNbVerts - 2) * 3;

					uint32_t vI0 = vertCounter;

					for (uint32_t vI = 0; vI < polygon.mNbVerts; vI++)
					{
						Vertex v;
						v.Position = FromPhysXVector(convexVertices[convexIndices[polygon.mIndexBase + vI]]);
						collisionVertices.push_back(v);
						vertCounter++;
					}

					for (uint32_t vI = 1; vI < uint32_t(polygon.mNbVerts) - 1; vI++)
					{
						Index index;
						index.V1 = uint32_t(vI0);
						index.V2 = uint32_t(vI0 + vI + 1);
						index.V3 = uint32_t(vI0 + vI);
						collisionIndices.push_back(index);
						indexCounter++;
					}

					processedMeshes.push_back(std::make_shared<Mesh>(collisionVertices, collisionIndices, FromPhysXTransform(shape->getLocalPose())));
				}
			}
		}

		s_CookingFactory->setParams(currentParams);
		return shapes;

	}

	std::vector<physx::PxShape*> PXPhysicsWrappers::CreateTriangleMesh(MeshColliderComponent& collider, const glm::vec3& scale, bool invalidateOld)
	{
		std::vector<physx::PxShape*> shapes;

		collider.GetProcessedMeshes().clear();

		auto& collisionMesh = collider.GetCollisionMesh();
		auto& processedMeshes = collider.GetProcessedMeshes();

		if (invalidateOld)
			PhysicsMeshSerializer::DeleteIfSerialized(collisionMesh->GetFilePath());

		if (!PhysicsMeshSerializer::IsSerialized(collisionMesh->GetFilePath()))
		{
			const std::vector<Vertex>& vertices = collisionMesh->GetStaticVertices();
			const std::vector<Index>& indices = collisionMesh->GetIndices();

			for (const auto& submesh : collisionMesh->GetSubmeshes())
			{
				physx::PxTriangleMeshDesc triangleDesc;
				triangleDesc.points.count = submesh.VertexCount;
				triangleDesc.points.stride = sizeof(Vertex);
				triangleDesc.points.data = &vertices[submesh.BaseVertex];
				triangleDesc.triangles.count = submesh.IndexCount / 3;
				triangleDesc.triangles.data = &indices[submesh.BaseIndex / 3];
				triangleDesc.triangles.stride = sizeof(Index);

				physx::PxDefaultMemoryOutputStream buf;
				physx::PxTriangleMeshCookingResult::Enum result;
				if (!s_CookingFactory->cookTriangleMesh(triangleDesc, buf, &result))
				{
					HE_CORE_ERROR("Failed to cook triangle mesh: {0}", submesh.MeshName);
					continue;
				}

				PhysicsMeshSerializer::SerializeMesh(collisionMesh->GetFilePath(), buf, submesh.MeshName);

				glm::vec3 submeshTranslation(0.0f) , submeshRotation(0.0f), submeshScale(1.0f);
				glm::quat submeshRotationQuat(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 skew(0.0f);
				glm::vec4 perspective(0.0f);
				glm::decompose(submesh.LocalTransform, submeshScale, submeshRotationQuat, submeshTranslation, skew, perspective);
				submeshRotation = glm::eulerAngles(submeshRotationQuat);
				//DecomposeTransform(submesh.LocalTransform, submeshTranslation, submeshRotation, submeshScale);

				physx::PxDefaultMemoryInputData input(buf.getData(), buf.getSize());
				physx::PxTriangleMesh* trimesh = s_Physics->createTriangleMesh(input);
				physx::PxTriangleMeshGeometry triangleGeometry = physx::PxTriangleMeshGeometry(trimesh, physx::PxMeshScale(ToPhysXVector(submeshScale * scale)));
				physx::PxMaterial* material = s_Physics->createMaterial(0, 0, 0); // Dummy material, will be replaced at runtime.
				physx::PxShape* shape = s_Physics->createShape(triangleGeometry, *material, true);
				shape->setLocalPose(ToPhysXTransform(submeshTranslation, submeshRotation));
				shapes.push_back(shape);
			}
		}
		else
		{
			for (const auto& submesh : collisionMesh->GetSubmeshes())
			{
				glm::vec3 submeshTranslation(0.0f), submeshRotation(0.0f), submeshScale(1.0f);
				glm::quat submeshRotationQuat(0.0f, 0.0f, 0.0f, 0.0f);
				glm::vec3 skew(0.0f);
				glm::vec4 perspective(0.0f);
				glm::decompose(submesh.LocalTransform, submeshScale, submeshRotationQuat, submeshTranslation, skew, perspective);
				submeshRotation = glm::eulerAngles(submeshRotationQuat);
				//DecomposeTransform(submesh.LocalTransform, submeshTranslation, submeshRotation, submeshScale);

				physx::PxDefaultMemoryInputData meshData = PhysicsMeshSerializer::DeserializeMesh(collisionMesh->GetFilePath(), submesh.MeshName);
				physx::PxTriangleMesh* trimesh = s_Physics->createTriangleMesh(meshData);
				physx::PxTriangleMeshGeometry triangleGeometry = physx::PxTriangleMeshGeometry(trimesh, physx::PxMeshScale(ToPhysXVector(submeshScale * scale)));
				physx::PxMaterial* material = s_Physics->createMaterial(0, 0, 0); // Dummy material, will be replaced at runtime.
				physx::PxShape* shape = s_Physics->createShape(triangleGeometry, *material, true);
				shape->setLocalPose(ToPhysXTransform(submeshTranslation, submeshRotation));
				shapes.push_back(shape);
			}
		}

		if (processedMeshes.size() <= 0)
		{
			for (auto shape : shapes)
			{
				physx::PxTriangleMeshGeometry triangleGeometry;
				shape->getTriangleMeshGeometry(triangleGeometry);
				physx::PxTriangleMesh* mesh = triangleGeometry.triangleMesh;

				const uint32_t nbVerts = mesh->getNbVertices();
				const physx::PxVec3* triangleVertices = mesh->getVertices();
				const uint32_t nbTriangles = mesh->getNbTriangles();
				const physx::PxU16* tris = (const physx::PxU16*)mesh->getTriangles();

				std::vector<Vertex> vertices;
				std::vector<Index> indices;

				for (uint32_t v = 0; v < nbVerts; v++)
				{
					Vertex v1;
					v1.Position = FromPhysXVector(triangleVertices[v]);
					vertices.push_back(v1);
				}

				for (uint32_t tri = 0; tri < nbTriangles; tri++)
				{
					Index index;
					index.V1 = tris[3 * tri + 0];
					index.V2 = tris[3 * tri + 1];
					index.V3 = tris[3 * tri + 2];
					indices.push_back(index);
				}

				glm::mat4 scale = glm::scale(glm::mat4(1.0f), *(glm::vec3*)&triangleGeometry.scale.scale);
				//scale = glm::mat4(1.0f);
				glm::mat4 transform = FromPhysXTransform(shape->getLocalPose()) * scale;
				processedMeshes.push_back(std::make_shared<Mesh>(vertices, indices, transform));
			}
		}

		return shapes;
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

	bool OverlapCapsule(const glm::vec3& origin, float radius, float halfHeight, std::array<physx::PxOverlapHit, OVERLAP_MAX_COLLIDERS>& buffer, uint32_t* count)
	{
		physx::PxScene* scene = static_cast<physx::PxScene*>(Physics::GetPhysicsScene());

		memset(s_OverlapBuffer, 0, sizeof(s_OverlapBuffer));
		physx::PxOverlapBuffer buf(s_OverlapBuffer, OVERLAP_MAX_COLLIDERS);
		physx::PxCapsuleGeometry geometry = physx::PxCapsuleGeometry(radius, halfHeight);
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

	bool OverlapSphere(const glm::vec3& origin, float radius, std::array<physx::PxOverlapHit, OVERLAP_MAX_COLLIDERS>& buffer, uint32_t* count)
	{
		physx::PxScene* scene = static_cast<physx::PxScene*>(Physics::GetPhysicsScene());

		memset(s_OverlapBuffer, 0, sizeof(s_OverlapBuffer));
		physx::PxOverlapBuffer buf(s_OverlapBuffer, OVERLAP_MAX_COLLIDERS);
		physx::PxSphereGeometry geometry = physx::PxSphereGeometry(radius);
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