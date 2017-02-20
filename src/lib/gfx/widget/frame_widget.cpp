
extern "C" {
#include <uzebox.h>
}

#include "frame_widget.hpp"
#include "../../inc/tilesheet.hpp"

using Lib::Gfx::FrameWidget;

FrameWidget::FrameWidget() :
        x(0), y(0), width(0), height(0) {

}

void FrameWidget::reset() {
    this->x = this->y = this->width = this->height = 0;
}

void FrameWidget::set_position(uint8_t x, uint8_t y) {
    this->x = x;
    this->y = y;

    update_printer();
}

void FrameWidget::set_size(uint8_t width, uint8_t height) {
    this->width = width;
    this->height = height;

    update_printer();
}

void FrameWidget::align_center() {
    this->x = SCREEN_TILES_H / 2 - (this->width + this->BORDER_WIDTH) / 2;

    update_printer();
}

void FrameWidget::draw_border() {
    SetTile(this->x, this->y, TilesheetData::main::tiles::frame_top_left);
    SetTile(this->x + this->width - 1, this->y,
            TilesheetData::main::tiles::frame_top_right);
    SetTile(this->x, this->y + this->height - 1,
            TilesheetData::main::tiles::frame_bottom_left);
    SetTile(this->x + this->width - 1, this->y + this->height - 1,
            TilesheetData::main::tiles::frame_bottom_right);

    for (uint8_t x = 1; x < this->width - 1; x++) {
        SetTile(this->x + x, this->y, TilesheetData::main::tiles::frame_top);
        SetTile(this->x + x, this->y + this->height - 1,
                TilesheetData::main::tiles::frame_bottom);
    }

    for (uint8_t y = 1; y < this->height - 1; y++) {
        SetTile(this->x, this->y + y, TilesheetData::main::tiles::frame_left);
        SetTile(this->x + this->width - 1, this->y + y,
                TilesheetData::main::tiles::frame_right);
    }
}

void FrameWidget::draw_background() {
    Fill(this->x + this->BORDER_WIDTH,
            this->y + this->BORDER_WIDTH,
            this->width - this->BORDER_WIDTH * 2,
            this->height - this->BORDER_WIDTH * 2,
            DEFAULT_TILE);
}

void FrameWidget::update_printer() {
    this->printer.origin_x = this->x + this->BORDER_WIDTH;
    this->printer.origin_y = this->y + this->BORDER_WIDTH;
    this->printer.width = this->width - this->BORDER_WIDTH * 2;
}
