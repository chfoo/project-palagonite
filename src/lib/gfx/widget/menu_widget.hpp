#pragma once

#include <stdint.h>

#include "frame_widget.hpp"
#include "../../inc/menu_item.hpp"
#include "../model/menu_model.hpp"

namespace Lib {
namespace Gfx {

class MenuWidget : public FrameWidget {
public:
    static const uint8_t LIST_MAX_DISPLAY_ITEMS = 6;
    MenuModel model;
    uint8_t item_spacing = 2;
    Inc::MenuItemID_t * menu_items;

    void reset();
    void add_menu_item(Inc::MenuItemID_t menu_item_id);
    void draw(bool full);

protected:
    void print_menu_item(uint8_t index, const char *string, const char *icon_map);
    void draw_menu_cursor();
    void draw_more_icon();
    void draw_menu_items();
    uint8_t compute_menu_item_length(Inc::MenuItemID_t menu_item_id);
};

}
}

