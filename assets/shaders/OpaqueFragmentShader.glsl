#line 2

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outNormalGBuffer;
layout(location = 2) out vec4 outSpecularityGBuffer;

layout(location = 0) in vec2 inTexcoord;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inTangent4;
layout(location = 3) in vec3 inViewPosition;

layout(set=2, binding=0, std140) uniform MaterialStateBlock
{
	SceneMetallicRoughnessMaterial MaterialState;
};

void main()
{
    vec3 P = inViewPosition;
    vec3 V = normalize(-inViewPosition);
    vec3 N = normalize(inNormal);
    vec3 lightedColor = GlobalLightingState.ambientLighting;

    SurfaceLightingParameters surfaceParameters;
    surfaceParameters.baseColor = MaterialState.baseColorFactor;
    surfaceParameters.emissiveFactor = MaterialState.emissiveFactor;
    surfaceParameters.occlusionFactor = MaterialState.occlusionFactor;
    surfaceParameters.N = N;
    surfaceParameters.metallicFactor = MaterialState.metallicFactor;

    surfaceParameters.V = V;
    surfaceParameters.roughnessFactor = MaterialState.roughnessFactor;

    surfaceParameters.P = inViewPosition;

    outColor = performLightingModelComputation(surfaceParameters, outNormalGBuffer, outSpecularityGBuffer);
}
