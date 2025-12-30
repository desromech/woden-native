#line 2
layout(local_size_x = 128) in;

void main()
{
    uint invocationIndex = gl_GlobalInvocationID.x;
    if(invocationIndex >= GlobalLightingState.numberOfLights)
        return;

    LightSourceState lightSource = WorldLightSourceStateList.list[invocationIndex];
    lightSource.positionOrDirection = CurrentCameraState.inverseTransformation * lightSource.positionOrDirection;
    //lightSource.spotDirection = (CurrentCameraState.viewMatrix * vec4(lightSource.spotDirection, 0.0)).xyz;

    ViewLightSourceStateList.list[invocationIndex] = lightSource;
}