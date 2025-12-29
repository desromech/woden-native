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

mat3 topLeft3x3FromMat4(mat4 mat)
{
    return mat3(mat[0].xyz, mat[1].xyz, mat[2].xyz);
}

vec3 transformNormalToWorld(vec3 normal)
{ 
    return normal * topLeft3x3FromMat4(SceneObjectStateList.list[PushConstants.objectStateIndex].inverseTransformation);
}

vec3 transformNormalWorldToView(vec3 normal)
{ 
    return normal * topLeft3x3FromMat4(SceneCameraStateList.list[PushConstants.cameraStateIndex].transformation);
}

vec3 transformNormalToView(vec3 normal)
{ 
	return transformNormalWorldToView(transformNormalToWorld(normal));
}
