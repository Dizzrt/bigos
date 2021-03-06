#include "dev/timer.h"
#include "io.h"

void set_pt_value(uint8_t sc, uint8_t rw, uint16_t val) {
    uint16_t port;
    switch (sc) {
        case 0: port = 0x40; break;
        case 1: port = 0x41; break;
        case 2: port = 0x42; break;
        default: port = 0x40; break;
    }

    __outb__(port, (uint8_t)val);
    if (rw == 0x3)
        __outb__(port, (uint8_t)(val >> 8));

    return;
}

void pt_init(uint8_t cw, uint16_t val) {
    __outb__(0x43, cw);
    set_pt_value(cw >> 6, (cw & 0x30) >> 4, val);
    return;
}