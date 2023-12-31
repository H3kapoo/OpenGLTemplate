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
void RenderHelper::renderRectMesh(meshHelpers::RectMesh& mesh, meshHelpers::MeshStyle& style)
{
    gShInstance.setActiveShaderId(mesh.getShaderId());
    gShInstance.setVec3f("uResolution", mesh.gBox.scale);

    gShInstance.setMatrix4("uProjMatrix", gProjectionMat);
    gShInstance.setMatrix4("uModelMatrix", mesh.getTransform());

    gShInstance.setVec4f("uInnerColor", mesh.gColor);
    gShInstance.setVec4f("uBorderColor", style.gBorderColor);

    // gShInstance.setVec4f("uInnerColor", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    // gShInstance.setVec4f("uBorderColor", glm::vec4(0.5f, 0.0f, 0.0f, 1.0f));
    gShInstance.setVec4f("uBorderSize", style.gBorderSize);

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