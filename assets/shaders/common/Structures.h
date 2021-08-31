#ifndef STRUCTURES_H
#define STRUCTURES_H

#ifndef HE_SHADER

#include <glm/glm.hpp>

using uint = uint32_t;

using vec2 = glm::vec2;
using vec3 = glm::vec3;
using vec4 = glm::vec4;

//using uvec3 = glm::uvec2;
//using uvec3 = glm::uvec3;
//using uvec4 = glm::uvec4;

using mat3 = glm::mat3;
using mat4 = glm::mat4;

#endif

#ifdef HE_SHADER

// Define all textures id
#define HE_TEXTURE_DIFFUSE_ID 0
//#define HE_TEXTURE_NORMAL_ID  1
#define HE_SAMPLER_TEXTURES_COUNT 11

#endif

#define HE_MAX_LIGHT_COUNT 16
#define HE_MAX_JOINT_COUNT 128 

struct MeshMatrixStruct
{
	mat4 transform;
};
/*
// TODO: [HAR-80] 
struct SkinMatrixStruct
{
	mat4 jointMatrices[HE_MAX_JOINT_COUNT];
};
*/

struct LightMatrixStruct
{
	// .xyz = position
	// .w   = range 
	vec4 position;

	// .xyz = color
	// .w   = intensity
	vec4 color;

	// .x = inner cone angle
	// .y = outer cone angle
	// .z = light angle scale
	// .w = light angle offset
	vec4 spotLightParameters;
};

struct LightStruct
{
	uint directionalLightBeginIndex;
	uint directionalLightCount;

	uint pointLightBeginIndex;
	uint pointLightCount;

	uint spotLightBeginIndex;
	uint spotLightCount;

	LightMatrixStruct light;
};

struct EnvironmentStruct
{
	// .rgb = tint
	// .a   = intensity
	vec4 specularColorFactor; 

	// .rgb = tint
	// .a   = intensity
	vec4 diffuseColorFactor;

	// .rgb = tint
	// .a   = intensity
	vec4 colorFactor;

	// Environment rotation matrix
	mat4 rotation;

	// .r  = radiance cubemap mip count
	// .g  = specular cubemap lod level
	// .bw = ?
	//vec4 



};

struct FrameData
{
	// .x = scene delta time (ms)
	// .y = tick  delta time (ms)
	// .z = tick  total time (s),
	// .w = frame index 
	vec4 timingData;

	// .x = width (in pixels)
	// .y = height (in pixels)
	// .z = zNear 
	// .w = zFar
	vec4 viewportData;

};

struct CameraMatrixStruct
{
	mat4 view;
	mat4 proj;
	mat4 projView;
	
	mat4 viewInv;
	mat4 projInv;
	mat4 projViewInv;

	mat4 viewPrevFrame;
	mat4 projPrevFrame;
	mat4 projViewPrevFrame;

	// vec4 frustumPlanes[FRUSTUM_PLANES_COUNT];
};

#endif // STRUCTURES_H