#include <avr/pgmspace.h>

extern "C" {
#include <uzebox.h>
}

#include "menu_widget.hpp"
#include "../../util.hpp"
#include "../../inc/menu.hpp"

using Lib::Gfx::MenuWidget;
using Lib::Gfx::MenuModel;

void MenuWidget::print_menu_item(uint8_t index, const char *string, const char *icon_map) {
    const uint8_t menu_cursor_space_offset_x = 2;

    if (this->model.orientation == MenuModel::MenuOrientation::vertical) {
        this->printer.set_cursor(menu_cursor_space_offset_x, index * this->item_spacing);
    } else {
        this->printer.set_cursor(index * this->item_spacing, menu_cursor_space_offset_x);
    }

    if (icon_map) {
        this->printer.draw_map_2(icon_map);
    }
    this->printer.print(string);
}

void MenuWidget::reset() {
    FrameWidget::reset();
    this->item_spacing = 2;
}

void MenuWidget::add_menu_item(Inc::MenuItemID_t menu_item_id) {
    assert(this->menu_items != nullptr);

    this->menu_items[this->model.num_items] = menu_item_id;
    this->model.num_items += 1;

    const uint8_t item_width = this->compute_menu_item_length(menu_item_id);

    this->set_size(
            max(this->width, item_width),
            min(2 + this->model.num_items * this->item_spacing,
                    2 + LIST_MAX_DISPLAY_ITEMS * this->item_spacing)
    );

    if (this->model.num_items > LIST_MAX_DISPLAY_ITEMS) {
        this->model.window_size = LIST_MAX_DISPLAY_ITEMS;
    }
}

uint8_t MenuWidget::compute_menu_item_length(Inc::MenuItemID_t menu_item_id) {
    const Inc::MenuItem * menu_item = &MenuData::MenuItems[menu_item_id];
    PGM_P const label = (PGM_P) pgm_read_ptr(&menu_item->label);
    const uint8_t label_len = strlen_P(label);
    const uint8_t cursor_spacing = 2;

    uint8_t width = label_len + MenuWidget::BORDER_WIDTH * 2 + cursor_spacing;
    PGM_P icon_map = (PGM_P) pgm_read_ptr(&menu_item->icon_map);
    if (icon_map) {
        width += pgm_read_byte(&icon_map[0]);
    }

    return width;
}


void MenuWidget::draw(bool full) {
    if (full) {
        this->draw_border();
        this->draw_background();
        this->draw_menu_items();
    }

    this->draw_menu_cursor();
    this->draw_more_icon();
}

void MenuWidget::draw_menu_cursor() {
    uint8_t list_size;

    if (this->model.window_size) {
        list_size = this->model.window_size;
    } else {
        list_size = this->model.num_items;
    }

    // Clear the cursor tiles
    for (uint8_t index = 0; index < list_size; index++) {
        if (this->model.orientation == MenuModel::MenuOrientation::vertical) {
            this->printer.set_cursor(0, index * this->item_spacing);
        } else {
            this->printer.set_cursor(index * this->item_spacing, 0);
        }

        this->printer.set_tile(DEFAULT_TILE);
    }

    uint8_t cursor_index;

    if (this->model.window_size) {
        cursor_index = this->model.window_current_index * this->item_spacing;
    } else {
        cursor_index = this->model.current_index * this->item_spacing;
    }

    // Draw the cursor
    if (this->model.orientation == MenuModel::MenuOrientation::vertical) {
        this->printer.set_cursor(0, cursor_index);
        this->printer.set_tile(TilesheetData::main::tiles::triangle_pointer_right);
    } else {
        this->printer.set_cursor(cursor_index, 0);
        this->printer.set_tile(TilesheetData::main::tiles::triangle_pointer_down);
    }
}

void MenuWidget::draw_more_icon() {
    if (this->model.window_has_more_begining()) {
        SetTile(this->x + this->width / 2, this->y, TilesheetData::main::tiles::frame_top_more);
    }
    if (this->model.window_has_more_ending()) {
        SetTile(this->x + this->width / 2, this->y + this->height - 1, TilesheetData::main::tiles::frame_bottom_more);
    }
}

void MenuWidget::draw_menu_items() {
    uint8_t start_index;

    if (this->model.window_size) {
        start_index = this->model.window_offset;
    } else {
        start_index = 0;
    }

    for (uint8_t item_index = 0; item_index < LIST_MAX_DISPLAY_ITEMS; item_index++) {
        if (start_index + item_index >= this->model.num_items) {
            break;
        }

        const uint8_t menu_item_id = this->menu_items[start_index + item_index];
        const Inc::MenuItem * menu_item = &MenuData::MenuItems[menu_item_id];

        this->print_menu_item(
                item_index,
                (PGM_P) pgm_read_ptr(&menu_item->label),
                (PGM_P) pgm_read_ptr(&menu_item->icon_map)
        );
    }
}
