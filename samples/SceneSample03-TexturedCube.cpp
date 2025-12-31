#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/LightSource.hpp"
#include "Woden/Rendering/MeshBuilder.hpp"
#include "Woden/Rendering/MetallicRoughnessMaterial.hpp"
#include "Woden/Rendering/Renderable.hpp"
#include "Woden/Assets/Texture.hpp"
#include "Woden/Morphic/Morph.hpp"
#include "Woden/SceneGraph/Scene.hpp"

using namespace Woden::Morphic;
using namespace Woden::SceneGraph;
using namespace Woden::Math;

Woden::Assets::ImagePtr computeCheckboardImage()
{
    auto image = std::make_shared<Woden::Assets::Image> ();
    image->width = 128;
    image->height = 128;
    image->pitch = 4*image->width;
    image->format = Woden::Assets::PixelFormat::B8G8R8A8_UNorm_SRGB;
    image->pixels.resize(image->pitch*image->height);

    image->renderNormalizedPixel32([](Scalar x, Scalar y){
        auto xp = floorFractionPart(x*5.0) < 0.5;
        auto yp = floorFractionPart(y*5.0) < 0.5;
        return (xp ^ yp) ? 0xffffffff : 0xff000000;
    });

    image->saveToTGA("checkboard.tga");

    return image;
}

Woden::Assets::TexturePtr computeCheckboardTexture()
{
    auto image = computeCheckboardImage();
    auto texture = std::make_shared<Woden::Assets::Texture> ();
    texture->miplevels.push_back(image);
    return texture;
}

int main(int argc, const char *argv[])
{
    if(!Woden::Rendering::RenderingContext::getOrCreateMainContext(argc, argv))
        return 1;

    auto image = computeCheckboardImage();

    auto scene = MakeScene();
    {
        auto material = std::make_shared<Woden::Rendering::MetallicRoughnessMaterial> ();
        material->roughnessFactor = 0.4;
        material->metallicFactor = 0.0;
        material->baseColorTexture = computeCheckboardTexture();

        auto builder = Woden::Rendering::MeshBuilder();
        builder.currentMaterial = material;
        builder.addCubeWithExtent(Vector3(1, 1, 1));
        builder.generateTexcoordsWithFacePlanarTransformWithScale(Vector2(1, 1));
        scene->normalLayer->addChild(builder.finishMesh()->asSceneNode());
    }

    {
        auto pointLightSource = std::make_shared<Woden::Rendering::PointLightSource> ();
        pointLightSource->color = Vector3(0.8, 0.8, 0.2);
        pointLightSource->intensity = 5;
        pointLightSource->influenceRadius = 4;

        scene->normalLayer->addChild(pointLightSource->asSceneNodeWithPosition(Vector3(-1.5, 1.5, 1.6)));

    }

    scene->openInSystemWindow();

    int exitCode = Morph::runMainLoop();
    Woden::Rendering::RenderingContext::releaseMainContext();
    return exitCode;
}