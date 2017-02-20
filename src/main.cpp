extern "C" {
#include <uzebox.h>
}

#include "lib/scene.hpp"
#include "game_app.hpp"
#include "title_scene.hpp"
#include "test_scene.hpp"
#include "play_scene.hpp"

int main() {
    GameApp app;
    app.init();

#ifndef NDEBUG
    RUN_SCENE(TestScene, &app);
#endif

    while (true) {
        switch(app.current_scene) {
        case GameApp::SceneName::title:
            RUN_SCENE(TitleScene, &app);
            break;
        case GameApp::SceneName::play:
            RUN_SCENE(PlayScene, &app);
            break;
        }
    }

    while (true) {
        WaitVsync(1);
    }

    return 0;
}
