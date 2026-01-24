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

void RootMorph::popupAtPosition(const MorphPtr &popupMorph, const Vector2 &globalPosition)
{
    popupMorph->setOrigin(globalPosition);
    activePopups.push_back(popupMorph);
    addMorph(popupMorph);
}

void RootMorph::popupUniqueAtPosition(const MorphPtr &popupMorph, const Vector2 &globalPosition)
{
    for(auto &popup : activePopups)
    {
        if(popup == popupMorph)
            return;
    }

    killPopUps();
    popupAtPosition(popupMorph, globalPosition);
}

void RootMorph::killPopUps()
{
    for(size_t i = 0; i < activePopups.size(); ++i)
    {
        auto popup = activePopups[i];
        removeMorph(popup);
    }
        
    activePopups.clear();
}

void RootMorph::close()
{
}

} // End of namespace Morphic
} // End of namespace Woden