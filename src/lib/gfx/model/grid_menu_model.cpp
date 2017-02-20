
#include <stdlib.h>

extern "C" {
#include <uzebox.h>
}

#include "grid_menu_model.hpp"
using Lib::Gfx::GridMenuModel;

GridMenuModel::GridMenuModel() : current_index(0), num_items(0), grid_width(4) {
}

void GridMenuModel::reset(uint8_t num_items, uint8_t grid_width) {
    this->current_index = 0;
    this->num_items = num_items;
    this->grid_width = grid_width;
}

void GridMenuModel::up() {
    this->move_grid(MenuSelection::up);
}

void GridMenuModel::down() {
    this->move_grid(MenuSelection::down);
}
void GridMenuModel::left() {
    this->move_grid(MenuSelection::left);
}
void GridMenuModel::right() {
    this->move_grid(MenuSelection::right);
}

void GridMenuModel::move_grid(MenuSelection button) {
    div_t div_result = div(this->current_index, this->grid_width);
    int8_t col = div_result.rem;
    int8_t row = div_result.quot;

    div_result = div(this->num_items, this->grid_width);
    int8_t max_col = div_result.rem;
    int8_t max_row = div_result.quot;

    switch (button) {
    case MenuSelection::up:
        row -= 1;

        if (row < 0) {
            row = max_row;
        }
        break;
    case MenuSelection::down:
        row += 1;

        if (row > max_row) {
            row = 0;
        }
        break;
    case MenuSelection::left:
        col -= 1;

        if (row == max_row && col < 0) {
            col = max_col;
        } else if (col < 0) {
            col = this->grid_width;
        }
        break;
    case MenuSelection::right:
        col += 1;

        if ((row == max_row && col > max_row) || col >= this->grid_width) {
            col = 0;
        }
        break;
    default:
        break;
    }

    this->current_index = row * this->grid_width + col;
}

GridMenuModel::MenuSelection GridMenuModel::read_input(Lib::Input *input) {
    MenuSelection selection;

    if (input->button_1_pressed & BTN_UP) {
        selection = MenuSelection::up;
    } else if (input->button_1_pressed & BTN_DOWN) {
        selection = MenuSelection::down;
    } else if (input->button_1_pressed & BTN_LEFT) {
        selection = MenuSelection::left;
    } else if (input->button_1_pressed & BTN_RIGHT) {
        selection = MenuSelection::right;
    } else {
        return MenuSelection::null;
    }

    this->move_grid(selection);

    return selection;
}

