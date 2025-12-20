#ifndef WODEN_MORPHIC_MORPH_HPP
#define WODEN_MORPHIC_MORPH_HPP

#include "Woden/Math/Rectangle.hpp"
#include "Woden/Math/Vector4.hpp"
#include <memory>
#include <vector>

namespace Woden
{
namespace Morphic
{
using namespace Woden::Math;

typedef std::shared_ptr<class Morph> MorphPtr;
typedef std::shared_ptr<class SystemWindow> SystemWindowPtr;

/**
 * I am morph
 */
class Morph : public std::enable_shared_from_this<Morph>
{
public:

    static int runMainLoop();
    SystemWindowPtr openInSystemWindow();

    MorphPtr owner;
    Rectangle bounds = Rectangle(Vector2(0, 0), Vector2(50, 40));
    Vector4 color = Vector4(0, 0, 1, 1);
    std::vector<MorphPtr> submorphs;
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_MORPH_HPP