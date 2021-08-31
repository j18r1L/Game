#type vertex
#version 450 core

// Defines

#define HE_VERTEX_LAYOUT_INPUT
#define HE_VERTEX_LAYOUT_OUTPUT
#define HE_SHADER

// Includes

//#include "common/Structures.h"
//#include "common/VertexLayout.h"
//#include "common/Layouts.h"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec3 inTangent;
layout(location = 3) in vec3 inBinormal;
layout(location = 4) in vec2 inTexCoord;

layout(location = 0) out vec3 outPosition;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec4 outTangent;
layout(location = 3) out vec2 outTexCoord;
layout(location = 4) out vec3 outCamPos;

struct MeshMatrixStruct
{
	mat4 transform;
};

struct FrameData
{
	vec4 timingData;

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
};

uniform CameraMatrixStruct u_CameraData;

uniform FrameData u_FrameData;

uniform MeshMatrixStruct u_MeshData;


mat4 getWorldMatrix()
{
	/*
	// TODO: [HAR-80]
	if (HE_SKIN_BIT)
	{
		mat4 worldMatrix = (u_SkinData.jointMatrices[a_Index.x] * a_Weight.x);
		worldMatrix += (u_SkinData.jointMatrices[a_Index.y] * a_Weight.y);
		worldMatrix += (u_SkinData.jointMatrices[a_Index.z] * a_Weight.z);
		worldMatrix += (u_SkinData.jointMatrices[a_Index.w] * a_Weight.w);
		return u_MeshData.transform * worldMatrix
	}
	else
	*/
	{
		return u_MeshData.transform;
	}
}

// Main

void main()
{
	const mat4 worldMatrix = getWorldMatrix();

	outPosition = vec3(worldMatrix * vec4(inPosition, 1.0));
	outNormal = normalize(mat3(worldMatrix) * inNormal.xyz);
	outTangent = vec4(normalize((mat3(worldMatrix) * inTangent.xyz)), 1.0);
	outCamPos = u_CameraData.viewInv[3].xyz;
	outTexCoord = inTexCoord;

	gl_Position = u_CameraData.projView * worldMatrix * vec4(inPosition, 1.0);
}