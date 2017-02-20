extern "C" {
#include <uzebox.h>
}
#include <stdlib.h>
#include <avr/pgmspace.h>

#include "dialog.hpp"
#include "util.hpp"
#include "inc/tilesheet.hpp"

using Lib::Dialog;

Dialog::Dialog(Input * input) : input(input), text(NULL), text_index(0),
        flags(0), state(DialogState::empty), dirty(false) {
    this->text_dialog.set_size(SCREEN_TILES_H - SCREEN_OVERSCAN_BORDER * 2, 6);
}

void Dialog::update() {
    if ((this->input->button_1_pressed & BTN_A) &&
            (this->state == DialogState::waiting_for_next ||
                    this->state == DialogState::waiting_for_dismiss)) {
       this->continue_next_paragraph();
    }
}

void Dialog::draw() {
    if (!(this->state == DialogState::painting_row_1 ||
            this->state == DialogState::painting_row_2)) {
        return;
    }

    if (this->dirty) {
        this->text_dialog.draw_background();
        this->text_dialog.draw_border();
        this->dirty = false;
    }

    if (this->flags & DialogTextFlags::PAINT_ON) {
        this->draw_next_letter();
    } else {
        while (this->state == DialogState::painting_row_1 ||
                this->state == DialogState::painting_row_2) {
            this->draw_next_letter();
        }
    }
}

void Dialog::show_text(const char * text, DialogTextFlags_t flags) {
    this->text = text;
    this->flags = flags;
    this->text_index = 0;
    this->state = DialogState::painting_row_1;

    if (flags & DialogTextFlags::BOTTOM) {
        this->text_dialog.set_position(SCREEN_OVERSCAN_BORDER,
                SCREEN_TILES_V - 6 - SCREEN_OVERSCAN_BORDER);
    } else {
        this->text_dialog.set_position(SCREEN_OVERSCAN_BORDER,
                SCREEN_OVERSCAN_BORDER);
    }

    this->dirty = true;
    this->text_dialog.printer.set_cursor(0, 0);
}

void Dialog::continue_next_paragraph() {
    if (this->state == DialogState::waiting_for_next) {
        this->text_dialog.printer.set_cursor(0, 0);
        this->dirty = true;
        this->text_index += 1;
        this->state = DialogState::painting_row_1;
    } else {
        this->text = NULL;
        this->state = DialogState::empty;
    }
}

void Dialog::draw_next_letter() {
    char letter = pgm_read_byte(&this->text[this->text_index]);

    if (letter == '\n') {
        if (this->state == DialogState::painting_row_1) {
            this->text_dialog.printer.newline();
            this->text_index += 1;
            this->state = DialogState::painting_row_2;
        } else {
            this->text_dialog.printer.set_tile(
                    TilesheetData::main::tiles::triangle_pointer_down);
            this->state = DialogState::waiting_for_next;
        }
    } else if (letter) {
        this->text_dialog.printer.print_char(letter);
        this->text_index += 1;
    } else {
        this->state = DialogState::waiting_for_dismiss;
    }
}

bool Dialog::is_text_pending() {
    return this->state != DialogState::empty;
}
