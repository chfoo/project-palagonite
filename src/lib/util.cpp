
extern "C" {
#include <uzebox.h>
}

#include "util.hpp"
using namespace Lib;

uint16_t Util::rand_int(uint16_t lower, uint16_t upper) {
    // Quick, but not uniform!
	return GetPrngNumber(0) % (upper - lower + 1) + lower;
}

int16_t Util::get_free_memory() {
    // http://uzebox.org/forums/viewtopic.php?f=3&t=2349#p17305
    uint8_t thing = 1;
    return (uint16_t) &thing - (uint16_t) &__bss_end;
}

void Util::shuffle_array(char * array, uint8_t length) {
    // Quick, but likely not even close to random
    for (uint8_t index = 0; index < length; index++) {
        uint8_t other_index = Util::rand_int(0, length - 1);
        uint8_t other_value = array[other_index];
        array[other_index] = array[index];
        array[index] = other_value;
    }
}
