#include "RectNodeABC.hpp"

#include <stdio.h>

namespace treeHelpers
{

RectNodeABC::RectNodeABC(const std::string& vertPath, const std::string& fragPath)
    : gShInstance{ shaderHelpers::ShaderHelper::get() }
    , gMesh{ vertPath, fragPath }
{}

RectNodeABC::~RectNodeABC()
{
    if (gFastTreeSortPtr)
    {
        delete gFastTreeSortPtr;
    }
}

/**
 * @brief Enable fast tree sort option for the *root* object.
 *
 * Enabling this option on the root makes it possible to have fast searching
 * or selected/hovered over/etc nodes.
 */
void RectNodeABC::enableFastTreeSort()
{
    if (!gFastTreeSortPtr)
    {
        gFastTreeSortPtr = new FastTreeSort();
        printf("FastTreeSort enabled for node with id: %d\n", gTreeStruct.getId());
    }
}

/**
 * @brief Update internal state of fast tree node.
 *
 * Function required to be called in order to keep the fast tree up to date
 * after each insertion/deletion of a node (or batch of nodes).
 */
void RectNodeABC::updateFastTree()
{
    if (gFastTreeSortPtr)
    {
        gFastTreeSortPtr->sortBuffer(this, [](const RectNodeABC* x, const RectNodeABC* y) -> bool
            {
                /* Sort so that vec is ordered by highest level first */
                return x->gTreeStruct.getLevel() > y->gTreeStruct.getLevel();
            });
        printf("FastTree updated!\n");
        return;
    }

    fprintf(stderr, "FastTreeSort is not enabled!\n");
}


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
        onMouseHover();
        break;
    case inputHelpers::Event::RenderDone:
        onRenderDone();
        break;
    case inputHelpers::Event::ItemsDrop:
        //TODO: Bad design for now
        searchForMouseDropLoc();
        onRenderDone();
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

    if (gTreeStruct.isRootNode() && gFastTreeSortPtr != nullptr)
    {
        int32_t x = 0;
        for (const auto& c : gFastTreeSortPtr->getBuffer())
        {
            int mouseX = gStatePtr->mouseX;
            int mouseY = gStatePtr->mouseY;
            auto& mesh = c->gMesh;
            if ((mouseX > mesh.gBox.pos.x) && (mouseX < mesh.gBox.pos.x + mesh.gBox.scale.x) &&
                (mouseY > mesh.gBox.pos.y) && (mouseY < mesh.gBox.pos.y + mesh.gBox.scale.y))
            {
                gStatePtr->selectedId = c->gTreeStruct.getId();
                // printf("%f Iter: %d Child clicked with level: %d\n", glfwGetTime(), x, c->gTreeStruct.getLevel());
                c->onMouseButton();
                break;
            }
            x++;
        }
    }
}


void RectNodeABC::searchForMouseHover()
{
    if (!gStatePtr)
    {
        fprintf(stderr, "Window State pointer is not set for node with id: %d\n", gTreeStruct.getId()); exit(1);
    }

    if (gTreeStruct.isRootNode() && gFastTreeSortPtr != nullptr)
    {
        bool changeNeeded{ false };
        for (const auto& c : gFastTreeSortPtr->getBuffer())
        {
            int mouseX = gStatePtr->mouseX;
            int mouseY = gStatePtr->mouseY;
            auto& mesh = c->gMesh;
            if ((mouseX > mesh.gBox.pos.x) && (mouseX < mesh.gBox.pos.x + mesh.gBox.scale.x) &&
                (mouseY > mesh.gBox.pos.y) && (mouseY < mesh.gBox.pos.y + mesh.gBox.scale.y))
            {
                auto newHoveredId = c->gTreeStruct.getId();
                if (newHoveredId != gStatePtr->hoveredId)
                {
                    gStatePtr->prevHoveredId = gStatePtr->hoveredId;
                    changeNeeded = true;
                }

                gStatePtr->hoveredId = newHoveredId;
                c->onMouseHover();
                break;
            }
        }

        bool notifiedExit{ false }, notifiedEnter{ false };
        for (const auto& c : gFastTreeSortPtr->getBuffer())
        {
            if (!changeNeeded || (notifiedEnter && notifiedExit))
            {
                break;
            }

            auto id = c->gTreeStruct.getId();
            if (id == gStatePtr->hoveredId)
            {
                c->onMouseEnter();
                notifiedEnter = true;
            }

            if (id == gStatePtr->prevHoveredId)
            {
                c->onMouseExit();
                notifiedExit = true;
            }
        }
    }

    // printf("{%d} Prev hovered %d actuall %d\n", gTreeStruct.getId(), gStatePtr->prevHoveredId, gStatePtr->hoveredId);

    // guaranteed parent already ran it's block
    // if (gStatePtr->prevHoveredId == gTreeStruct.getId())
    // {
    //     // onMouseExit();
    //     printf("Mouse exited {%d}\n", gTreeStruct.getId());
    // }

    // if (gStatePtr->hoveredId == gTreeStruct.getId())
    // {
    //     //     // onMouseEnter();
    //     printf("Mouse entered {%d}\n", gTreeStruct.getId());
    // }
}


void RectNodeABC::searchForMouseDropLoc()
{
    if (!gStatePtr)
    {
        fprintf(stderr, "Window State pointer is not set for node with id: %d\n", gTreeStruct.getId()); exit(1);
    }

    if (gTreeStruct.isRootNode() && gFastTreeSortPtr != nullptr)
    {
        for (const auto& c : gFastTreeSortPtr->getBuffer())
        {
            int mouseX = gStatePtr->mouseX;
            int mouseY = gStatePtr->mouseY;
            auto& mesh = c->gMesh;
            if ((mouseX > mesh.gBox.pos.x) && (mouseX < mesh.gBox.pos.x + mesh.gBox.scale.x) &&
                (mouseY > mesh.gBox.pos.y) && (mouseY < mesh.gBox.pos.y + mesh.gBox.scale.y))
            {
                c->onItemsDrop();
                break;
            }
        }
    }
}
}