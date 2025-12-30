#line 2

const float DistanceEpsilon = 0.00001;
const float Pi = 3.141592653589793;
const float PiReciprocal = 0.3183098861837907;

struct SurfaceLightingParameters
{
	vec4 baseColor;

	vec3 emissiveFactor;
	float occlusionFactor;

	vec3 N;
	float metallicFactor;

	vec3 V;
	float roughnessFactor;

	vec3 P;
	
	vec3 worldP;
	vec3 worldSurfaceN;
};

vec3 fresnelSchlick(vec3 F0, float cosTheta)
{
    float powFactor = 1.0 - cosTheta;
    float powFactor2 = powFactor * powFactor;
    float powFactor4 = powFactor2 * powFactor2;
    float powValue = powFactor4 * powFactor;

    return F0 + (vec3(1.0) - F0) * powValue;
}

vec3 fresnelSchlickRoughness(vec3 F0, float cosTheta, float roughness)
{
	// Function obtained from: https://learnopengl.com/PBR/IBL/Diffuse-irradiance
    float powFactor = 1.0 - cosTheta;
    float powFactor2 = powFactor * powFactor;
    float powFactor4 = powFactor2 * powFactor2;
    float powValue = powFactor4 * powFactor;

    return F0 + (max(vec3(1.0) - roughness, F0) - F0) * powValue;
}

float ggxSpecularDistribution(float alpha, float cosTheta)
{
	float alphaSquare = alpha*alpha;
	float den = cosTheta*cosTheta*(alphaSquare - 1.0) + 1.0;
	return alphaSquare / (Pi * den*den);
}

float smithSchlickBeckmannReciprocalFunction(float k, float cosTheta)
{
    return cosTheta*(1.0 - k) + k;
}

float cookTorranceSmithSchlickGGXMasking(float k, float NdotL, float NdotV)
{
	return 1.0 / (4.0*smithSchlickBeckmannReciprocalFunction(k, NdotL)*smithSchlickBeckmannReciprocalFunction(k, NdotV));
}

float computeLightAttenuation(float distance, float influenceRadius)
{
    float a = distance / max(influenceRadius, DistanceEpsilon);
    float a2 = a*a;
    float a4 = a2*a2;
    float num = clamp(1.0f - a4, 0.0, 1.0);
    return num*num / (distance*distance + 1.0);
}

vec4 performLightingModelComputation(in SurfaceLightingParameters surfaceParameters, out vec2 gbufferNormal, out vec4 gbufferSpecularity)
{
	vec3 dielectricF0 = vec3(0.04);
	vec3 Cdiffuse = mix(surfaceParameters.baseColor.rgb * (1.0 - dielectricF0), vec3(0.0), surfaceParameters.metallicFactor);
	vec3 diffuse = Cdiffuse * PiReciprocal;
	vec3 F0 = mix(dielectricF0, surfaceParameters.baseColor.rgb, surfaceParameters.metallicFactor);

	gbufferNormal = surfaceParameters.N.xy;
	gbufferSpecularity = vec4(F0, surfaceParameters.roughnessFactor);

	float directRoughness = mix(0.01, 1.0, surfaceParameters.roughnessFactor);
	float directAlpha = directRoughness*directRoughness;

	float directKRoughness = (directRoughness + 1.0);
	float directK = directKRoughness*directKRoughness / 8.0;

    float NdotV = max(0.0, dot(surfaceParameters.N, surfaceParameters.V));

    vec3 lightedColor = vec3(0);

    // Ambient lighting
    lightedColor += GlobalLightingState.ambientLighting * diffuse;

    // Ambient occlusion and emissive factor.
	lightedColor *= surfaceParameters.occlusionFactor;
	lightedColor += surfaceParameters.emissiveFactor;

    for(uint lightIndex = 0; lightIndex < GlobalLightingState.numberOfLights; ++lightIndex)
    {
#define currentLightSource LightSourceStateList.list[lightIndex]
		vec4 lightSourcePosition = currentLightSource.positionOrDirection;
		vec3 L = lightSourcePosition.xyz;
		float lightDistance = 0.0;
		float attenuation = 1.0;
		float lightMaxDistance = 1.0;
		if(lightSourcePosition.w != 0)
		{ 
			L = L - surfaceParameters.P;
			lightDistance = length(L);
			L = L/ max(lightDistance, DistanceEpsilon);
			lightMaxDistance = currentLightSource.influenceRadius;
			attenuation = computeLightAttenuation(lightDistance, currentLightSource.influenceRadius);
		}
		else
		{
			L = normalize(L);
		}

        float NdotL = max(0.0, dot(surfaceParameters.N, L));
		if(NdotL > 0.0 && attenuation > 0.0)
        {
			vec3 H = normalize(L + surfaceParameters.V);
			float NdotH = max(0.0, dot(surfaceParameters.N, H));
			float VdotH = max(0.0, dot(surfaceParameters.V, H));
			
			vec3 F = fresnelSchlick(F0, VdotH);
			float D = ggxSpecularDistribution(directAlpha, NdotH);
			float G = cookTorranceSmithSchlickGGXMasking(directK, NdotL, NdotV);

			lightedColor += (currentLightSource.intensity * (diffuse + F*(D*G))) * (attenuation*NdotL*Pi);
        }

        //color += lightSource.intensity;
#undef currentLightSource
    }

    return vec4(lightedColor, surfaceParameters.occlusionFactor);
}
