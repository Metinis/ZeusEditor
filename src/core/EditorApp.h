#pragma once
#include <ZeusEngine.h>
#include "EditorLayer.h"

namespace ZED {

    class EditorApp : public ZEN::Application {
    public:
        EditorApp(ZEN::eRendererAPI api);
        ~EditorApp() override;
    };


}
