#include "scene.hpp"

using Lib::Scene;

Scene::Scene(App * app) : app(app), updating(false) {

}

void Scene::set_up() {

}

void Scene::update() {

}

void Scene::tear_down() {

}

void Scene::exit_update() {
    this->updating = false;
}
