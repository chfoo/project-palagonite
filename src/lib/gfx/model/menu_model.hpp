#pragma once
#include <ctype.h>

#include "../../input.hpp"

namespace Lib {
namespace Gfx {

class MenuModel {
public:
    enum class MenuSelection {
        null, next, previous
    };

    enum class MenuOrientation {
        horizontal, vertical
    };

    uint8_t current_index;
    uint8_t num_items;
    uint8_t window_size;
    uint8_t window_offset;
    uint8_t window_current_index;
    MenuOrientation orientation;

    MenuModel();
    void reset(uint8_t num_items = 0, MenuOrientation orientation = MenuOrientation::vertical);
    void previous();
    void next();
    MenuSelection read_input(Lib::Input *input);
    bool window_has_more_begining();
    bool window_has_more_ending();
};

}
}
