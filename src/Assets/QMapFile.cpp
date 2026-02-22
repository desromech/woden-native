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

QMapFilePtr QMapFile::parseFromFileNamed(const std::string &filename)
{
    auto string = Utility::readWholeTextFile(filename);
    if(string.empty())
        return nullptr;
    return parseFromString(string);
}

QMapFilePtr QMapFile::parseFromString(const std::string &string)
{
    auto map = std::make_shared<QMapFile> ();
    auto tokens = QMapFileTokenizer(string).tokenizeString();
    printf("TODO: parse qmap file. %zu tokens\n", tokens.size());
    for(auto &token : tokens)
    {
        printf("%d: %s\n", int(token.type), token.content.c_str());
    }
    return map;
}

} // End of namespace Assets
} // namespace Woden
