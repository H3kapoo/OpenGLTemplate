#pragma once

#include <string>

#include "ConcreteNode.hpp"
#include "../textHelpers/TextHelper.hpp"
#include "../renderHelpers/RenderHelper.hpp"

namespace treeHelpers
{

class TextNode : public RectNodeABC
{
public:
    TextNode(const std::string& vertPath, const std::string& fragPath);

    void registerOnClick(const MouseClickCb callback);
    void registerOnItemsDrop(const MouseDropCb callback);

    void setText(const std::string& text);

private:
    void onRenderDone();

    IMPL_OF_PARENT(onMouseButton);
    IMPL_OF_PARENT(onItemsDrop);

    /* Concept for text rendering, experimental */
    std::string gTextVertPath{ "src/assets/shaders/textV.glsl" };
    std::string gTextFragPath{ "src/assets/shaders/textF.glsl" };
    ConcreteNode node; //TODO: This shall be replaced by render batcher

    MouseClickCb gMouseClickCb{ nullptr };
    MouseDropCb gMouseDropCb{ nullptr };

    textHelpers::LoadedFontPtr lfPtr{ nullptr };

    std::string gText;
    uint32_t gLetterIdx{ 0 };

    renderHelpers::RenderHelper& gRenderInstance;
    textHelpers::TextHelper& gTextHelperInstance;
};

}
