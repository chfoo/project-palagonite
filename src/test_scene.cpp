#include <avr/pgmspace.h>

#include "test_scene.hpp"
#include "lib/inc/menu.hpp"

TestScene::TestScene(GameApp * app) : BaseScene(app), dialog(&app->input),
        menu(&app->input), state(State::show_dialog_1) {

}

void TestScene::set_up() {
    printer.set_cursor(5, 10);
    printer.print_center(PSTR("Test scene"));
}

void TestScene::update() {
    switch (state) {
    case State::show_dialog_1:
        dialog.show_text(PSTR("This is some\ndialog text\nwith paint on."),
                Lib::Dialog::DialogTextFlags::PAINT_ON);
        state = State::poll_dialog_1;
        break;

    case State::poll_dialog_1:
       if (dialog.is_text_pending()) {
           dialog.update();
           dialog.draw();
       } else {
           state = State::show_dialog_2;
           ClearVram();
       }
       break;

    case State::show_dialog_2:
        dialog.show_text(PSTR("This one is\nat the bottom\nand centered."),
                Lib::Dialog::DialogTextFlags::BOTTOM |
                Lib::Dialog::DialogTextFlags::CENTER);
        state = State::poll_dialog_2;
        break;

    case State::poll_dialog_2:
        if (dialog.is_text_pending()) {
            dialog.update();
            dialog.draw();
        } else {
            state = State::show_menu_1;
            ClearVram();
        }
        break;

    case State::show_menu_1:
        menu.push_menu();
        menu.add_menu_item(MenuData::Index::yes);
        menu.add_menu_item(MenuData::Index::no);
        state = State::poll_menu_1;

        break;

    case State::poll_menu_1:
        menu.update();
        menu.draw();
        if (menu.response.action == Lib::Menu::MenuAction::affirmative) {
            switch (menu.response.menu_item_id) {
            case MenuData::Index::yes:
                state = State::show_menu_2;
                break;
            case MenuData::Index::no:
                this->menu.pop_menu();
                state = State::quit;
                ClearVram();
                break;
            }
        } else if (menu.response.action == Lib::Menu::MenuAction::dismissive) {
            this->menu.pop_menu();
            state = State::quit;
            ClearVram();
        }
        break;

    case State::show_menu_2:
        menu.push_menu();
        menu.add_menu_item(MenuData::Index::yes);
        menu.add_menu_item(MenuData::Index::yes);
        menu.add_menu_item(MenuData::Index::yes);
        state = State::poll_menu_2;

        break;

    case State::poll_menu_2:
        menu.update();

        if (menu.response.action == Lib::Menu::MenuAction::affirmative ||
                menu.response.action == Lib::Menu::MenuAction::dismissive) {
            this->menu.pop_menu();
            state = State::poll_menu_1;
            ClearVram();
        }
        menu.draw();

        break;

    case State::quit:
        this->exit_update();
        break;

    }
}
