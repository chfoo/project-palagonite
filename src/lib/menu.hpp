#pragma once

#include <stdint.h>

#include "gfx/model/menu_model.hpp"
#include "gfx/widget/menu_widget.hpp"
#include "inc/menu_item.hpp"

using namespace Lib::Gfx;

namespace Lib {

class MenuStack {
public:
    static const uint8_t MAX_MENUS = 5;
    static const uint8_t MAX_MENU_ITEMS = 32;
    static const uint8_t LIST_MAX_DISPLAY_ITEMS = 6;

    struct MenuStackResponse {
        MenuModel::MenuSelection selection;
        Inc::MenuItemID_t menu_item_id;
        uint8_t item_index;
    };

protected:
    Input * input;
    MenuWidget menu_widgets[MAX_MENUS];
    Inc::MenuItemID_t menu_items[MAX_MENU_ITEMS];
    int8_t menu_index; // -1 is no menu

    enum class DirtyState {
        clean, dirty, full_dirty
    };

    DirtyState dirty;

public:
    MenuStackResponse response;

    MenuStack(Input * input);
    MenuWidget * push_menu();
    MenuWidget * get_current_menu();
    void pop_menu();
    void draw();
    MenuModel::MenuSelection update();
};


class Menu {
    MenuStack menu_stack;
    Input * input;

public:
    enum class MenuAction {
        none, affirmative, dismissive
    };

    struct MenuResponse {
        MenuAction action;
        Inc::MenuItemID_t menu_item_id;
        uint8_t item_index;
    };
    MenuResponse response;

    Menu(Input * input);

    void update();
    void draw();
    void push_menu();
    void add_menu_item(Inc::MenuItemID_t menu_item_id);
    void pop_menu();
};

}
