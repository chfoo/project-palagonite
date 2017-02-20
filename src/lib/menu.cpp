#include "menu.hpp"

#include "gfx/widget/menu_widget.hpp"
#include "util.hpp"

using Lib::MenuStack;
using Lib::Menu;
using namespace Lib::Gfx;

MenuStack::MenuStack(Input * input) : input(input), menu_index(-1), dirty(DirtyState::clean) {
}

MenuWidget * MenuStack::push_menu() {
    this->menu_index += 1;
    assert(this->menu_index < MenuStack::MAX_MENUS);

    MenuWidget * menu_widget = this->get_current_menu();
    menu_widget->reset();
    menu_widget->model.reset();
    menu_widget->set_size(2, 2);

    if (this->menu_index == 0) {
        menu_widget->set_position(SCREEN_OVERSCAN_BORDER, SCREEN_OVERSCAN_BORDER);
        menu_widget->menu_items = this->menu_items;
    } else {
        MenuWidget * parent_menu_widget = &this->menu_widgets[this->menu_index - 1];
        menu_widget->set_position(parent_menu_widget->x + 2, parent_menu_widget->y + 2);
        menu_widget->menu_items = &parent_menu_widget->menu_items[parent_menu_widget->model.num_items];
    }

    this->dirty = DirtyState::full_dirty;

    return menu_widget;
}

MenuWidget * MenuStack::get_current_menu() {
    if (this->menu_index >= 0) {
        return &this->menu_widgets[this->menu_index];
    } else {
        return nullptr;
    }
}

void MenuStack::pop_menu() {
    this->menu_index -= 1;
    assert(this->menu_index >= -1);

    this->dirty = DirtyState::full_dirty;
}

void MenuStack::draw() {
    if (this->menu_index < 0) {
        return;
    }

    if (this->dirty == DirtyState::full_dirty) {
        for (uint8_t menu_index = 0; menu_index <= this->menu_index; menu_index++) {
            MenuWidget * menu_widget = &menu_widgets[menu_index];
            menu_widget->draw(true);
        }
        this->dirty = DirtyState::clean;
    } else if (this->dirty == DirtyState::dirty) {
        MenuWidget * menu_widget = &menu_widgets[menu_index];
        menu_widget->draw(false);
    }

    assert(this->menu_index >= 0);
}

MenuModel::MenuSelection MenuStack::update() {
    MenuWidget * menu_widget = this->get_current_menu();
    MenuModel::MenuSelection const selection = menu_widget->model.read_input(this->input);

    this->response.selection = selection;
    this->response.menu_item_id = menu_widget->menu_items[menu_widget->model.current_index];
    this->response.item_index = menu_widget->model.current_index;

    if (selection != MenuModel::MenuSelection::null) {
        this->dirty = DirtyState::dirty;
    }

    return selection;
}


Menu::Menu(Input * input) : menu_stack(input), input(input) {

}

void Menu::update() {
    this->menu_stack.update();

    this->response.menu_item_id = this->menu_stack.response.menu_item_id;
    this->response.item_index = this->menu_stack.response.item_index;

    if (this->input->button_1_pressed & BTN_A) {
        this->response.action = MenuAction::affirmative;
    } else if (this->input->button_1_pressed & BTN_B) {
        this->response.action = MenuAction::dismissive;
    } else {
        this->response.action = MenuAction::none;
    }
}

void Menu::draw() {
    this->menu_stack.draw();
}

void Menu::push_menu() {
    this->menu_stack.push_menu();
}

void Menu::add_menu_item(Inc::MenuItemID_t menu_item_id) {
    this->menu_stack.get_current_menu()->add_menu_item(menu_item_id);
}

void Menu::pop_menu() {
    this->menu_stack.pop_menu();
}
