#pragma once

#include <GLFW/glfw3.h>
#include <cstdint>
#include "../treeHelpers/Types.hpp"

namespace stateHelpers
{
/**
 * @brief Class that represents the state of the working window.
 *
 * Represents things such as where the cursor is currently at, what's the selected
 * or hovered node and many more. Use it as a per window global storage.
 */
struct WindowState
{
    int32_t winWidth{ 0 }, winHeight{ 0 };

    double mouseX{ 0 }, mouseY{ 0 };
    bool mouseClicked{ false };
    int button{ 0 };

    treeHelpers::treeNodeId selectedId{ 0 };
};
}