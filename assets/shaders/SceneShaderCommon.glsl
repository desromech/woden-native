struct SceneObjectState
{
    mat4 transformation;
    mat4 inverseTransformation;
};

layout(set=1, binding=0, std430) buffer SceneObjectStateBlock
{
	SceneObjectState[] list;
} SceneObjectStateList;

struct SceneCameraState
{
    mat4 transformation;
    mat4 inverseTransformation;

    mat4 projectionMatrix;
    mat4 inverseProjectionMatrix;
};

layout(set=1, binding=1, std430) buffer SceneCameraStateBlock
{
	SceneCameraState[] list;
} SceneCameraStateList;

layout( push_constant ) uniform constants
{
    uint objectStateIndex;
    uint cameraStateIndex;
    uint materialStateIndex;
} PushConstants;
