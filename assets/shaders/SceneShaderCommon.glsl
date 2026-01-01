layout(set=0, binding=0) uniform sampler LinearMipmapLinearTextureSampler;
layout(set=0, binding=1) uniform sampler NearestTextureSampler;
layout(set=0, binding=2) uniform sampler ShadowMapSampler;

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
    vec2 framebufferExtent;
    vec2 framebufferReciprocalExtent;

    uint flipVertically;
    float nearDistance;
    float farDistance;
    float exposure;
    
    vec2 lightGridDepthSliceScaleOffset;
	bool hasTopLeftNDCOrigin;
	bool hasBottomLeftTextureCoordinates;

    uvec3 lightGridExtent;
    float padding;

    mat4 transformation;
    mat4 inverseTransformation;

    mat4 projectionMatrix;
    mat4 inverseProjectionMatrix;
};

layout(set=1, binding=1, std430) buffer SceneCameraStateBlock
{
	SceneCameraState[] list;
} SceneCameraStateList;

layout(set=1, binding=2, std140) uniform GlobalLightingStateBlock
{
	vec3 ambientLighting;
    uint numberOfLights;
} GlobalLightingState;

struct LightSourceState
{
    vec4 positionOrDirection;

    vec3 intensity;
    float influenceRadius;

	vec3 spotDirection;
	float innerSpotCosCutoff;

	float outerSpotCosCutoff;
	bool castShadows;
	vec2 shadowMapViewportScale;

	float shadowMapNormalBiasFactor;
    float padding[3];

	vec4 shadowMapCascadeDistanceWorldTransform;
	vec4 shadowMapCascadeOffsets;
	
	mat4 modelMatrix[6];
	mat4 inverseModelMatrix[6];

	mat4 projectionMatrix[6];
	mat4 inverseProjectionMatrix[6];

	vec2 shadowMapViewportOffsets[6];

};

layout(set=1, binding=3, std430) buffer WorldLightSourceStateBlock
{
	LightSourceState[] list;
} WorldLightSourceStateList;

layout(set=1, binding=4, std430) buffer ViewLightSourceStateBlock
{
	LightSourceState[] list;
} ViewLightSourceStateList;

struct LightCluster
{
	vec3 min;
	vec3 max;
};

layout(set=1, binding=5, std430) buffer LightClustersBlock
{
	LightCluster[] list;
} LightClusterList;

layout(set=1, binding=6, std430) buffer TileLightIndicesBlock
{
	uint[] indices;
} TileLightIndices;

layout(set=1, binding=7, std430) buffer LightClusterListsBlock
{
    uint listSize;
    uint padding;
	uvec2[] lists;
} LightClusterLists;

layout(set=1, binding=8) uniform texture2D ShadowMapAtlasTexture;

struct SceneSurfaceMaterial
{
    float alphaCutoff;
    bool doubleSided;
};

struct SceneMetallicRoughnessMaterial
{
    float alphaCutoff;
    bool doubleSided;

    vec4 baseColorFactor;

    vec3 emissiveFactor;
    float occlusionFactor;

    float roughnessFactor;
    float metallicFactor;
    vec2 texcoordOffset;

    vec2 texcoordScale;
    vec2 texcoordOffsetVelocity;
};

layout( push_constant ) uniform constants
{
    uint objectStateIndex;
    uint cameraStateIndex;
    uint lightStateIndex;
    uint lightStateComponentIndex;
} PushConstants;

#define CurrentObjectState SceneObjectStateList.list[PushConstants.objectStateIndex]
#define CurrentCameraState SceneCameraStateList.list[PushConstants.cameraStateIndex]
#define CurrentWorldLightSource WorldLightSourceStateList.list[PushConstants.lightStateIndex]

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
