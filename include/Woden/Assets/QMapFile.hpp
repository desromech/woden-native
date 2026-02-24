#ifndef WODEN_ASSETS_MAP_FILE_HPP
#define WODEN_ASSETS_MAP_FILE_HPP

#include "Woden/Math/Vector2.hpp"
#include "Woden/Math/Vector3.hpp"
#include "Woden/Math/Plane.hpp"
#include "Woden/SceneGraph/Scene.hpp"
#include "Woden/Rendering/MeshBuilder.hpp"

#include <map>
#include <memory>
#include <vector>
#include <string>

namespace Woden
{
namespace Assets
{

typedef std::shared_ptr<class QMapFile> QMapFilePtr;
typedef std::shared_ptr<class QMapEntity> QMapEntityPtr;
typedef std::shared_ptr<class QMapBrush> QMapBrushPtr;
typedef std::shared_ptr<class QMapFace> QMapFacePtr;

inline Math::Vector3 quakeToWodenCoordinates(const Math::Vector3 &v)
{
    return Math::Vector3(v.x, v.z, -v.y);
}

class QMapFile
{
public:
    static QMapFilePtr parseFromFileNamed(const std::string &filename);
    static QMapFilePtr parseFromString(const std::string &string);

    void computeGeometry();

    void addToSceneWithInverseScale(const SceneGraph::ScenePtr &scene, Math::Scalar inverseScale);

    std::vector<QMapEntityPtr> entities;
};

class QMapEntity
{
public: 
    void addProperty(const std::string &key, const std::string &value);

    bool getBooleanProperty(const std::string &key, bool defaultValue = 0);
    Math::Scalar getScalarProperty(const std::string &key, Math::Scalar defaultValue = 0);
    Math::Vector3 getVector3Property(const std::string &key, const Math::Vector3 &defaultValue = Math::Vector3(0));
    Math::Vector3 getOrigin();
    Math::Vector3 getAngles();
    Math::Quaternion getOrientation();
    bool isLightEntity() const;

    void computeGeometry();
    void groupFacesPerMaterial();
    void computeGroupedTexcoords();

    void addToSceneWithInverseScale(const SceneGraph::ScenePtr &scene, Math::Scalar inverseScale);
    Rendering::LightSourcePtr parseLightSource();
    Rendering::LightSourcePtr parseDirectionalLightSource();
    Rendering::LightSourcePtr parsePointLightSource();
    Rendering::LightSourcePtr parseSpotLightSource();

    std::string className;

    std::map<std::string, std::string> properties;
    std::vector<QMapBrushPtr> brushes;
    std::map<std::string, std::vector<QMapFacePtr>> groupedFaces;
};

class QMapBrush
{
public:
    void computeGeometry();
    bool isPointInBrush(const Math::Vector3 &p) const;

    std::vector<QMapFacePtr> faces;
    std::vector<Math::Vector3> vertices;
};

class QMapFace
{
public:
    void addToMeshWithInverseScale(Woden::Rendering::MeshBuilder &meshBuilder, Math::Scalar inverseScale);
    void resetGeometryComputation();
    void sortVertices();
    void computeTexcoords(const Math::Vector2 &textureExtent);

    Math::Vector3 firstPlanePoint;
    Math::Vector3 secondPlanePoint;
    Math::Vector3 thirdPlanePoint;
    Math::Plane firstTexturePlane;
    Math::Plane secondTexturePlane;
    Math::Scalar rotation;
    Math::Scalar xScale;
    Math::Scalar yScale;
    std::string materialName;

    Math::Plane plane;
    std::vector<Math::Vector3> vertices;
    std::vector<Math::Vector2> texcoords;
};

} // End of namespace Assets
} // namespace Woden

#endif // WODEN_ASSETS_MAP_FILE_HPP