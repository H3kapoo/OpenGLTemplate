#pragma once

#include "RectNodeABC.hpp"

namespace treeHelpers
{
class ConcreteNode : public RectNodeABC
{
public:
    ConcreteNode(const std::string& vertPath, const std::string& fragPath);

    // IMPL_FROM_PARENT(onMouseButton);

private:
    /* Basic shader */
    std::string gVertPath{ "src/assets/shaders/basicV.glsl" };
    std::string gFragPath{ "src/assets/shaders/basicF.glsl" };
};
}