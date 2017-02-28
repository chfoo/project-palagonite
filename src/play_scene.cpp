#include <avr/pgmspace.h>
extern "C" {
#include <uzebox.h>
}

#include "lib/gfx/screen_util.hpp"
#include "lib/inc/tilesheet.hpp"
#include "lib/gfx/printer.hpp"
#include "lib/tables.hpp"
#include "play_scene.hpp"

PlayScene::PlayScene(GameApp * app) : BaseScene(app), scroll_tile_offset_x(0),
        scroll_speed(0), scroll_next_cell_counter(false) {

}

void PlayScene::set_up() {
    this->set_up_hud();
    this->set_up_sprites();
    this->draw_init_tracks();
}

void PlayScene::set_up_hud() {
    Screen.overlayHeight = this->OVERLAY_HEIGHT;
    Screen.scrollHeight = VRAM_TILES_H - Screen.overlayHeight;
    Screen.overlayTileTable = (const char *) TilesheetData::main::tile_data;
    this->overlay_printer.set_origin(0, VRAM_TILES_H - Screen.overlayHeight);
    this->overlay_printer.move_cursor(1, 3);
    this->overlay_printer.print(PSTR("Points"));
}

void PlayScene::set_up_sprites() {
    SetSpritesTileBank(SPRITE_BANK0,
            (const char *) TilesheetData::main::tile_data);
    SetSpritesTileBank(SPRITE_BANK1,
            (const char *) TilesheetData::main::tile_data);
    SetSpritesTileBank(SPRITE_BANK2,
            (const char *) TilesheetData::main::tile_data);
    SetSpritesTileBank(SPRITE_BANK3,
            (const char *) TilesheetData::main::tile_data);

    MapSprite2(
            this->PLAYER_SPRITE_INDEX,
            (const char *) TilesheetData::main::maps::player,
            SPRITE_BANK0
    );
    sprites[TRACK_0_CONTROLS_SPRITE_INDEX].tileIndex =
            TilesheetData::main::tiles::controls_up;
    sprites[TRACK_1_CONTROLS_SPRITE_INDEX].tileIndex =
            TilesheetData::main::tiles::controls_left;
    sprites[TRACK_2_CONTROLS_SPRITE_INDEX].tileIndex =
            TilesheetData::main::tiles::controls_right;
    sprites[TRACK_3_CONTROLS_SPRITE_INDEX].tileIndex =
            TilesheetData::main::tiles::controls_down;

    const uint8_t CONTROLS_X = TILE_WIDTH * this->TRACK_CONTROLS_TILE_X;

#define move_controls_sprite(track_index) { \
    MoveSprite( \
            TRACK_ ##track_index ## _CONTROLS_SPRITE_INDEX, \
            CONTROLS_X, \
            TILE_HEIGHT * ( \
                this->FIRST_TRACK_TILE_Y + \
                (this->TRACK_TILE_HEIGHT + this->TRACK_TILE_SPACING) \
                * track_index) + TILE_HEIGHT / 2, \
            1, 1 \
    ); \
}
    move_controls_sprite(0);
    move_controls_sprite(1);
    move_controls_sprite(2);
    move_controls_sprite(3);

#undef move_controls_sprite

    SetSpriteVisibility(true);
}

void PlayScene::draw_init_tracks() {
    for (uint8_t column = 0; column <= this->TRACK_CELLS_VISIBLE_AHEAD; column++) {
        draw_track_column(column);
    }
}

void PlayScene::update() {
    this->update_player();
    this->update_hud();
    this->update_scroll();
}

void PlayScene::update_hud() {
    this->overlay_printer.set_cursor(2, 4);
    this->overlay_printer.print_int(this->level_model.player_cell_index, 4);
}

void PlayScene::update_player() {
    if (this->app->input.button_1_pressed & BTN_UP) {
        this->level_model.move_player_track(0);
    } else if (this->app->input.button_1_pressed & BTN_LEFT) {
       this->level_model.move_player_track(1);
    } else if (this->app->input.button_1_pressed & BTN_RIGHT) {
       this->level_model.move_player_track(2);
    } else if (this->app->input.button_1_pressed & BTN_DOWN) {
       this->level_model.move_player_track(3);
    }

    MoveSprite(
           PLAYER_SPRITE_INDEX,
           TILE_WIDTH * this->TRACK_PLAYER_TILE_X,
           TILE_HEIGHT * (this->FIRST_TRACK_TILE_Y +
                   (this->TRACK_TILE_HEIGHT + this->TRACK_TILE_SPACING)
                   * this->level_model.player_track_index),
           TilesheetData::main::maps::player_width,
           TilesheetData::main::maps::player_height
    );
}

