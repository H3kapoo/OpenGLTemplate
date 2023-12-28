#pragma once

#include <vector>

#include "Types.hpp"

namespace treeHelpers
{

class RectNodeABC;
class TreeStruct
{
public:
    TreeStruct() : gId{ generateId() } {}
    void append(RectNodeABC* node);
    void remove(const RectNodeABC* node);
    void find(const RectNodeABC* node);
    void findDeep(const RectNodeABC* node);
    void setParent(RectNodeABC* node);

    treeNodeId getId() const;
    treeNodeLevel getLevel() const;
    const std::vector<RectNodeABC*> getChildren() const;

    void setLevel(const treeNodeLevel level);

private:
    static uint32_t generateId()
    {
        static uint32_t currentId = 1; // Start giving IDs from 1
        return currentId++;
    }

    treeNodeId gId;
    treeNodeLevel gLevel{ 1 };
    RectNodeABC* gParent{ nullptr };
    std::vector<RectNodeABC*> gChildren;
};

}