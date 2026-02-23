#include "Woden/Assets/QMapFile.hpp"
#include "Woden/Assets/ResourceCache.hpp"
#include "Woden/Utility/ReadWholeFile.hpp"
#include "Woden/Rendering/LightSource.hpp"
#include <algorithm>
#include <stdint.h>
#include <sstream>

namespace Woden
{
namespace Assets
{

enum class QMapFileTokenType
{
    Null,
    EndOfFile,
    LeftCBracket,
    RightCBracket,
    LeftBracket,
    RightBracket,
    LeftParent,
    RightParent,
    Name,
    String,
    Number,
};

struct QMapFileToken
{
    QMapFileTokenType type;
    std::string content;
};

class QMapFileTokenizer
{
public:
    QMapFileTokenizer(const std::string &initString)
        : string(initString)
    {        
    }

    bool atEnd() const
    {
        return position >= string.size();
    }

    int peek(int offset = 0)
    {
        size_t peekPosition = position + offset;
        if (peekPosition < string.size())
            return string[peekPosition];
        return -1;
    }

    void advance(int amount = 1)
    {
        position += amount;
    }

    void skipWhite()
    {
        bool hasSeenComment = true;
        while(hasSeenComment)
        {
            hasSeenComment = false;

            while(!atEnd() && peek() <= ' ')
                advance();

            // Single line comment
            if(peek() == '/' && peek(1) == '/')
            {
                advance(2);
                hasSeenComment = true;

                // Eat until the newline or the end of file.
                while(!atEnd())
                {
                    if(peek() == '\n' || peek() == '\r')
                        break;

                    advance();
                }
            }
        }
    }

    QMapFileToken makeTokenStartingFrom(QMapFileTokenType kind, size_t startingPosition)
    {
        QMapFileToken token = {};
        token.type = kind;
        token.content = string.substr(startingPosition, position - startingPosition);
        return token;
    }

    bool isDigit(char c)
    {
        return '0' <= c && c <= '9';
    }

    QMapFileToken scanNextToken()
    {
        skipWhite();
        if(atEnd())
        {
            QMapFileToken eof = {};
            eof.type = QMapFileTokenType::EndOfFile;
            return eof;
        }

        auto startingPosition = position;
        if((peek() == '-' && isDigit(peek(1))) || isDigit(peek()))
        {
            advance();
            while(isDigit(peek()))
                advance();

            if(peek() == '.')
            {
                advance();
                while(isDigit(peek()))
                    advance();
            }

            // Exponent
            if(peek() == 'e' || peek() == 'E')
            {
                advance();
                if(peek() == '-' || peek() == '+')
                    advance();

                while(isDigit(peek()))
                    advance();
            }

            return makeTokenStartingFrom(QMapFileTokenType::Number, startingPosition);
        }

        if(peek() == '"')
        {
            advance();
            size_t stringStartingPosition = position;

            while(!atEnd() && peek() != '"')
            {
                if(peek() == '\\')
                    advance(2);
                else
                    advance();
            }

            auto stringToken = makeTokenStartingFrom(QMapFileTokenType::String, stringStartingPosition);
            if(peek() == '"')
                advance();
            return stringToken;
        }


        switch(peek())
        {
        case '{':
            advance();
            return makeTokenStartingFrom(QMapFileTokenType::LeftCBracket, startingPosition);
        case '}':
            advance();
            return makeTokenStartingFrom(QMapFileTokenType::RightCBracket, startingPosition);
        case '[':
            advance();
            return makeTokenStartingFrom(QMapFileTokenType::LeftBracket, startingPosition);
        case ']':
            advance();
            return makeTokenStartingFrom(QMapFileTokenType::RightBracket, startingPosition);
        case '(':
            advance();
            return makeTokenStartingFrom(QMapFileTokenType::LeftParent, startingPosition);
        case ')':
            advance();
            return makeTokenStartingFrom(QMapFileTokenType::RightParent, startingPosition);
        default:
            break;
        }

        // Fallback to the name
        while(peek() > ' ')
            advance();

        return makeTokenStartingFrom(QMapFileTokenType::Name, startingPosition);
    }

    std::vector<QMapFileToken> tokenizeString()
    {
        std::vector<QMapFileToken> tokens;
        while(position < string.size())
        {
            auto nextToken = scanNextToken();
            tokens.push_back(nextToken);
        }

        // Ensure the last token is an EOF.
        if(tokens.empty() ||tokens.back().type != QMapFileTokenType::EndOfFile)
        {
            QMapFileToken eof = {};
            eof.type = QMapFileTokenType::EndOfFile;
            tokens.push_back(eof);
        }


        return tokens;
    }

