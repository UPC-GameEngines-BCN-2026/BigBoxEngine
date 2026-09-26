#pragma once

#include <string>
#include <utility>

#include <imgui.h>

/*

Base class for all editor windows.
The Editor keeps a list of EditorWindow and the View menu is generated from it.

*/
class EditorWindow
{
public:
    explicit EditorWindow(std::string name, bool visible = true, bool listedInViewMenu = true)
        : m_name(std::move(name)), m_visible(visible), m_listedInViewMenu(listedInViewMenu)
    {
    }
    virtual ~EditorWindow() = default; //virtual pointer, because EditorWindow is used in std::unique_ptr<EditorWindow>


    virtual void Draw()
    {
        if (!m_visible)
            return;

        // Parsing &m_visible makes it so that the "X" of the window changes the same bool used by the View menu: both stay always synchronized.
        if (ImGui::Begin(m_name.c_str(), &m_visible, m_flags))
            DrawContent();
        ImGui::End();   // always End to avoid crashing next frame
    }

    const std::string& GetName() const { return m_name; }
    bool  IsVisible() const { return m_visible; }
    void  SetVisible(bool visible) { m_visible = visible; }
    bool* GetVisiblePtr() { return &m_visible; }
    bool  IsListedInViewMenu() const { return m_listedInViewMenu; }

protected:
    virtual void DrawContent() {}

    std::string      m_name;                // is unique, ImGui uses it as ID
    bool             m_visible;             // if false, the window is not drawn (but still exists in the list)
    bool             m_listedInViewMenu;    // if false, the window is not listed in the View menu (e.g. About is in Help)
    ImGuiWindowFlags m_flags = ImGuiWindowFlags_None;
};