#ifndef WODEN_MORPHIC_TEXT_INPUT_MORPH_H
#define WODEN_MORPHIC_TEXT_INPUT_MORPH_H

#include "BorderedMorph.hpp"

namespace Woden
{
namespace Morphic
{

/**
 * I am a text input morph
 */
class TextInputMorph : public BorderedMorph
{
public:
    TextInputMorph();
    ~TextInputMorph();

    virtual void drawWith(const Rendering::GUIRendererPtr &renderer) override;

    virtual void handleMouseButtonDownEvent(const MouseButtonDownEventPtr &event) override;

    virtual void handleKeyboardDownEvent(const KeyboardDownEventPtr &event) override;
    virtual void handleKeyboardUpEvent(const KeyboardUpEventPtr &event) override;
    virtual void handleTextInputEvent(const TextInputEventPtr &event) override;


    std::string text;
    Vector4 textColor;
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_TEXT_INPUT_MORPH_H