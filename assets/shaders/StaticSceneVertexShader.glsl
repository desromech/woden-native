#line 2

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexcoord;
layout(location = 3) in vec4 inTangent4;

layout(location = 0) out vec3 outNormal;

void main()
{
    vec4 worldPosition = SceneObjectStateList.list[PushConstants.objectStateIndex].transformation * vec4(inPosition, 1.0);
    vec4 viewPosition = SceneCameraStateList.list[PushConstants.cameraStateIndex].inverseTransformation * worldPosition;

    outNormal = inNormal;
    gl_Position = SceneCameraStateList.list[PushConstants.cameraStateIndex].projectionMatrix * viewPosition;
}

