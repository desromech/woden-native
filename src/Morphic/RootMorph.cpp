#include "Woden/Morphic/RootMorph.hpp"

namespace Woden
{
namespace Morphic
{

void RootMorph::processKeyboardEvent(const EventPtr &event)
{
    if(currentKeyboardFocus)
        currentKeyboardFocus->processEvent(event);
}

void RootMorph::setNewKeyboardFocus(const MorphPtr &newKeyboardFocus)
{
    if(currentKeyboardFocus)
        currentKeyboardFocus->lostKeyboardFocus();

    currentKeyboardFocus = newKeyboardFocus;

    if(currentKeyboardFocus)
        currentKeyboardFocus->gotKeyboardFocus();
}

void RootMorph::setNewMouseFocus(const MorphPtr &newMouseFocus)
{
    if(currentMouseFocus)
        currentMouseFocus->lostMouseFocus();

    currentMouseFocus = newMouseFocus;

    if(currentMouseFocus)
        currentMouseFocus->gotMouseFocus();
}

MorphPtr RootMorph::getMouseFocus() const
{
    return currentMouseFocus;
}

} // End of namespace Morphic
} // End of namespace Woden