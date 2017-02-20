#pragma once

#include <stdint.h>

#include "input.hpp"
#include "gfx/widget/frame_widget.hpp"

namespace Lib {

class Dialog {
public:
    class DialogTextFlags {
    public:
    static const uint8_t PAINT_ON = 1;
    static const uint8_t CENTER = 1 << 1;
    static const uint8_t BOTTOM = 1 << 2;
    };

    typedef uint8_t DialogTextFlags_t;
protected:
    enum DialogState {
        empty, painting_row_1, painting_row_2, waiting_for_next,
        waiting_for_dismiss
    };


    Input * input;
    const char * text;
    uint8_t text_index;
    DialogTextFlags_t flags;
    DialogState state;
    bool dirty;

public:

    Gfx::FrameWidget text_dialog;
    static const uint8_t END_OF_DIALOG = 255;

    Dialog(Input * input);
    void update();
    void draw();
    void show_text(const char * text, DialogTextFlags_t flags = 0);
    bool is_text_pending();

protected:
    void continue_next_paragraph();
    void draw_next_letter();
};


}
