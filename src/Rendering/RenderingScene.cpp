#include "Woden/Rendering/RenderingScene.hpp"
#include "Woden/Rendering/LightSource.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Math/AABox.hpp"

namespace Woden
{
namespace Rendering
{

void RenderingScene::addDirectionalLightSource(const DirectionalLightSource *lightSource)
{
    auto direction = currentModelMatrix.topLeftMatrix3x3().thirdColumn();

    RenderingLightSourceObject renderingLight = {};
    renderingLight.castShadows = lightSource->castShadows;
    renderingLight.positionOrDirection = Math::Vector4(direction, 0.0);
    renderingLight.intensityAndColor = lightSource->color*Math::Vector3(lightSource->intensity);
    renderingLight.shadowMapNormalBiasFactor = lightSource->shadowMapNormalBiasFactor;

    if(lightSource->castShadows && currentCamera)
    {
        auto nearDistance = currentCamera->nearDistance;
        auto farDistance = currentCamera->farDistance;
        auto cascadeSplitDistributionLambda = currentCamera->cascadeSplitDistributionLambda;

        auto numSlices = 4;

        Math::Scalar splitDistributionDistances[5] = {};

        // Use a different slice distribution for perspective vs orthographic
        if(currentCamera->isPerspective)
        {
            for(int i = 0; i < 5; ++i)
            {
                Math::Scalar uniformSlice = (farDistance - nearDistance) / Math::Scalar(numSlices) * i + nearDistance;
                Math::Scalar exponentialSlice = nearDistance * pow(farDistance / nearDistance, i / Math::Scalar(numSlices));
                splitDistributionDistances[i] = Math::mix(uniformSlice, exponentialSlice, cascadeSplitDistributionLambda);
            }
        }
        else
        {
            // Use the uniform slices for non-perspective camers.
            for(int i = 0; i < 5; ++i)
                splitDistributionDistances[i] = (farDistance - nearDistance) / Math::Scalar(numSlices) * i + nearDistance;
        }

        Math::Scalar splitDistributionLambdas[5] = {};
        for(int i = 0; i < 5; ++i)
        {
            splitDistributionLambdas[i] = Math::clamp((splitDistributionDistances[i] - nearDistance) / (farDistance - nearDistance), Math::Scalar(0), Math::Scalar(1));
        }

	    renderingLight.shadowMapCascadeDistanceWorldTransform = -currentViewMatrix.thirdRow();
	    renderingLight.shadowMapCascadeOffsets = Math::Vector4(splitDistributionDistances[1], splitDistributionDistances[2], splitDistributionDistances[3], splitDistributionDistances[4]);

        auto device = RenderingContext::getMainContext()->device;
        bool flipProjectionVertically = device->hasTopLeftNdcOrigin();

        for(int sliceIndex = 0; sliceIndex < numSlices; ++sliceIndex)
        {
            auto sliceFrustum = currentWorldFrustum.splitAtLambdas(splitDistributionLambdas[sliceIndex], splitDistributionLambdas[sliceIndex + 1]);

            std::vector<Math::Vector3> shadowVolume;
            shadowVolume.reserve(8);
            sliceFrustum.cornersDo([&](const Math::Vector3 &corner){
                shadowVolume.push_back(corner);
            });


            auto shadowCastingVolumeWorldBoundingBox = Math::AABox::Encompassing(shadowVolume);
            auto shadowCastingVolumeWorldCenter = shadowCastingVolumeWorldBoundingBox.center();

            auto shadowModelMatrix = Math::Matrix4x4::WithMatrix3x3AndTranslation(currentModelMatrix.topLeftMatrix3x3(), shadowCastingVolumeWorldCenter);
            auto inverseShadowModel = shadowModelMatrix.inverse();

            renderingLight.modelMatrix[sliceIndex] = shadowModelMatrix;
            renderingLight.inverseModelMatrix[sliceIndex] = inverseShadowModel;

            auto localVolume = Math::AABox::Empty();
            for(auto &point : shadowVolume)
                localVolume.insertPoint(inverseShadowModel.transformPosition(point));

            localVolume.minCorner.z -= lightSource->shadowCastingRadius;
            localVolume.maxCorner.z += lightSource->shadowCastingRadius;

            auto projectionMatrix = Math::Matrix4x4::ReverseDepthOrtho(
                localVolume.minCorner.x, localVolume.maxCorner.x,
                localVolume.minCorner.y, localVolume.maxCorner.y,
                localVolume.minCorner.z, localVolume.maxCorner.z
            );

            if(flipProjectionVertically)
                projectionMatrix = Math::Matrix4x4::ProjectionInvertYMatrix() * projectionMatrix;

            renderingLight.projectionMatrix[sliceIndex] = projectionMatrix;
            renderingLight.inverseProjectionMatrix[sliceIndex] = projectionMatrix.inverse();
        }
        
        renderingLight.shadowMapPartCount = numSlices;
    }

    lightSources.push_back(renderingLight);
}

void RenderingScene::addPointLightSource(const PointLightSource *lightSource)
{
    RenderingLightSourceObject renderingLight = {};
    renderingLight.castShadows = lightSource->castShadows;
    renderingLight.positionOrDirection = currentModelMatrix.fourthColumn();
    renderingLight.intensityAndColor = lightSource->color*Math::Vector3(lightSource->intensity);
    renderingLight.influenceRadius = lightSource->influenceRadius;
    renderingLight.shadowMapNormalBiasFactor = lightSource->shadowMapNormalBiasFactor;

    if(lightSource->castShadows)
    {
        renderingLight.shadowMapPartCount = 6;

        auto fovy = 90;
        auto aspect = 1.0;
        auto projectionMatrix = Math::Matrix4x4::ReverseDepthPerspective(fovy, aspect, 0.01, lightSource->influenceRadius);

        auto device = RenderingContext::getMainContext()->device;
        bool flipProjectionVertically = device->hasTopLeftNdcOrigin();
        if(flipProjectionVertically)
            projectionMatrix = Math::Matrix4x4::ProjectionInvertYMatrix() * projectionMatrix;
        auto inverseProjectionMatrix = projectionMatrix.inverse();

        for(uint32_t partIndex = 0; partIndex < renderingLight.shadowMapPartCount; ++partIndex)
        {
            auto cubeFaceMatrix = Math::Matrix3x3::CubeMapFaceRotations[partIndex];

            renderingLight.modelMatrix[partIndex] = currentModelMatrix * Math::Matrix4x4::WithMatrix3x3(cubeFaceMatrix);
            renderingLight.inverseModelMatrix[partIndex] = Math::Matrix4x4::WithMatrix3x3(cubeFaceMatrix.transpose()) * currentInverseModelMatrix;

            renderingLight.projectionMatrix[partIndex] = projectionMatrix;
            renderingLight.inverseProjectionMatrix[partIndex] = inverseProjectionMatrix;
        }
    }

    lightSources.push_back(renderingLight);
}



void RenderingScene::addSpotLightSource(const SpotLightSource *lightSource)
{
    RenderingLightSourceObject renderingLight = {};
    renderingLight.castShadows = lightSource->castShadows;
    renderingLight.positionOrDirection = currentModelMatrix.fourthColumn();
    renderingLight.spotDirection = currentModelMatrix.topLeftMatrix3x3().thirdColumn();
    renderingLight.intensityAndColor = lightSource->color*Math::Vector3(lightSource->intensity);
    renderingLight.influenceRadius = lightSource->influenceRadius;
    renderingLight.innerSpotCosCutoff = cos(lightSource->innerCutoff * (M_PI / 180.0));
    renderingLight.outerSpotCosCutoff = cos(lightSource->outerCutoff * (M_PI / 180.0));
    renderingLight.shadowMapNormalBiasFactor = lightSource->shadowMapNormalBiasFactor;

    if(lightSource->castShadows)
    {
        renderingLight.shadowMapPartCount = 1;

        auto fovy = lightSource->outerCutoff*2;
        auto aspect = 1.0;
        auto projectionMatrix = Math::Matrix4x4::ReverseDepthPerspective(fovy, aspect, 0.01, lightSource->influenceRadius);

        auto device = RenderingContext::getMainContext()->device;
        bool flipProjectionVertically = device->hasTopLeftNdcOrigin();
        if(flipProjectionVertically)
            projectionMatrix = Math::Matrix4x4::ProjectionInvertYMatrix() * projectionMatrix;

        renderingLight.modelMatrix[0] = currentModelMatrix;
        renderingLight.inverseModelMatrix[0] = currentInverseModelMatrix;

        renderingLight.projectionMatrix[0] = projectionMatrix;
        renderingLight.inverseProjectionMatrix[0] = projectionMatrix.inverse();
    }

    lightSources.push_back(renderingLight);
}

void RenderingScene::addObjectWithRenderable(const RenderablePtr &renderable)
{
    RenderingSceneObject sceneObject = {};
    sceneObject.renderable = renderable;
    sceneObject.modelMatrix = currentModelMatrix;
    sceneObject.inverseModelMatrix = currentInverseModelMatrix;
    opaqueObjects.push_back(sceneObject);
}

} // End of namespace Rendering
} // End of namespace Woden