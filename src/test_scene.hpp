#pragma once

#include "lib/dialog.hpp"
#include "lib/menu.hpp"
#include "lib/gfx/printer.hpp"
#include "base_scene.hpp"
#include "game_app.hpp"

class TestScene: public BaseScene {
    Lib::Gfx::Printer printer;
    Lib::Dialog dialog;
    Lib::Menu menu;

    enum class State {
        show_dialog_1,
        poll_dialog_1,
        show_dialog_2,
        poll_dialog_2,
        show_menu_1,
        poll_menu_1,
        show_menu_2,
        poll_menu_2,
        quit,
    };

    State state;
public:
    TestScene(GameApp * app);
    void set_up();
    void update();
};
