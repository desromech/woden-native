#ifndef WODEN_MORPHIC_BORDERED_MORPH
#define WODEN_MORPHIC_BORDERED_MORPH

#include "Morph.hpp"

namespace Woden
{
namespace Morphic
{

class BorderedMorph : public Morph
{
public:
    virtual Vector4 getCurrentBorderColor() const
    {
        return borderColor;
    }

    virtual void drawWith(const Rendering::GUIRendererPtr &renderer);
    
    Vector4 borderColor = Vector4(0, 0, 0, 1);
    Scalar borderWidth = 1;
};

} // End of namespace Morphic
} // End of namespace Woden

#endif // WODEN_MORPHIC_BORDERED_MORPH