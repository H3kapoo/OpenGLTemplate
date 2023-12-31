#pragma once

#include "RectNodeABC.hpp"

namespace treeHelpers
{
class ConcreteNode : public RectNodeABC
{
    typedef std::function<void(ConcreteNode& self, int button, int x, int y)> MouseClickWithSelfCb;

public:
    ConcreteNode(const std::string& vertPath, const std::string& fragPath);

    void registerOnClick(const MouseClickCb callback);
    void registerOnClick(const MouseClickWithSelfCb callback);
    void registerOnRelease(const MouseReleaseCb callback);

private:
    IMPL_OF_PARENT(onMouseButton);

    MouseClickCb gMouseClickCb{ nullptr };
    MouseClickWithSelfCb gMouseClickWithSelfCb{ nullptr };
    MouseReleaseCb gMouseReleaseCb{ nullptr };

    /* Basic shader */
    std::string gVertPath{ "src/assets/shaders/basicV.glsl" };
    std::string gFragPath{ "src/assets/shaders/basicF.glsl" };
};
}