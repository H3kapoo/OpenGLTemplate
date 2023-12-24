#include "RectMesh.hpp"

namespace meshHelpers
{

RectMesh::RectMesh(const shaderHelpers::shaderIdPtr shaderIdPtr, const uint32_t vaoId)
    : gShaderIdPtr{ shaderIdPtr }, gVaoId{ vaoId }
{}

glm::mat4 RectMesh::getTransform()
{
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, gBox.pos); // it goes negative, expected
    modelMat = glm::translate(modelMat, glm::vec3(gBox.scale.x / 2, gBox.scale.y / 2, 0));
    modelMat = glm::rotate(modelMat, glm::radians(gBox.rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMat = glm::translate(modelMat, glm::vec3(-gBox.scale.x / 2, -gBox.scale.y / 2, 0));
    modelMat = glm::scale(modelMat, gBox.scale);
    return modelMat;
}

inline shaderHelpers::shaderId RectMesh::getShaderId() const
{
    return gShaderIdPtr == nullptr ? 0 : *gShaderIdPtr;
}

}