#include "Woden/Morphic/Morph.hpp"
#include "Woden/Rendering/GuiRenderer.hpp"

namespace Woden
{
namespace Morphic
{

Morph::Morph()
{
}

Morph::~Morph()
{
}

void Morph::fullUpdate(Math::Scalar deltaTime)
{
    updateChildren(deltaTime);
    update(deltaTime);
}

void Morph::updateChildren(Math::Scalar deltaTime)
{
    for(auto &child : submorphs)
        child->fullUpdate(deltaTime);
}

void Morph::update(Math::Scalar deltaTime)
{
    (void)deltaTime;
}


void Morph::fullDrawWith(const Rendering::GUIRendererPtr &renderer)
{
    drawWith(renderer);
    drawChildrenWith(renderer);
}

void Morph::drawWith(const Rendering::GUIRendererPtr &renderer)
{
    renderer->fillRectangleWithColor(getLocalBounds(), color);
}

void Morph::drawChildrenWith(const Rendering::GUIRendererPtr &renderer)
{
    for(auto &submorph : submorphs)
        submorph->fullDrawWith(renderer);
}

void Morph::processEvent(const EventPtr &event)
{
    event->sentToMorph(shared_from_this());
}

void Morph::handleUnknownEvent(const EventPtr &event)
{
    (void)event;
}

void Morph::handleKeyboardDownEvent(const KeyboardDownEventPtr &event)
{
    (void)event;
}

void Morph::handleKeyboardUpEvent(const KeyboardUpEventPtr &event)
{
    (void)event;
}

void Morph::handleMouseButtonDownEvent(const MouseButtonDownEventPtr &event)
{
    event->withTranslationInverseDo(getOrigin(), [&](){
        for(auto &submorph : submorphs)
        {
            if(submorph->bounds.containsPoint(event->position))
                submorph->processEvent(event);
        }
    });
}

void Morph::handleMouseButtonUpEvent(const MouseButtonUpEventPtr &event)
{
    event->withTranslationInverseDo(getOrigin(), [&](){
        for(auto &submorph : submorphs)
        {
            if(submorph->bounds.containsPoint(event->position))
                submorph->processEvent(event);
        }
    });
}

void Morph::handleMouseMotionEvent(const MouseMotionEventPtr &event)
{
    event->withTranslationInverseDo(getOrigin(), [&](){
        for(auto &submorph : submorphs)
        {
            if(submorph->bounds.containsPoint(event->position))
                submorph->processEvent(event);
        }
    });
}

void Morph::handleMouseWheelEvent(const MouseWheelEventPtr &event)
{
    event->withTranslationInverseDo(getOrigin(), [&](){
        for(auto &submorph : submorphs)
        {
            if(submorph->bounds.containsPoint(event->position))
                submorph->processEvent(event);
        }
    });
}

void Morph::gotKeyboardFocus()
{
}

void Morph::lostKeyboardFocus()
{
}

void Morph::takeKeyboardFocus()
{
    setNewKeyboardFocus(shared_from_this());
}

void Morph::setNewKeyboardFocus(const MorphPtr &newKeyboardFocus)
{
    auto ownerMorph = owner.lock();
    if(ownerMorph)
        ownerMorph->setNewKeyboardFocus(newKeyboardFocus);
}


} // End of namespace Morphic
} // End of namespace Woden