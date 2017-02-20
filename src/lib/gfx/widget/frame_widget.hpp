#pragma once

#include <stdint.h>

#include "../printer.hpp"

namespace Lib {
namespace Gfx {

class FrameWidget {
public:
    static const uint8_t BORDER_WIDTH = 1;

    uint8_t x;
    uint8_t y;
    uint8_t width;
    uint8_t height;
    Lib::Gfx::Printer printer;

    FrameWidget();
    void reset();
    void set_position(uint8_t x, uint8_t y);
    void set_size(uint8_t width, uint8_t height);
    void align_center();
    void draw_border();
    void draw_background();

protected:
    void update_printer();
};

}
}
