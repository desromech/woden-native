#ifndef WODEN_MORPHIC_ROOT_MORPH_HPP
#define WODEN_MORPHIC_ROOT_MORPH_HPP

#include "Morph.hpp"

namespace Woden
{
namespace Morphic
{
// I am a root hierarchy morph.
class RootMorph : public Morph
{
public:
    void processKeyboardEvent(const EventPtr &event);
    virtual void setNewKeyboardFocus(const MorphPtr &newKeyboardFocus) override;

    virtual void setNewMouseFocus(const MorphPtr &newMouseFocus);
    virtual MorphPtr getMouseFocus()const override;

    virtual bool isRootMorph() const
    {
        return true;
    }

    virtual RootMorphPtr getRootMorph()
    {
        return std::static_pointer_cast<RootMorph> (shared_from_this());
    }

    void popupAtPosition(const MorphPtr &popupMorph, const Vector2 &globalPosition);
    void popupUniqueAtPosition(const MorphPtr &popupMorph, const Vector2 &globalPosition);
    void killPopUps();

    MorphPtr currentKeyboardFocus;
    MorphPtr currentMouseFocus;
    std::vector<MorphPtr> activePopups;
};

} // End of namespace Morphic
} // End of namespace Woden
#endif //WODEN_MORPHIC_ROOT_MORPH_HPP