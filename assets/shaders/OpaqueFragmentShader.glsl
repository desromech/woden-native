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
    vec3 V = normalize(-inViewPosition);
    vec3 N = normalize(inNormal);
    float VdotN = max(dot(V, N), 0.0);
    
    outColor = vec4(VdotN.xxx, 1.0);
    outNormal = N.xy;
    outSpecular = vec4(0.2);
}
