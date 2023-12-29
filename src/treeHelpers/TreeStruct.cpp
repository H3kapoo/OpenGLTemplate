#include "TreeStruct.hpp"

#include "RectNodeABC.hpp"

namespace treeHelpers
{
TreeStruct::TreeStruct()
    : gId{ generateId() } {}

TreeStruct::~TreeStruct()
{
    if (gFast) { delete gFast; }
}

void TreeStruct::append(RectNodeABC* node)
{
    node->gTreeStruct.setLevel(gLevel + 1);
    gChildren.push_back(node);
}

void TreeStruct::remove(const RectNodeABC* node)
{

}

void TreeStruct::find(const RectNodeABC* node)
{
}

void TreeStruct::findDeep(const RectNodeABC* node)
{
}

void TreeStruct::setParent(RectNodeABC* node)
{
    gParent = node;
}

bool TreeStruct::isRootNode() const
{
    return gLevel == 1 && gParent == nullptr;
}

treeNodeId TreeStruct::getId() const
{
    return gId;
}

treeNodeLevel TreeStruct::getLevel() const
{
    return gLevel;
}

const std::vector<RectNodeABC*> TreeStruct::getChildren() const
{
    return gChildren;
}

void TreeStruct::setLevel(const treeNodeLevel level)
{
    gLevel = level;
}

void TreeStruct::enableFastTreeSearch()
{
    gFast = new FastTreeSort();
}

}