#include "LevelModel.hpp"

namespace Woden
{
namespace LevelEditor
{

std::string LevelElement::asString() const
{
    return "[LE]" + name;
}

Morphic::TableDataSourceElementPtr LevelModel::getElementAtIndex(size_t index)
{
    if(index < elements.size())
        return elements[index];
    return nullptr;
}

bool LevelModel::isValidIndex(size_t index)
{
    return index < elements.size();
}

size_t LevelModel::getNumberOfElements()
{
    return elements.size();
}

} // End of namespace LevelEditor
} // End of namespace Woden
