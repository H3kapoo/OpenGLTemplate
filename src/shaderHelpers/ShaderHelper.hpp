#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <GL/glew.h>

#include "Types.hpp"

namespace shaderHelpers
{

/**
 * @brief Singleton providing shader load/reload and uniform access capabilities.
 *
 * Singleton class providing easy means to load/reload shaders and access to their uniforms.
 * Class also provides caching capabilities for shaders.
 */
class ShaderHelper
{
public:
    ~ShaderHelper();

    /* Setters */
    shaderIdPtr loadFromPath(const std::string& vertPath, const std::string& fragPath);
    void reloadFromPath(const std::string& vertPath, const std::string& fragPath);
    void setActiveShaderId(const shaderId id);
    void resetBoundShader() const;

    /* Uniforms */
    void setInt(const char* location, int value);
    void setIntVec(const char* location, uint32_t amount, int* flatValues);
    void setVec1f(const char* location, float value);
    void setVec2f(const char* location, glm::vec2 value);
    void setVec3f(const char* location, glm::vec3 value);
    void setVec4f(const char* location, glm::vec4 value);
    void setMatrix4(const char* location, const glm::mat4 value);
    void setMatrix4Vec(const char* location, uint32_t amount, const float* flatValues);

    /* Getters */
    shaderId getActiveShaderId() const;

private:
    /* In case a lot of objects use the same shader, it's pointless to allocate memory for each SAME shader.
       We can reference the same one, just changing uniforms as needed. */
    std::map<std::string, int32_t*> gShaderPathToGenId;

    void handleNotFound(const char* location);
    int linkShaders(int vertShaderId, int fragShaderId);
    int compileShader(const std::string& sourcePath, int32_t shaderType);

    shaderId gActiveShaderId{ 0 };
};
}