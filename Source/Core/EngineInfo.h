#pragma once

#include <string>
#include <vector>

// @ Data/Config/engine.json
struct EngineInfo
{
    std::string              name = "BigBoxEngine";
    std::string              version = "0.0.0";
    std::string              description;
    std::string              organization;
    std::vector<std::string> authors;
    std::string              licenseText;
};

bool LoadEngineInfo(const char* path, EngineInfo& out);
