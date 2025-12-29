#line 2

layout(location = 0) out vec4 outColor;

layout(location = 0) in vec3 inNormal;

void main()
{
    vec3 N = normalize(inNormal);
    outColor = vec4(N*0.5 + 0.5, 1.0);
}
