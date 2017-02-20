#pragma once

namespace Lib {

class Input {

public:
    Input();
    void process();
    unsigned int button_1_down;
    unsigned int button_1_pressed;
    unsigned int button_1_released;
};

}
