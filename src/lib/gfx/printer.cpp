#include <avr/pgmspace.h>
#include <stdlib.h>

extern "C" {
#include <uzebox.h>
}

#include "printer.hpp"
using Lib::Gfx::Printer;

Printer::Printer() :
        origin_x(0), origin_y(0), cursor_x(0), cursor_y(0), width(
                SCREEN_TILES_H) {

}

void Printer::set_origin(uint8_t x, uint8_t y) {
    this->origin_x = x;
    this->origin_y = y;
}

void Printer::set_cursor(uint8_t x, uint8_t y) {
    this->cursor_x = x;
    this->cursor_y = y;
}

void Printer::move_cursor(uint8_t x, uint8_t y) {
    this->cursor_x += x;
    this->cursor_y += y;
}

void Printer::align_center(uint8_t content_width) {
    this->cursor_x = this->width / 2 - content_width / 2;
}

uint8_t Printer::abs_x() {
    return this->origin_x + this->cursor_x;
}

uint8_t Printer::abs_y() {
    return this->origin_y + this->cursor_y;
}

void Printer::newline() {
    this->cursor_x = 0;
    this->cursor_y += 2;

}

void Printer::print(const char *string) {
    uint8_t index = 0;
    while (true) {
        char letter = pgm_read_byte(&string[index]);
        if (letter == '\n') {
            this->newline();
        } else if (letter) {
            this->print_char(letter);
        } else {
            break;
        }
        index += 1;
    }
}

void Printer::print_center(const char *string) {
    this->align_center(strlen_P(string));
    this->print(string);
}

void Printer::print_char(char letter) {
    PrintChar(this->abs_x(), this->abs_y(), letter);
    this->cursor_x += 1;
}

void Printer::draw_map_2(const char *map) {
    DrawMap2(this->abs_x(), this->abs_y(), map);
    this->cursor_x += pgm_read_byte(map + 0);
}

void Printer::set_tile(unsigned int tileId) {
    SetTile(this->abs_x(), this->abs_y(), tileId);
    this->cursor_x += 1;
}

void Printer::print_int(uint8_t value, uint8_t width, bool zero_pad) {
    for (uint8_t index = width - 1; index < 0xff; index--) {
        div_t result = div(value, 10);
        if (result.quot != 0 || result.rem != 0 || index == width - 1 || zero_pad) {
            PrintChar(this->abs_x() + index, this->abs_y(), result.rem + '0');
        }
        value = result.quot;
    }

    this->cursor_x += width;
}
