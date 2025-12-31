#line 2

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outNormalGBuffer;
layout(location = 2) out vec4 outSpecularityGBuffer;

layout(location = 0) in vec2 inTexcoord;
layout(location = 1) in vec3 inViewNormal;
layout(location = 2) in vec4 inViewTangent4;
layout(location = 3) in vec3 inViewPosition;
layout(location = 4) in vec3 inWorldPosition;
layout(location = 5) in vec3 inWorldSurfaceN;

layout(set=2, binding=0, std140) uniform MaterialStateBlock
{
	SceneMetallicRoughnessMaterial MaterialState;
};

layout(set=2, binding=1) uniform texture2D BaseColorTexture;
layout(set=2, binding=2) uniform texture2D EmissiveTexture;
layout(set=2, binding=3) uniform texture2D NormalTexture;
layout(set=2, binding=4) uniform texture2D OcclusionTexture;
layout(set=2, binding=5) uniform texture2D MetallicRoughnessTexture;

void main()
{
    // Discard back side.
    if(!MaterialState.doubleSided && !gl_FrontFacing)
        discard;

    // Tex coord.
    vec2 texcoord = inTexcoord*MaterialState.texcoordScale + MaterialState.texcoordOffset;

    // Discard alpha masking.
    vec4 baseColor = MaterialState.baseColorFactor * texture(sampler2D(BaseColorTexture, LinearMipmapLinearTextureSampler), texcoord);
    if(baseColor.a < MaterialState.alphaCutoff)
        discard;

    vec3 P = inViewPosition;
    vec3 V = normalize(-inViewPosition);
    vec3 lightedColor = GlobalLightingState.ambientLighting;

    vec3 surfaceTangent = normalize(inViewTangent4.xyz);
    vec3 surfaceNormal = normalize(inViewNormal);
    vec3 surfaceBitangent = normalize(cross(surfaceNormal, surfaceTangent)*inViewTangent4.w);
    mat3 TBN = mat3(surfaceTangent, surfaceBitangent, surfaceNormal);
        
    vec3 tangentSpaceN = texture(sampler2D(NormalTexture, LinearMipmapLinearTextureSampler), texcoord).xyz*2.0 - 1.0;
    vec3 N = normalize(TBN * tangentSpaceN);

    vec4 mrSample = texture(sampler2D(MetallicRoughnessTexture, LinearMipmapLinearTextureSampler), texcoord);

    SurfaceLightingParameters surfaceParameters;
    surfaceParameters.baseColor = baseColor;
    surfaceParameters.emissiveFactor = MaterialState.emissiveFactor * texture(sampler2D(EmissiveTexture, LinearMipmapLinearTextureSampler), texcoord).rgb;
    surfaceParameters.occlusionFactor = MaterialState.occlusionFactor * texture(sampler2D(OcclusionTexture, LinearMipmapLinearTextureSampler), texcoord).r;
    surfaceParameters.N = N;
    surfaceParameters.metallicFactor = MaterialState.metallicFactor*mrSample.b;

    surfaceParameters.V = V;
    surfaceParameters.roughnessFactor = MaterialState.roughnessFactor*mrSample.g;

    surfaceParameters.P = inViewPosition;

    surfaceParameters.worldP = inWorldPosition;
    surfaceParameters.worldSurfaceN = normalize(inWorldSurfaceN);

    if(!gl_FrontFacing)
	{
		surfaceParameters.N = -surfaceParameters.N;
		surfaceParameters.worldSurfaceN = -surfaceParameters.worldSurfaceN;
	}

    outColor = performLightingModelComputation(surfaceParameters, outNormalGBuffer, outSpecularityGBuffer);
}
