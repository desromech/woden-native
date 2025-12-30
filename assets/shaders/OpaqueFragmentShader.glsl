#line 2

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outNormal;
layout(location = 2) out vec4 outSpecular;

layout(location = 0) in vec2 inTexcoord;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec4 inTangent4;
layout(location = 3) in vec3 inViewPosition;

void main()
{
    vec3 P = inViewPosition;
    vec3 V = normalize(-inViewPosition);
    vec3 N = normalize(inNormal);
    float VdotN = max(dot(V, N), 0.0);
    vec3 lightedColor = GlobalLightingState.ambientLighting;

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
			L = L - P;
			lightDistance = length(L);
			L = L/ max(lightDistance, DistanceEpsilon);
			lightMaxDistance = currentLightSource.influenceRadius;
			attenuation = computeLightAttenuation(lightDistance, currentLightSource.influenceRadius);
		}
		else
		{
			L = normalize(L);
		}

        float NdotL = max(0.0, dot(N, L));
		if(NdotL > 0.0 && attenuation > 0.0)
        {
            lightedColor += currentLightSource.intensity*attenuation*NdotL;
        }

        //color += lightSource.intensity;
#undef currentLightSource
    }

    
    outColor = vec4(lightedColor, 1.0);
    //outColor = vec4(VdotN.xxx, 1.0);
    outNormal = N.xy;
    outSpecular = vec4(0.2);
}
