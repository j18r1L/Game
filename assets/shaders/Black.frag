#type fragment
#version 450 core

// Defines

#define HE_FRAGMENT_LAYOUT_INPUT
#define HE_SHADER
// Includes

//#include "common/Structures.h"
//#include "common/FragmentLayout.h"
//#include "common/Layouts.h"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inTangent;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in vec3 inCamPos;

layout(location = 0) out vec4 outColor;

struct LightMatrixStruct
{
	vec4 position;

	vec4 color;

	vec4 spotLightParameters;
};

struct LightStruct
{
	int directionalLightBeginIndex;
	int directionalLightCount;

	int pointLightBeginIndex;
	int pointLightCount;

	int spotLightBeginIndex;
	int spotLightCount;

	LightMatrixStruct light;
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


uniform LightStruct u_LightData;


// Layout


// Functions

vec4 unlit()
{
	//const mediump vec4 baseColor = texture(uSampTextures[HE_TEXTURE_DIFFUSE_ID], inUv.xy) * inColor;
	//return baseColor;
	return vec4(1.0);
}

// Main

void main()
{
	// Currently unlit 
	// TODO: [HAR-169]
	outColor = unlit();

	// Post processing
	// TODO: [HAR-170]
}