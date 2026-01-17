#include "Woden/Morphic/TextInputMorph.hpp"
#include "Woden/Rendering/Context.hpp"
#include "Woden/Rendering/GuiRenderer.hpp"

namespace Woden
{
namespace Morphic
{

TextInputMorph::TextInputMorph()
{
    color = Vector4(1, 1, 1, 1);
    borderColor = Vector4(0, 0, 0, 1);
    textColor = Vector4(0, 0, 0, 1);
    setExtent(Vector2(100, 25));
}

TextInputMorph::~TextInputMorph()
{
}

void TextInputMorph::drawWith(const Rendering::GUIRendererPtr &renderer)
{
    BorderedMorph::drawWith(renderer);
    renderer->drawTextInRectangleWithColor(getLocalBounds(), text, textColor);
}

void TextInputMorph::handleMouseButtonDownEvent(const MouseButtonDownEventPtr &event)
{
    if(event->wasHandled)
        return;

    takeKeyboardFocus();
    event->wasHandled = true;
}

void TextInputMorph::handleKeyboardDownEvent(const KeyboardDownEventPtr &event)
{
    if(event->wasHandled)
        return;
        
}

void TextInputMorph::handleKeyboardUpEvent(const KeyboardUpEventPtr &event)
{
    if(event->wasHandled)
        return;
        

}

void TextInputMorph::handleTextInputEvent(const TextInputEventPtr &event)
{
    text += event->text;
    event->wasHandled = true;
}

    
} // End of namespace Morphic
} // End of namespace Woden
