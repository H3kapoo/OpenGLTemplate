#pragma once

#include <cstdint>

#include "../treeHelpers/Types.hpp"

namespace stateHelpers
{
struct WindowState
{
    double mouseX{ 0 }, mouseY{ 0 };
    bool mouseClicked{ false };

    treeHelpers::treeNodeId selectedId{ 0 };
};
}