    const std::string &string;
    size_t position = 0;
};

class QMapFileParser
{
public:
    QMapFileParser(const std::vector<QMapFileToken> &initTokens)
        : tokens(initTokens)
    {
    }

    void advance(int amount = 1)
    {
        position += amount;
    }

    QMapFileTokenType peekTokenType(int offset = 0)
    {
        size_t peekPosition = position + offset;
        if(peekPosition < tokens.size())
            return tokens[peekPosition].type;
        return QMapFileTokenType::EndOfFile;        
    }

    bool match(QMapFileTokenType expectedTokenType)
    {
        if(peekTokenType() != expectedTokenType)
            return false;
        advance();
        return true;
    }

    QMapFileToken nextToken()
    {
        return tokens[position++];
    }

    Math::Scalar parseNumber()
    {
        if(peekTokenType() != QMapFileTokenType::Number)
            return 0;
        return Math::Scalar(atof(nextToken().content.c_str()));
    }

    Math::Vector3 parsePoint()
    {
        match(QMapFileTokenType::LeftParent);
        auto x = parseNumber();
        auto y = parseNumber();
        auto z = parseNumber();
        match(QMapFileTokenType::RightParent);
        return Math::Vector3(x, y, z);
    }

    Math::Plane parseTexturePlane()
    {
        match(QMapFileTokenType::LeftBracket);
        auto nx = parseNumber();
        auto ny = parseNumber();
        auto nz = parseNumber();
        auto d = parseNumber();
        match(QMapFileTokenType::RightBracket);
        return Math::Plane(Math::Vector3(nx, ny, nz), d);
    }

    std::string parseName()
    {
        if(peekTokenType() != QMapFileTokenType::Name)
            return std::string();

        return nextToken().content;
    }

    QMapFacePtr parseFace()
    {
        auto face = std::make_shared<QMapFace> ();
        face->firstPlanePoint = parsePoint();
        face->secondPlanePoint = parsePoint();
        face->thirdPlanePoint = parsePoint();
        face->materialName = parseName();
        face->firstTexturePlane = parseTexturePlane();
        face->secondTexturePlane = parseTexturePlane();
        face->rotation = parseNumber();
        face->xScale = parseNumber();
        face->yScale = parseNumber();
        return face;
    }

    QMapBrushPtr parseBrush()
    {
        if(!match(QMapFileTokenType::LeftCBracket))
            return nullptr;

        auto brush = std::make_shared<QMapBrush> ();

        while(peekTokenType() == QMapFileTokenType::LeftParent)
        {
            auto face = parseFace();
            if(face)
                brush->faces.push_back(face);
        }

        match(QMapFileTokenType::RightCBracket);
        return brush;
    }

    QMapEntityPtr parseEntity()
    {
        if(!match(QMapFileTokenType::LeftCBracket))
            return nullptr;

        auto entity = std::make_shared<QMapEntity> ();

        // Properties.
        while (peekTokenType() == QMapFileTokenType::String)
        {
            std::string key = nextToken().content;
            std::string value;
            if(peekTokenType() == QMapFileTokenType::String)
                value = nextToken().content;
            entity->addProperty(key, value);
        }

        // Brushes
        while(peekTokenType() == QMapFileTokenType::LeftCBracket)
        {
            auto brush = parseBrush();
            entity->brushes.push_back(brush);
        }

        match(QMapFileTokenType::RightCBracket);
        return entity;
    }

    QMapFilePtr parseMap()
    {
        auto map = std::make_shared<QMapFile> ();
        while(peekTokenType() == QMapFileTokenType::LeftCBracket)
        {
            auto entity = parseEntity();
            if(!entity)
                return map;
            map->entities.push_back(entity);
        }

        return map;
    }

