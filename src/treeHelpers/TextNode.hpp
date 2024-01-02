#pragma once

#include <string>

#include "RectNodeABC.hpp"
#include "../textHelpers/TextHelper.hpp"
// #include "../textHelpers/Types.hpp"
#include "../renderHelpers/RenderHelper.hpp"
namespace treeHelpers
{

class TextNode : public RectNodeABC
{
public:
    TextNode(const std::string& vertPath, const std::string& fragPath);

    void registerOnClick(const MouseClickCb callback);

    void setText(const std::string& text);

private:
    IMPL_OF_PARENT(onMouseButton);
    IMPL_OF_PARENT(onRenderDone);

    /* COncept for text rendering, experimental */
    std::string gTextVertPath{ "src/assets/shaders/textV.glsl" };
    std::string gTextFragPath{ "src/assets/shaders/textF.glsl" };
    RectNodeABC node{ gTextVertPath, gTextFragPath };

    MouseClickCb gMouseClickCb{ nullptr };

    textHelpers::LoadedFontPtr lfPtr{ nullptr };

    textHelpers::TextHelper& gTextHelperInstance;
    renderHelpers::RenderHelper& gRenderInstance;
};

}
