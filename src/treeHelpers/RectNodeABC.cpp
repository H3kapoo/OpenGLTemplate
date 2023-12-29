#include "RectNodeABC.hpp"

#include <stdio.h>

namespace treeHelpers
{

RectNodeABC::RectNodeABC(const std::string& vertPath, const std::string& fragPath)
    : gMesh{ vertPath, fragPath }
{}

/**
 * @brief Append new children to this node.
 *
 * Appends new children to current node by setting appropriate depth level,
 * Z coords, parent, window state and calling the append method on the internal **Tree Structure**.
 *
 * @param child - Node to be added as a child.
 */
void RectNodeABC::append(RectNodeABC* child)
{
    /* Z axis shall not be user modified and it's soley used with the depth buffer to
       prevent overdraw. */
    child->gMesh.gBox.pos.z = gTreeStruct.getLevel() + 1;
    child->gTreeStruct.setParent(this);
    child->gStatePtr = gStatePtr;

    gTreeStruct.append(child);
}


/**
 * @brief Emit event throughout the tree structure's nodes.
 *
 * Emit events down the tree structure to notify interested nodes
 * of such events like mouse button actions, mouse moves, keyboard pressed, etc.
 *
 * @param evt Event to be emmited.
 */
void RectNodeABC::emitEvent(const inputHelpers::Event& evt)
{
    switch (evt)
    {
    case inputHelpers::Event::MouseButton:
        searchForMouseSelection();
        break;
    case inputHelpers::Event::MouseMove:
        searchForMouseHover();
        break;
    default:
        fprintf(stderr, "Unknown base event: %d\n", static_cast<int>(evt));
        break;
    }
}


/**
 * @brief Sets the unique set of states for the node.
 *
 * Each window has a global window state that each node shall have access to read
 * and populate according to events that take place throughout the tree structure.
 *
 * @param state State to share.
 */
void RectNodeABC::setStateSource(stateHelpers::WindowState* state)
{
    gStatePtr = state;
}


/**
 * @brief Interanl - just find out who will be the selected node upon mouse being
 *        clicked and trigger the event on the concrete class of the node.
 */
void RectNodeABC::searchForMouseSelection()
{
    if (!gStatePtr)
    {
        fprintf(stderr, "Window State pointer is not set for node with id: %d\n", gTreeStruct.getId()); exit(1);
    }

    int mouseX = gStatePtr->mouseX;
    int mouseY = gStatePtr->mouseY;
    if ((mouseX > gMesh.gBox.pos.x) && (mouseX < gMesh.gBox.pos.x + gMesh.gBox.scale.x) &&
        (mouseY > gMesh.gBox.pos.y) && (mouseY < gMesh.gBox.pos.y + gMesh.gBox.scale.y))
    {
        gStatePtr->selectedId = gTreeStruct.getId();
    }

    /* Ask child nodes (which should be higher in Z) if there's a hit.
       Last hit node is the actually selected one. */
    for (const auto& ch : gTreeStruct.getChildren())
    {
        ch->emitEvent(inputHelpers::Event::MouseButton);
    }

    /* Root will always have level 1. If root is the selected one, fire onMouseButton, otherwise
       find selected node id in root's children. */
    if (gTreeStruct.isRootNode())
    {
        if (gStatePtr->selectedId == gTreeStruct.getId())
        {
            onMouseButton();
            return;
        }

        for (const auto& ch : gTreeStruct.getChildren())
        {
            if (gStatePtr->selectedId == ch->gTreeStruct.getId())
            {
                ch->onMouseButton();
            }
        }
    }
}


void RectNodeABC::searchForMouseHover()
{
    if (gTreeStruct.isRootNode())
    {

    }

}
}