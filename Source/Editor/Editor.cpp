#include "Editor/Editor.h"

#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_opengl3.h>

#include "Core/JsonUtils.h"
#include "Editor/Windows/AboutWindow.h"
#include "Editor/Windows/DemoWindow.h"
#include "Editor/Windows/DummyWindow.h"

static const char* ENGINE_INFO_PATH = "Data/Config/engine.json";
static const char* HELP_MENU_PATH   = "Data/Config/help_menu.json";
static const int   DUMMY_WINDOWS    = 3;

bool Editor::Init(SDL_Window* window, SDL_GLContext glContext, const char* glslVersion)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui::StyleColorsDark();

    if (!ImGui_ImplSDL3_InitForOpenGL(window, glContext))
        return false;
    if (!ImGui_ImplOpenGL3_Init(glslVersion))
        return false;
    SDL_Log("ImGui initialised (%s)", IMGUI_VERSION);

    if (!LoadEngineInfo(ENGINE_INFO_PATH, m_engineInfo))
        SDL_Log("Using default engine information");
    SDL_SetWindowTitle(window, m_engineInfo.name.c_str());

    if (!LoadHelpMenu(HELP_MENU_PATH))
    {
        HelpMenuItem about;
        about.label = "About";
        about.action = "about";
        m_helpItems.push_back(about);
    }

    AddWindow<DemoWindow>();
    for (int i = 1; i <= DUMMY_WINDOWS; ++i)
        AddWindow<DummyWindow>("Dummy Window " + std::to_string(i));
    m_aboutWindow = AddWindow<AboutWindow>(m_engineInfo);

    SDL_Log("Editor initialised with %d windows", static_cast<int>(m_windows.size()));
    return true;
}

bool Editor::LoadHelpMenu(const char* path)
{
    nlohmann::json j;
    if (!JsonUtils::LoadJsonFile(path, j))
        return false;

    auto items = j.find("items");
    if (items == j.end() || !items->is_array())
    {
        SDL_Log("'%s' does not have an array \"items\"", path);
        return false;
    }

    for (const auto& entry : *items)
    {
        HelpMenuItem item;
        item.separator = JsonUtils::GetBool(entry, "separator");
        item.label     = JsonUtils::GetString(entry, "label");
        item.url       = JsonUtils::GetString(entry, "url");
        item.action    = JsonUtils::GetString(entry, "action");

        if (!item.separator && item.label.empty())
			continue; // skip invalid entries

        m_helpItems.push_back(item);
    }

    SDL_Log("Help menu loaded: %d entries", static_cast<int>(m_helpItems.size()));
    return true;
}

void Editor::ProcessEvent(const SDL_Event& event)
{
    ImGui_ImplSDL3_ProcessEvent(&event);
}

void Editor::Update()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();

    DrawMainMenuBar();
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport(), ImGuiDockNodeFlags_PassthruCentralNode);

    for (auto& window : m_windows)
        window->Draw();

    ImGui::Render();
}

void Editor::Render()
{
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Editor::Shutdown()
{
    m_windows.clear();
    m_aboutWindow = nullptr;

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
}

void Editor::DrawMainMenuBar()
{
    if (ImGui::BeginMainMenuBar())
    {
        DrawFileMenu();
        DrawViewMenu();
        DrawHelpMenu();
        ImGui::EndMainMenuBar();
    }
}

void Editor::DrawFileMenu()
{
    if (ImGui::BeginMenu("File"))
    {
        if (ImGui::MenuItem("Exit", "Alt+F4"))
            m_wantsToQuit = true;
        ImGui::EndMenu();
    }
}

void Editor::DrawViewMenu()
{
    if (ImGui::BeginMenu("View"))
    {
        for (auto& window : m_windows)
        {
            if (window->IsListedInViewMenu())
                ImGui::MenuItem(window->GetName().c_str(), nullptr, window->GetVisiblePtr());
        }

        ImGui::Separator();
        if (ImGui::MenuItem("Show all"))
            for (auto& window : m_windows)
                if (window->IsListedInViewMenu()) window->SetVisible(true);
        if (ImGui::MenuItem("Hide all"))
            for (auto& window : m_windows)
                if (window->IsListedInViewMenu()) window->SetVisible(false);

        ImGui::EndMenu();
    }
}

void Editor::DrawHelpMenu()
{
    if (ImGui::BeginMenu("Help"))
    {
        for (const HelpMenuItem& item : m_helpItems)
        {
            if (item.separator)
            {
                ImGui::Separator();
                continue;
            }

            if (ImGui::MenuItem(item.label.c_str()))
            {
                if (item.action == "about")
                {
                    if (m_aboutWindow)
                        m_aboutWindow->SetVisible(true);
                }
                else if (!item.url.empty())
                {
                    if (!SDL_OpenURL(item.url.c_str()))
                        SDL_Log("No se pudo abrir '%s': %s", item.url.c_str(), SDL_GetError());
                }
                else if (!item.action.empty())
                {
                    SDL_Log("Accion de Help desconocida: '%s'", item.action.c_str());
                }
            }
        }
        ImGui::EndMenu();
    }
}