void PlayScene::update_scroll() {
//    if (this->app->input.button_1_down & BTN_A) {
//        if (this->scroll_speed < 32) {
//            this->scroll_speed += 1;
//        }
//    } else if (this->scroll_speed) {
//        this->scroll_speed -= 1;
//    }
//
//    if (this->scroll_speed) {
//        uint8_t new_sub_cell_index = this->level_model.player_cell_sub_index + this->scroll_speed;
//
//        if (new_sub_cell_index < this->level_model.player_cell_sub_index) {
//            this->set_scroll(this->level_model.player_cell_index + 1, new_sub_cell_index);
//            this->draw_track_column(this->level_model.player_cell_index + this->TRACK_CELLS_VISIBLE_AHEAD);
//        } else {
//            this->set_scroll(this->level_model.player_cell_index, new_sub_cell_index);
//        }
//    }
    if ((this->app->input.button_1_pressed & BTN_A) && this->scroll_next_cell_counter == 0) {
        this->scroll_next_cell_counter = 1;
    }

    if (this->scroll_next_cell_counter != 0) {
        if (this->scroll_next_cell_counter != 255) {
            uint8_t new_sub_cell_index = pgm_read_byte_near(&Lib::Tables::EASE_IN_OUT_LOOKUP_TABLE_U8[this->scroll_next_cell_counter]);
            this->set_scroll(this->level_model.player_cell_index, new_sub_cell_index);

            uint8_t prev_scroll_counter = this->scroll_next_cell_counter;
            this->scroll_next_cell_counter += 32;

            if (this->scroll_next_cell_counter < prev_scroll_counter) {
                this->scroll_next_cell_counter = 255;
            }
        } else {
            this->set_scroll(this->level_model.player_cell_index + 1, 0);
            this->draw_track_column(this->level_model.player_cell_index + this->TRACK_CELLS_VISIBLE_AHEAD);
            this->scroll_next_cell_counter = 0;
        }
    }
}

void PlayScene::tear_down() {
    SetSpriteVisibility(false);
}

void PlayScene::set_scroll(TrackCellIndex_t cell_index, TrackCellSubIndex_t cell_sub_index) {
    this->level_model.move_player_cell(cell_index, cell_sub_index);
    this->scroll_tile_offset_x = (cell_index * this->TRACK_CELL_TO_TILE_SCALE) % VRAM_TILES_H;
    Screen.scrollX = this->scroll_tile_offset_x * TILE_WIDTH + this->TRACK_CELL_TO_TILE_SCALE * TILE_WIDTH * cell_sub_index / 255;
}

void PlayScene::draw_track_column(TrackCellIndex_t cell_index) {
    TrackCellType cells[4];
    this->level_model.get_track_cell_column(cell_index, cells);

    const uint8_t tile_x = this->TRACK_PLAYER_TILE_X + scroll_tile_offset_x +
            (cell_index - this->level_model.player_cell_index) * this->TRACK_CELL_TO_TILE_SCALE;

    for (uint8_t track_index = 0; track_index < 4; track_index++) {
        const uint8_t tile_y = this->FIRST_TRACK_TILE_Y +
                (TRACK_TILE_HEIGHT + TRACK_TILE_SPACING) * track_index;

        switch (cells[track_index]) {
        case TrackCellType::empty:
            Fill(tile_x, tile_y, 2, 2, 0);
            break;
        case TrackCellType::ground:
            DrawMap2(tile_x, tile_y,
                    (const char *) TilesheetData::main::maps::track_ground);
            break;
        case TrackCellType::obstacle:
            DrawMap2(tile_x, tile_y,
                    (const char *) TilesheetData::main::maps::track_obstacle);
            break;
        }
    }

    Lib::Gfx::Printer printer;
    printer.set_origin(tile_x, 0);
    printer.print_int(cell_index, 2, true);
}
