#pragma once

#include <string>
#include <map>
#include <stdio.h>

#include "Types.hpp"

namespace textHelpers
{
class TextHelper
{
public:
    static TextHelper& get();

    LoadedFontPtr loadFont(const std::string& fontPath, const int32_t fontSize);

private:
    TextHelper() = default;
    TextHelper(const TextHelper&) = delete;
    TextHelper& operator=(const TextHelper&) = delete;

    std::map<std::string, LoadedFont> gFontPathToGenFont;
};
}