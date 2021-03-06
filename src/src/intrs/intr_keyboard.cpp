/*
 * @Author: Dizzrt
 * @LastEditTime: 2021-12-02 20:32:54
 */
#include "dev/svga.h"
#include "interrupt.h"
#include "io.h"
struct Key {
    uint8_t normal, capslock, shift;
};

Key keyMap[] = {
    /*0*/
    {}, {}, {'1', '1', '!'}, {'2', '2', '@'}, {'3', '3', '#'}, {'4', '4', '$'}, {'5', '5', '%'}, {'6', '6', '^'},
    /*8*/
    {'7', '7', '&'}, {'8', '8', '*'}, {'9', '9', '('}, {'0', '0', ')'}, {'-', '-', '_'}, {'=', '=', '+'}, {0, 0, 0},
    {'\t', '\t', '\t'},
    /*10*/ {'q', 'Q', 'Q'}, {'w', 'W', 'W'}, {'e', 'E', 'E'}, {'r', 'R', 'R'}, {'t', 'T', 'T'}, {'y', 'Y', 'Y'},
    {'u', 'U', 'U'}, {'i', 'I', 'I'},
    /*18*/ {'o', 'O', 'O'}, {'p', 'P', 'P'}, {'[', '[', '{'}, {']', ']', '}'}, {'\n', '\n', '\n'}, {}, {'a', 'A', 'A'},
    {'s', 'S', 'S'},
    /*20*/ {'d', 'D', 'D'}, {'f', 'F', 'F'}, {'g', 'G', 'G'}, {'h', 'H', 'H'}, {'j', 'J', 'J'}, {'k', 'K', 'K'},
    {'l', 'L', 'L'}, {';', ';', ':'},
    /*28*/ {'\'', '\'', '"'}, {'`', '`', '~'}, {}, {'\\', '\\', '|'}, {'z', 'Z', 'Z'}, {'x', 'X', 'X'}, {'c', 'C', 'C'},
    {'v', 'V', 'V'},
    /*30*/ {'b', 'B', 'B'}, {'n', 'N', 'N'}, {'m', 'M', 'M'}, {',', ',', '<'}, {'.', '.', '>'}, {'/', '/', '?'}, {}, {},
    /*38*/ {}, {}, {}, {}, {}, {}, {}, {},
    /*40*/ {}, {}, {}, {}, {}, {}, {}, {},
    /*48*/ {}, {}, {}, {}, {}, {}, {}, {},
    /*50*/ {}, {}, {}, {}, {}, {}, {}, {},
    /*58*/ {}, {}, {}, {}, {}, {}, {}, {},
    /*60*/ {}, {}, {}, {}, {}, {}, {}, {}};

bool capslock = false;
bool shift_l = false, shift_r = false;
bool ctrl_l = false, ctrl_r = false;
bool alt_l = false, alt_r = false;

void intr_keyboard(uint64_t ecode) {
    __outb__(0x20, 0x61);
    uint8_t keyCode = __inb__(0x60);

    if (keyCode == 0xe0) {
        // TODO extensino scan code
    } else {
        if ((keyCode & 0x80) == 0) // pressed
        {
            if (keyCode == 0x0e) {
                svga_MoveCursor(1, false);
                putk_svag(0);
                svga_MoveCursor(1, false); // FIXME backspace
                return;
            }
            switch (keyCode) {
                case 0x2a: shift_l = true; break;
                case 0x36: shift_r = true; break;
                case 0x1d: ctrl_l = true; break;
                case 0x38: alt_l = true; break;
                case 0x3a: capslock = !capslock; break;
                default:
                    if ((shift_r || shift_l))
                        putk_svag(keyMap[keyCode].shift);
                    else if (capslock)
                        putk_svag(keyMap[keyCode].capslock);
                    else
                        putk_svag(keyMap[keyCode].normal);
                    break;
            }
        } else // released
        {
            keyCode -= 0x80;

            switch (keyCode) {
                case 0x2a: shift_l = false; break;
                case 0x36: shift_r = false; break;
                case 0x1d: ctrl_l = false; break;
                case 0x38: alt_l = false; break;
            }
        }
    }

    return;
}