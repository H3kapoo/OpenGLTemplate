#pragma once

#include <cstdint>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
    RectMesh(const int32_t shaderId, const uint32_t vaoId);
    glm::mat4 getTransform();
public:
    // private:
    BoxModel box_;
    int32_t shaderId_{ 0 };
    uint32_t vaoId_{ 0 };
};
}