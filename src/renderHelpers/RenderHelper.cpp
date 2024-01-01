#include "RenderHelper.hpp"

namespace renderHelpers
{

RenderHelper& RenderHelper::get()
{
    static RenderHelper instance;
    return instance;
}

RenderHelper::RenderHelper()
    : gShInstance{ shaderHelpers::ShaderHelper::get() }
{}


/**
 * @brief Sets the projection matrix that will be used on next draw call
 *
 * @param projMatrix Projection Matrix to be used.
 */
void RenderHelper::setProjectionMatrix(const glm::mat4& projMatrix)
{
    gProjectionMat = projMatrix;
}


/**
 * @brief Render node of type **meshHelpers::RectNodeABC** .
 *
 * Quickly render **meshHelpers::RectNodeABC** type. This enables it's
 * stored shader to be the active one while also uploads uniforms such as
 * projection/model matrix and scale of the object to the shader.
 *
 * @param node - Rectangle node to be rendered.
 */
void RenderHelper::renderRectNode(treeHelpers::RectNodeABC& node)
{
    /* Set very common uniforms and activate shader id*/
    gShInstance.setActiveShaderId(node.gMesh.getShaderId());

    gShInstance.setMatrix4("uProjMatrix", gProjectionMat);
    gShInstance.setMatrix4("uModelMatrix", node.gMesh.getTransform());

    /* IMPORTANT: Keep in mind that if the value referenced happens to not exist
       anymore, we will get a segfault, so be careful. */
    auto& uniKeeper = node.gMesh.gUniKeeper;
    for (const auto& uNameValPair : uniKeeper.gVec3Ptrs)
    {
        gShInstance.setVec3f(uNameValPair.first.c_str(), *uNameValPair.second);
    }

    for (const auto& uNameValPair : uniKeeper.gVec4Ptrs)
    {
        gShInstance.setVec4f(uNameValPair.first.c_str(), *uNameValPair.second);
    }

    glBindVertexArray(node.gMesh.getVaoId());
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}


/**
 * @brief Clear currently bound window depth and color bit.
 *
 */
void RenderHelper::clearScreen()
{
    glClearColor(.3f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

}