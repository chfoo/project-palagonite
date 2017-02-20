#pragma once

#include "lib/gfx/printer.hpp"
#include "game_app.hpp"
#include "base_scene.hpp"

class TitleScene: public BaseScene {
    Lib::Gfx::Printer printer;
    GameApp * app;
public:
    TitleScene(GameApp * app);
    void set_up();
    void update();
};
