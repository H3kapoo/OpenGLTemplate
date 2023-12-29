#include "ConcreteNode.hpp"

namespace treeHelpers
{
ConcreteNode::ConcreteNode(const std::string& vertPath, const std::string& fragPath)
    : RectNodeABC(vertPath, fragPath)
{}

void ConcreteNode::onMouseButton()
{
    if (gMouseClickCb && gStatePtr && gStatePtr->mouseClicked)
    {
        gMouseClickCb(gStatePtr->button, gStatePtr->mouseX, gStatePtr->mouseY);
    }

    if (gMouseReleaseCb && gStatePtr && !gStatePtr->mouseClicked)
    {
        gMouseReleaseCb(gStatePtr->button, gStatePtr->mouseX, gStatePtr->mouseY);
    }
}

void ConcreteNode::registerOnClick(const MouseClickCb callback)
{
    gMouseClickCb = callback;
}

void ConcreteNode::registerOnRelease(const MouseReleaseCb callback)
{
    gMouseReleaseCb = callback;
}

}