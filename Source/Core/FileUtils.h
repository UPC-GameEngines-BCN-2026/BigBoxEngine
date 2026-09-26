#pragma once

#include <string>

namespace FileUtils
{
    bool LoadTextFile(const char* path, std::string& outText);
}
