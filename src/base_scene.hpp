#pragma once

#include "lib/scene.hpp"
#include "game_app.hpp"

class BaseScene : public Lib::Scene {
protected:
    GameApp * app;
public:
    BaseScene(GameApp * app);
    void switch_scene(GameApp::SceneName scene_name);
};
