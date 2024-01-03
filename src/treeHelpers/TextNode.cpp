#include "TextNode.hpp"

#include "../utils/CommonUtils.hpp"

namespace treeHelpers
{

TextNode::TextNode(const std::string& vertPath, const std::string& fragPath)
    : RectNodeABC("src/assets/shaders/borderedV.glsl", "src/assets/shaders/borderedF.glsl")
    , node{ vertPath, fragPath }
    , gRenderInstance{ renderHelpers::RenderHelper::get() }
    , gTextHelperInstance{ textHelpers::TextHelper::get() }
{
    lfPtr = gTextHelperInstance.loadFont("src/assets/fonts/LiberationSerif-Regular.ttf", 32);
    /* What are we even supposed to do if we don't manage to load the font? */
    if (!lfPtr) { exit(1); }

    /* Watch out shader's uniforms for changes. Set default if any not used. */
    gMesh.gUniKeeper.watch("uInnerColor", &gMesh.gColor);
    gMesh.gUniKeeper.watch("uBorderColor", &gStyle.gBorderColor);
    gMesh.gUniKeeper.watch("uBorderSize", &gStyle.gBorderSize);
    gMesh.gUniKeeper.watch("uResolution", &gMesh.gBox.scale);

    /* Texture is not watched and simply set in the renderer instead. Its not hot for now*/
    node.gStyle.gTextureId = lfPtr->id;
    node.gMesh.gUniKeeper.watch("uColor", &node.gMesh.gColor);
    node.gMesh.gUniKeeper.watch("uCharIndex", &gLetterIdx);
}

void TextNode::setText(const std::string& text)
{
    gText = text;

    const float textDepth = gTreeStruct.getLevel() + 0.5f; // slightly push it upwards
    node.gMesh.gBox.pos.z = textDepth;
    node.gMesh.gBox.pos.x = gMesh.gBox.pos.x + 10;
    node.gMesh.gBox.pos.y = gMesh.gBox.pos.y + 32;
    node.gMesh.gBox.scale.x = 32;
    node.gMesh.gBox.scale.y = 32;

    node.gMesh.gColor = utils::hexToVec4("#d1d1d1");
}

void TextNode::onRenderDone()
{
    //TODO: Implement text batching

    /* Try to render text on top now */
    glDepthMask(GL_FALSE);

    float bringDown = 32;
    float x = gMesh.gBox.pos.x;
    float y = gMesh.gBox.pos.y + bringDown;
    for (uint32_t i = 0; i < gText.length(); i++)
    {
        textHelpers::ASCIIChar& chData = lfPtr->data[gText[i]];

        gLetterIdx = chData.charCode;

        float xPos = x + chData.bearing.x;
        float yPos = y - chData.bearing.y;
        if (xPos > (gMesh.gBox.pos.x + gMesh.gBox.scale.x - 32))
        {
            x = gMesh.gBox.pos.x;
            y += bringDown;
            xPos = x;
            yPos = y - chData.bearing.y;
        }

        node.gMesh.gBox.pos.x = xPos;
        node.gMesh.gBox.pos.y = yPos;
        gRenderInstance.renderRectNode(node);

        x += (chData.hAdvance >> 6);
    }
    glDepthMask(GL_TRUE);

    // gRenderInstance.beginTextBatch(some settings);
    // gRenderInstance.addToTextBatch(char);
    // gRenderInstance.endTextBatch();

    // printf("Rendering is done!\n");
}

void TextNode::onMouseButton()
{
    if (gMouseClickCb && gStatePtr && gStatePtr->mouseClicked)
    {
        gMouseClickCb(gStatePtr->button, gStatePtr->mouseX, gStatePtr->mouseY);
    }
}

void TextNode::onItemsDrop()
{
    if (gMouseDropCb && gStatePtr)
    {
        gMouseDropCb(gStatePtr->dropCount, gStatePtr->droppedPaths);
    }
    // we shall call drop callback
    // printf("Im node %d and you dropped:\n", gTreeStruct.getId());
    // gText.clear();
    // for (int i = 0; i < gStatePtr->dropCount; i++)
    // {
    //     gText.append(gStatePtr->droppedPaths[i]);
    //     gText.append(" - ");
    // }
}

void TextNode::registerOnClick(const MouseClickCb callback)
{
    gMouseClickCb = callback;
}

void TextNode::registerOnItemsDrop(const MouseDropCb callback)
{
    gMouseDropCb = callback;
}
}