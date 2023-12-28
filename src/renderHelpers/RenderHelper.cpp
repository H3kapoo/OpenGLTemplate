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
 * @brief Render mesh of type **meshHelpers::RectMesh** .
 *
 * Quickly render **meshHelpers::RectMesh** type. This enables it's
 * stored shader to be the active one while also uploads uniforms such as
 * projection/model matrix and scale of the object to the shader.
 *
 * @param mesh - Rectangle mesh to be rendered.
 */
void RenderHelper::renderRectMesh(meshHelpers::RectMesh& mesh)
{
    gShInstance.setActiveShaderId(mesh.getShaderId());
    gShInstance.setMatrix4("projMatrix", gProjectionMat);
    gShInstance.setMatrix4("modelMatrix", mesh.getTransform());
    gShInstance.setVec4f("color", mesh.gColor);
    // gShInstance.setVec3f("res", mesh.gBox.scale);

    glBindVertexArray(mesh.getVaoId());
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