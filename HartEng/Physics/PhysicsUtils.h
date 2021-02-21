#pragma once

#include "HartEng/Scene/Components/TransformComponent.h"

#include <string>
#include <PxPhysicsAPI.h>
#include <glm/gtc/type_ptr.hpp>

namespace HE
{
	physx::PxTransform ToPhysXTransform(const TransformComponent& transform);
	physx::PxTransform ToPhysXTransform(const glm::mat4& transform);
	physx::PxTransform ToPhysXTransform(const glm::vec3& translation, const glm::vec3& rotation);
	physx::PxMat44 ToPhysXMatrix(const glm::mat4& matrix);
	physx::PxVec3 ToPhysXVector(const glm::vec3& vector);
	physx::PxVec4 ToPhysXVector(const glm::vec4& vector);
	physx::PxQuat ToPhysXQuat(const glm::quat& quat);

	glm::mat4 FromPhysXTransform(const physx::PxTransform& transform);
	glm::mat4 FromPhysXMatrix(const physx::PxMat44& matrix);
	glm::vec3 FromPhysXVector(const physx::PxVec3& vector);
	glm::vec4 FromPhysXVector(const physx::PxVec4& vector);
	glm::quat FromPhysXQuat(const physx::PxQuat& quat);

	physx::PxFilterFlags HEFilterShader(physx::PxFilterObjectAttributes attributes0, 
		physx::PxFilterData filterData0, 
		physx::PxFilterObjectAttributes attributes1,
		physx::PxFilterData filterData1, 
		physx::PxPairFlags& pairFlags, 
		const void* constantBlock, 
		physx::PxU32 constantBlockSize);

}