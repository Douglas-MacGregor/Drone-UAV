#ifndef DUALSHOCK_UTILS_H
#define DUALSHOCK_UTILS_H
#include <stdint.h>

typedef struct __attribute__((packed))
{
    uint8_t report_id;
    uint8_t left_stick_x;   // L R -> 0 is left, 255 is right
    uint8_t left_stick_y;   // U D -> 0 is up, 255 is down
    uint8_t right_stick_x;  // L R -> 0 is left, 255 is right
    uint8_t right_stick_y;  // U D -> 0 is up, 255 is down
    uint8_t buttons_shapes; // triangle circle cross square // Keypad see wiki for more info
    uint8_t buttons_misc;   // R3 L3 options share R2 R2 R1 L1
    uint8_t counter;
    uint8_t left_trigger;
    uint8_t right_trigger; // 0xFF fully pressed 0x00 not pressed
    uint8_t padding[54];
} DualShockReport;

#endif // DUALSHOCK_UTILS_H