    const std::vector<QMapFileToken> &tokens;
    size_t position = 0;
};

QMapFilePtr QMapFile::parseFromFileNamed(const std::string &filename)
{
    auto string = Utility::readWholeTextFile(filename);
    if(string.empty())
        return nullptr;
    return parseFromString(string);
}

QMapFilePtr QMapFile::parseFromString(const std::string &string)
{
    auto tokens = QMapFileTokenizer(string).tokenizeString();
    return QMapFileParser(tokens).parseMap();
}

void QMapFile::computeGeometry()
{
    for(auto &entity : entities)
        entity->computeGeometry();
}

void QMapFile::addToSceneWithInverseScale(const SceneGraph::ScenePtr &scene, Math::Scalar inverseScale)
{
    for(auto &entity : entities)
        entity->addToSceneWithInverseScale(scene, inverseScale);
}

void QMapEntity::computeGeometry()
{
    for(auto &brush : brushes)
        brush->computeGeometry();

    groupFacesPerMaterial();
    computeGroupedTexcoords();
}

void QMapEntity::groupFacesPerMaterial()
{
    groupedFaces.clear();

    for(auto &brush : brushes)
    {
        for(auto &face : brush->faces)
        {
            if(groupedFaces.find(face->materialName) == groupedFaces.end())
                groupedFaces[face->materialName] = std::vector<QMapFacePtr> ();
            groupedFaces.find(face->materialName)->second.push_back(face);
        }
    }
}

void QMapEntity::computeGroupedTexcoords()
{
    for(auto &group : groupedFaces)
    {
        auto materialName = group.first;
        auto &faces = group.second;

        auto textureExtent = Woden::Assets::ResourceCache::Get()->fetchTextureExtent(materialName);
        //printf("material %s extent %f %f\n", materialName.c_str(), textureExtent.x, textureExtent.y);
        for (auto &face : faces)
            face->computeTexcoords(textureExtent);
    }
}

void QMapEntity::addToSceneWithInverseScale(const SceneGraph::ScenePtr &scene, Math::Scalar inverseScale)
{
    auto origin = quakeToWodenCoordinates(getOrigin()) / inverseScale;
    auto sceneNode = std::make_shared<SceneGraph::SceneNode> ();
    sceneNode->transform.translation = origin;

    if(!groupedFaces.empty())
    {
        Woden::Rendering::MeshBuilder meshBuilder;

        for(auto faceGroup : groupedFaces)
        {
            auto &materialName = faceGroup.first;
            auto &faces = faceGroup.second;
            auto material = Woden::Assets::ResourceCache::Get()->getOrLoadMaterial(materialName);

            meshBuilder.setMaterial(material);
            for (auto &face : faces)
                face->addToMeshWithInverseScale(meshBuilder, inverseScale);
        }

        meshBuilder.generateTangentSpaceFrame();
        sceneNode->addRenderable(meshBuilder.finishMesh());
    }

    if(isLightEntity())
    {
        auto lightSource = parseLightSource();
        if(lightSource)
            sceneNode->addLightSource(lightSource);
    }

    scene->normalLayer->addChild(sceneNode);
}

void QMapEntity::addProperty(const std::string &key, const std::string &value)
{
    properties[key] = value;
    if(key == "classname")
    {
        className = value;
        return;
    }
}

bool QMapEntity::getBooleanProperty(const std::string &key, bool defaultValue)
{
    bool result = defaultValue;
    auto it = properties.find(key);
    if(it != properties.end())
        result = atoi(it->second.c_str()) != 0;

    return result;
}

Math::Scalar QMapEntity::getScalarProperty(const std::string &key, Math::Scalar defaultValue)
{
    Math::Scalar result = defaultValue;
    auto it = properties.find(key);
    if(it != properties.end())
        result = atof(it->second.c_str());

    return result;
}

Math::Vector3 QMapEntity::getVector3Property(const std::string &key, const Math::Vector3 &defaultValue)
{
    Math::Vector3 v = defaultValue;
    auto it = properties.find(key);
    if(it != properties.end())
    {
        std::istringstream in(it->second);
        in >> v.x >> v.y >> v.z;
    }

    return v;
}

Math::Vector3 QMapEntity::getOrigin()
{
    return getVector3Property("origin");
}

bool QMapEntity::isLightEntity() const
{
    return className == "light" || className == "light_spot" || className == "light_directional";
}

Rendering::LightSourcePtr QMapEntity::parseLightSource()
{
    if(className == "light_spot")
        return parseSpotLightSource();
    if(className == "light_directional")
        return parseDirectionalLightSource();
    return parsePointLightSource();
}

Rendering::LightSourcePtr QMapEntity::parseDirectionalLightSource()
{
    auto directionalLightSource = std::make_shared<Rendering::DirectionalLightSource> ();
    directionalLightSource->castShadows = getBooleanProperty("castShadows", false);
    directionalLightSource->color = getVector3Property("color", directionalLightSource->color);
    directionalLightSource->intensity = getScalarProperty("intensity", 1.0);
    return directionalLightSource;
}

Rendering::LightSourcePtr QMapEntity::parsePointLightSource()
{
    auto pointLightSource = std::make_shared<Rendering::PointLightSource> ();
    pointLightSource->castShadows = getBooleanProperty("castShadows", false);
    pointLightSource->color = getVector3Property("color", pointLightSource->color);
    pointLightSource->intensity = getScalarProperty("intensity", 1.0);
    pointLightSource->influenceRadius = getScalarProperty("influenceRadius", 1.0);
    return pointLightSource;
}

Rendering::LightSourcePtr QMapEntity::parseSpotLightSource()
{
    auto spotLightSource = std::make_shared<Rendering::SpotLightSource> ();
    spotLightSource->castShadows = getBooleanProperty("castShadows", false);
    spotLightSource->color = getVector3Property("color", spotLightSource->color);
    spotLightSource->intensity = getScalarProperty("intensity", 1.0);
    spotLightSource->influenceRadius = getScalarProperty("influenceRadius", 1.0);
    spotLightSource->innerCutoff = getScalarProperty("innerCutoff", spotLightSource->innerCutoff);
    spotLightSource->outerCutoff = getScalarProperty("outerCutoff", spotLightSource->outerCutoff);
    return spotLightSource;
}


void QMapBrush::computeGeometry()
{
    // Reset the face geometry.
    vertices.clear();
    for(auto &face : faces)
    {
        face->resetGeometryComputation();
    }

    // Compute the vertices.
    for(size_t i = 0; i < faces.size(); ++i)
    {
        auto &firstFace = faces[i];
        auto &firstPlane = firstFace->plane;
        for(size_t j = i + 1; j < faces.size(); ++j)
        {
            auto &secondFace = faces[j];
            auto &secondPlane = secondFace->plane;
            for(size_t k = j + 1; k < faces.size(); ++k)
            {
                auto &thirdFace = faces[k];
                auto &thirdPlane = thirdFace->plane;

                auto m = Math::Matrix3x3::WithRows(firstPlane.normal, secondPlane.normal, thirdPlane.normal);
                auto det = m.determinant();
                if(det != 0)
                {
                    auto v = Math::Vector3(firstPlane.distance, secondPlane.distance, thirdPlane.distance);
                    auto p = m.inverse() * v;
                    if(isPointInBrush(p))
                    {
                        //printf("p %f %f %f\n", p.x, p.y, p.z);
                        vertices.push_back(p);
                        firstFace->vertices.push_back(p);
                        secondFace->vertices.push_back(p);
                        thirdFace->vertices.push_back(p);
                    }
                }
            }
        }
    }
    
    // Sort the vertices.
    for(auto &face : faces)
        face->sortVertices();
}

bool QMapBrush::isPointInBrush(const Math::Vector3 &p) const
{
    Math::Scalar Epsilon = 1e-5;
    //printf("ib: %f %f %f\n", p.x, p.y, p.z);
    for(auto &face : faces)
    {
        //auto sd = face->plane.signedDistanceToPoint(p);
        //printf("sd: %f\n", sd);
        if(!face->plane.isPointInsideOrBehind(p, Epsilon))
            return false;
    }
    return true;
}

void QMapFace::addToMeshWithInverseScale(Woden::Rendering::MeshBuilder &meshBuilder, Math::Scalar inverseScale)
{
    meshBuilder.beginTriangles();

    for(size_t i = 0; i < vertices.size(); ++i)
    {
        auto &vertex = vertices[i];
        auto &texcoord = texcoords[i];

        meshBuilder.positions.push_back(quakeToWodenCoordinates(vertex) / inverseScale);
        meshBuilder.texcoords.push_back(texcoord);
        meshBuilder.normals.push_back(quakeToWodenCoordinates(plane.normal));
    }

    for(size_t i = 2; i < vertices.size(); ++i)
        meshBuilder.addTriangleI012(0, i - 1, i);
}

void QMapFace::resetGeometryComputation()
{
    plane = Math::Plane::WithQPoints(firstPlanePoint, secondPlanePoint, thirdPlanePoint);
    //printf("plane %f %f %f = %f\n", plane.normal.x, plane.normal.y, plane.normal.z, plane.distance);
    vertices.clear();
    texcoords.clear();
}

void QMapFace::sortVertices()
{
    if(vertices.empty())
        return;
    
    Math::Vector3 centroid = Math::Vector3(0);
    for(auto &v : vertices)
        centroid += v;
    centroid /= Math::Vector3(Math::Scalar(vertices.size()));

    auto n = plane.normal;
    std::sort(vertices.begin(), vertices.end(), [&] (const Math::Vector3 &a, const Math::Vector3 &b) {
        auto u = centroid - a;
        auto v = centroid - b;
        return u.cross(v).dot(n) >= 0;
    });
}

void QMapFace::computeTexcoords(const Math::Vector2 &textureExtent)
{
    texcoords.clear();
    texcoords.reserve(vertices.size());
    for(auto &vertex : vertices)
    {
        auto tu = firstTexturePlane.normal.dot(vertex)/textureExtent.x;
        tu /= xScale;
        tu += firstTexturePlane.distance / textureExtent.x;

        auto tv = secondTexturePlane.normal.dot(vertex)/textureExtent.y;
        tv /= yScale;
        tv += firstTexturePlane.distance / textureExtent.y;

        texcoords.push_back(Math::Vector2(tu, tv));
    }
}


} // End of namespace Assets
} // namespace Woden
