#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../shaderHelpers/Types.hpp"

namespace meshHelpers
{
class RectMesh
{
    struct BoxModel
    {
        glm::vec3 pos{ 0, 0, -1 };
        glm::vec3 scale{ 10, 10, 1 };
        glm::vec3 rot{ 0 };
    };

public:
    RectMesh(const shaderHelpers::shaderIdPtr shaderIdPtr, const uint32_t vaoId);
    glm::mat4 getTransform();
    inline shaderHelpers::shaderId getShaderId() const;
public:
    // private:
    BoxModel gBox;
    shaderHelpers::shaderIdPtr gShaderIdPtr{ nullptr };
    uint32_t gVaoId{ 0 };
};
}