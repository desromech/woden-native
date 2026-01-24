#ifndef WODEN_EDITOR_LEVEl_MODEL_HPP
#define WODEN_EDITOR_LEVEl_MODEL_HPP

#include <memory>
#include <vector>

namespace Woden
{
namespace LevelEditor
{

typedef std::shared_ptr<class LevelModel> LevelModelPtr;
typedef std::shared_ptr<class LevelElement> LevelElementPtr;

class LevelElement
{
public:
};

class LevelModel
{
public:
    std::vector<LevelElementPtr> elements;
};

} // End of namespace LevelEditor
} // End of namespace Woden

#endif //WODEN_EDITOR_LEVEl_MODEL_HPP