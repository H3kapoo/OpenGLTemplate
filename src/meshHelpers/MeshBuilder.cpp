#include "MeshBuilder.hpp"

#include <numeric>

namespace meshHelpers
{
uint32_t MeshBuilder::generateWith(const std::vector<float>& verts, const std::vector<uint32_t>& indices,
    const std::vector<uint32_t>& layout)
{
    unsigned int vaoId, vboId, eboId;
    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboId);
    glGenBuffers(1, &eboId);

    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboId);

    /* Buffer data about the vertices to the GPU */
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), &verts[0], GL_STATIC_DRAW);

    /* Buffer data about the indices to the GPU */
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices.size(), &indices[0], GL_STATIC_DRAW);

    uint32_t totalVertexLen = std::accumulate(layout.begin(), layout.end(), 0);
    int32_t* currentOffset = 0;
    for (uint32_t i = 0; i < layout.size(); i++)
    {
        //               attrId  attrLenBytes attrType bNorm               vertexLen              offesetToAttr
        glVertexAttribPointer(i, layout[i], GL_FLOAT, GL_FALSE, totalVertexLen * sizeof(float), (void*)currentOffset);
        glEnableVertexAttribArray(i);
        currentOffset += sizeof(float) * layout[i];
    }
    return vaoId;
}
}