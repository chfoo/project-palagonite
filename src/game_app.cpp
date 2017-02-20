extern "C" {
#include <uzebox.h>
}

#include "game_app.hpp"
#include "lib/inc/tilesheet.hpp"

void GameApp::init() {
    GetPrngNumber(59);

    this->init_tilesheet();
}

void GameApp::pre_update() {
    input.process();
}

void GameApp::switch_scene(SceneName scene_name) {
    this->current_scene = scene_name;
}

void GameApp::init_tilesheet() {

    SetTileTable((const char*)TilesheetData::main::tile_data);
    SetFontTilesIndex(TilesheetData::main::tiles::fonts);
//    SetSpritesTileBank(0, game_tiles);
//    SetSpriteVisibility(true);
}
