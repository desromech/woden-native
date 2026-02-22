#ifndef WODEN_ASSETS_MAP_FILE_HPP
#define WODEN_ASSETS_MAP_FILE_HPP

#include "Woden/Math/Vector3.hpp"
#include "Woden/Math/Plane.hpp"

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

class QMapFile
{
public:
    static QMapFilePtr parseFromFileNamed(const std::string &filename);
    static QMapFilePtr parseFromString(const std::string &string);

    std::vector<QMapEntityPtr> entities;

};

class QMapEntity
{
public: 
    std::map<std::string, std::string> properties;
    std::vector<QMapBrushPtr> brushes;
};

class QMapBrush
{
public: 
    std::vector<QMapFacePtr> faces;
};

class QMapFace
{
public:
    Math::Vector3 firstPoint;
    Math::Vector3 secondPoint;
    Math::Vector3 thirdPoint;
    Math::Plane firstTexturePlane;
    Math::Plane secondTexturePlane;
    Math::Scalar rotation;
    Math::Scalar xScale;
    Math::Scalar yScale;
    std::string materialName;
};

} // End of namespace Assets
} // namespace Woden

#endif // WODEN_ASSETS_MAP_FILE_HPP