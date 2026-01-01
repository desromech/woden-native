#line 2
layout(local_size_x = 128) in;

void main()
{
    uint invocationIndex = gl_GlobalInvocationID.x;
    if(invocationIndex >= GlobalLightingState.numberOfLights)
        return;

    LightSourceState lightSource = WorldLightSourceStateList.list[invocationIndex];
    lightSource.positionOrDirection = CurrentCameraState.inverseTransformation * lightSource.positionOrDirection;
    lightSource.spotDirection = (CurrentCameraState.inverseTransformation * vec4(lightSource.spotDirection, 0.0)).xyz;

    for(int i = 0; i < 6; ++i)
    {
        lightSource.modelMatrix[i] = CurrentCameraState.inverseTransformation * lightSource.modelMatrix[i];
        lightSource.inverseModelMatrix[i] = lightSource.inverseModelMatrix[i] * CurrentCameraState.transformation;
    }

    ViewLightSourceStateList.list[invocationIndex] = lightSource;
}