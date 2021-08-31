#ifndef LAYOUTS_H
#define LAYOUTS_H

#ifdef HE_SHADER

#ifdef HE_VERTEX_LAYOUT_INPUT
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inTangent;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in vec4 inWeight;
layout(location = 5) in vec4 inColor;
#endif

#ifdef HE_VERTEX_LAYOUT_OUTPUT
layout(location = 0) out vec3 outPosition;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec4 outTangent
layout(location = 3) out vec2 outTexCoord;
layout(location = 4) out vec3 outCamPos;

#endif

#ifdef HE_FRAGMENT_LAYOUT_INPUT
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inTangent;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in vec3 inCamPos;

#endif

#endif

#endif // LAYOUTS_H