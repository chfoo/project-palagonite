
extern "C" {
#include <uzebox.h>
}

#include "input.hpp"

using Lib::Input;

Input::Input() :
        button_1_down(0), button_1_pressed(0), button_1_released(0) {
}

void Input::process() {
    unsigned int prev_button_1 = this->button_1_down;
    this->button_1_down = ReadJoypad(0);
    this->button_1_pressed = this->button_1_down
            & (this->button_1_down ^ prev_button_1);
    this->button_1_released = prev_button_1
            & (this->button_1_down ^ prev_button_1);

}
