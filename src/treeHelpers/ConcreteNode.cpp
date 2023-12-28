#include "ConcreteNode.hpp"

namespace treeHelpers
{
ConcreteNode::ConcreteNode(const std::string& vertPath, const std::string& fragPath)
    : RectNodeABC(vertPath, fragPath)
{}
}