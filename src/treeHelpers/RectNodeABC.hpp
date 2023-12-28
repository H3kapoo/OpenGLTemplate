#pragma once

#include <string>

#include "TreeStruct.hpp"
#include "Types.hpp"
#include "../meshHelpers/RectMesh.hpp"
#include "../inputHelpers/Types.hpp"
#include "../stateHelpers/WindowState.hpp"

namespace treeHelpers
{
// #define CHILD_WILL_IMPLEMENT(x) virtual void x ()
// #define IMPL_FROM_PARENT(x) void x () override

class RectNodeABC
{
public:
    RectNodeABC(const std::string& vertPath, const std::string& fragPath);

    void setStateSource(stateHelpers::WindowState* state);

    void emitEvent(const inputHelpers::Event evt);
    void scanTree();

    virtual void onMouseButton() {}
    // CHILD_WILL_IMPLEMENT(onMouseButton);

    TreeStruct gTreeStruct;

private:
    /* Basic shader */
    std::string gVertPath{ "src/assets/shaders/basicV.glsl" };
    std::string gFragPath{ "src/assets/shaders/basicF.glsl" };

    stateHelpers::WindowState* gStatePtr;
public:
    /* Basic mesh */
    meshHelpers::RectMesh gMesh;
};
}