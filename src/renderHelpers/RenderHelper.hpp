#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../shaderHelpers/ShaderHelper.hpp"
#include "../meshHelpers/RectMesh.hpp"

namespace renderHelpers
{

/**
 * @brief Class for rendering basic meshes.
 *
 */
class RenderHelper
{
public:
    static RenderHelper& get();
    static constexpr float MAX_LAYERS{ -100 };

    void setProjectionMatrix(const glm::mat4& projMatrix);
    void renderRectMesh(meshHelpers::RectMesh& mesh);
    void clearScreen();

private:
    RenderHelper();
    RenderHelper(const RenderHelper&) = delete;
    RenderHelper& operator=(const RenderHelper&) = delete;

    shaderHelpers::ShaderHelper& gShInstance; /* Justified use of ref& */

    glm::mat4 gProjectionMat;
};
}