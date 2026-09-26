#pragma once

#include <imgui.h>

#include "Editor/EditorWindow.h"

// Envuelve ImGui::ShowDemoWindow() para que aparezca en el menu View.
class DemoWindow : public EditorWindow
{
public:
    DemoWindow() : EditorWindow("ImGui Demo", false) {}

    void Draw() override
    {
        // ShowDemoWindow hace su propio Begin/End, por eso sobrescribimos Draw()
        if (m_visible)
            ImGui::ShowDemoWindow(&m_visible);
    }
};
