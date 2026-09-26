#pragma once

#include <string>
#include <utility>

#include <imgui.h>

#include "Editor/EditorWindow.h"

// Ventana de prueba para comprobar que el menu View se actualiza solo
class DummyWindow : public EditorWindow
{
public:
    explicit DummyWindow(std::string name) : EditorWindow(std::move(name), false) {}

protected:
    void DrawContent() override
    {
        ImGui::Text("I am '%s'", m_name.c_str());
        ImGui::TextDisabled("Test window to check the View menu.");
        ImGui::SliderFloat("Value", &m_value, 0.0f, 1.0f);
    }

private:
    float m_value = 0.5f;
};
