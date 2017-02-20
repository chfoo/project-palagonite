#pragma once
#include "lib/app.hpp"
#include "lib/input.hpp"

class GameApp : public Lib::App {
public:
    enum class SceneName {
        title, play
    };
    SceneName current_scene;

    Lib::Input input;

    void init();
    void pre_update();
    void switch_scene(SceneName scene_name);
protected:
    void init_tilesheet();
};
