#pragma once
#include <ctype.h>

#include "../../input.hpp"
#include "menu_model.hpp"

namespace Lib {
namespace Gfx {

class GridMenuModel {
public:
    enum class MenuSelection {
        null, up, down, left, right
    };

    uint8_t current_index;
    uint8_t num_items;
    uint8_t grid_width;

    GridMenuModel();
    void reset(uint8_t num_items = 0, uint8_t grid_width = 4);
    void up();
    void down();
    void left();
    void right();
    MenuSelection read_input(Lib::Input *input);

protected:
    void move_grid(MenuSelection button);
};

}
}
