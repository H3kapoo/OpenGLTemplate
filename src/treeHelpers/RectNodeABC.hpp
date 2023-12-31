#pragma once

#include <string>

#include "TreeStruct.hpp"
#include "Types.hpp"
#include "FastTreeSort.hpp"
#include "../meshHelpers/RectMesh.hpp"
#include "../meshHelpers/MeshStyle.hpp"
#include "../inputHelpers/Types.hpp"
#include "../stateHelpers/WindowState.hpp"

namespace treeHelpers
{
#define CHILD_WILL_IMPLEMENT(x) virtual void x () {}
#define IMPL_OF_PARENT(x) void x () override

/**
 * @brief Abstract base class representing rectangle node.
 *
 * Class used to be inherited from to create new types of nodes sharing
 * basic event/structure functionality.
 *
 */
class RectNodeABC
{
public:
    RectNodeABC(const std::string& vertPath, const std::string& fragPath);
    virtual ~RectNodeABC();

    void enableFastTreeSort();
    void updateFastTree();

    void append(RectNodeABC* child);

    void setStateSource(stateHelpers::WindowState* state);

    void emitEvent(const inputHelpers::Event& evt);

    TreeStruct gTreeStruct;

protected:
    CHILD_WILL_IMPLEMENT(onMouseButton);
    CHILD_WILL_IMPLEMENT(onMouseHover);

    stateHelpers::WindowState* gStatePtr{ nullptr };
private:
    void searchForMouseSelection();
    void searchForMouseHover();

    FastTreeSort* gFastTreeSortPtr{ nullptr };
public:
    /* Basic mesh */
    meshHelpers::RectMesh gMesh;
    meshHelpers::MeshStyle gStyle;
};
}