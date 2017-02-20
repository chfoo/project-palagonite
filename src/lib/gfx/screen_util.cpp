#include <avr/pgmspace.h>
#include <stdlib.h>

extern "C" {
#include <uzebox.h>
}

#include "screen_util.hpp"

using namespace Lib::Gfx;

void ScreenUtil::print_garbage() {
    for (u8 x = 0; x < VRAM_TILES_V; x++) {
        for (u8 y = 0; y < VRAM_TILES_H; y++) {
            SetTile(x, y, GetPrngNumber(0) % 256);
        }
    }
}
