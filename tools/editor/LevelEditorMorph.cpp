#include "LevelEditorMorph.hpp"

namespace Woden
{
namespace LevelEditor
{

LevelEditorMorph::LevelEditorMorph()
{
    setExtent(Vector2(1024, 576));
}

bool LevelEditorMorph::initialize()
{
    return true;
}

} // End of namespace LevelEditor
} // End of namespace Woden