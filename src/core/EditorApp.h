#pragma once
#include <ZeusEngineCore/API.h>
#include <ZeusEngineCore/Application.h>

namespace ZED {

    class EditorApp : public ZEN::Application {
    public:
        EditorApp(ZEN::eRendererAPI api);

        ~EditorApp();
    };
}
