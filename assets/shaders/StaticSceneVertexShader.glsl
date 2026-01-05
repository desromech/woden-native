#include "SceneShaderCommon.glsl"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexcoord;
layout(location = 3) in vec4 inTangent4;

layout(location = 0) out vec2 outTexcoord;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec4 outTangent4;
layout(location = 3) out vec3 outViewPosition;
layout(location = 4) out vec3 outWorldPosition;
layout(location = 5) out vec3 outWorldSurfaceN;

void main()
{
    outTexcoord = inTexcoord;
    outTangent4 = vec4(transformNormalToView(inTangent4.xyz), inTangent4.w);

    vec4 worldPosition4 = CurrentObjectState.transformation * vec4(inPosition, 1.0);
    outWorldPosition = worldPosition4.xyz;

    vec4 viewPosition = CurrentCameraState.inverseTransformation * worldPosition4;

    outViewPosition = viewPosition.xyz;
    outNormal = transformNormalToView(inNormal);
    outWorldSurfaceN = transformNormalToWorld(inNormal);
    gl_Position = CurrentCameraState.projectionMatrix * viewPosition;
}

