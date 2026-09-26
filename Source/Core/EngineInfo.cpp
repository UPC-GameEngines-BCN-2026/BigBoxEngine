#include "Core/EngineInfo.h"

#include <SDL3/SDL.h>

#include "Core/FileUtils.h"
#include "Core/JsonUtils.h"

bool LoadEngineInfo(const char* path, EngineInfo& out)
{
    nlohmann::json j;
    if (!JsonUtils::LoadJsonFile(path, j))
        //if no engine.json is found, the default values in EngineInfo will be used
        return false;

    out.name         = JsonUtils::GetString(j, "name", out.name);
    out.version      = JsonUtils::GetString(j, "version", out.version);
    out.description  = JsonUtils::GetString(j, "description");
    out.organization = JsonUtils::GetString(j, "organization");
    out.authors      = JsonUtils::GetStringArray(j, "authors");

    const std::string licenseFile = JsonUtils::GetString(j, "license_file");
    if (!licenseFile.empty())
        FileUtils::LoadTextFile(licenseFile.c_str(), out.licenseText);

    SDL_Log("Engine info cargada: %s v%s", out.name.c_str(), out.version.c_str());
    return true;
}
