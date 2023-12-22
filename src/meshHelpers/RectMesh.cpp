#include "RectMesh.hpp"

namespace meshHelpers
{

RectMesh::RectMesh(const int32_t shaderId, const uint32_t vaoId)
    : shaderId_{ shaderId }, vaoId_{ vaoId }
{}

glm::mat4 RectMesh::getTransform()
{
    glm::mat4 modelMat = glm::mat4(1.0f);
    modelMat = glm::translate(modelMat, box_.pos); // it goes negative, expected
    modelMat = glm::translate(modelMat, glm::vec3(box_.scale.x / 2, box_.scale.y / 2, 0));
    modelMat = glm::rotate(modelMat, glm::radians(box_.rot.z), glm::vec3(0.0f, 0.0f, 1.0f));
    modelMat = glm::translate(modelMat, glm::vec3(-box_.scale.x / 2, -box_.scale.y / 2, 0));
    modelMat = glm::scale(modelMat, box_.scale);
    return modelMat;
}

}