#pragma once

#include <cstdint>
#include <functional>

namespace treeHelpers
{
typedef uint32_t treeNodeId;
typedef uint32_t treeNodeLevel;
typedef std::function<void(int button, int x, int y)> MouseClickCb;
typedef std::function<void(int button, int x, int y)> MouseReleaseCb;
typedef std::function<void(int count, const char** paths)> MouseDropCb;
}