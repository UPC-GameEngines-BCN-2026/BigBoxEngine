#include "Editor/Windows/AboutWindow.h"

#include <cstdio>

#include <glad/glad.h>
#include <SDL3/SDL.h>
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <imgui.h>

AboutWindow::AboutWindow(const EngineInfo& info)
    : EditorWindow("About", /*visible*/ false, /*listedInViewMenu*/ false)
    , m_info(info)
{
    // "By A, B, ..., & C"
    for (size_t i = 0; i < m_info.authors.size(); ++i)
    {
        if (i == 0)                               m_authorsLine = "By ";
        else if (i == m_info.authors.size() - 1)  m_authorsLine += " & ";
        else                                      m_authorsLine += ", ";
        m_authorsLine += m_info.authors[i];
    }

    CollectLibraryVersions();
}

// Library versions are read at runtime/compile time, so they never become outdated when vcpkg updates a library.
void AboutWindow::CollectLibraryVersions()
{
    char buffer[64];

    const int sdl = SDL_GetVersion();
    std::snprintf(buffer, sizeof(buffer), "%d.%d.%d",
                  SDL_VERSIONNUM_MAJOR(sdl), SDL_VERSIONNUM_MINOR(sdl), SDL_VERSIONNUM_MICRO(sdl));
    m_libraries.push_back({ "SDL", buffer });

    const char* glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    m_libraries.push_back({ "OpenGL", glVersion ? glVersion : "unknown" });

    m_libraries.push_back({ "GLAD", "0.1 (loader)" });

    m_libraries.push_back({ "Dear ImGui", IMGUI_VERSION });

    std::snprintf(buffer, sizeof(buffer), "%d.%d.%d",
                  GLM_VERSION_MAJOR, GLM_VERSION_MINOR, GLM_VERSION_PATCH);
    m_libraries.push_back({ "GLM", buffer });

    std::snprintf(buffer, sizeof(buffer), "%d.%d.%d",
                  NLOHMANN_JSON_VERSION_MAJOR, NLOHMANN_JSON_VERSION_MINOR, NLOHMANN_JSON_VERSION_PATCH);
    m_libraries.push_back({ "nlohmann/json", buffer });

    m_libraries.push_back({ "ImGuizmo", "" });

}

void AboutWindow::Draw()
{
    if (!m_visible)
        return;

    // Initial size the first time (afterwards ImGui remembers it in imgui.ini)
    ImGui::SetNextWindowSize(ImVec2(520.0f, 620.0f), ImGuiCond_FirstUseEver);
    EditorWindow::Draw();
}

void AboutWindow::DrawContent()
{
    ImGui::Text("%s v%s", m_info.name.c_str(), m_info.version.c_str());
    if (!m_info.description.empty())
        ImGui::TextWrapped("%s", m_info.description.c_str());
    if (!m_authorsLine.empty())
        ImGui::TextUnformatted(m_authorsLine.c_str());

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("3rd Party Libraries used:");
    for (const Library& lib : m_libraries)
    {
        if (lib.version.empty())
            ImGui::BulletText("%s", lib.name.c_str());
        else
            ImGui::BulletText("%s %s", lib.name.c_str(), lib.version.c_str());
    }

    ImGui::Spacing();
    ImGui::Separator();
    ImGui::Text("License:");
    if (m_info.licenseText.empty())
        ImGui::TextDisabled("(license file not found)");
    else
        ImGui::TextWrapped("%s", m_info.licenseText.c_str());
}
