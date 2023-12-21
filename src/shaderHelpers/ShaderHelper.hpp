#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

namespace shaderHelpers
{
class ShaderHelper
{
public:
    /* Setters */
    int32_t loadFromPath(const std::string& vertPath, const std::string& fragPath);
    int32_t reloadFromPath(const std::string& vertPath, const std::string& fragPath);
    void setActiveShaderId(const uint32_t id);
    void resetBoundShader() const;

    /* Uniforms */
    void setInt(const char* location, int value);
    void setIntVec(const char* location, uint32_t amount, int* flatValues);
    void setVec3f(const char* location, glm::vec3 value);
    void setVec4f(const char* location, glm::vec4 value);
    void setMatrix4(const char* location, const glm::mat4 transform);
    void setMatrix4Vec(const char* location, uint32_t amount, const float* flatTransforms);

    /* Getters */
    uint32_t getActiveShaderId() const;

private:
    void handleNotFound(const char* location);
    int linkShaders(int vertShaderId, int fragShaderId);
    int compileShader(const std::string& sourcePath, int32_t shaderType);

    uint32_t activeShaderId_{ 0 };
};
}