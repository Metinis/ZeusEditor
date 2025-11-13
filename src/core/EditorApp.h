#pragma once
#include <ZeusEngineCore/API.h>
#include <ZeusEngineCore/Application.h>

class ScenePanel;
class ProjectPanel;
class InspectorPanel;
class ViewPanel;
class MenuBarPanel;

namespace ZED {

    class EditorApp : public ZEN::Application {
    public:
        EditorApp(ZEN::eRendererAPI api);

        ~EditorApp() override;
    };


}
