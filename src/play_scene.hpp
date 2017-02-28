#pragma once

#include "lib/gfx/printer.hpp"
#include "base_scene.hpp"
#include "game_app.hpp"
#include "level_model.hpp"

class PlayScene: public BaseScene {
protected:
    static const uint8_t PLAYER_SPRITE_INDEX = 0;
    static const uint8_t TRACK_0_CONTROLS_SPRITE_INDEX = 4;
    static const uint8_t TRACK_1_CONTROLS_SPRITE_INDEX = 5;
    static const uint8_t TRACK_2_CONTROLS_SPRITE_INDEX = 6;
    static const uint8_t TRACK_3_CONTROLS_SPRITE_INDEX = 7;
    static const uint8_t OVERLAY_HEIGHT = 8;
    static const uint8_t TRACK_CELL_TO_TILE_SCALE = 2;
    static const uint8_t TRACK_CELLS_VISIBLE_BEHIND = 2;
    static const uint8_t TRACK_CELLS_VISIBLE_AHEAD = 12;
    static const uint8_t FIRST_TRACK_TILE_Y = 2;
    static const uint8_t TRACK_TILE_HEIGHT = 2;
    static const uint8_t TRACK_TILE_SPACING = 2;
    static const uint8_t TRACK_PLAYER_TILE_X = 4;
    static const uint8_t TRACK_CONTROLS_TILE_X = 2;

    Lib::Gfx::Printer overlay_printer;
    LevelModel level_model;
    uint8_t scroll_tile_offset_x;
    uint8_t scroll_speed;
    uint8_t scroll_next_cell_counter;

public:
    PlayScene(GameApp * app);

    void set_up();
    void update();
    void tear_down();

protected:
    void set_up_hud();
    void set_up_sprites();
    void draw_init_tracks();
    void update_hud();
    void update_player();
    void update_scroll();
    void set_scroll(TrackCellIndex_t cell_index, TrackCellSubIndex_t cell_sub_index);
    void draw_track_column(TrackCellIndex_t cell_index);
};
