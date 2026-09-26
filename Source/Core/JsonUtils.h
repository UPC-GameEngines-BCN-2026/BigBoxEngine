#pragma once

#include <string>
#include <vector>

#include <nlohmann/json.hpp>
#include <SDL3/SDL.h>

#include "Core/FileUtils.h"

namespace JsonUtils
{
    inline bool LoadJsonFile(const char* path, nlohmann::json& out)
    {
        std::string text;
        if (!FileUtils::LoadTextFile(path, text))
            return false;

        out = nlohmann::json::parse(text, nullptr, /*allow_exceptions = */ false);
        if (out.is_discarded())
        {
            SDL_Log("JSON invalido en '%s'", path);
            return false;
        }
        return true;
    }

    inline std::string GetString(const nlohmann::json& j, const char* key,
                                 const std::string& fallback = "")
    {
        auto it = j.find(key);
        if (it != j.end() && it->is_string())
            return it->get<std::string>();
        return fallback;
    }

    inline bool GetBool(const nlohmann::json& j, const char* key, bool fallback = false)
    {
        auto it = j.find(key);
        if (it != j.end() && it->is_boolean())
            return it->get<bool>();
        return fallback;
    }

    inline std::vector<std::string> GetStringArray(const nlohmann::json& j, const char* key)
    {
        std::vector<std::string> result;
        auto it = j.find(key);
        if (it != j.end() && it->is_array())
        {
            for (const auto& element : *it)
                if (element.is_string())
                    result.push_back(element.get<std::string>());
        }
        return result;
    }
}
