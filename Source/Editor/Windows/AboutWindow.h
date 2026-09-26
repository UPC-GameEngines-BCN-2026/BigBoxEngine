#pragma once

#include <string>
#include <vector>

#include "Core/EngineInfo.h"
#include "Editor/EditorWindow.h"

class AboutWindow : public EditorWindow
{
public:
    explicit AboutWindow(const EngineInfo& info);

    void Draw() override;

protected:
    void DrawContent() override;

private:
    struct Library
    {
        std::string name;
        std::string version;
    };

    void CollectLibraryVersions();

    const EngineInfo&    m_info;
    std::string          m_authorsLine;
    std::vector<Library> m_libraries;
};
