#pragma once

#include <stdint.h>
#include <avr/pgmspace.h>

extern "C" {
#include <uzebox.h>
}

#define max(a,b) \
  ({ typeof (a) _a = (a); \
      typeof (b) _b = (b); \
    _a > _b ? _a : _b; })

#define min(a,b) \
  ({ typeof (a) _a = (a); \
      typeof (b) _b = (b); \
    _a > _b ? _b : _a; })

#define SCREEN_OVERSCAN_BORDER 1

#ifndef NDEBUG

#define MACRO_STRING(arg) #arg
#define ASSERT_PRINT(expression, file, line) \
    Print(0, 2, PSTR("* ASSERT *:")); \
    Print(0, 3, PSTR("F:" MACRO_STRING(file))); \
    Print(0, 4, PSTR("L:" MACRO_STRING(line))); \
    Print(0, 5, PSTR("E:" #expression));
#define assert(expression) \
    if (!(expression)) { \
        ASSERT_PRINT(expression, __FILE__, __LINE__) \
        while (true) { WaitVsync(60); } \
    }

#define TRACE_PRINT(message, line) \
     Print(0, 10, PSTR(MACRO_STRING(line) ":" message));

#define trace(message) TRACE_PRINT(message, __LINE__)

#else

#define assert(expression) ()
#define trace(message) ()

#endif

extern uint16_t __bss_end;

namespace Lib {
namespace Util {
uint16_t rand_int(uint16_t lower, uint16_t upper);
int16_t get_free_memory();
void shuffle_array(char * array, uint8_t length);
}
}
