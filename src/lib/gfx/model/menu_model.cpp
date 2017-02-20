#include <stdlib.h>

extern "C" {
#include <uzebox.h>
}

#include "menu_model.hpp"
using Lib::Gfx::MenuModel;

MenuModel::MenuModel() : current_index(0), num_items(0), window_size(0),
        window_offset(0), window_current_index(0),
        orientation(MenuOrientation::vertical) {
}

void MenuModel::reset(uint8_t num_items, MenuOrientation orientation) {
    this->current_index = 0;
    this->num_items = num_items;
    this->orientation = orientation;
    this->window_current_index = 0;
    this->window_size = 0;
    this->window_offset = 0;
}

void MenuModel::previous() {
    if (this->current_index == 0) {
        // Wrap to bottom of list
        this->current_index = this->num_items - 1;
        this->window_offset = this->num_items - this->window_size;
        this->window_current_index = this->window_size - 1;
    } else {
        this->current_index -= 1;

        if (this->window_current_index == 0) {
            // Display cursor already at top of display list
            this->window_offset -= 1;
        } else {
            this->window_current_index -= 1;
        }
    }
}

void MenuModel::next() {
    if (this->current_index + 1 > this->num_items - 1) {
        // Wrap to top of list
        this->current_index = 0;
        this->window_current_index = 0;
        this->window_offset = 0;
    } else {
        this->current_index += 1;

        if (this->window_current_index >= this->window_size - 1) {
            // Display Cursor already at bottom of display list
            this->window_offset += 1;
        } else {
            this->window_current_index += 1;
        }
    }
}

MenuModel::MenuSelection MenuModel::read_input(Lib::Input *input) {
    int next_button;
    int prev_button;

    if (this->orientation == MenuOrientation::horizontal) {
        next_button = BTN_RIGHT;
        prev_button = BTN_LEFT;
    } else {
        next_button = BTN_DOWN;
        prev_button = BTN_UP;
    }
    if (input->button_1_pressed & next_button) {
        this->next();
        return MenuSelection::next;
    } else if (input->button_1_pressed & prev_button) {
        this->previous();
        return MenuSelection::previous;
    }

    return MenuSelection::null;
}

bool MenuModel::window_has_more_begining() {
    return this->window_size && this->window_offset > 0;
}

bool MenuModel::window_has_more_ending() {
    return this->window_size &&
            this->window_offset + this->window_size < this->num_items;
}
