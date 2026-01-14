#include "Woden/Morphic/SimpleButtonMorph.hpp"

namespace Woden
{
namespace Morphic
{

SimpleButtonMorph::SimpleButtonMorph()
{
    color = Vector4(0.7f, 0.7f, 0.7f, 1.0f);

}

Vector4 SimpleButtonMorph::getCurrentColor() const
{
    return color;
}

void SimpleButtonMorph::setLabel(const std::string &labelText)
{
    if(label)
    {
        label->deleteMorph();
        label.reset();
        label.reset();
    }

    label = std::make_shared<StringMorph> ();
    label->label = labelText;
    addSubmorph(label);
}

} // End of namespace Morphic
} // End of namespace Woden
    