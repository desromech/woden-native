#include "SceneShaderCommon.glsl"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexcoord;
layout(location = 3) in vec4 inTangent4;

void main()
{
    vec4 worldPosition = CurrentObjectState.transformation * vec4(inPosition, 1.0);
    vec4 viewPosition = CurrentCameraState.inverseTransformation * worldPosition;
    gl_Position = CurrentCameraState.projectionMatrix * viewPosition;
}

