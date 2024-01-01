#include "ConcreteNode.hpp"

namespace treeHelpers
{
ConcreteNode::ConcreteNode(const std::string& vertPath, const std::string& fragPath)
    : RectNodeABC(vertPath, fragPath)
    , gVertPath(vertPath)
    , gFragPath(fragPath)
{}

void ConcreteNode::render()
{
    // gShInstance.setVec3f("uResolution", gMesh.gBox.scale);
    // gShInstance.setVec4f("uInnerColor", gMesh.gColor);
    // gShInstance.setVec4f("uBorderColor", gStyle.gBorderColor);
    // gShInstance.setVec4f("uBorderSize", gStyle.gBorderSize);


    // // shall be refactored
    // if (gStyle.gTextureId && false)
    // {
    //     /* As far as rendering a rectangle goes, we can assume just Tex Unit 0 will be used anyway */
    //     glActiveTexture(GL_TEXTURE0);
    //     gShInstance.setInt("uTexture", GL_TEXTURE0);
    //     glBindTexture(GL_TEXTURE_2D, gStyle.gTextureId);
    // }
}

void ConcreteNode::onMouseButton()
{
    /* If gMouseClickCb and gMouseClickWithSelfCb are both set, they will both fire.
       Just remmeber that. */
    if (gMouseClickCb && gStatePtr && gStatePtr->mouseClicked)
    {
        gMouseClickCb(gStatePtr->button, gStatePtr->mouseX, gStatePtr->mouseY);
    }

    if (gMouseClickWithSelfCb && gStatePtr && gStatePtr->mouseClicked)
    {
        gMouseClickWithSelfCb(*this, gStatePtr->button, gStatePtr->mouseX, gStatePtr->mouseY);
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

void ConcreteNode::registerOnClick(const MouseClickWithSelfCb callback)
{
    gMouseClickWithSelfCb = callback;
}

void ConcreteNode::registerOnRelease(const MouseReleaseCb callback)
{
    gMouseReleaseCb = callback;
}

}