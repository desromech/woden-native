#include "Woden/Utility/ShaderPreprocessor.hpp"
#include "Woden/Utility/ReadWholeFile.hpp"
#include "Woden/Utility/FileSystem.hpp"
#include <sstream>

namespace Woden
{
namespace Utility
{
std::string ShaderPreprocessor::preprocessFileNamed(const std::string &fileName)
{
    return preprocessFileNamedWithTextData(fileName, readWholeTextFile(fileName));
}

std::string ShaderPreprocessor::preprocessFileNamedWithTextData(const std::string &fileName, const std::string &textData)
{
    std::string directory = dirname(fileName);
    std::istringstream textDataInputStream;
    textDataInputStream.str(textData);
    std::ostringstream preprocessed;
    int lineIndex = 1;
    for(std::string line; std::getline(textDataInputStream, line); )
    {
        if(line.find("#include") != std::string::npos)
        {
            auto includeNameStartPosition  = line.find('"');
            auto includeNameEndPosition  = line.find('"', includeNameStartPosition + 1);
            auto includeName = line.substr(includeNameStartPosition + 1, includeNameEndPosition -1 - includeNameStartPosition);
            auto includePath = join(directory, includeName);

            auto includeData = preprocessFileNamed(includePath);
       
            preprocessed << "\n#line 1\n" << includeData;
            preprocessed << "\n#line " << (lineIndex + 1) << "\n";
            
        }
        else
        {
            preprocessed << line << '\n';
        }
        ++lineIndex;
    }

    return preprocessed.str();
}

} // End of namespace Utility
} // End of namespace Woden