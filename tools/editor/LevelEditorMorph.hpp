#ifndef WODEN_TOOLS_LEVEL_EDITOR_MORPH_HPP
#define WODEN_TOOLS_LEVEL_EDITOR_MORPH_HPP

#include "Woden/Morphic/Morph.hpp"

namespace Woden
{
namespace LevelEditor
{
using namespace Woden::Morphic;

class LevelEditorMorph : public Morph
{
public:
    LevelEditorMorph();
    
    bool initialize();
};

} // End of namespace LevelEditor
} // End of namespace Woden

#endif //WODEN_TOOLS_LEVEL_EDITOR_MORPH_HPP