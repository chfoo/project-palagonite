#include <avr/pgmspace.h>

#include "title_scene.hpp"

TitleScene::TitleScene(GameApp * app) : BaseScene(app), app(app) {

}

void TitleScene::set_up() {
    printer.set_origin(0, 10);
    printer.print_center(PSTR("hello world"));
}

void TitleScene::update() {
    if (this->app->input.button_1_pressed & BTN_A) {
        this->switch_scene(GameApp::SceneName::play);
    }
}
