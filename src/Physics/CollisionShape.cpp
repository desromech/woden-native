#include "Woden/Physics/CollisionShape.hpp"
#include "Woden/Math/Sphere.hpp"
#include "Woden/Rendering/MeshBuilder.hpp"
#include "Woden/Rendering/Renderable.hpp"
#include "Woden/SceneGraph/Scene.hpp"

namespace Woden
{
namespace Physics
{
// Collision shape
bool CollisionShape::isConvex() const
{
    return false;
}

std::optional<ShapeRayCastingResult> CollisionShape::rayCast(const Math::Ray3D &ray)
{
    (void)ray;
    return std::nullopt;
}

SceneGraph::SceneNodePtr CollisionShape::constructVisualizationSceneNode()
{
    return nullptr;
}

// Convex collision shape
bool ConvexCollisionShape::isConvex() const
{
    return true;
}

// Sphere collision shape
std::optional<ShapeRayCastingResult> SphereCollisionShape::rayCast(const Math::Ray3D &ray)
{
    auto sphere = Math::Sphere(Math::Vector3(0), radius);
    auto result = sphere.intersectionsWithRay(ray);
    if(result.isEmpty())
        return std::nullopt;

    ShapeRayCastingResult shapeResult;
    shapeResult.distance = result.intersectionPoints[0];
    shapeResult.point = ray.pointAtDistance(shapeResult.distance);
    shapeResult.normal = sphere.computeNormalForPoint(shapeResult.point);
    return shapeResult;
}

// Box collision shape
std::optional<ShapeRayCastingResult> BoxCollisionShape::rayCast(const Math::Ray3D &ray)
{
    auto box = Math::AABox::WithHalfExtent(halfExtent);

    auto result = box.intersectionsWithRay(ray);
    if(result.isEmpty())
        return std::nullopt;

    ShapeRayCastingResult shapeResult;
    shapeResult.distance = result.intersectionPoints[0];
    shapeResult.point = ray.pointAtDistance(shapeResult.distance);
    shapeResult.normal = box.computeNormalForPoint(shapeResult.point);
    return shapeResult;
}

SceneGraph::SceneNodePtr BoxCollisionShape::constructVisualizationSceneNode()
{
    return Woden::Rendering::MeshBuilder()
            .addCubeWithHalfExtent(halfExtent)
            .generateTexcoordsWithFacePlanarTransformWithScale(Math::Vector2(1, 1))
            .finishMesh()
            ->asSceneNode();
}

} // End of namespace Physics
} // End of namespace Woden