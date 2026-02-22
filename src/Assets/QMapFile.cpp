#include "Woden/Assets/QMapFile.hpp"
#include "Woden/Utility/ReadWholeFile.hpp"
#include <stdint.h>

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
        face->firstPoint = parsePoint();
        face->secondPoint = parsePoint();
        face->thirdPoint = parsePoint();
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

        printf("paseBrushFace %d\n", int(peekTokenType()));
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
            entity->properties[key] = value;
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

} // End of namespace Assets
} // namespace Woden
