#include "TextNode.hpp"

#include "../utils/CommonUtils.hpp"

namespace treeHelpers
{

TextNode::TextNode(const std::string& vertPath, const std::string& fragPath)
    : RectNodeABC(vertPath, fragPath)
    , gRenderInstance{ renderHelpers::RenderHelper::get() }
    , gTextHelperInstance{ textHelpers::TextHelper::get() }
{}

void TextNode::setText(const std::string& text)
{
    lfPtr = gTextHelperInstance.loadFont("src/assets/fonts/LiberationSerif-Regular.ttf", 32);
    // lfPtr = gTextHelperInstance.loadFont("src/assets/fonts/cmr10.ttf", 32);
    /* What are we even supposed to do if we don't manage to load the font? */
    if (!lfPtr) { exit(1); }

    // gTextHelper.loadFont("src/assets/fonts/cmr10.ttf");

    const float textDepth = gTreeStruct.getLevel() + 0.5f; // slightly push it upwards
    node.gMesh.gBox.pos.z = textDepth;
    node.gMesh.gBox.pos.x = gMesh.gBox.pos.x + 10;
    node.gMesh.gBox.pos.y = gMesh.gBox.pos.y + 32;
    node.gMesh.gBox.scale.x = 32;
    node.gMesh.gBox.scale.y = 32;

    node.gMesh.gColor = utils::hexToVec4("#d1d1d1");
    node.gStyle.gTextureId = lfPtr->id;

    node.gMesh.gUniKeeper.watch("uColor", &node.gMesh.gColor);
}

void TextNode::onRenderDone()
{
    /* Try to render text on top now */
    glDepthMask(GL_FALSE);

    std::string text{ "The brown blue fox, jumped over the" };
    uint32_t letterIdx = 0;
    float xCached = node.gMesh.gBox.pos.x;
    float yCached = node.gMesh.gBox.pos.y;
    for (int i = 0; i < text.length(); i++)
    {
        auto& chData = lfPtr->data[text.at(i)];

        letterIdx = chData.charCode;
        node.gMesh.gUniKeeper.watch("uCharIndex", &letterIdx);

        node.gMesh.gBox.pos.y -= chData.bearing.y;
        gRenderInstance.renderRectNode(node);
        node.gMesh.gBox.pos.x += (chData.hAdvance >> 6) + chData.bearing.x;
        node.gMesh.gBox.pos.y = yCached;
    }
    node.gMesh.gBox.pos.x = xCached;
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

void TextNode::registerOnClick(const MouseClickCb callback)
{
    gMouseClickCb = callback;
}
}