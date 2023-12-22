#include "ShaderHelper.hpp"

namespace shaderHelpers
{

int32_t ShaderHelper::loadFromPath(const std::string& vertPath, const std::string& fragPath)
{
    return linkShaders(compileShader(vertPath, GL_VERTEX_SHADER),
        compileShader(fragPath, GL_FRAGMENT_SHADER));
}

int32_t ShaderHelper::reloadFromPath(const std::string& vertPath, const std::string& fragPath)
{
    return linkShaders(compileShader(vertPath, GL_VERTEX_SHADER),
        compileShader(fragPath, GL_FRAGMENT_SHADER));
}

void ShaderHelper::handleNotFound(const char* location)
{
    fprintf(stderr, "Uniform '%s', has not been found in bound shader: {%d}\n", location, activeShaderId_);
    // exit(1); /* Exit immediately and fix the location error */
}

void ShaderHelper::setInt(const char* location, int value)
{
    int loc = glGetUniformLocation(activeShaderId_, location);
    if (loc == -1) return handleNotFound(location);
    glUniform1i(loc, value);
}

void ShaderHelper::setIntVec(const char* location, uint32_t amount, int* values)
{
    int loc = glGetUniformLocation(activeShaderId_, location);
    if (loc == -1) return handleNotFound(location);
    glUniform1iv(loc, amount, values);
}

void ShaderHelper::setVec1f(const char* location, float value)
{
    int loc = glGetUniformLocation(activeShaderId_, location);
    if (loc == -1) return handleNotFound(location);
    glUniform1f(loc, value);
}

void ShaderHelper::setVec2f(const char* location, glm::vec2 value)
{
    int loc = glGetUniformLocation(activeShaderId_, location);
    if (loc == -1) return handleNotFound(location);
    glUniform2f(loc, value.x, value.y);
}

void ShaderHelper::setVec3f(const char* location, glm::vec3 value)
{
    int loc = glGetUniformLocation(activeShaderId_, location);
    if (loc == -1) return handleNotFound(location);
    glUniform3f(loc, value.x, value.y, value.z);
}

void ShaderHelper::setVec4f(const char* location, glm::vec4 value)
{
    int loc = glGetUniformLocation(activeShaderId_, location);
    if (loc == -1) return handleNotFound(location);
    glUniform4f(loc, value.r, value.g, value.b, value.a);
}

void ShaderHelper::setMatrix4(const char* location, const glm::mat4 transform)
{
    int loc = glGetUniformLocation(activeShaderId_, location);
    if (loc == -1) return handleNotFound(location);
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(transform));
}

void ShaderHelper::setMatrix4Vec(const char* location, uint32_t amount, const float* flatTransforms)
{
    int loc = glGetUniformLocation(activeShaderId_, location);
    if (loc == -1) return handleNotFound(location);
    glUniformMatrix4fv(loc, amount, GL_FALSE, flatTransforms);
}

void ShaderHelper::setActiveShaderId(const uint32_t id)
{
    activeShaderId_ = id;
    glUseProgram(activeShaderId_);
}

void ShaderHelper::resetBoundShader() const { glUseProgram(0); }

int ShaderHelper::linkShaders(int vertShaderId, int fragShaderId)
{
    if (!vertShaderId || !fragShaderId)
        return -1;

    activeShaderId_ = glCreateProgram();

    glAttachShader(activeShaderId_, vertShaderId);
    glAttachShader(activeShaderId_, fragShaderId);
    glLinkProgram(activeShaderId_);

    int success;
    char infoLog[512];
    glGetProgramiv(activeShaderId_, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetProgramInfoLog(activeShaderId_, 512, nullptr, infoLog);
        fprintf(stderr, "Could not link program because:\n\t%s\n", infoLog);
        return -1;
    }

    glDeleteShader(vertShaderId);
    glDeleteShader(fragShaderId);

    return activeShaderId_;
}

int ShaderHelper::compileShader(const std::string& sourcePath, int32_t shaderType)
{
    std::string type = shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAG";
    std::ifstream shaderFile(sourcePath);
    if (!shaderFile)
    {
        fprintf(stderr, "Could not open %s shader file at %s\n", type.c_str(), sourcePath.c_str());
        return -1;
    }

    std::stringstream stream;
    stream << shaderFile.rdbuf();
    std::string content = stream.str();
    const char* src = content.c_str();
    shaderFile.close();

    uint32_t shaderPart = glCreateShader(shaderType);
    glShaderSource(shaderPart, 1, &src, NULL);
    glCompileShader(shaderPart);

    int success;
    char infoLog[512];
    glGetShaderiv(shaderPart, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shaderPart, 512, NULL, infoLog);
        fprintf(stderr, "Compile failed for shader %s because:\n\t%s\n", type.c_str(), infoLog);
        return -1;
    }

    return shaderPart;
}

uint32_t ShaderHelper::getActiveShaderId() const { return activeShaderId_; }

}