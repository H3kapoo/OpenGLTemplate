#include "RectNodeABC.hpp"

#include <stdio.h>

namespace treeHelpers
{

RectNodeABC::RectNodeABC(const std::string& vertPath, const std::string& fragPath)
    : gVertPath{ vertPath }
    , gFragPath{ fragPath }
    , gMesh{ gVertPath, gFragPath }
{}

void RectNodeABC::emitEvent(const inputHelpers::Event evt)
{
    switch (evt)
    {
    case inputHelpers::Event::MouseButton:
        scanTree();
        break;

    case inputHelpers::Event::MouseMove:
        break;
    default:
        fprintf(stderr, "Unknown base event: %d\n", static_cast<int>(evt));
        break;
    }

    for (const auto& ch : gTreeStruct.getChildren())
    {
        ch->emitEvent(evt);
    }
}

void RectNodeABC::setStateSource(stateHelpers::WindowState* state) { gStatePtr = state; }

void RectNodeABC::scanTree()
{
    int mouseX = gStatePtr->mouseX;
    int mouseY = gStatePtr->mouseY;
    if ((mouseX > gMesh.gBox.pos.x) && (mouseX < gMesh.gBox.pos.x + gMesh.gBox.scale.x) &&
        (mouseY > gMesh.gBox.pos.y) && (mouseY < gMesh.gBox.pos.y + gMesh.gBox.scale.y))
    {
        printf("Node clicked maybe id: %d\n", gTreeStruct.getId());
        onMouseButton();
    }
}

}