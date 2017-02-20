#include "base_scene.hpp"

BaseScene::BaseScene(GameApp * app) : Lib::Scene(app), app(app) {

}

void BaseScene::switch_scene(GameApp::SceneName scene_name) {
    this->exit_update();
    this->app->current_scene = scene_name;
}
