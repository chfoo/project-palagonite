#pragma once

extern "C" {
#include <uzebox.h>
}

#include "app.hpp"

#define RUN_SCENE(SceneClass, app) { \
    SceneClass scene(app); \
    scene.set_up(); \
    scene.updating = true; \
    while (scene.updating) { \
        WaitVsync(1); \
        (app)->pre_update(); \
        scene.update(); \
        (app)->post_update(); \
    } \
    scene.tear_down(); \
}

namespace Lib{
class Scene {
protected:
    App * app;

public:
    bool updating;

    Scene(App * app);
    void set_up();
    void update();
    void tear_down();
    void exit_update();
};
}
