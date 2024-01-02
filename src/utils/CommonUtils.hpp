#pragma once

#include <stdio.h>
#include <string>
#include <sstream>
#include <glm/glm.hpp>

namespace utils
{

glm::vec4 hexToVec4(const std::string& hexColor);

}