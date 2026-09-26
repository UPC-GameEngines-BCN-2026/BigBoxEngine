#include "Core/FileUtils.h"

#include <SDL3/SDL.h>

bool FileUtils::LoadTextFile(const char* path, std::string& outText)
{
    size_t size = 0;
    void* data = SDL_LoadFile(path, &size);
    if (data == nullptr)
    {
        SDL_Log("No se pudo abrir '%s': %s", path, SDL_GetError());
        return false;
    }

    outText.assign(static_cast<const char*>(data), size);
    SDL_free(data);
    return true;
}
