#pragma once

#include <stdint.h>

namespace Lib {
namespace Gfx {

class Printer {
public:
    uint8_t origin_x;
    uint8_t origin_y;
    uint8_t cursor_x;
    uint8_t cursor_y;
    uint8_t width;

    Printer();
    void set_origin(uint8_t x, uint8_t y);
    void set_cursor(uint8_t x, uint8_t y);
    void move_cursor(uint8_t x, uint8_t y);
    void align_center(uint8_t content_width);
    uint8_t abs_x();
    uint8_t abs_y();
    void newline();
    void print(const char *string);
    void print_center(const char *string);
    void print_char(char letter);
    void draw_map_2(const char *map);
    void set_tile(unsigned int tileId);
    void print_int(uint8_t value, uint8_t width, bool zero_pad = false);
};

}
}
