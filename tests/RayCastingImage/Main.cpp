#include "Woden/Assets/Image.hpp"
#include "Woden/Math/AABox.hpp"
#include "Woden/Math/Sphere.hpp"
#include "Woden/Physics/CollisionShape.hpp"

using namespace Woden::Math;

void renderRGTestImage()
{
    auto image = std::make_shared<Woden::Assets::Image> ();
    image->width = 640;
    image->height = 480;
    image->pitch = image->width*4;
    image->format = Woden::Assets::PixelFormat::B8G8R8A8_UNorm;
    image->pixels.resize(image->pitch*image->height);
    image->renderPixels32([](uint32_t x, uint32_t y, uint32_t width, uint32_t height) {
        (void)width;
        (void)height;
        return ((y & 255) << 8) | ((x & 255) << 16) | 0xff000000;
    });

    image->saveToTGA("rgTest.tga");
}

template<typename ST>
void renderRayCasting(const std::string &name, const ST &shape)
{
    auto image = std::make_shared<Woden::Assets::Image> ();
    image->width = 500;
    image->height = 500;
    image->pitch = image->width*4;
    image->format = Woden::Assets::PixelFormat::B8G8R8A8_UNorm;
    image->pixels.resize(image->pitch*image->height);
    image->renderPixels32([&](uint32_t x, uint32_t y, uint32_t width, uint32_t height) {

        auto vx = Scalar(x) / Scalar(width) * 2 - 1;
        auto vy = Scalar(y) / Scalar(height) * 2 - 1;
        auto rayDirection = Vector3(vx, vy, -1.0).normalized();
        auto rayOrigin = Vector3(0, 2, 5);
        auto ray = Ray3D(rayOrigin, rayDirection, 0, 1000);

        RayCastingResult rayCastResult = shape.intersectionsWithRay(ray);

        if(rayCastResult.isEmpty())
            return 0xff0000ff;

        auto intersectionPoint = ray.pointAtDistance(rayCastResult.intersectionPoints[0]);
        auto N = shape.computeNormalForPoint(intersectionPoint);
        auto V = -rayDirection;
        auto NdotV = N.dot(V);
        auto gray = 0;
        if(NdotV >= 0)
            gray = uint8_t(NdotV*255);
        return gray | (gray << 8) | (gray << 16) | 0xff000000;
    });

    image->saveToTGA(name);
}

void renderRayCollisionShape(const std::string &name, const Woden::Physics::CollisionShapePtr &shape)
{
    auto image = std::make_shared<Woden::Assets::Image> ();
    image->width = 500;
    image->height = 500;
    image->pitch = image->width*4;
    image->format = Woden::Assets::PixelFormat::B8G8R8A8_UNorm;
    image->pixels.resize(image->pitch*image->height);
    image->renderPixels32([&](uint32_t x, uint32_t y, uint32_t width, uint32_t height) {

        auto vx = Scalar(x) / Scalar(width) * 2 - 1;
        auto vy = Scalar(y) / Scalar(height) * 2 - 1;
        auto rayDirection = Vector3(vx, vy, -1.0).normalized();
        auto rayOrigin = Vector3(0, 2, 5);
        auto ray = Ray3D(rayOrigin, rayDirection, 0, 1000);

        std::optional<Woden::Physics::ShapeRayCastingResult> rayCastResult = shape->rayCast(ray);

        if(!rayCastResult.has_value())
            return 0xff0000ff;

        auto rayCastResultValue = rayCastResult.value();

        auto N = rayCastResultValue.normal;
        auto V = -rayDirection;
        auto NdotV = N.dot(V);
        auto gray = 0;
        if(NdotV >= 0)
            gray = uint8_t(NdotV*255);
        return gray | (gray << 8) | (gray << 16) | 0xff000000;
    });

    image->saveToTGA(name);
}

int main()
{
    renderRGTestImage();
    renderRayCasting("aabox.tga", Woden::Math::AABox::WithHalfExtent(Woden::Math::Vector3(1, 1, 1)));
    renderRayCasting("sphere.tga", Woden::Math::Sphere::WithRadius(1));

    // Collision sphere
    {
        auto shape = std::make_shared<Woden::Physics::SphereCollisionShape> ();
        shape->setRadius(1);
        renderRayCollisionShape("col-sphere.tga", shape);
    }

    // Collision aabox
    {
        auto shape = std::make_shared<Woden::Physics::BoxCollisionShape> ();
        shape->setHalfExtent(Woden::Math::Vector3(1, 1, 1));
        renderRayCollisionShape("col-aabox.tga", shape);
    }

    return 0;
}
