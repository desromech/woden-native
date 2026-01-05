#ifndef WODEN_UTILITY_SHADER_PREPROCESSOR
#define WODEN_UTILITY_SHADER_PREPROCESSOR

#include <string>

namespace Woden
{
namespace Utility
{
/**
 * Shader preprocessor. Utility for parsing and evaluating #include in shader source files. 
 */
class ShaderPreprocessor
{
public:
    std::string preprocessFileNamed(const std::string &fileName);
    std::string preprocessFileNamedWithTextData(const std::string &fileName, const std::string &textData);

};
} // End of namespace Utility
} // End of namespace Woden
#endif //WODEN_UTILITY_SHADER_PREPROCESSOR