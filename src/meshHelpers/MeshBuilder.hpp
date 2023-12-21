#pragma once

#include <cstdint>
#include <vector>

#include <GL/glew.h>

namespace meshHelpers
{
class MeshBuilder
{
public:
    uint32_t generateWith(const std::vector<float>& verts, const std::vector<uint32_t>& indices,
        const std::vector<uint32_t>& layout);
};
}