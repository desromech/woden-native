#ifndef WODEN_MORPHIC_TEXT_INPUT_MORPH_H
#define WODEN_MORPHIC_TEXT_INPUT_MORPH_H

#include "BorderedMorph.hpp"

namespace Woden
{
namespace Morphic
{

typedef std::shared_ptr<class TextInputMorph> TextInputMorphPtr;

/**
 * I am a text input morph
 */
class TextInputMorph : public BorderedMorph
{
public:
    TextInputMorph();
    ~TextInputMorph();

    virtual void drawWith(const Rendering::GUIRendererPtr &renderer) override;

    virtual void onMouseButtonDownEvent(const MouseButtonDownEventPtr &event) override;

    virtual void onKeyboardDownEvent(const KeyboardDownEventPtr &event) override;
    virtual void onKeyboardUpEvent(const KeyboardUpEventPtr &event) override;
    virtual void onTextInputEvent(const TextInputEventPtr &event) override;


    std::string text;
    Vector4 textColor;
};

} // End of namespace Morphic
} // End of namespace Woden

#endif //WODEN_MORPHIC_TEXT_INPUT_MORPH_H