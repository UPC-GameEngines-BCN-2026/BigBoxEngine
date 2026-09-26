#pragma once

#include <memory>
#include <string>
#include <utility>
#include <vector>

#include <SDL3/SDL.h>

#include "Core/EngineInfo.h"
#include "Editor/EditorWindow.h"

class AboutWindow;

// from Data/Config/help_menu.json
struct HelpMenuItem
{
    std::string label;
    std::string url;    // if it has a url, it must open in the browser
	std::string action; // internal actions: "about". more can be added in the future
    bool separator = false;
};

class Editor
{
public:
    bool Init(SDL_Window* window, SDL_GLContext glContext, const char* glslVersion);
    void ProcessEvent(const SDL_Event& event);
    void Update(); 
    void Render(); 
    void Shutdown();

    bool WantsToQuit() const { return m_wantsToQuit; }
    const EngineInfo& GetEngineInfo() const { return m_engineInfo; }

    // registers windows.
	template <typename T, typename... Args> //we use template... to allow passing any number of arguments to the constructor of T (can be empty, a string, pointers, etc)
	T* AddWindow(Args&&... args) //we use forwarding references (&&) to perfectly forward the arguments to the constructor of T, preserving their value category (lvalue/rvalue)
    {
		std::unique_ptr<T> window = std::make_unique<T>(std::forward<Args>(args)...); //perfectly forward the arguments to the constructor of T
		/*
        this combination of Args&& + std::forward<Args>(args)... allows us to create windows of any type derived from EditorWindow,
        with any constructor signature, and store them in the m_windows vector as unique_ptr<EditorWindow> while still returning a raw pointer of the correct type T* to the caller.
       */
        T* ptr = window.get();
        m_windows.push_back(std::move(window));
        return ptr;
    }

private:
    void DrawMainMenuBar();
    void DrawFileMenu();
    void DrawViewMenu();
    void DrawHelpMenu();
    bool LoadHelpMenu(const char* path);

    std::vector<std::unique_ptr<EditorWindow>> m_windows;
    std::vector<HelpMenuItem> m_helpItems;
    EngineInfo m_engineInfo;
    AboutWindow* m_aboutWindow = nullptr;
    bool m_wantsToQuit = false;
};
