#ifndef VERTEX_LAYOUT_H
#define VERTEX_LAYOUT_H

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

// Set 1 for dynamic uniform buffers
layout(set = 1, binding = 0, std140) uniform uMeshMatrix
{
	MeshMatrixStruct u_MeshData;
};

/*
// TODO: [HAR-80] 
layout(set = 1, binding = 1, std140) uniform uSkinMatrices
{
	SkinMatrixStruct u_SkinData;
};
*/

#endif

#endif // VERTEX_LAYOUT_H