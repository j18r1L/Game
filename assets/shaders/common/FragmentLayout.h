#ifndef FRAGMENT_LAYOUT_H
#define FRAGMENT_LAYOUT_H

#ifdef HE_SHADER
// Sets

// Set 0 for camera, frame data
layout(set = 0, binding = 0, std140) uniform uCameraMatrices
{
	CameraMatrixStruct u_CameraData;
};

layout(set = 0, binding = 1, std140) uniform uFrameData
{
	FrameData u_FrameData;
};
/*
layout(set = 0, binding = 2, std140) uniform uEnvironmentData
{
	EnvironmentStruct u_EnvironmentData;
};
*/

layout(set = 0, binding = 3, std140) uniform uLightData
{
	LightStruct u_LightData;
};

// Set 1 for dynamic uniform buffers
/*
layout (set = 1, binding = 2, std140) uniform uMaterialData
{
	MaterialStruct u_MaterialData;
};
*/

// Set 2 for material data 
layout(set = 2, binding = 1) uniform sampler2D uSampTextures[HE_SAMPLER_TEXTURES_COUNT];


#endif

#endif // FRAGMENT_LAYOUT_H